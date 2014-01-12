/**
 * Autogenerated by TData Compiler (0.0.1)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */

#ifndef _H_LANGUAGE_LANGUAGE_TYPES_H
#define _H_LANGUAGE_LANGUAGE_TYPES_H

#include "platform/tlibc_platform.h"



typedef enum _EN_TD_LANGUAGE_STRING EN_TD_LANGUAGE_STRING;
enum _EN_TD_LANGUAGE_STRING
{
	E_LS_UNKNOW = 0,
	E_LS_CANNOT_USE_RESERVED_LANGUAGE_KEYWORD = 1,
	E_LS_SYNTAX_ERROR = 2,
};

#define TD_LANGUAGE_STRING_NUM 3

#define TD_MAX_LANGUAGE_STRING_LENGTH 1024

typedef struct _ST_TD_LANGUAGE_STRING ST_TD_LANGUAGE_STRING;
typedef struct _ST_TD_LANGUAGE_STRING
{
	EN_TD_LANGUAGE_STRING language_string_number;
	tchar language_string[TD_MAX_LANGUAGE_STRING_LENGTH];
};

typedef struct _ST_TD_LANGUAGE_STRING_LIBRARY ST_TD_LANGUAGE_STRING_LIBRARY;
struct _ST_TD_LANGUAGE_STRING_LIBRARY
{
	tuint16 language_string_list_num;
	ST_TD_LANGUAGE_STRING language_string_list[TD_LANGUAGE_STRING_NUM];
};

#endif //_H_LANGUAGE_LANGUAGE_TYPES_H

