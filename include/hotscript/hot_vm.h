#ifndef _H_HOT_VM
#define _H_HOT_VM


#include "hotpot/hp_platform.h"

#include "hotscript/hotobject.h"
#include "hotpot/hp_value.h"

#define HOT_ECHO                               0
#define HOT_PUSH							   1
#define HOT_PUSH_INDEX						   2
#define HOT_POP								   3
#define HOT_ECHO_TRIE						   4
#define HOT_JMP								   5

//�����������
typedef struct _HotOperand
{
	hpint32 num;
	const char *str;
	hpuint32 str_len;
}HotOperand;


//�����ָ��
typedef struct _HotOp
{
	hpint32 op;
	HPVar op0;
	HPVar op1;
	HPVar op2;
	hpuint32 lineno;
}HotOp;

typedef struct _HotOpArr
{
	HotOp *oparr;
	hpuint32 oparr_size;
	hpuint32 next_oparr;
}HotOpArr;

#define MAX_VM_HO_SIZE 1024


hpint32 hotoparr_init(HotOpArr *self);

HotOp *hotoparr_get_next_op(HotOpArr *self);

hpuint32 hotoparr_get_next_op_number(HotOpArr *self);

#define MAX_FUNCTION_STACK_DEEP 1024
typedef struct _HotVM
{
	const HotOpArr *hotoparr;

	hpuint32 current_op;

	HPAbstractReader *reader;
}HotVM;

hpint32 hotvm_execute(HotVM *self, const HotOpArr *hotoparr, HPAbstractReader *reader);

#endif//_H_HOT_VM
