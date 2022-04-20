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
#ifndef HAVE_PHP_UI_MATRIX
#define HAVE_PHP_UI_MATRIX
#include <ui.h>

#include "php.h"

#include <classes/_macro.h>

#include <classes/point.h>
#include <classes/size.h>
#include <classes/matrix.h>

zend_object_handlers php_ui_matrix_handlers;

zend_class_entry *uiDrawMatrix_ce;

zend_object* php_ui_matrix_create(zend_class_entry *ce) {
	php_ui_matrix_t *matrix = 
		(php_ui_matrix_t*) ecalloc(1, sizeof(php_ui_matrix_t) + zend_object_properties_size(ce));

	zend_object_std_init(&matrix->std, ce);

	object_properties_init(&matrix->std, ce);

	matrix->std.handlers = &php_ui_matrix_handlers;

	uiDrawMatrixSetIdentity(&matrix->m);

	return &matrix->std;
}

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_OBJECT_INFO_EX(php_ui_matrix_translate_info, 0, 1, UI\\Draw\\Matrix, 0)
	ZEND_ARG_OBJ_INFO(0, point, UI\\Point, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Matrix UI\Draw\Matrix::translate(Point point) */
PHP_METHOD(DrawMatrix, translate)
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());
	zval *point = NULL;
	php_ui_point_t *p;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &point, uiPoint_ce) != SUCCESS) {
		return;
	}

	p = php_ui_point_fetch(point);

	uiDrawMatrixTranslate(&matrix->m, p->x, p->y);

	RETURN_ZVAL(getThis(), 1, 0)
} /* }}} */

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_OBJECT_INFO_EX(php_ui_matrix_scale_info, 0, 2, UI\\Draw\\Matrix, 0)
	ZEND_ARG_OBJ_INFO(0, center, UI\\Point, 0)
	ZEND_ARG_OBJ_INFO(0, point, UI\\Point, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Matrix UI\Draw\Matrix::scale(Point center, Point point) */
PHP_METHOD(DrawMatrix, scale)
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());
	zval *center = NULL, *point = NULL;
	php_ui_point_t *p, *c;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "OO", &center, uiPoint_ce, &point, uiPoint_ce) != SUCCESS) {
		return;
	}

	p = php_ui_point_fetch(point);
	c = php_ui_point_fetch(center);

	uiDrawMatrixScale(&matrix->m, c->x, c->y, p->x, p->y);

	RETURN_ZVAL(getThis(), 1, 0)
} /* }}} */

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_OBJECT_INFO_EX(php_ui_matrix_rotate_info, 0, 2, UI\\Draw\\Matrix, 0)
	ZEND_ARG_OBJ_INFO(0, point, UI\\Point, 0)
	ZEND_ARG_TYPE_INFO(0, amount, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Matrix UI\Draw\Matrix::rotate(Point point, double radians) */
PHP_METHOD(DrawMatrix, rotate)
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());
	zval *point = NULL;
	php_ui_point_t *p;
	double radians = 0;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "Od", &point, uiPoint_ce, &radians) != SUCCESS) {
		return;
	}

	p = php_ui_point_fetch(point);

	uiDrawMatrixRotate(&matrix->m, p->x, p->y, radians);

	RETURN_ZVAL(getThis(), 1, 0)
} /* }}} */

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_OBJECT_INFO_EX(php_ui_matrix_skew_info, 0, 2, UI\\Draw\\Matrix, 0)
	ZEND_ARG_OBJ_INFO(0, point, UI\\Point, 0)
	ZEND_ARG_OBJ_INFO(0, amount, UI\\Point, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Matrix UI\Draw\Matrix::skew(Point point, Point amount) */
PHP_METHOD(DrawMatrix, skew)
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());
	zval *point = NULL, *amount = NULL;
	php_ui_point_t *p, *a;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "OO", &point, uiPoint_ce, &amount, uiPoint_ce) != SUCCESS) {
		return;
	}

	p = php_ui_point_fetch(point);
	a = php_ui_point_fetch(amount);

	uiDrawMatrixSkew(&matrix->m, p->x, p->y, a->x, a->y);

	RETURN_ZVAL(getThis(), 1, 0)
} /* }}} */

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(php_ui_matrix_multiply_info, 0, 1, UI\\Draw\\Matrix, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_matrix_multiply_info, 0, 1, IS_OBJECT, "UI\\Draw\\Matrix", 0)
#endif
	ZEND_ARG_OBJ_INFO(0, matrix, UI\\Draw\\Matrix, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Matrix UI\Draw\Matrix::multiply(Matrix multiply) */
PHP_METHOD(DrawMatrix, multiply)
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());
	php_ui_matrix_t *dest;

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	object_init_ex(return_value, uiDrawMatrix_ce);

	dest = php_ui_matrix_fetch(return_value);

	uiDrawMatrixMultiply(&dest->m, &matrix->m);
} /* }}} */

#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_matrix_is_invertible_info, 0, 0, _IS_BOOL, 1)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_matrix_is_invertible_info, 0, 0, _IS_BOOL, NULL, 1)
#endif
ZEND_END_ARG_INFO()

/* {{{ proto bool UI\Draw\Matrix::isInvertible(void) */
PHP_METHOD(DrawMatrix, isInvertible)
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	if (uiDrawMatrixInvertible(&matrix->m)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
} /* }}} */

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_OBJECT_INFO_EX(php_ui_matrix_invert_info, 0, 0, UI\\Draw\\Matrix, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Matrix UI\Draw\Matrix::invert(void) */
PHP_METHOD(DrawMatrix, invert)
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());
	
	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiDrawMatrixInvert(&matrix->m);

	RETURN_ZVAL(getThis(), 1, 0)
} /* }}} */

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_OBJECT_INFO_EX(php_ui_matrix_transform_point_info, 0, 1, UI\\Point, 0)
	ZEND_ARG_OBJ_INFO(0, point, UI\\Point, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Point UI\Draw\Matrix::transformPoint(Point point) */
PHP_METHOD(DrawMatrix, transformPoint)
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());
	zval *point = NULL;
	php_ui_point_t *p, *r;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &point, uiPoint_ce) != SUCCESS) {
		return;
	}

	p = php_ui_point_fetch(point);

	object_init_ex(return_value, uiPoint_ce);

	r = php_ui_point_fetch(return_value);
	r->x = p->x;
	r->y = p->y;

	uiDrawMatrixTransformPoint(&matrix->m, &r->x, &r->y);

	return;
} /* }}} */

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_OBJECT_INFO_EX(php_ui_matrix_transform_size_info, 0, 1, UI\\Point, 0)
	ZEND_ARG_OBJ_INFO(0, point, UI\\Size, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Size UI\Draw\Matrix::translate(Size size) */
PHP_METHOD(DrawMatrix, transformSize)
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());
	zval *size = NULL;
	php_ui_size_t *s, *r;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &size, uiSize_ce) != SUCCESS) {
		return;
	}

	s = php_ui_size_fetch(size);

	object_init_ex(return_value, uiSize_ce);

	r = php_ui_size_fetch(return_value);
	r->width = s->width;
	r->height = s->height;

	uiDrawMatrixTransformSize(&matrix->m, &r->width, &r->height);

	return;
} /* }}} */

void php_ui_matrix_points(zval *zv, php_ui_matrix_t *matrix) {
	zval point1, point2, point3;

	array_init(zv);

	php_ui_point_construct(&point1,
		matrix->m.M11,
		matrix->m.M12);
	add_index_zval(zv, 0, &point1);

	php_ui_point_construct(&point2,
		matrix->m.M21,
		matrix->m.M22);
	add_index_zval(zv, 1, &point2);

	php_ui_point_construct(&point3,
		matrix->m.M31,
		matrix->m.M32);
	add_index_zval(zv, 2, &point3);
}

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_matrix_get_points_info, 0, 0, IS_ARRAY, 0)
ZEND_END_ARG_INFO()

/* {{{ proto array Matrix::getPoints(void) */
PHP_METHOD(DrawMatrix, getPoints)
{
	php_ui_matrix_t *matrix = php_ui_matrix_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	php_ui_matrix_points(return_value, matrix);
} /* }}} */

/* {{{ */
const zend_function_entry php_ui_matrix_methods[] = {
	PHP_ME(DrawMatrix, translate,       php_ui_matrix_translate_info,       ZEND_ACC_PUBLIC)
	PHP_ME(DrawMatrix, scale,           php_ui_matrix_scale_info,           ZEND_ACC_PUBLIC)
	PHP_ME(DrawMatrix, rotate,          php_ui_matrix_rotate_info,          ZEND_ACC_PUBLIC)
	PHP_ME(DrawMatrix, skew,            php_ui_matrix_skew_info,            ZEND_ACC_PUBLIC)
	PHP_ME(DrawMatrix, multiply,        php_ui_matrix_multiply_info,        ZEND_ACC_PUBLIC)
	PHP_ME(DrawMatrix, isInvertible,    php_ui_matrix_is_invertible_info,   ZEND_ACC_PUBLIC)
	PHP_ME(DrawMatrix, invert,          php_ui_matrix_invert_info,          ZEND_ACC_PUBLIC)
	PHP_ME(DrawMatrix, transformPoint,  php_ui_matrix_transform_point_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawMatrix, transformSize,   php_ui_matrix_transform_size_info,  ZEND_ACC_PUBLIC)
	PHP_ME(DrawMatrix, getPoints,       php_ui_matrix_get_points_info,      ZEND_ACC_PUBLIC)
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_DrawMatrix) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw", "Matrix", php_ui_matrix_methods);

	uiDrawMatrix_ce = zend_register_internal_class(&ce);
	uiDrawMatrix_ce->create_object = php_ui_matrix_create;

	memcpy(&php_ui_matrix_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));
	
	php_ui_matrix_handlers.offset = XtOffsetOf(php_ui_matrix_t, std);

	return SUCCESS;
} /* }}} */
#endif
