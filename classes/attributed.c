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
#ifndef HAVE_PHP_UI_ATTRIBUTED
#define HAVE_PHP_UI_ATTRIBUTED
#include <ui.h>

#include "php.h"

#include <classes/descriptor.h>
#include <classes/attributed.h>

zend_object_handlers php_ui_attributed_handlers;

zend_class_entry *uiAttributedString_ce;
zend_class_entry *uiDrawTextAlign_ce;

zend_object* php_ui_attributed_create(zend_class_entry *ce) {
	php_ui_attributed_t *string = 
		(php_ui_attributed_t*) ecalloc(1, sizeof(php_ui_attributed_t) + zend_object_properties_size(ce));

	zend_object_std_init(&string->std, ce);

	object_properties_init(&string->std, ce);

	string->std.handlers = &php_ui_attributed_handlers;

	return &string->std;
}

void php_ui_attributed_free(zend_object *o) {
	php_ui_attributed_t *string = php_ui_attributed_from(o);

	if (string->s) {
		uiFreeAttributedString(string->s);
	}

	zend_object_std_dtor(o);
}
ZEND_BEGIN_ARG_INFO_EX(php_ui_attributed_construct_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, text, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Text\Attributed UI\Draw\Text\Attributed::__construct(string text) */
PHP_METHOD(DrawTextAttributedString, __construct) 
{
	php_ui_attributed_t *string = php_ui_attributed_fetch(getThis());
	zend_string *text = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &text) != SUCCESS) {
		return;
	}

	string->s = uiNewAttributedString(ZSTR_VAL(text));

	if (!string->s) {
		/* throw */
	}
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_attributed_methods[] = {
	PHP_ME(DrawTextAttributedString, __construct,             php_ui_attributed_construct_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_DrawTextAttributedString) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Text", "Attributed", php_ui_attributed_methods);

	uiAttributedString_ce = zend_register_internal_class(&ce);
	uiAttributedString_ce->create_object = php_ui_attributed_create;

	memcpy(&php_ui_attributed_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_attributed_handlers.offset = XtOffsetOf(php_ui_attributed_t, std);
	php_ui_attributed_handlers.free_obj = php_ui_attributed_free;
	
	return SUCCESS;
} /* }}} */
#endif
