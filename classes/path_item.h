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
#ifndef HAVE_PHP_UI_PATH_ITEM_H
#define HAVE_PHP_UI_PATH_ITEM_H

extern zend_class_entry *uiDrawPathItem_ce;

typedef struct _php_ui_path_item_t {
	uiDrawPathItem *i;
	zend_object std;
} php_ui_path_item_t;

#define php_ui_path_item_from(o) ((php_ui_path_item_t*) ((char*) o - XtOffsetOf(php_ui_path_item_t, std)))
#define php_ui_path_item_fetch(z) php_ui_path_item_from(Z_OBJ_P(z))

zval* php_ui_path_item_construct(zval *path_item, uiDrawPathItem *pathItem);

PHP_MINIT_FUNCTION(UI_DrawPathItem);
#endif
