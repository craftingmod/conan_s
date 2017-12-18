#pragma once
# include <wchar.h>
#ifndef _DATA_H
#define _DATA_H



struct Music {
	wchar_t *title;
	wchar_t  *artist;
	int tags[15];
};


struct Tag {
	wchar_t *name;
	wchar_t *question;

};


#endif