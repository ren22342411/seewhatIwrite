#define PRINT_CASE(case_code, info) case case_code:\
	{\
	KdPrint(("%s " #case_code "(0x%x)\n",(info),(case_code)));\
	}\
	break;