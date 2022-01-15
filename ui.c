/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2016 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: krakjoe                                                      |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_ui.h"

#include <ui.h>

#ifdef HAVE_UI_X_THREADS
#	include <X11/Xlib.h>
#endif

#include <pthread.h>

#include <classes/exceptions.h>
#include <classes/executor.h>

#include <classes/point.h>
#include <classes/size.h>

#include <classes/control.h>
#include <classes/window.h>
#include <classes/tab.h>
#include <classes/form.h>
#include <classes/grid.h>
#include <classes/box.h>
#include <classes/check.h>
#include <classes/button.h>
#include <classes/cbutton.h>
#include <classes/fbutton.h>
#include <classes/label.h>
#include <classes/entry.h>
#include <classes/multi.h>
#include <classes/group.h>
#include <classes/spin.h>
#include <classes/slider.h>
#include <classes/progress.h>
#include <classes/separator.h>
#include <classes/combo.h>
#include <classes/ecombo.h>
#include <classes/radio.h>
#include <classes/picker.h>
#include <classes/menu.h>
#include <classes/item.h>

#include <classes/area.h>
#include <classes/pen.h>
#include <classes/path.h>
#include <classes/color.h>
#include <classes/brush.h>
#include <classes/stroke.h>
#include <classes/matrix.h>

#include <classes/descriptor.h>
#include <classes/layout.h>

#define PHP_UI_LOOP	1<<0
#define PHP_UI_WAIT	1<<1

// hoping won't have to care about https://www.phpinternalsbook.com/php7/extensions_design/globals_management.html
zend_fcall_info php_ui_should_quit_fci;
zend_fcall_info_cache php_ui_should_quit_fcc;

void php_ui_set_call(zend_object *object, const char *name, size_t nlength, zend_fcall_info *fci, zend_fcall_info_cache *fcc) {
	zend_function *function = zend_hash_str_find_ptr(&object->ce->function_table, name, nlength);

	if (!function || function->type == ZEND_INTERNAL_FUNCTION) {
		return;
	}

	fci->size = sizeof(zend_fcall_info);
	fci->object = object;
#if PHP_VERSION_ID < 80000
	fci->no_separation = 1; // "This is where the copy-on-write concept comes in: […] Separation is just a fancy word for duplicating the structure." - https://www.phpinternalsbook.com/php7/zvals/memory_management.html
#endif

#if PHP_VERSION_ID < 70300
	fcc->initialized = 1;
#endif
	fcc->object = object;
	fcc->function_handler = function;
	fcc->calling_scope = object->ce;
	fcc->called_scope = object->ce;
}

static inline zend_object* php_ui_top(zend_object *object) {
	if (!object) {
		return NULL;
	}

	do {
		php_ui_control_t *control = php_ui_control_from(object);

		if (instanceof_function(object->ce, uiWindow_ce)) {
			return object;
		}

		if (!control->parent) {
			break;
		}

		object = php_ui_control_from(object)->parent;
	} while (object);

	return NULL;
}

int php_ui_call(zend_fcall_info *fci, zend_fcall_info_cache *fcc) {
	int result = zend_call_function(fci, fcc);

	if (0) {
_php_ui_call_zend_exception_handler:
		zend_try_exception_handler();

		return result;
	}

	if (result != SUCCESS) {
		if (EG(exception)) {
			zend_object *top = php_ui_top(fci->object);

			if (top) {
				php_ui_window_t *window = php_ui_window_from(top);

				if (window->uncaught.fci.size) {
					zval ctrl, ex, rv;
					zend_object *exception = EG(exception);

					EG(exception) = NULL;

					ZVAL_OBJ(&ctrl, fci->object);
					ZVAL_OBJ(&ex, exception);
					ZVAL_UNDEF(&rv);

					window->uncaught.fci.retval = &rv;

					zend_fcall_info_argn(&window->uncaught.fci, 2, &ctrl, &ex);

					if (php_ui_call(&window->uncaught.fci, &window->uncaught.fcc) != SUCCESS) {
						EG(exception) = exception;
					} else {
						if (EG(exception)) {
							OBJ_RELEASE(EG(exception));
							EG(exception) = NULL;
						}

						OBJ_RELEASE(exception);
						zend_fcall_info_args_clear(&window->uncaught.fci, 1);
						zval_ptr_dtor(&rv);

						return SUCCESS;
					}

					return result;
				}
			}
			goto _php_ui_call_zend_exception_handler;
		}
	}

	return result;
}

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(ui)
{
#ifdef HAVE_UI_X_THREADS
	XInitThreads();
#endif

	REGISTER_NS_LONG_CONSTANT("UI", "Loop", PHP_UI_LOOP, CONST_CS|CONST_PERSISTENT);
	REGISTER_NS_LONG_CONSTANT("UI", "Wait", PHP_UI_WAIT, CONST_CS|CONST_PERSISTENT);

	PHP_MINIT(UI_Exceptions)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Executor)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(UI_Point)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Size)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(UI_Control)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Window)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Form)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Grid)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Tab)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Box)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Check)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Button)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_ColorButton)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_FontButton)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Label)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Entry)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Multi)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Group)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Spin)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Slider)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Progress)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Separator)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Combo)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_EditableCombo)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Radio)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Picker)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_Menu)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_MenuItem)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(UI_Area)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_DrawPen)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_DrawPath)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_DrawColor)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_DrawBrush)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_DrawStroke)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_DrawMatrix)(INIT_FUNC_ARGS_PASSTHRU);

	PHP_MINIT(UI_DrawTextAttributedString)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_DrawTextFontDescriptor)(INIT_FUNC_ARGS_PASSTHRU);
	PHP_MINIT(UI_DrawTextLayout)(INIT_FUNC_ARGS_PASSTHRU);

	php_ui_control_finalize();

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(ui)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(ui)
{
	uiInitOptions options;

	const char *initError = NULL;

#if defined(COMPILE_DL_UI) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	memset(&options, 0, sizeof(uiInitOptions));

	initError = uiInit(&options);

	if (initError) {
		zend_error(E_ERROR,
			"Cannot initialize libui: %s", initError);
		uiFreeInitError(initError);

		return FAILURE;
	}

	php_ui_should_quit_fci = empty_fcall_info;
	php_ui_should_quit_fcc = empty_fcall_info_cache;

	uiMainSteps();

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(ui)
{
#if 0
	uiUninit();
#endif
	if (php_ui_should_quit_fci.size) {
		zval_ptr_dtor(&php_ui_should_quit_fci.function_name);
		php_ui_should_quit_fci.size = 0;
	}

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(ui)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "ui support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ nothing happening here ... ignore this ... */
typedef struct _php_ui_object_t {
	void *p;
	zend_object std;
} php_ui_object_t;

int php_ui_serialize(zval *object, unsigned char **buffer, size_t *buflen, zend_serialize_data *data) {
	void *address = ((char*) Z_OBJ_P(object) - XtOffsetOf(php_ui_object_t, std));

#ifdef _WIN64
	(*buflen) = snprintf(NULL, 0, ":%I64u:", (unsigned __int64) address);
#else
	(*buflen) = snprintf(NULL, 0, ":%lu:", (long unsigned int) address);
#endif
	(*buffer) = emalloc((*buflen) + 1);
	sprintf(
#ifdef _WIN64
		(char*) (*buffer), ":%I64u:", (unsigned __int64) address);
#else
		(char*) (*buffer), ":%lu:", (long unsigned int) address);
#endif
	(*buffer)[(*buflen)] = 0;

	return SUCCESS;
}

#define php_ui_object_from(o) ((php_ui_object_t*) ((char*) o - XtOffsetOf(php_ui_object_t, std)))
#define php_ui_object_fetch(o) php_ui_object_from(Z_OBJ_P(o))

int php_ui_unserialize(zval *object, zend_class_entry *ce, const unsigned char *buffer, size_t blen, zend_unserialize_data *data) {
	php_ui_object_t *address = NULL;
	php_ui_object_t *o;

#ifdef _WIN64
	if (!sscanf((const char*) buffer, ":%I64u:", (unsigned __int64*)&address)) {
#else
	if (!sscanf((const char*) buffer, ":%lu:", (long unsigned int*)&address)) {
#endif
		return FAILURE;
	}

	object_init_ex(object, ce);

	o = php_ui_object_fetch(object);
	o->p = address->p;

	return SUCCESS;
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_run_info, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, flags, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void UI\run([int flags = 0])*/
PHP_FUNCTION(UI_run)
{
	zend_long flags = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|l", &flags) != SUCCESS) {
		return;
	}

	if (!(flags & PHP_UI_LOOP)) {
		uiMain();
		return;
	}

	uiMainStep((flags & PHP_UI_WAIT) == PHP_UI_WAIT);
} /* }}} */

int php_ui_should_quit_handler(void *arg) {
	zval rv;
	int result = 0;

	if (!php_ui_should_quit_fci.size) { // will never enter here until some resetting variant of UI\onShouldQuit (possibly by allowing null arg), setting … = empty_fcall_info… (again) and connecting equivalent void func w/ uiOnShouldQuit
//		uiQuit();

		return 1;
	}

	ZVAL_UNDEF(&rv);

	php_ui_should_quit_fci.retval = &rv;

	if (php_ui_call(&php_ui_should_quit_fci, &php_ui_should_quit_fcc) != SUCCESS) {
		return 0;
	}

	if (Z_TYPE(rv) != IS_UNDEF) {
		result =
			(int) zval_get_long(&rv);
		zval_ptr_dtor(&rv);
	}

	return result;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_should_quit_info, 0, 0, 1)
	ZEND_ARG_CALLABLE_INFO(0, shouldQuit, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void UI\run(callable shouldQuit(): bool)*/
PHP_FUNCTION(UI_onShouldQuit)
{
	if (php_ui_should_quit_fci.size) {
		zval_ptr_dtor(&php_ui_should_quit_fci.function_name);
		php_ui_should_quit_fci.size = 0;
	}

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "f", &php_ui_should_quit_fci, &php_ui_should_quit_fcc) == FAILURE) {
		return;
	}

	Z_ADDREF(php_ui_should_quit_fci.function_name);

	uiOnShouldQuit(php_ui_should_quit_handler, NULL);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_quit_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ */
PHP_FUNCTION(UI_quit)
{
	uiQuit();
} /* }}} */

/* {{{ ui_functions[]
 */
const zend_function_entry ui_functions[] = {
	ZEND_NS_NAMED_FE("UI", run, zif_UI_run, php_ui_run_info)
	ZEND_NS_NAMED_FE("UI", quit, zif_UI_quit, php_ui_quit_info)
	ZEND_NS_NAMED_FE("UI", onShouldQuit, zif_UI_onShouldQuit, php_ui_should_quit_info)
	PHP_FE_END
};
/* }}} */

/* {{{ ui_module_entry
 */
zend_module_entry ui_module_entry = {
	STANDARD_MODULE_HEADER,
	"ui",
	ui_functions,
	PHP_MINIT(ui),
	PHP_MSHUTDOWN(ui),
	PHP_RINIT(ui),
	PHP_RSHUTDOWN(ui),
	PHP_MINFO(ui),
	PHP_UI_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_UI
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(ui)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
