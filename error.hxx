/*
 * error.hxx
 *
 * Copyright 2015 constroy <constroy.li@gmail.com>
 *
 *
 */


#ifndef ERROR_HXX
#define ERROR_HXX

#include <cstdio>

enum Error
{
	open_failed,
	unexpected_EOF,
	unknown_character
};
enum Warning
{
	integer_overflow
};

extern const char error_msg[][64];
extern const char warning_msg[][64];

void error(int r,int c,Error e);
void warning(int r,int c,Warning w);

#endif /* ERROR_HXX */
