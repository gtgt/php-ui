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
#ifndef HAVE_PHP_UI_LAYOUT
#define HAVE_PHP_UI_LAYOUT
#include <ui.h>

#include "php.h"

#include <classes/exceptions.h>
#include <classes/descriptor.h>
#include <classes/attributed.h>
#include <classes/layout.h>
#include <classes/color.h>

zend_object_handlers php_ui_layout_handlers;

zend_class_entry *uiDrawTextLayout_ce;

zend_object* php_ui_layout_create(zend_class_entry *ce) {
	php_ui_layout_t *layout = 
		(php_ui_layout_t*) ecalloc(1, sizeof(php_ui_layout_t) + zend_object_properties_size(ce));

	zend_object_std_init(&layout->std, ce);

	object_properties_init(&layout->std, ce);

	layout->std.handlers = &php_ui_layout_handlers;

	return &layout->std;
}

void php_ui_layout_free(zend_object *o) {
	php_ui_layout_t *layout = php_ui_layout_from(o);

	if (layout->l) {
		uiDrawFreeTextLayout(layout->l);
	}

	zend_object_std_dtor(o);
}
ZEND_BEGIN_ARG_INFO_EX(php_ui_layout_construct_info, 0, 0, 3)
	ZEND_ARG_OBJ_INFO(0,  text, UI\\Draw\\Text\\Attributed, 0)
	ZEND_ARG_OBJ_INFO(0,  defaultFont, UI\\Draw\\Text\\Font\\Descriptor, 0)
	ZEND_ARG_TYPE_INFO(0, width, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, align, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Text\Layout UI\Draw\Text\Layout::__construct(UI\Draw\Text\String text, UI\Draw\Text\Font\Descriptor defaultFont, double width, int|UI\Draw\Text\Align align) */
PHP_METHOD(DrawTextLayout, __construct) 
{
	php_ui_layout_t *layout = php_ui_layout_fetch(getThis());
	zval *attributedString = NULL;
	php_ui_attributed_t *s;
	zval *defaultFont = NULL;
	php_ui_descriptor_t *d;
	double width = 0;
	zend_long align = uiDrawTextAlignLeft;
	uiDrawTextLayoutParams p;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "OOdl", &attributedString, uiAttributedString_ce, &defaultFont, uiDrawTextFontDescriptor_ce, &width, &align) != SUCCESS) {
		return;
	}

	s = php_ui_attributed_fetch(attributedString);
	d = php_ui_descriptor_fetch(defaultFont);

	p.String = s->s;
	p.DefaultFont = d;
	p.Width = width;
	p.Align = align;

	layout->l = 
		uiDrawNewTextLayout(&p);
	layout->end = uiAttributedStringLen((uiAttributedString*)attributedString);
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_layout_methods[] = {
	PHP_ME(DrawTextLayout, __construct, php_ui_layout_construct_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_DrawTextLayout) 
{
	zend_class_entry ce;

	// casing like UI\Loop == PHP_UI_LOOP
	REGISTER_NS_LONG_CONSTANT("UI\\Draw\\Text\\Align", "Left", uiDrawTextAlignLeft, CONST_CS|CONST_PERSISTENT);
	REGISTER_NS_LONG_CONSTANT("UI\\Draw\\Text\\Align", "Center", uiDrawTextAlignCenter, CONST_CS|CONST_PERSISTENT);
	REGISTER_NS_LONG_CONSTANT("UI\\Draw\\Text\\Align", "Right", uiDrawTextAlignRight, CONST_CS|CONST_PERSISTENT);

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Text", "Layout", php_ui_layout_methods);

	uiDrawTextLayout_ce = zend_register_internal_class(&ce);
	uiDrawTextLayout_ce->create_object = php_ui_layout_create;

	memcpy(&php_ui_layout_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_layout_handlers.offset = XtOffsetOf(php_ui_layout_t, std);
	php_ui_layout_handlers.free_obj = php_ui_layout_free;
	
	return SUCCESS;
} /* }}} */
#endif
