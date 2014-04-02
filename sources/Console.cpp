// you probably dont want to modify this file

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "Console.h"

#ifdef WIN32
#include <windows.h>
//#include <wincon.h>

// disable useless warnings
#pragma warning(disable:4996)
#endif

static char __internal_console_buffer__[8192] = {'\0'};

#define TEXT_NORMAL   "\033[0m"
#define TEXT_RED   "\033[1;31m"
#define TEXT_GREEN "\033[1;32m"
#define TEXT_PINK "\033[1;35m"


#ifdef WIN32
static void cprintf(const char *s) {
  unsigned short attr = 0;
	int code = 0, state = 0;
	while (*s) {
		char ch = *s++;
		if (ch == 27)
			state = 1;
		else if (state == 1) {
			if (ch == '[') {
				state = 2;
				code = 0;
			}
			else
				state = 0;
		}
		else if (state == 2) {
      if (ch == ';' || ch == 'm') { // finished with this one
				fflush(stdout);
				switch (code) {
				  case 0: attr = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
				  case 1: attr |= FOREGROUND_INTENSITY; break;
				  case 3: attr |= COMMON_LVB_UNDERSCORE; break;
				  case 7: attr |= COMMON_LVB_REVERSE_VIDEO; break;
				  case 31: attr |= FOREGROUND_RED; break;
				  case 32: attr |= FOREGROUND_GREEN; break;
				  case 33: attr |= FOREGROUND_RED | FOREGROUND_GREEN; break;
				  case 34: attr |= FOREGROUND_BLUE | FOREGROUND_INTENSITY; break;
				  case 35: attr |= FOREGROUND_RED | FOREGROUND_BLUE; break;
				  case 36: attr |= FOREGROUND_GREEN | FOREGROUND_BLUE; break;
				  case 37: attr |= FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE; break;
				  case 38: attr |= FOREGROUND_BLUE | FOREGROUND_BLUE; break;
				  case 41: attr |= BACKGROUND_INTENSITY | BACKGROUND_RED; break;
				  case 42: attr |= BACKGROUND_INTENSITY | BACKGROUND_GREEN; break;
				  case 43: attr |= BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN; break;
				  case 44: attr |= BACKGROUND_INTENSITY | BACKGROUND_BLUE; break;
				  case 45: attr |= BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE; break;
				  case 46: attr |= BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE; break;
				  case 47: attr |= BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE; break;
				}
        code = 0;
        if (ch == 'm') {
          //setConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),attr);
				  state = 0;
        }
			}
			else
				code = (code * 10) + (ch - '0'); // leading digit
    } else {
  	  putchar(ch);
      attr = 0;
    }
	}
}
#else
#define cprintf printf
#define _vsnprintf vsnprintf
#endif

void warning(const char *fmt,...)
{
	va_list msg;
	va_start (msg, fmt);
	_vsnprintf(__internal_console_buffer__, 8191, fmt, msg);
	va_end (msg);

  cprintf(TEXT_PINK "warning: " TEXT_NORMAL);
  cprintf(__internal_console_buffer__);
}

void error(const char *fmt,...)
{
	va_list msg;
	va_start (msg, fmt);
	_vsnprintf(__internal_console_buffer__, 8191, fmt, msg);
	va_end (msg);

  cprintf(TEXT_RED "error: " TEXT_NORMAL);
  cprintf(__internal_console_buffer__);
}

void debug(const char *fmt,...)
{
	va_list msg;
	va_start (msg, fmt);
	_vsnprintf(__internal_console_buffer__, 8191, fmt, msg);
	va_end (msg);

  cprintf(TEXT_GREEN "debug: " TEXT_NORMAL);
  cprintf(__internal_console_buffer__);
}

void fatal(const char *fmt,...)
{
	va_list msg;
	va_start (msg, fmt);
	_vsnprintf(__internal_console_buffer__, 8191, fmt, msg);
	va_end (msg);

  cprintf(TEXT_RED "fatal error: " TEXT_NORMAL);
  cprintf(__internal_console_buffer__);
  exit(-1);
}

/*
cprintf("          Misc Options\n"
         "\033[7m  Hi-Lite Font = \\033[7m        \033[0m\n"
         "\033[4mUnderLine Font = \\033[4m        \033[0m\n"
         "\033[0mBack To Normal = \\033[0m         \033[0m\n\n"

         "          Font Colors\n"
         "\033[40m\033[1;37m         White = \\033[1;37m     \033[0m\n"
         "\033[40m\033[1;30m          Gray = \\033[1;30m     \033[0m\n"
         "\033[40m\033[37m    Light Gray = \\033[37m       \033[0m\n"
         "\033[47m\033[30m         Black = \\033[30m       \033[0m\n"
         "\033[40m\033[31m           Red = \\033[31m       \033[0m\n"
         "\033[40m\033[1;31m     Light Red = \\033[1;31m     \033[0m\n"
         "\033[40m\033[32m         Green = \\033[32m       \033[0m\n"
         "\033[40m\033[1;32m   Light Green = \\033[1;32m     \033[0m\n"
         "\033[40m\033[33m        Yellow = \\033[33m       \033[0m\n"
         "\033[40m\033[1;33m  Light Yellow = \\033[1;33m     \033[0m\n"
         "\033[40m\033[34m          Blue = \\033[34m       \033[0m\n"
         "\033[40m\033[1;34m    Light Blue = \\033[1;34m     \033[0m\n"
         "\033[40m\033[35m        Purple = \\033[35m       \033[0m\n"
         "\033[40m\033[1;35m          Pink = \\033[1;35m     \033[0m\n"
         "\033[40m\033[36m          Cyan = \\033[36m       \033[0m\n"
         "\033[40m\033[1;36m    Light Cyan = \\033[1;36m     \033[0m\n"
         "\033[40m\033[0mBack To Normal = \\033[0m     \033[0m\n");

  cprintf("\n        BackGround Colors\n"
         "\033[1;37m\033[40m         Black = \\033[40m       \033[0m\n"
         "\033[1;37m\033[41m           Red = \\033[41m       \033[0m\n"
         "\033[1;37m\033[42m         Green = \\033[42m       \033[0m\n"
         "\033[1;37m\033[43m        Yellow = \\033[43m       \033[0m\n"
         "\033[1;37m\033[44m          Blue = \\033[44m       \033[0m\n"
         "\033[1;37m\033[45m        Purple = \\033[45m       \033[0m\n"
         "\033[1;37m\033[46m          Cyan = \\033[46m       \033[0m\n"
         "\033[1;37m\033[47m         Gray  = \\033[47m       \033[0m\n"
         "\033[1;30m\033[5;47m        White  = \\033[5;47m     \033[0m\n");
  cprintf("\033[1;37m\033[0mBack To Normal = \\033[0m        \033[0m\n\n");
  
*/
