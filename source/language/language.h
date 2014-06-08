#ifndef _H_LANGUAGE_H
#define _H_LANGUAGE_H

typedef enum language_string_e
{
	E_LS_UNKNOW = 0,
	E_LS_IDENTIFIER_NOT_DEFINED = 1,
	E_LS_IDENTIFIER_REDEFINITION = 2,
	E_LS_IDENTIFIER_NOT_INTEGER = 3,
	E_LS_STRING_LENGTH_NOT_DEFINED = 4,
	E_LS_IDENTIFIER_LENGTH_ERROR = 5,
	E_LS_IDENTIFIER_NOT_STRING = 6,
	E_LS_CANNOT_USE_RESERVED_LANGUAGE_KEYWORD = 7,
	E_LS_CANNOT_OPEN_FILE = 8,
	E_LS_SYNTAX_ERROR = 9,
	E_LS_OUT_OF_MEMORY = 10,
	E_LS_CHARACTER_CONSTANT_FORMAT_ERROR = 11,
	E_LS_STRING_CONSTANT_FORMAT_ERROR = 12,
	E_LS_NUMBER_ERROR_RANGE = 13,
	E_LS_CAN_NOT_DEFINE_STRING_LENGTH_HERE = 14,
	E_LS_MUST_DEFINE_STRING_LENGTH_HERE = 15,
	E_LS_CONSTANT_TYPES_DO_NOT_MATCH = 16,
	E_LS_TYPE_ERROR = 17,
	E_LS_ENUM_HAVE_EQUAL_ELEMENTS = 18,
	E_LS_TOO_MANY_MEMBERS = 19,
	E_LS_TOO_MANY_PARAMETERS = 20,
	E_LS_TOO_MANY_ARGUMENTS = 21,
	E_LS_NOT_INTEGER_VALUE = 22,
	E_LS_NOT_INTEGER_TYPE = 23,
	E_LS_ARGUMENT_TYPE_MISMATCH = 24,
	E_LS_NOT_ENUM_TYPE = 25,
	E_LS_SQL_NOT_SUPPORT_CONDITION = 26,
	E_LS_SCRIPT_ERROR = 27,
	E_LS_SCRIPT_HALT= 28,
	E_LS_MAX,
}language_string_t;

const char* tdr_strerror(language_string_t sid);


#endif//_H_LANGUAGE_H
