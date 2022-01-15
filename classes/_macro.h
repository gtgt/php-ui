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