// stdafx.cpp : 표준 포함 파일만 들어 있는 소스 파일입니다.
// Conan.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj에는 미리 컴파일된 형식 정보가 포함됩니다.

#include "stdafx.h"

// TODO: 필요한 추가 헤더는
// 이 파일이 아닌 STDAFX.H에서 참조합니다.
char * _strncpy(char *, char *,size_t);
char * _strcpy_s(char *, size_t,char *);

char * _strncpy(char *to, char *from,size_t size){
#ifdef _MSC_VER
	return strcpy_s(to, size,  from);
#else
	return strncpy(to, from, size);
#endif
}
char * _strcpy_s(char *to, size_t size,char *from){
	return _strncpy(to,from,size);
}