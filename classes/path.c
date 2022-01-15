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
#ifndef HAVE_PHP_UI_PATH
#define HAVE_PHP_UI_PATH
#include <ui.h>

#include "php.h"

#include <classes/_macro.h>

#include <classes/point.h>
#include <classes/size.h>
#include <classes/path.h>
#include <classes/matrix.h>

zend_object_handlers php_ui_path_handlers;

zend_class_entry *uiDrawPath_ce;

zend_object* php_ui_path_create(zend_class_entry *ce) {
	php_ui_path_t *path = 
		(php_ui_path_t*) ecalloc(1, sizeof(php_ui_path_t) + zend_object_properties_size(ce));

	zend_object_std_init(&path->std, ce);

	object_properties_init(&path->std, ce);

	path->std.handlers = &php_ui_path_handlers;

	return &path->std;
}

void php_ui_path_free(zend_object *o) {
	php_ui_path_t *path = php_ui_path_from(o);

	if (path->p) {
		uiDrawFreePath(path->p);
	}

	zend_object_std_dtor(o);
}

ZEND_BEGIN_ARG_INFO_EX(php_ui_path_construct_info, 0, 0, 0)
	ZEND_ARG_TYPE_INFO(0, mode, IS_LONG, 0)
ZEND_END_ARG_INFO()

/* {{{ proto DrawPath UI\Draw\Path::__construct([int mode = UI\Draw\Path::Winding]) */
PHP_METHOD(DrawPath, __construct) 
{
	php_ui_path_t *path = php_ui_path_fetch(getThis());
	zend_long mode = uiDrawFillModeWinding;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "|l", &mode) != SUCCESS) {
		return;
	}

	path->p = uiDrawNewPath(mode);
} /* }}} */

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_OBJECT_INFO_EX(php_ui_path_new_figure_info, 0, 1, UI\\Draw\\Path, 0)
	ZEND_ARG_OBJ_INFO(0, point, UI\\Point, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Path UI\Draw\Path::newFigure(Point point) */
PHP_METHOD(DrawPath, newFigure) 
{
	php_ui_path_t *path = php_ui_path_fetch(getThis());
	zval *point = NULL;
	php_ui_point_t *p;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &point, uiPoint_ce) != SUCCESS) {
		return;
	}

	p = php_ui_point_fetch(point);

	uiDrawPathNewFigure(path->p, p->x, p->y);

	RETURN_ZVAL(getThis(), 1, 0)
} /* }}} */

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_OBJECT_INFO_EX(php_ui_path_draw_to_info, 0, 5, UI\\Draw\\Path, 0)
	ZEND_ARG_OBJ_INFO(0, point, UI\\Point, 0)
	ZEND_ARG_TYPE_INFO(0, radius, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, angle, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, sweep, IS_DOUBLE, 0)
	ZEND_ARG_TYPE_INFO(0, negative, IS_DOUBLE, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Path UI\Draw\Path::newFigureWithArc(Point point, double radius, double angle, double sweep, double negative) */
PHP_METHOD(DrawPath, newFigureWithArc) 
{
	php_ui_path_t *path = php_ui_path_fetch(getThis());
	zval *point = NULL;
	php_ui_point_t *p;
	double radius = 0, angle = 0, sweep = 0, negative = 0;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "Odddd", &point, uiPoint_ce, &radius, &angle, &sweep, &negative) != SUCCESS) {
		return;
	}

	p = php_ui_point_fetch(point);

	uiDrawPathNewFigureWithArc(path->p, p->x, p->y, radius, angle, sweep, negative);

	RETURN_ZVAL(getThis(), 1, 0)
} /* }}} */

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_OBJECT_INFO_EX(php_ui_path_line_to_info, 0, 1, UI\\Draw\\Path, 0)
	ZEND_ARG_OBJ_INFO(0, point, UI\\Point, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Path UI\Draw\Path::lineTo(Point point) */
PHP_METHOD(DrawPath, lineTo) 
{
	php_ui_path_t *path = php_ui_path_fetch(getThis());
	zval *point = NULL;
	php_ui_point_t *p;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &point, uiPoint_ce) != SUCCESS) {
		return;
	}

	p = php_ui_point_fetch(point);

	uiDrawPathLineTo(path->p, p->x, p->y);

	RETURN_ZVAL(getThis(), 1, 0)
} /* }}} */

/* {{{ proto Path UI\Draw\Path::arcTo(Point point, double radius, double angle, double sweep, double negative) */
PHP_METHOD(DrawPath, arcTo) 
{
	php_ui_path_t *path = php_ui_path_fetch(getThis());
	zval *point = NULL;
	php_ui_point_t *p;
	double radius = 0, angle = 0, sweep = 0, negative = 0;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "Odddd", &point, uiPoint_ce, &radius, &angle, &sweep, &negative) != SUCCESS) {
		return;
	}

	p = php_ui_point_fetch(point);

	uiDrawPathArcTo(path->p, p->x, p->y, radius, angle, sweep, negative);

	RETURN_ZVAL(getThis(), 1, 0)
} /* }}} */

/* {{{ proto Path UI\Draw\Path::bezierTo(Point point, double radius, double angle, double sweep, double negative) */
PHP_METHOD(DrawPath, bezierTo)
{
	php_ui_path_t *path = php_ui_path_fetch(getThis());
	zval *point = NULL;
	php_ui_point_t *p;
	double radius = 0, angle = 0, sweep = 0, negative = 0;
	
	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "Odddd", &point, uiPoint_ce, &radius, &angle, &sweep, &negative) != SUCCESS) {
		return;
	}

	p = php_ui_point_fetch(point);

	uiDrawPathBezierTo(path->p, p->x, p->y, radius, angle, sweep, negative);

	RETURN_ZVAL(getThis(), 1, 0)
} /* }}} */

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_OBJECT_INFO_EX(php_ui_path_close_figure_info, 0, 0, UI\\Draw\\Path, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Path UI\Draw\Path::closeFigure(void) */
PHP_METHOD(DrawPath, closeFigure)
{
	php_ui_path_t *path = php_ui_path_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiDrawPathCloseFigure(path->p);

	RETURN_ZVAL(getThis(), 1, 0)
} /* }}} */

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_OBJECT_INFO_EX(php_ui_path_add_rectangle_info, 0, 2, UI\\Draw\\Path, 0)
	ZEND_ARG_OBJ_INFO(0, point, UI\\Point, 0)
	ZEND_ARG_OBJ_INFO(0, size, UI\\Size, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Path UI\Draw\Path::addRectangle(Point point, Size size) */
PHP_METHOD(DrawPath, addRectangle)
{
	php_ui_path_t *path = php_ui_path_fetch(getThis());
	zval *point = NULL, *size = NULL;
	php_ui_point_t *p;
	php_ui_size_t *s;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "OO", &point, uiPoint_ce, &size, uiSize_ce) != SUCCESS) {
		return;
	}

	p = php_ui_point_fetch(point);
	s = php_ui_size_fetch(size);

	uiDrawPathAddRectangle(path->p, p->x, p->y, s->width, s->height);

	RETURN_ZVAL(getThis(), 1, 0)
} /* }}} */

PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_OBJECT_INFO_EX(php_ui_path_end_info, 0, 0, UI\\Draw\\Path, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Path UI\Draw\Path::end(void) */
PHP_METHOD(DrawPath, end)
{
	php_ui_path_t *path = php_ui_path_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	uiDrawPathEnd(path->p);

	RETURN_ZVAL(getThis(), 1, 0)
} /* }}} */

// Requires github.com/vike2000/libui
#ifdef LIBUI_HAS_DRAW_PATH_ENDED
#if PHP_VERSION_ID >= 70200
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_path_is_ended_info, 0, 0, _IS_BOOL, 0)
#else
ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(php_ui_path_is_ended_info, 0, 0, _IS_BOOL, NULL, 0)
#endif
ZEND_END_ARG_INFO()

/* {{{ proto bool UI\Draw\Path::isEnded(void) */
PHP_METHOD(DrawPath, isEnded)
{
	php_ui_path_t *path = php_ui_path_fetch(getThis());

	if (zend_parse_parameters_none() != SUCCESS) {
		return;
	}

	if (uiDrawPathEnded(path->p)) {
		RETURN_TRUE;
	} else {
		RETURN_FALSE;
	}
} /* }}} */
#endif

#ifdef LIBUI_HAS_DRAW_PATH_COPY_BY_TRANSFORM
PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_OBJECT_INFO_EX(php_ui_path_transform_info, 0, 1, UI\\Draw\\Path, 0)
	ZEND_ARG_OBJ_INFO(0, matrix, UI\\Draw\\Matrix, 0)
ZEND_END_ARG_INFO()

/* {{{ proto Path UI\Draw\Path::transform(Matrix matrix) */
PHP_METHOD(DrawPath, transform)
{
	php_ui_path_t *p = php_ui_path_fetch(getThis());
	php_ui_path_t *r;
	zval *matrix = NULL;
	php_ui_matrix_t *m;

	if (zend_parse_parameters_throw(ZEND_NUM_ARGS(), "O", &matrix, uiDrawMatrix_ce) != SUCCESS) {
		return;
	}

	m = php_ui_matrix_fetch(matrix);

	object_init_ex(return_value, uiDrawPath_ce);

	r = php_ui_path_fetch(return_value);

	r->p = uiDrawPathCopyByTransform(p->p, &m->m);
} /* }}} */
#endif

/* {{{ */
const zend_function_entry php_ui_path_methods[] = {
	PHP_ME(DrawPath, __construct, php_ui_path_construct_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawPath, newFigure,   php_ui_path_new_figure_info,  ZEND_ACC_PUBLIC)
	PHP_ME(DrawPath, newFigureWithArc,   php_ui_path_draw_to_info,  ZEND_ACC_PUBLIC)
	PHP_ME(DrawPath, lineTo,   php_ui_path_line_to_info,  ZEND_ACC_PUBLIC)
	PHP_ME(DrawPath, arcTo,   php_ui_path_draw_to_info,  ZEND_ACC_PUBLIC)
	PHP_ME(DrawPath, bezierTo, php_ui_path_draw_to_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawPath, closeFigure, php_ui_path_close_figure_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawPath, addRectangle, php_ui_path_add_rectangle_info, ZEND_ACC_PUBLIC)
	PHP_ME(DrawPath, end, php_ui_path_end_info, ZEND_ACC_PUBLIC)
	#ifdef LIBUI_HAS_DRAW_PATH_ENDED
	PHP_ME(DrawPath, isEnded, php_ui_path_is_ended_info, ZEND_ACC_PUBLIC)
	#endif
	#ifdef LIBUI_HAS_DRAW_PATH_COPY_BY_TRANSFORM
	PHP_ME(DrawPath, transform, php_ui_path_transform_info, ZEND_ACC_PUBLIC)
	#endif
	PHP_FE_END
}; /* }}} */

/* {{{ */
PHP_MINIT_FUNCTION(UI_DrawPath) 
{
	zend_class_entry ce;

	INIT_NS_CLASS_ENTRY(ce, "UI\\Draw", "Path", php_ui_path_methods);

	uiDrawPath_ce = zend_register_internal_class(&ce);
	uiDrawPath_ce->create_object = php_ui_path_create;

	zend_declare_class_constant_long(uiDrawPath_ce, ZEND_STRL("Winding"), uiDrawFillModeWinding);
	zend_declare_class_constant_long(uiDrawPath_ce, ZEND_STRL("Alternate"), uiDrawFillModeAlternate);

	memcpy(&php_ui_path_handlers, zend_get_std_object_handlers(), sizeof(zend_object_handlers));

	php_ui_path_handlers.offset = XtOffsetOf(php_ui_path_t, std);
	php_ui_path_handlers.free_obj = php_ui_path_free;

	return SUCCESS;
} /* }}} */
#endif
