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
#ifndef HAVE_PHP_UI_ATTRIBUTE_H
#define HAVE_PHP_UI_ATTRIBUTE_H

extern zend_class_entry *uiDrawTextAttribute_ce;
extern zend_class_entry *uiDrawTextAttributesFamily_ce;
extern zend_class_entry *uiDrawTextAttributesSize_ce;
extern zend_class_entry *uiDrawTextAttributesWeight_ce;
extern zend_class_entry *uiDrawTextAttributesItalic_ce;
extern zend_class_entry *uiDrawTextAttributesStretch_ce;
extern zend_class_entry *uiDrawTextAttributesColor_ce;
extern zend_class_entry *uiDrawTextAttributesBackground_ce;
extern zend_class_entry *uiDrawTextAttributesUnderline_ce;
extern zend_class_entry *uiDrawTextAttributesUnderlineColor_ce;
extern zend_class_entry *uiDrawTextAttributesFeatures_ce;
extern zend_class_entry *uiDrawTextAttributesFeaturesCollection_ce;

typedef struct _php_ui_attribute_t {
	uiAttribute *a;
	zend_object std;
} php_ui_attribute_t;

#define php_ui_attribute_from(o) ((php_ui_attribute_t*) ((char*) o - XtOffsetOf(php_ui_attribute_t, std)))
#define php_ui_attribute_fetch(z) php_ui_attribute_from(Z_OBJ_P(z))

typedef struct _php_ui_attribute_features_t {
	uiOpenTypeFeatures *f;
	zend_object std;
} php_ui_attribute_features_t;

#define php_ui_attribute_features_from(o) ((php_ui_attribute_features_t*) ((char*) o - XtOffsetOf(php_ui_attribute_features_t, std)))
#define php_ui_attribute_features_fetch(z) php_ui_attribute_features_from(Z_OBJ_P(z))

PHP_MINIT_FUNCTION(UI_DrawTextAttribute);
#endif
