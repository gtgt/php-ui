/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2022 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: vike2000                                                     |
  +----------------------------------------------------------------------+
*/
#ifndef HAVE_PHP_UI_ATTRIBUTE
#define HAVE_PHP_UI_ATTRIBUTE
#include <ui.h>

#include "php.h"
#include "zend_API.h"
#include "zend_string.h"

#include <classes/_macro.h>

#include <classes/exceptions.h>

#include <classes/attribute.h>
#include <classes/color.h>

zend_object_handlers php_ui_attribute_handlers;
zend_object_handlers php_ui_attribute_features_handlers;

zend_class_entry *uiDrawTextAttribute_ce;
zend_class_entry *uiDrawTextAttributesFamily_ce;
zend_class_entry *uiDrawTextAttributesSize_ce;
zend_class_entry *uiDrawTextAttributesWeight_ce;
zend_class_entry *uiDrawTextAttributesItalic_ce;
zend_class_entry *uiDrawTextAttributesStretch_ce;
zend_class_entry *uiDrawTextAttributesColor_ce;
zend_class_entry *uiDrawTextAttributesBackground_ce;
zend_class_entry *uiDrawTextAttributesUnderline_ce;
zend_class_entry *uiDrawTextAttributesUnderlineColor_ce;
zend_class_entry *uiDrawTextAttributesFeatures_ce;
zend_class_entry *uiDrawTextAttributesFeaturesCollection_ce;

zend_object* php_ui_attribute_create(zend_class_entry *ce) {
	php_ui_attribute_t *attribute = 
		(php_ui_attribute_t*) ecalloc(1, sizeof(php_ui_attribute_t) + zend_object_properties_size(ce));

	zend_object_std_init(&attribute->std, ce);

	object_properties_init(&attribute->std, ce);

	attribute->std.handlers = &php_ui_attribute_handlers;

	return &attribute->std;
}

void php_ui_attribute_free(zend_object *o) {
	php_ui_attribute_t *attribute = php_ui_attribute_from(o);

	if (attribute->a) {
		uiFreeAttribute(attribute->a);
	}

	zend_object_std_dtor(o);
}

// TODO getters


ZEND_BEGIN_ARG_INFO_EX(php_ui_attributes_family_construct_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, value, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Text\Attributes\Family UI\Draw\Text\Attributes\Family::__construct(string value) */
PHP_METHOD(DrawTextAttributeFamily, __construct) 
{
	php_ui_attribute_t *attribute = php_ui_attribute_fetch(getThis());
	zend_string *value = NULL;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &value) != SUCCESS) {
		return;
	}

	attribute->a = uiNewFamilyAttribute(ZSTR_VAL(value));

	if (!attribute->a) {
		/* throw */
	}
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_attributes_family_methods[] = {
	PHP_ME(DrawTextAttributeFamily, __construct, php_ui_attributes_family_construct_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */


ZEND_BEGIN_ARG_INFO_EX(php_ui_attributes_size_construct_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Text\Attributes\Size UI\Draw\Text\Attributes\Size::__construct(int value) */
PHP_METHOD(DrawTextAttributeSize, __construct) 
{
	php_ui_attribute_t *attribute = php_ui_attribute_fetch(getThis());
	int value;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &value) != SUCCESS) {
		return;
	}

	attribute->a = uiNewSizeAttribute(value);

	if (!attribute->a) {
		/* throw */
	}
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_attributes_size_methods[] = {
	PHP_ME(DrawTextAttributeSize, __construct, php_ui_attributes_size_construct_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */


ZEND_BEGIN_ARG_INFO_EX(php_ui_attributes_weight_construct_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Text\Attributes\Weight UI\Draw\Text\Attributes\Weight::__construct(int value) */
PHP_METHOD(DrawTextAttributeWeight, __construct) 
{
	php_ui_attribute_t *attribute = php_ui_attribute_fetch(getThis());
	int value;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &value) != SUCCESS) {
		return;
	}

	attribute->a = uiNewWeightAttribute(value);

	if (!attribute->a) {
		/* throw */
	}
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_attributes_weight_methods[] = {
	PHP_ME(DrawTextAttributeWeight, __construct, php_ui_attributes_weight_construct_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */


ZEND_BEGIN_ARG_INFO_EX(php_ui_attributes_italic_construct_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Text\Attributes\Italic UI\Draw\Text\Attributes\Italic::__construct(int value) */
PHP_METHOD(DrawTextAttributeItalic, __construct) 
{
	php_ui_attribute_t *attribute = php_ui_attribute_fetch(getThis());
	int value;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &value) != SUCCESS) {
		return;
	}

	attribute->a = uiNewItalicAttribute(value);

	if (!attribute->a) {
		/* throw */
	}
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_attributes_italic_methods[] = {
	PHP_ME(DrawTextAttributeItalic, __construct, php_ui_attributes_italic_construct_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */


ZEND_BEGIN_ARG_INFO_EX(php_ui_attributes_stretch_construct_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Text\Attributes\Stretch UI\Draw\Text\Attributes\Stretch::__construct(int value) */
PHP_METHOD(DrawTextAttributeStretch, __construct) 
{
	php_ui_attribute_t *attribute = php_ui_attribute_fetch(getThis());
	int value;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &value) != SUCCESS) {
		return;
	}

	attribute->a = uiNewStretchAttribute(value);

	if (!attribute->a) {
		/* throw */
	}
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_attributes_stretch_methods[] = {
	PHP_ME(DrawTextAttributeStretch, __construct, php_ui_attributes_stretch_construct_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */


ZEND_BEGIN_ARG_INFO_EX(php_ui_attributes_color_construct_info, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Text\Attributes\Color UI\Draw\Text\Attributes\Color::__construct(UI\Draw\Color value)
	   proto UI\Draw\Text\Attributes\Color UI\Draw\Text\Attributes\Color::__construct(int value) */
PHP_METHOD(DrawTextAttributeColor, __construct) 
{
	php_ui_attribute_t *attribute = php_ui_attribute_fetch(getThis());
	zval *value = NULL;
	double r, g, b, a;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "z", &value) != SUCCESS) {
		return;
	}

	if (!php_ui_color_set(value, &r, &g, &b, &a)) {
		/* throw */
	}

	attribute->a = uiNewColorAttribute(r, g, b, a);

	if (!attribute->a) {
		/* throw */
	}
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_attributes_color_methods[] = {
	PHP_ME(DrawTextAttributeColor, __construct, php_ui_attributes_color_construct_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */


ZEND_BEGIN_ARG_INFO_EX(php_ui_attributes_background_construct_info, 0, 0, 1)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Text\Attributes\Background UI\Draw\Text\Attributes\Background::__construct(UI\Draw\Color value)
	   proto UI\Draw\Text\Attributes\Background UI\Draw\Text\Attributes\Background::__construct(int value) */
PHP_METHOD(DrawTextAttributeBackground, __construct) 
{
	php_ui_attribute_t *attribute = php_ui_attribute_fetch(getThis());
	zval *value = NULL;
	double r, g, b, a;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "z", &value) != SUCCESS) {
		return;
	}

	if (!php_ui_color_set(value, &r, &g, &b, &a)) {
		/* throw */
	}

	attribute->a = uiNewBackgroundAttribute(r, g, b, a);

	if (!attribute->a) {
		/* throw */
	}
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_attributes_background_methods[] = {
	PHP_ME(DrawTextAttributeBackground, __construct, php_ui_attributes_background_construct_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */


ZEND_BEGIN_ARG_INFO_EX(php_ui_attributes_underline_construct_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Text\Attributes\Underline UI\Draw\Text\Attributes\Underline::__construct(int value) */
PHP_METHOD(DrawTextAttributeUnderline, __construct) 
{
	php_ui_attribute_t *attribute = php_ui_attribute_fetch(getThis());
	int value;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "l", &value) != SUCCESS) {
		return;
	}

	attribute->a = uiNewUnderlineAttribute(value);

	if (!attribute->a) {
		/* throw */
	}
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_attributes_underline_methods[] = {
	PHP_ME(DrawTextAttributeUnderline, __construct, php_ui_attributes_underline_construct_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */


ZEND_BEGIN_ARG_INFO_EX(php_ui_attributes_underline_color_construct_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, category, IS_LONG, 0)
	ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Text\Attributes\UnderlineColor UI\Draw\Text\Attributes\UnderlineColor::__construct(int suggestive, UI\Draw\Color value)
	   proto UI\Draw\Text\Attributes\UnderlineColor UI\Draw\Text\Attributes\UnderlineColor::__construct(int suggestive, int value) */
PHP_METHOD(DrawTextAttributeUnderlineColor, __construct) 
{
	php_ui_attribute_t *attribute = php_ui_attribute_fetch(getThis());
	zval *value = NULL;
	long suggestive;
	double r, g, b, a;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "lz", &suggestive, &value) != SUCCESS) {
		return;
	}

	if (!php_ui_color_set(value, &r, &g, &b, &a)) {
		/* throw */
	}

	attribute->a = uiNewUnderlineColorAttribute(suggestive, r, g, b, a);

	if (!attribute->a) {
		/* throw */
	}
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_attributes_underline_color_methods[] = {
	PHP_ME(DrawTextAttributeUnderlineColor, __construct, php_ui_attributes_underline_color_construct_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */


ZEND_BEGIN_ARG_INFO_EX(php_ui_attributes_features_construct_info, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, value, UI\\Draw\\Text\\Attributes\\Features\\Collection, 0)
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Text\Attributes\Features UI\Draw\Text\Attributes\Features::__construct(UI\Draw\Text\Attributes\Features\Collection value) */
PHP_METHOD(DrawTextAttributeFeatures, __construct) 
{
	php_ui_attribute_t *attribute = php_ui_attribute_fetch(getThis());
	zval *value = NULL;
	php_ui_attribute_features_t *f;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &value, uiDrawTextAttributesFeaturesCollection_ce) != SUCCESS) {
		return;
	}

	f = php_ui_attribute_features_fetch(value);

	attribute->a = uiNewFeaturesAttribute(f->f);

	if (!attribute->a) {
		/* throw */
	}
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_attributes_features_methods[] = {
	PHP_ME(DrawTextAttributeUnderlineColor, __construct, php_ui_attributes_features_construct_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

zend_object* php_ui_attribute_features_create(zend_class_entry *ce) {
	php_ui_attribute_features_t *features = 
		(php_ui_attribute_features_t*) ecalloc(1, sizeof(php_ui_attribute_features_t) + zend_object_properties_size(ce));

	zend_object_std_init(&features->std, ce);

	object_properties_init(&features->std, ce);

	// TODO uiOpenTypeFeaturesClone

	features->std.handlers = &php_ui_attribute_features_handlers;

	return &features->std;
}

void php_ui_attribute_features_free(zend_object *o) {
	php_ui_attribute_features_t *features = php_ui_attribute_features_from(o);

	if (features->f) {
		uiFreeOpenTypeFeatures(features->f);
	}

	zend_object_std_dtor(o);
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_attributes_features_collection_construct_info, 0, 0, 1)
	ZEND_ARG_OBJ_INFO(0, value, UI\\Draw\\Text\\Attributes\\Features\\Collection, 0)
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Text\Attributes\Features\Collection UI\Draw\Text\Attributes\Features\Collection::__construct() */
PHP_METHOD(DrawTextAttributeFeaturesCollection, __construct) 
{
	php_ui_attribute_features_t *features = php_ui_attribute_features_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	features->f = uiNewOpenTypeFeatures();

	if (!features->f) {
		/* throw */
	}
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_attributes_features_collection_add_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, four_char_code, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO(0, value, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Text\Attributes\Features\Collection UI\Draw\Text\Attributes\Features\Collection::add(string four_char_code, int value) */
PHP_METHOD(DrawTextAttributeFeaturesCollection, add) 
{
	php_ui_attribute_features_t *features = php_ui_attribute_features_fetch(getThis());
	zend_string *four_char_code;
	zend_long value; // could be uint32_t
	char *fcc;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "Sl", &four_char_code, &value) != SUCCESS) {
		return;
	}

	if (ZSTR_LEN(four_char_code) != 4) {
		php_ui_exception_ex(InvalidArgumentException,
			"length of argument #1 (four_char_code) must be 4, got length of %zu", ZSTR_LEN(four_char_code));
		return;
	}
	fcc = ZSTR_VAL(four_char_code);

	if (value < 0 || value > 4294967296) {
		php_ui_exception_ex(InvalidArgumentException,
			"argument #2 (value) should be between 0 and 4294967296 (unsigned 32-bit integer), got %lld", value);
		return;
	}

	uiOpenTypeFeaturesAdd(features->f, fcc[0], fcc[1], fcc[2], fcc[3], value);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_attributes_features_collection_remove_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, four_char_code, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto UI\Draw\Text\Attributes\Features\Collection UI\Draw\Text\Attributes\Features\Collection::remove(string four_char_code) */
PHP_METHOD(DrawTextAttributeFeaturesCollection, remove) 
{
	php_ui_attribute_features_t *features = php_ui_attribute_features_fetch(getThis());
	zend_string *four_char_code;
	char *fcc;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &four_char_code) != SUCCESS) {
		return;
	}

	if (ZSTR_LEN(four_char_code) != 4) {
		php_ui_exception_ex(InvalidArgumentException,
			"length of argument #1 (four_char_code) must be 4, got length of %zu", ZSTR_LEN(four_char_code));
		return;
	}
	fcc = ZSTR_VAL(four_char_code);

	uiOpenTypeFeaturesRemove(features->f, fcc[0], fcc[1], fcc[2], fcc[3]);
} /* }}} */

ZEND_BEGIN_ARG_INFO_EX(php_ui_attributes_features_collection_get_info, 0, 0, 1)
	ZEND_ARG_TYPE_INFO(0, four_char_code, IS_STRING, 0)
ZEND_END_ARG_INFO()

/* {{{ proto null|int UI\Draw\Text\Attributes\Features\Collection::get(string four_char_code) */
PHP_METHOD(DrawTextAttributeFeaturesCollection, get) 
{
	php_ui_attribute_features_t *features = php_ui_attribute_features_fetch(getThis());
	zend_string *four_char_code;
	char *fcc;
	uint32_t value;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "S", &four_char_code) != SUCCESS) {
		return;
	}

	if (ZSTR_LEN(four_char_code) != 4) {
		php_ui_exception_ex(InvalidArgumentException,
			"length of argument #1 (four_char_code) must be 4, got length of %zu", ZSTR_LEN(four_char_code));
		return;
	}
	fcc = ZSTR_VAL(four_char_code);

	if (uiOpenTypeFeaturesGet(features->f, fcc[0], fcc[1], fcc[2], fcc[3], (uint32_t *) &value)) {
		RETURN_LONG(value);
	} else {
		RETURN_NULL();
	}
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_attributes_features_collection_methods[] = {
	PHP_ME(DrawTextAttributeFeaturesCollection, __construct, php_ui_attributes_features_collection_construct_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawTextAttributeFeaturesCollection, add,         php_ui_attributes_features_collection_add_info,       ZEND_ACC_PUBLIC)
	PHP_ME(DrawTextAttributeFeaturesCollection, remove,      php_ui_attributes_features_collection_remove_info,    ZEND_ACC_PUBLIC)
	PHP_ME(DrawTextAttributeFeaturesCollection, get,         php_ui_attributes_features_collection_get_info,       ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */


//PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_attribute_get_type_info, 0, 0, IS_LONG, 0)
//ZEND_END_ARG_INFO()
//
///* {{{ proto int DrawTextAttribute::getType(void) */
//PHP_METHOD(DrawTextAttribute, getType)
//{
//	php_ui_attribute_t *attribute = php_ui_attribute_fetch(getThis());
//
//	if (zend_parse_parameters_none() != SUCCESS) {
//		return;
//	}
//
//	RETURN_LONG(attribute->a->type); // Incomplete definition of type 'struct uiAttribute' - clang (typecheck_incomplete_tag) \ ui.h(554, 16): Forward declaration of 'struct uiAttribute'
//} /* }}} */

/* {{{ */
const zend_function_entry php_ui_attribute_methods[] = {
//	PHP_ME(DrawTextAttribute, getType, php_ui_attribute_get_type_info, ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_DrawTextAttribute) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Text", "Attribute", php_ui_attribute_methods);

	uiDrawTextAttribute_ce = zend_register_internal_class(&ce);
	uiDrawTextAttribute_ce->create_object = php_ui_attribute_create;
	uiDrawTextAttribute_ce->ce_flags |= ZEND_ACC_ABSTRACT;

	memcpy(&php_ui_attribute_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_ui_attribute_handlers.offset = XtOffsetOf(php_ui_attribute_t, std);

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Text\\Attributes", "Family", php_ui_attributes_family_methods);
	uiDrawTextAttributesFamily_ce = zend_register_internal_class_ex(&ce, uiDrawTextAttribute_ce);
	
	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Text\\Attributes", "Size", php_ui_attributes_size_methods);
	uiDrawTextAttributesSize_ce = zend_register_internal_class_ex(&ce, uiDrawTextAttribute_ce);

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Text\\Attributes", "Weight", php_ui_attributes_weight_methods);
	uiDrawTextAttributesWeight_ce = zend_register_internal_class_ex(&ce, uiDrawTextAttribute_ce);
	zend_declare_class_constant_long(uiDrawTextAttributesWeight_ce,         ZEND_STRL("MINIMUM"),          uiTextWeightMinimum);
	zend_declare_class_constant_long(uiDrawTextAttributesWeight_ce,         ZEND_STRL("THIN"),             uiTextWeightThin);
	zend_declare_class_constant_long(uiDrawTextAttributesWeight_ce,         ZEND_STRL("ULTRA_LIGHT"),      uiTextWeightUltraLight);
	zend_declare_class_constant_long(uiDrawTextAttributesWeight_ce,         ZEND_STRL("LIGHT"),            uiTextWeightLight);
	zend_declare_class_constant_long(uiDrawTextAttributesWeight_ce,         ZEND_STRL("BOOK"),             uiTextWeightBook);
	zend_declare_class_constant_long(uiDrawTextAttributesWeight_ce,         ZEND_STRL("NORMAL"),           uiTextWeightNormal);
	zend_declare_class_constant_long(uiDrawTextAttributesWeight_ce,         ZEND_STRL("MEDIUM"),           uiTextWeightMedium);
	zend_declare_class_constant_long(uiDrawTextAttributesWeight_ce,         ZEND_STRL("SEMI_BOLD"),        uiTextWeightSemiBold);
	zend_declare_class_constant_long(uiDrawTextAttributesWeight_ce,         ZEND_STRL("BOLD"),             uiTextWeightBold);
	zend_declare_class_constant_long(uiDrawTextAttributesWeight_ce,         ZEND_STRL("ULTRA_BOLD"),       uiTextWeightUltraBold);
	zend_declare_class_constant_long(uiDrawTextAttributesWeight_ce,         ZEND_STRL("HEAVY"),            uiTextWeightHeavy);
	zend_declare_class_constant_long(uiDrawTextAttributesWeight_ce,         ZEND_STRL("ULTRA_HEAVY"),      uiTextWeightUltraHeavy);
	zend_declare_class_constant_long(uiDrawTextAttributesWeight_ce,         ZEND_STRL("MAXIMUM"),          uiTextWeightMaximum);

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Text\\Attributes", "Italic", php_ui_attributes_italic_methods);
	uiDrawTextAttributesItalic_ce = zend_register_internal_class_ex(&ce, uiDrawTextAttribute_ce);
	zend_declare_class_constant_long(uiDrawTextAttributesItalic_ce,         ZEND_STRL("NORMAL"),           uiTextItalicNormal);
	zend_declare_class_constant_long(uiDrawTextAttributesItalic_ce,         ZEND_STRL("OBLIQUE"),          uiTextItalicOblique);
	zend_declare_class_constant_long(uiDrawTextAttributesItalic_ce,         ZEND_STRL("ITALIC"),           uiTextItalicItalic);

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Text\\Attributes", "Stretch", php_ui_attributes_stretch_methods);
	uiDrawTextAttributesStretch_ce = zend_register_internal_class_ex(&ce, uiDrawTextAttribute_ce);
	zend_declare_class_constant_long(uiDrawTextAttributesStretch_ce,        ZEND_STRL("ULTRA_CONDENSED"),  uiTextStretchUltraCondensed);
	zend_declare_class_constant_long(uiDrawTextAttributesStretch_ce,        ZEND_STRL("EXTRA_CONDENSED"),  uiTextStretchExtraCondensed);
	zend_declare_class_constant_long(uiDrawTextAttributesStretch_ce,        ZEND_STRL("CONDENSED"),        uiTextStretchCondensed);
	zend_declare_class_constant_long(uiDrawTextAttributesStretch_ce,        ZEND_STRL("SEMI_CONDENSED"),   uiTextStretchSemiCondensed);
	zend_declare_class_constant_long(uiDrawTextAttributesStretch_ce,        ZEND_STRL("NORMAL"),           uiTextStretchNormal);
	zend_declare_class_constant_long(uiDrawTextAttributesStretch_ce,        ZEND_STRL("SEMI_EXPANDED"),    uiTextStretchSemiExpanded);
	zend_declare_class_constant_long(uiDrawTextAttributesStretch_ce,        ZEND_STRL("EXPANDED"),         uiTextStretchExpanded);
	zend_declare_class_constant_long(uiDrawTextAttributesStretch_ce,        ZEND_STRL("EXTRA_EXPANDED"),   uiTextStretchExtraExpanded);
	zend_declare_class_constant_long(uiDrawTextAttributesStretch_ce,        ZEND_STRL("ULTRA_EXPANDED"),   uiTextStretchUltraExpanded);

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Text\\Attributes", "Color", php_ui_attributes_color_methods);
	uiDrawTextAttributesColor_ce = zend_register_internal_class_ex(&ce, uiDrawTextAttribute_ce);

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Text\\Attributes", "Background", php_ui_attributes_background_methods);
	uiDrawTextAttributesBackground_ce = zend_register_internal_class_ex(&ce, uiDrawTextAttribute_ce);

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Text\\Attributes", "Underline", php_ui_attributes_underline_methods);
	uiDrawTextAttributesUnderline_ce = zend_register_internal_class_ex(&ce, uiDrawTextAttribute_ce);
	zend_declare_class_constant_long(uiDrawTextAttributesUnderline_ce,      ZEND_STRL("NONE"),            uiUnderlineNone);
	zend_declare_class_constant_long(uiDrawTextAttributesUnderline_ce,      ZEND_STRL("SINGLE"),          uiUnderlineSingle);
	zend_declare_class_constant_long(uiDrawTextAttributesUnderline_ce,      ZEND_STRL("DOUBLE"),          uiUnderlineDouble);
	zend_declare_class_constant_long(uiDrawTextAttributesUnderline_ce,      ZEND_STRL("SUGGESTION"),      uiUnderlineSuggestion); // "wavy or dotted underlines used for spelling/grammar checkers" - ui.h

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Text\\Attributes", "UnderlineColor", php_ui_attributes_underline_color_methods);
	uiDrawTextAttributesUnderlineColor_ce = zend_register_internal_class_ex(&ce, uiDrawTextAttribute_ce);
	zend_declare_class_constant_long(uiDrawTextAttributesUnderlineColor_ce, ZEND_STRL("CUSTOM"),          uiUnderlineColorCustom);
	zend_declare_class_constant_long(uiDrawTextAttributesUnderlineColor_ce, ZEND_STRL("SPELLING"),        uiUnderlineColorSpelling);
	zend_declare_class_constant_long(uiDrawTextAttributesUnderlineColor_ce, ZEND_STRL("GRAMMAR"),         uiUnderlineColorGrammar);
	zend_declare_class_constant_long(uiDrawTextAttributesUnderlineColor_ce, ZEND_STRL("AUXILIARY"),       uiUnderlineColorAuxiliary); // "// for instance, the color used by smart replacements on macOS or in Microsoft Office" - ui.h

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Text\\Attributes", "Features", php_ui_attributes_features_methods);
	uiDrawTextAttributesFeatures_ce = zend_register_internal_class_ex(&ce, uiDrawTextAttribute_ce);

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw\\Text\\Attributes\\Features", "Collection", php_ui_attributes_features_collection_methods);
	uiDrawTextAttributesFeaturesCollection_ce = zend_register_internal_class_ex(&ce, uiDrawTextAttribute_ce);

	memcpy(&php_ui_attribute_features_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_ui_attribute_features_handlers.offset = XtOffsetOf(php_ui_attribute_features_t, std);

	return SUCCESS;
} /* }}} */

#endif
