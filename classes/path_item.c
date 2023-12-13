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
#ifndef HAVE_PHP_UI_PATH_ITEM
#define HAVE_PHP_UI_PATH_ITEM
#include <ui.h>

#include "php.h"
#include "zend_API.h"
#include "zend_string.h"

#include <classes/_macro.h>

#include <classes/exceptions.h>
#include <classes/point.h>

#include <classes/path_item.h>

zend_object_handlers php_ui_path_item_handlers;

zend_class_entry *uiDrawPathItem_ce;

void php_ui_path_item_points(zval *zv, php_ui_path_item_t *path_item) {
	zval point1, point2, point3;

	array_init(zv);

	switch (path_item->i->Type) {
		case uiDrawPathItemTypeNewFigure:
		case uiDrawPathItemTypeLineTo: {
			php_ui_point_construct(&point1,
				path_item->i->X1,
				path_item->i->Y1);
			add_index_zval(zv, 0, &point1);
			break;
		}
		case uiDrawPathItemTypeArc:
		case uiDrawPathItemTypeNewFigureArc: {
			php_ui_point_construct(&point1,
				path_item->i->X1,
				path_item->i->Y1);
			add_index_zval(zv, 0, &point1);

			break;
		}
		case uiDrawPathItemTypeQuadraticCurveTo: {
			php_ui_point_construct(&point1,
				path_item->i->X1,
				path_item->i->Y1);
			add_index_zval(zv, 0, &point1);

			php_ui_point_construct(&point2,
				path_item->i->X2,
				path_item->i->Y2);
			add_index_zval(zv, 1, &point2);

			break;
		}
		case uiDrawPathItemTypeBezierTo: {
			php_ui_point_construct(&point1,
				path_item->i->X1,
				path_item->i->Y1);
			add_index_zval(zv, 0, &point1);

			php_ui_point_construct(&point2,
				path_item->i->X2,
				path_item->i->Y2);
			add_index_zval(zv, 1, &point2);

			php_ui_point_construct(&point3,
				path_item->i->X3,
				path_item->i->Y3);
			add_index_zval(zv, 2, &point3);

			break;
		}
	}
}

zval *php_ui_path_item_construct(zval *path_item, uiDrawPathItem *pathItem)
{
	php_ui_path_item_t *i;

	object_init_ex(path_item, uiDrawPathItem_ce);

	i = php_ui_path_item_fetch(path_item);

	i->i = pathItem;

	return path_item;
}

zend_object* php_ui_path_item_create(zend_class_entry *ce) {
	php_ui_path_item_t *path_item =
		(php_ui_path_item_t*) ecalloc(1, sizeof(php_ui_path_item_t) + zend_object_properties_size(ce));

	zend_object_std_init(&path_item->std, ce);

	object_properties_init(&path_item->std, ce);

	path_item->std.handlers = &php_ui_path_item_handlers;

	return &path_item->std;
}

/* {{{ */
static zval* php_ui_path_item_read(zval *object, zval *member, int type, void **cache, zval *rv) {
	php_ui_path_item_t *path_item = php_ui_path_item_fetch(object);

	if (Z_TYPE_P(member) != IS_STRING) {
		return &EG(uninitialized_zval);
	}

	if (type == BP_VAR_RW || type == BP_VAR_W) {
		php_ui_exception("Failed to fetch reference to %s, not allowed", Z_STRVAL_P(member));
		return &EG(uninitialized_zval);
	}

	if (!path_item->i) { // eg $ php74 -d extension=modules/ui.so -r 'var_dump((new ReflectionClass("UI\\Draw\\PathItem"))->newInstanceWithoutConstructor()->type);'
		return &EG(uninitialized_zval);
	}

	if (zend_string_equals_literal_ci(Z_STR_P(member), "type")) {
		ZVAL_LONG(rv, path_item->i->Type);
		return rv;
	}

	if (zend_string_equals_literal_ci(Z_STR_P(member), "points")) {
		php_ui_path_item_points(rv, path_item);
		return rv;
	}

	if (zend_string_equals_literal_ci(Z_STR_P(member), "point1")) {
		php_ui_point_construct(rv,
			path_item->i->X1,
			path_item->i->Y1);
		return rv;
	}

	if (zend_string_equals_literal_ci(Z_STR_P(member), "point2")) {
		php_ui_point_construct(rv,
			path_item->i->X2,
			path_item->i->Y2);
		return rv;
	}

	if (zend_string_equals_literal_ci(Z_STR_P(member), "point3")) {
		php_ui_point_construct(rv,
			path_item->i->X3,
			path_item->i->Y3);
		return rv;
	}

	if (zend_string_equals_literal_ci(Z_STR_P(member), "radius")) {
		ZVAL_LONG(rv, path_item->i->Radius);
		return rv;
	}
	if (zend_string_equals_literal_ci(Z_STR_P(member), "start")) {
		ZVAL_LONG(rv, path_item->i->Start);
		return rv;
	}
	if (zend_string_equals_literal_ci(Z_STR_P(member), "sweep")) {
		ZVAL_LONG(rv, path_item->i->Sweep);
		return rv;
	}

	php_ui_exception("Failed to fetch %s, does not exist", Z_STRVAL_P(member));
	return &EG(uninitialized_zval);
} /* }}} */

/* {{{ */
zval* php_ui_path_item_noref(zval *object, zval *member, int type, void **cache) {
	return NULL;
} /* }}} */

/* {{{ */
HashTable* php_ui_path_item_debug(zval *object, int *is_temp) {
	php_ui_path_item_t *path_item = php_ui_path_item_fetch(object);
	zval points;
	zval debug;

	array_init(&debug);

	php_ui_path_item_points(&points, path_item);

	add_assoc_long(&debug, "type", path_item->i->Type);
	add_assoc_zval(&debug, "points", &points);

	*is_temp = 1;

	return Z_ARRVAL(debug);
} /* }}} */

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_path_item_get_points_info, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

/* {{{ proto array PathItem::getPoints(void) */
PHP_METHOD(PathItem, getPoints)
{
	php_ui_path_item_t *path_item = php_ui_path_item_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	php_ui_path_item_points(return_value, path_item);
} /* }}} */

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_path_item_get_type_info, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto int PathItem::getType(void) */
PHP_METHOD(PathItem, getType)
{
	php_ui_path_item_t *path_item = php_ui_path_item_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_LONG(path_item->i->Type);
} /* }}} */

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_OBJECT_INFO_EX(php_ui_path_item_get_point1_info, 0, 0, UI\\Point, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Point PathItem::getPoint1(void) */
PHP_METHOD(PathItem, getPoint1)
{
	php_ui_path_item_t *path_item = php_ui_path_item_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	php_ui_point_construct(return_value,
		path_item->i->X1,
		path_item->i->Y1);

} /* }}} */

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_OBJECT_INFO_EX(php_ui_path_item_get_point2_info, 0, 0, UI\\Point, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Point PathItem::getPoint2(void) */
PHP_METHOD(PathItem, getPoint2)
{
	php_ui_path_item_t *path_item = php_ui_path_item_fetch(getThis());
	zval point;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	object_init_ex(return_value, uiPoint_ce);

	php_ui_point_construct(return_value,
		path_item->i->X2,
		path_item->i->Y2);
} /* }}} */

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_OBJECT_INFO_EX(php_ui_path_item_get_point3_info, 0, 0, UI\\Point, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Point PathItem::getPoint3(void) */
PHP_METHOD(PathItem, getPoint3)
{
	php_ui_path_item_t *path_item = php_ui_path_item_fetch(getThis());
	zval point;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	object_init_ex(return_value, uiPoint_ce);

	php_ui_point_construct(return_value,
		path_item->i->X3,
		path_item->i->Y3);
} /* }}} */

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_path_item_get_radius_info, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto float PathItem::getRadius(void) */
PHP_METHOD(PathItem, getRadius)
{
	php_ui_path_item_t *path_item = php_ui_path_item_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_DOUBLE(path_item->i->Radius);
} /* }}} */

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_path_item_get_start_info, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto float PathItem::getStart(void) */
PHP_METHOD(PathItem, getStart)
{
	php_ui_path_item_t *path_item = php_ui_path_item_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_DOUBLE(path_item->i->Start);
} /* }}} */

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_path_item_get_sweep_info, 0, 0, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto float PathItem::getSweep(void) */
PHP_METHOD(PathItem, getSweep)
{
	php_ui_path_item_t *path_item = php_ui_path_item_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	RETURN_DOUBLE(path_item->i->Sweep);
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_path_item_methods[] = {
	PHP_ME(PathItem, getType,      php_ui_path_item_get_type_info,    ZEND_ACC_PUBLIC)
	PHP_ME(PathItem, getPoints,    php_ui_path_item_get_points_info,  ZEND_ACC_PUBLIC)
	PHP_ME(PathItem, getPoint1,    php_ui_path_item_get_point1_info,  ZEND_ACC_PUBLIC)
	PHP_ME(PathItem, getPoint2,    php_ui_path_item_get_point2_info,  ZEND_ACC_PUBLIC)
	PHP_ME(PathItem, getPoint3,    php_ui_path_item_get_point3_info,  ZEND_ACC_PUBLIC)
	PHP_ME(PathItem, getRadius,    php_ui_path_item_get_radius_info,  ZEND_ACC_PUBLIC)
	PHP_ME(PathItem, getStart,     php_ui_path_item_get_start_info,   ZEND_ACC_PUBLIC)
	PHP_ME(PathItem, getSweep,     php_ui_path_item_get_sweep_info,   ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_DrawPathItem)
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw", "PathItem", php_ui_path_item_methods);

	uiDrawPathItem_ce = zend_register_internal_class(&ce);
	uiDrawPathItem_ce->create_object = php_ui_path_item_create;
//	uiDrawPathItem_ce->ce_flags |= ZEND_ACC_FINAL;

//	zend_declare_property_null(uiDrawPathItem_ce, ZEND_STRL("type"), 0, ZEND_ACC_PUBLIC);
//	zend_string *str = zend_string_init(ZEND_STRL("points"), 1);
//	zend_declare_typed_property(uiDrawPathItem_ce, str, NULL,           ZEND_ACC_PUBLIC, NULL, IS_ARRAY);
//	zend_string_release(str);
	zend_declare_property_null(uiDrawPathItem_ce, ZEND_STRL("type"),    ZEND_ACC_PUBLIC);
	zend_declare_property_null(uiDrawPathItem_ce, ZEND_STRL("points"),  ZEND_ACC_PUBLIC);
//	zend_declare_property_null(uiDrawPathItem_ce, ZEND_STRL("point1"),  ZEND_ACC_PUBLIC);
//	zend_declare_property_null(uiDrawPathItem_ce, ZEND_STRL("point2"),  ZEND_ACC_PUBLIC);
//	zend_declare_property_null(uiDrawPathItem_ce, ZEND_STRL("point3"),  ZEND_ACC_PUBLIC);

	memcpy(&php_ui_path_item_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_ui_path_item_handlers.offset = XtOffsetOf(php_ui_path_item_t, std);
//	php_ui_path_item_handlers.clone_obj = php_ui_path_item_clone;
//	php_ui_path_item_handlers.compare_objects = php_ui_path_item_compare;
	php_ui_path_item_handlers.read_property = php_ui_path_item_read;
	php_ui_path_item_handlers.get_property_ptr_ptr = php_ui_path_item_noref;
//	php_ui_path_item_handlers.write_property = php_ui_path_item_write;
	php_ui_path_item_handlers.get_debug_info = php_ui_path_item_debug;

	return SUCCESS;
} /* }}} */
#endif
