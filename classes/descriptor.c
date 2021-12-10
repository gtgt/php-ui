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
#ifndef HAVE_PHP_UI_DESCRIPTOR
#define HAVE_PHP_UI_DESCRIPTOR
#include <ui.h>

#include "php.h"

#include <classes/descriptor.h>

zend_object_handlers php_ui_descriptor_handlers;

zend_class_entry *uiDrawTextFontDescriptor_ce;
zend_class_entry *uiDrawTextFontWeight_ce;
zend_class_entry *uiDrawTextFontItalic_ce;
zend_class_entry *uiDrawTextFontStretch_ce;

zend_object* php_ui_descriptor_create(zend_class_entry *ce) {
	php_ui_descriptor_t *descriptor = 
		(php_ui_descriptor_t*) ecalloc(1, sizeof(php_ui_descriptor_t) + zend_object_properties_size(ce));

	zend_object_std_init(&descriptor->std, ce);

	object_properties_init(&descriptor->std, ce);

	descriptor->std.handlers = &php_ui_descriptor_handlers;

	return &descriptor->std;
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_descriptor_construct_info, 0, 0, 2)
	ZEND_ARG_TYPE_INFO(0, family, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, size, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, weight, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, italic, IS_LONG, 0)
	ZEND_ARG_TYPE_INFO(0, stretch, IS_LONG, 0)
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_descriptor_string_info, 0, 0, IS_STRING, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_descriptor_string_info, 0, 0, IS_STRING, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_descriptor_double_info, 0, 0, IS_DOUBLE, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_descriptor_double_info, 0, 0, IS_DOUBLE, NULL, 0)
#endif
ZEND_END_ARG_INFO()

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_descriptor_long_info, 0, 0, IS_LONG, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_descriptor_long_info, 0, 0, IS_LONG, NULL, 0)
#endif
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Text\Font\Descriptor UI\Draw\Text\Font\Descriptor::__construct(string family, double size [, int weight = UI\Draw\Text\Font\Weight::NORMAL]) */
PHP_METHOD(DrawTextFontDescriptor, __construct) 
{
	php_ui_descriptor_t *descriptor = php_ui_descriptor_fetch(getThis());
	zend_string *family = NULL;
	double size = 0;
	int weight = uiTextWeightNormal;
	int italic = uiTextItalicNormal;
	int stretch = uiTextStretchNormal;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "Sd|lll", &family, &size, &weight, &italic, &stretch) != SUCCESS) {
		return;
	}

	descriptor->d.Family = ZSTR_VAL(family);
	descriptor->d.Size = size;
	descriptor->d.Weight = weight;
	descriptor->d.Italic = italic;
	descriptor->d.Stretch = stretch;
} /* }}} */

/* {{{ proto string UI\Draw\Text\Font\Descriptor::geFamily(void) */
PHP_METHOD(DrawTextFontDescriptor, getFamily)
{
	php_ui_descriptor_t *descriptor = php_ui_descriptor_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_STRING(descriptor->d.Family);
} /* }}} */

/* {{{ proto double UI\Draw\Text\Font\Descriptor::getSize(void) */
PHP_METHOD(DrawTextFontDescriptor, getSize)
{
	php_ui_descriptor_t *descriptor = php_ui_descriptor_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_DOUBLE(descriptor->d.Size);
} /* }}} */

/* {{{ proto int UI\Draw\Text\Font\Descriptor::getWeight(void) */
PHP_METHOD(DrawTextFontDescriptor, getWeight)
{
	php_ui_descriptor_t *descriptor = php_ui_descriptor_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_LONG(descriptor->d.Weight);
} /* }}} */

/* {{{ proto int UI\Draw\Text\Font\Descriptor::getItalic(void) */
PHP_METHOD(DrawTextFontDescriptor, getItalic)
{
	php_ui_descriptor_t *descriptor = php_ui_descriptor_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_LONG(descriptor->d.Italic);
} /* }}} */

/* {{{ proto int UI\Draw\Text\Font\Descriptor::getStretch(void) */
PHP_METHOD(DrawTextFontDescriptor, getStretch)
{
	php_ui_descriptor_t *descriptor = php_ui_descriptor_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_LONG(descriptor->d.Stretch);
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_descriptor_methods[] = {
	PHP_ME(DrawTextFontDescriptor, __construct, php_ui_descriptor_construct_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawTextFontDescriptor, getFamily,   php_ui_descriptor_string_info,    ZEND_ACC_PUBLIC)
	PHP_ME(DrawTextFontDescriptor, getSize,     php_ui_descriptor_double_info,    ZEND_ACC_PUBLIC)
	PHP_ME(DrawTextFontDescriptor, getWeight,     php_ui_descriptor_long_info,    ZEND_ACC_PUBLIC)
	PHP_ME(DrawTextFontDescriptor, getItalic,     php_ui_descriptor_long_info,    ZEND_ACC_PUBLIC)
	PHP_ME(DrawTextFontDescriptor, getStretch,     php_ui_descriptor_long_info,    ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_DrawTextFontDescriptor) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Text\\Font", "Descriptor", php_ui_descriptor_methods);

	uiDrawTextFontDescriptor_ce = zend_register_internal_class(&ce);
	uiDrawTextFontDescriptor_ce->create_object = php_ui_descriptor_create;

	memcpy(&php_ui_descriptor_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_descriptor_handlers.offset = XtOffsetOf(php_ui_descriptor_t, std);

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Text\\Font", "Weight", NULL);

	uiDrawTextFontWeight_ce = zend_register_internal_class(&ce);
	uiDrawTextFontWeight_ce->ce_flags |= ZEND_ACC_FINAL;

#define php_ui_register_weight(c, n) zend_declare_class_constant_long(c, ZEND_STRL(#n), uiTextWeight##n)
	php_ui_register_weight(uiDrawTextFontWeight_ce, Minimum);
	php_ui_register_weight(uiDrawTextFontWeight_ce, Thin);
	php_ui_register_weight(uiDrawTextFontWeight_ce, UltraLight);
	php_ui_register_weight(uiDrawTextFontWeight_ce, Light);
	php_ui_register_weight(uiDrawTextFontWeight_ce, Book);
	php_ui_register_weight(uiDrawTextFontWeight_ce, Normal);
	php_ui_register_weight(uiDrawTextFontWeight_ce, Medium);
	php_ui_register_weight(uiDrawTextFontWeight_ce, SemiBold);
	php_ui_register_weight(uiDrawTextFontWeight_ce, Bold);
	php_ui_register_weight(uiDrawTextFontWeight_ce, UltraBold);
	php_ui_register_weight(uiDrawTextFontWeight_ce, Heavy);
	php_ui_register_weight(uiDrawTextFontWeight_ce, UltraHeavy);
	php_ui_register_weight(uiDrawTextFontWeight_ce, Maximum);
#undef php_ui_register_weight

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Text\\Font", "Italic", NULL);

	uiDrawTextFontItalic_ce = zend_register_internal_class(&ce);
	uiDrawTextFontItalic_ce->ce_flags |= ZEND_ACC_FINAL;

#define php_ui_register_italic(c, n) zend_declare_class_constant_long(c, ZEND_STRL(#n), uiTextItalic##n)
	php_ui_register_italic(uiDrawTextFontItalic_ce, Normal);
	php_ui_register_italic(uiDrawTextFontItalic_ce, Oblique);
	php_ui_register_italic(uiDrawTextFontItalic_ce, Italic);
#undef php_ui_register_italic

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Text\\Font", "Stretch", NULL);

	uiDrawTextFontStretch_ce = zend_register_internal_class(&ce);
	uiDrawTextFontStretch_ce->ce_flags |= ZEND_ACC_FINAL;

#define php_ui_register_stretch(c, n) zend_declare_class_constant_long(c, ZEND_STRL(#n), uiTextStretch##n)
	php_ui_register_stretch(uiDrawTextFontStretch_ce, UltraCondensed);
	php_ui_register_stretch(uiDrawTextFontStretch_ce, ExtraCondensed);
	php_ui_register_stretch(uiDrawTextFontStretch_ce, Condensed);
	php_ui_register_stretch(uiDrawTextFontStretch_ce, SemiCondensed);
	php_ui_register_stretch(uiDrawTextFontStretch_ce, Normal);
	php_ui_register_stretch(uiDrawTextFontStretch_ce, SemiExpanded);
	php_ui_register_stretch(uiDrawTextFontStretch_ce, Expanded);
	php_ui_register_stretch(uiDrawTextFontStretch_ce, ExtraExpanded);
	php_ui_register_stretch(uiDrawTextFontStretch_ce, UltraExpanded);
#undef php_ui_register_stretch

	
	return SUCCESS;
} /* }}} */
#endif
