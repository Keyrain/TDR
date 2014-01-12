#include "symbols.h"
#include <string.h>
#include "error.h"


void symbols_init(SYMBOLS *self)
{
	AlphaMap *alpha_map = NULL;
	self->domain[0] = 0;

	alpha_map = alpha_map_new();

	alpha_map_add_range(alpha_map, 'a', 'z');
	alpha_map_add_range(alpha_map, 'A', 'Z');
	alpha_map_add_range(alpha_map, '0', '9');
	alpha_map_add_range(alpha_map, '_', '_');
	self->symbols = trie_new(alpha_map);
	alpha_map_free(alpha_map);
}

void symbols_fini(SYMBOLS *self)
{
	trie_free(self->symbols);
}

const SYMBOL* symbols_search_identifier(SYMBOLS *self, const tbytes* tok_identifier, tbool back_searching)
{
	char name[TLIBC_MAX_IDENTIFIER_LENGTH];
	memcpy(name, tok_identifier->ptr, tok_identifier->len);
	name[tok_identifier->len] = 0;
	
	return symbols_search_string(self, name, back_searching);
}

tint32 symbols_save(SYMBOLS *self, const char *name, const SYMBOL *symbol)
{
	char global_name[TLIBC_MAX_IDENTIFIER_LENGTH];

	if(self->domain[0])
	{
		snprintf(global_name, TLIBC_MAX_IDENTIFIER_LENGTH, "%s:%s", self->domain, name);
		global_name[TLIBC_MAX_IDENTIFIER_LENGTH - 1] = 0;
	}
	else
	{
		snprintf(global_name, TLIBC_MAX_IDENTIFIER_LENGTH, "%s", name);
		global_name[TLIBC_MAX_IDENTIFIER_LENGTH - 1] = 0;
	}

	//printf("save %s : %x\n", global_name, symbol);
	if(!trie_store_if_absent(self->symbols, global_name, symbol))
	{
		return E_TD_ERROR;
	}
	return E_TD_NOERROR;
}

void symbols_domain_begin(SYMBOLS *self, const tbytes *tok_identifier)
{
	memcpy(self->domain, tok_identifier->ptr, tok_identifier->len);
	self->domain[tok_identifier->len] = 0;
}

void symbols_domain_end(SYMBOLS *self)
{
	self->domain[0] = 0;
}


const SYMBOL* symbols_search_string(SYMBOLS *self, const char* name, tbool back_searching)
{
	const SYMBOL *symbol;
	char global_name[TLIBC_MAX_IDENTIFIER_LENGTH];

	if(self->domain[0])
	{
		snprintf(global_name, TLIBC_MAX_IDENTIFIER_LENGTH, "%s:%s", self->domain, name);
		global_name[TLIBC_MAX_IDENTIFIER_LENGTH - 1] = 0;
	}
	else
	{
		snprintf(global_name, TLIBC_MAX_IDENTIFIER_LENGTH, "%s", name);
		global_name[TLIBC_MAX_IDENTIFIER_LENGTH - 1] = 0;
	}
	
	if(!trie_retrieve(self->symbols, global_name, (void**)&symbol))
	{
		if(!back_searching)
		{
			goto ERROR_RET;	
		}
		if(!trie_retrieve(self->symbols, name, (void**)&symbol))
		{
			goto ERROR_RET;
		}
	}

	return symbol;
ERROR_RET:
	return NULL;
}

const ST_SIMPLE_TYPE* symbols_get_real_type(SYMBOLS *self, const ST_SIMPLE_TYPE* sn_type)
{
	if(sn_type->st == E_ST_REFER)
	{
		const SYMBOL *ptr = symbols_search_string(self, sn_type->st_refer, hpfalse);
		if(ptr == NULL)
		{
			goto ERROR_RET;
		}
		if(ptr->type != EN_HST_TYPE)
		{
			goto ERROR_RET;
		}
		return symbols_get_real_type(self, &ptr->body.type);
	}
	else
	{
		return sn_type;
	}

	return sn_type;
ERROR_RET:
	return NULL;
}

const ST_VALUE* symbols_get_real_value(SYMBOLS *self, const ST_VALUE* sn_value)
{
	if(sn_value->type == E_SNVT_IDENTIFIER)
	{
		const SYMBOL *ptr = symbols_search_string(self, sn_value->val.identifier, hpfalse);
		if(ptr == NULL)
		{
			goto ERROR_RET;
		}

		if(ptr->type != EN_HST_VALUE)
		{
			goto ERROR_RET;			
		}
		return symbols_get_real_value(self, &ptr->body.val);
	}

	return sn_value;
ERROR_RET:
	return NULL;
}


void symbols_add_Const(SYMBOLS *self, const ST_Const *pn_const)
{
	SYMBOL *ptr = (SYMBOL*)malloc(sizeof(SYMBOL));
	const ST_VALUE *val = symbols_get_real_value(self, &pn_const->val);

	if(val == NULL)
	{
		goto done;
	}

	ptr->type = EN_HST_VALUE;
	ptr->body.val = *val;

	if(symbols_save(self, pn_const->identifier, ptr) != E_TD_NOERROR)
	{
		goto done;
	}
done:
	return;
}

void symbols_add_Typedef(SYMBOLS *self, const ST_TYPEDEF *pn_typedef)
{
	SYMBOL *symbol = (SYMBOL*)malloc(sizeof(SYMBOL));
	symbol->type = EN_HST_TYPE;
	symbol->body.type = pn_typedef->type;
	if(symbols_save(self, pn_typedef->name, symbol) != E_TD_NOERROR)
	{
		goto done;
	}
done:
	return;
}

void symbols_add_Enum(SYMBOLS *self, const ST_ENUM *pn_enum)
{
	SYMBOL *ptr = (SYMBOL*)malloc(sizeof(SYMBOL));
	tuint32 i;

	ptr->type = EN_HST_ENUM;
	ptr->body.enum_def_list_num = pn_enum->enum_def_list_num;

	if(symbols_save(self, pn_enum->name, ptr) != E_TD_NOERROR)
	{
		goto done;
	}

done:
	return;
}

void symbols_add_EnumDef(SYMBOLS *self, const ST_ENUM_DEF *pn_enum_def)
{
	SYMBOL *ptr = (SYMBOL*)malloc(sizeof(SYMBOL));
	const ST_VALUE *val = symbols_get_real_value(self, &pn_enum_def->val);

	if(val == NULL)
	{
		goto done;
	}

	ptr->type = EN_HST_VALUE;
	ptr->body.val = *val;

	if(symbols_save(self, pn_enum_def->identifier, ptr) != E_TD_NOERROR)
	{
		goto done;
	}
done:
	return;

}

void symbols_add_Parameter(SYMBOLS *self, const ST_Parameter *pn_parameter)
{
	SYMBOL *ptr = (SYMBOL*)malloc(sizeof(SYMBOL));

	ptr->type = EN_HST_PARAMETER;
	ptr->body.para = *pn_parameter;

	symbols_save(self, pn_parameter->identifier, ptr);
}

void symbols_add_Field(SYMBOLS *self, const ST_FIELD *pn_field)
{
	SYMBOL *ptr = (SYMBOL*)malloc(sizeof(SYMBOL));

	ptr->type = EN_HST_FIELD;
	ptr->body.field = *pn_field;

	symbols_save(self, pn_field->identifier, ptr);
}

void symbols_add_Struct(SYMBOLS *self, const ST_STRUCT *de_struct)
{
	SYMBOL *ptr = (SYMBOL*)malloc(sizeof(SYMBOL));

	ptr->type = EN_HST_STRUCT;
	ptr->body.field_list_num = de_struct->field_list.field_list_num;

	symbols_save(self, de_struct->name, ptr);
}

void symbols_add_Union(SYMBOLS *self, const ST_UNION *de_union)
{
	SYMBOL *ptr = (SYMBOL*)malloc(sizeof(SYMBOL));

	ptr->type = EN_HST_UNION;
	ptr->body.field_list_num = de_union->union_field_list.union_field_list_num;

	symbols_save(self, de_union->name, ptr);
}