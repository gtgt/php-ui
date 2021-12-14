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
#ifndef HAVE_PHP_UI_FBUTTON
#define HAVE_PHP_UI_FBUTTON
#include <ui.h>

#include "php.h"

#include <classes/exceptions.h>
#include <classes/control.h>
#include <classes/fbutton.h>
#include <classes/descriptor.h>

zend_object_handlers php_ui_fbutton_handlers;

zend_class_entry *uiFontButton_ce;

extern void php_ui_set_call(zend_object *object, const char *name, size_t nlength, zend_fcall_info *fci, zend_fcall_info_cache *fcc);
extern int php_ui_call(zend_fcall_info *fci, zend_fcall_info_cache *fcc);

void php_ui_fbutton_change_handler(uiFontButton *u, void *_button) {
	php_ui_fbutton_t *button = (php_ui_fbutton_t*) _button;

	if (button->change.fci.size) {
		zval rv;

		ZVAL_UNDEF(&rv);

		button->change.fci.retval = &rv;

		if (php_ui_call(&button->change.fci, &button->change.fcc) != SUCCESS) {
			return;
		}

		if (Z_TYPE(rv) != IS_UNDEF) {
			zval_ptr_dtor(&rv);
		}
	}
}

zend_object* php_ui_fbutton_create(zend_class_entry *ce) {
	php_ui_fbutton_t *fbutton = 
		(php_ui_fbutton_t*) ecalloc(1, sizeof(php_ui_fbutton_t) + zend_object_properties_size(ce));

	zend_object_std_init(&fbutton->std, ce);

	object_properties_init(&fbutton->std, ce);

	fbutton->std.handlers = &php_ui_fbutton_handlers;

	fbutton->b = uiNewFontButton();

	php_ui_set_call(&fbutton->std, ZEND_STRL("onchange"), &fbutton->change.fci, &fbutton->change.fcc);

	uiFontButtonOnChanged(fbutton->b, php_ui_fbutton_change_handler, fbutton);

	return &fbutton->std;
}

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(php_ui_fbutton_get_font_info, 0, 0, UI\\Draw\\Text\\Font\\Descriptor, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_fbutton_get_font_info, 0, 0, IS_OBJECT, "UI\\Draw\\Text\\Font\\Descriptor", 0)
#endif
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Font\Descriptor FontButton::getFont(void) */
PHP_METHOD(FontButton, getFont)
{
	php_ui_fbutton_t *fbutton = php_ui_fbutton_fetch(getThis());
	php_ui_descriptor_t *descriptor;
	uiFontDescriptor d;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiFontButtonFont(fbutton->b, &d);

	object_init_ex(return_value, uiDrawTextFontDescriptor_ce);
	
	descriptor = php_ui_descriptor_fetch(return_value);
	descriptor->d = d;
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_fbutton_on_change_info, 0, 0, 0)
ZEND_END_ARG_INFO()

/* {{{ proto void FontButton::onChange(c) */
PHP_METHOD(FontButton, onChange)
{
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_fbutton_methods[] = {
	PHP_ME(FontButton, getFont,               php_ui_fbutton_get_font_info,               ZEND_ACC_PUBLIC)
	PHP_ME(FontButton, onChange,              php_ui_fbutton_on_change_info,              ZEND_ACC_PROTECTED)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_FontButton) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Controls", "FontButton", php_ui_fbutton_methods);

	uiFontButton_ce = zend_register_internal_class_ex(&ce, uiControl_ce);
	uiFontButton_ce->create_object = php_ui_fbutton_create;

	memcpy(&php_ui_fbutton_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_ui_fbutton_handlers.offset = XtOffsetOf(php_ui_fbutton_t, std);

	return SUCCESS;
} /* }}} */
#endif
