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
#ifndef HAVE_PHP_UI_MACRO
#define HAVE_PHP_UI_MACRO


#if PHP_VERSION_ID >= 70200
#define PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_OBJECT_INFO_EX(name, return_reference, required_num_args, class_name, allow_null)\
	ZEND_BEGIN_ARG_WITH_RETURN_OBJ_INFO_EX(name, return_reference, required_num_args, class_name, allow_null)
#else
#define PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_OBJECT_INFO_EX(name, return_reference, required_num_args, class_name, allow_null)\
	ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, IS_OBJECT, #class_name, allow_null)
#endif

#if PHP_VERSION_ID >= 70200
#define PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, type, allow_null)\
	ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, type, allow_null)
#else
#define PHP_UI_ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, type, allow_null)\
	ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(name, return_reference, required_num_args, type, NULL, allow_null)
#endif

#endif