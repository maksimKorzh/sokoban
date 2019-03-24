
/*********************************************************************************\
;                                     game.h                                      ;
;---------------------------------------------------------------------------------;
;                    A very basic console game library dedicated                  ;
;                for the beginners who want to write their own games              ;
;---------------------------------------------------------------------------------;
;                                by Maksim Korzh                                  ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;        Copyright © 2018 Maksim Korzh <freesoft.for.people@gmail.com>            ;
;---------------------------------------------------------------------------------;
;     This work is free. You can redistribute it and/or modify it under the       ;
;      terms of the Do What The Fuck You Want To Public License, Version 2,       ;
;       as published by Sam Hocevar. See the COPYING file for more details.       ;
;---------------------------------------------------------------------------------;
'       THIS PROGRAM IS FREE SOFTWARE. IT COMES WITHOUT ANY WARRANTY, TO          ;
;        THE EXTENT PERMITTED BY APPLICABLE LAW. YOU CAN REDISTRIBUTE IT          ;
;       AND/OR MODIFY IT UNDER THE TERMS OF THE DO WHAT THE FUCK YOU WANT         ;
;          TO PUBLIC LICENCE, VERSION 2, AS PUBLISHED BY SAM HOCEVAR.             ;
;                SEE http://www.wtfpl.net/ FOR MORE DETAILS.                      ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;                   DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE                   ;
;                           Version 2, December 2004                              ;
;                                                                                 ;
;        Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>                         ;
;                                                                                 ;
;        Everyone is permitted to copy and distribute verbatim or modified        ;
;        copies of this license document, and changing it is allowed as long      ;
;        as the name is changed.                                                  ;
;                                                                                 ;
;                   DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE                   ;
;          TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION        ;
;                                                                                 ;
;         0. You just DO WHAT THE FUCK YOU WANT TO.                               ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

#include <stdio.h>

// default console screen size
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define SCREEN_SIZE SCREEN_WIDTH * SCREEN_HEIGHT

// screen buffer that replaces stdout
char screen[SCREEN_SIZE];

// init windows headers
#ifdef WIN32

#include <windows.h>
#include <stdlib.h>
HANDLE console;
CONSOLE_CURSOR_INFO cursor;
COORD coord;
DWORD chars_to_write = 0;

#endif

// init unix headers
#ifdef unix

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#endif

void InitScreen()
{
    // print directly to screen buffer
    #ifdef WIN32
    console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);    
    cursor.dwSize = 100;
    cursor.bVisible = FALSE;    
    coord.X = 0;
    coord.Y = 0;
    SetConsoleActiveScreenBuffer(console);
    SetConsoleCursorInfo(console, &cursor);        
    #endif

    // use ANSI VT100 escape sequences hide cursor and clear screen 
    #ifdef unix
    printf("\x1b[?25l");
    printf("\x1b[2J");
    #endif
}

void RefreshScreen()
{
    // might not be needed on windows natively, needed on wine though
    for(int scr_cell = 0; scr_cell < SCREEN_SIZE; scr_cell++)
        if(!screen[scr_cell]) screen[scr_cell] = ' ';

    // update screen buffer
    #ifdef WIN32
    screen[SCREEN_SIZE - 1] = 0;
	WriteConsoleOutputCharacter(console, screen, SCREEN_WIDTH * SCREEN_HEIGHT, coord, &chars_to_write);
    #endif

    // print screen buffer to stdout at coordinates 0, 0
    #ifdef unix
    printf("\x1b[0;0H%s", screen);
    #endif
}

void PrintMap(int pos_x, int pos_y, int map_width, int map_height, char *map)
{
    for (int row = 0; row < map_height; row++)
    {
        for (int col = 0; col < map_width; col++)
        {
            screen[(row + pos_y) * SCREEN_WIDTH + col + pos_x] = map[row * map_width + col];
        }
    }

    RefreshScreen();    
}        

// getchar() for windows without echoing
#ifdef WIN32

int getch()
{
    DWORD mode, chars_to_read;
    HANDLE console = GetStdHandle(STD_INPUT_HANDLE);
        
    GetConsoleMode(console, &mode);
    SetConsoleMode(console, mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
    int key = 0;
    ReadConsole(console, &key, 1, &chars_to_read, NULL);
    SetConsoleMode(console, mode);
    
    return key;
}

#endif

// getchar() for unix without echoing
#ifdef unix

int getch()
{
    struct termios oldattr, newattr;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    int key = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );

    return key;   
}

#endif

void Leave()
{
    #ifdef WIN32
    cursor.bVisible = TRUE;
    SetConsoleCursorInfo(console, &cursor);
    #endif

    // show cursor escape sequence
    #ifdef unix
    printf("\x1b[?25h");
    #endif
}
