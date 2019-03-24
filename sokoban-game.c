
/*********************************************************************************\
;                                  SOKOBAN GAME                                   ;
;---------------------------------------------------------------------------------;
;                  Simple implementation of sokoban game dedicated                ;
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

#include "game.h"
                
#define MAP_WIDTH 8
#define MAP_HEIGHT 10
#define PLAYER_POSITION pos_y * MAP_WIDTH + pos_x

char map[] = {

    "#####   "
    "#xB ### "
    "###   # "
    "#x@B  # "
    "### Bx# "
    "#x##B # "
    "# # x ##"
    "#B OBBx#"
    "#   x  #"
    "########"

};

int dest_squares[10];                                                   // array to store cell indexes for 'x' cells

int GetDestSquares()                                                    // init 'x' cells indexes
{
    int count = 0, cell;                                                // 'x' cell number, current cell index

    for(int row = 0; row < MAP_HEIGHT; row++)                           // loop ower map rows
    {
        for(int col = 0; col < MAP_WIDTH; col++)                        // loop ower map columns
        {
            cell = row * MAP_WIDTH + col;                               // init current cell index
        
            if(map[cell] == 'x' || map[cell] == 'O')                    // if 'x' cell is emty or with box on it
                dest_squares[count++] = cell;                           // store it in array
        }
    }

    return count;                                                       // return number of 'x' cells
}

void GetPosition(int *pos_x, int *pos_y)
{
    int cell;                                                           // current cell index

    for(int row = 0; row < MAP_HEIGHT; row++)                           // loop ower map rows
    {
        for(int col = 0; col < MAP_WIDTH; col++)                        // loop ower map columns
        {
            cell = row * MAP_WIDTH + col;                               // init current cell index
        
            if(map[cell] == '@')                                        // if current cell on the map contains player
            {
                *pos_x = col;                                           // store player's x coordinate
                *pos_y = row;                                           // store player's y coordinate
            }
        }
    }
}

void MoveCharacter(int pos_x, int pos_y, int offset)
{
    if(map[PLAYER_POSITION + offset] != '#')                            // if player doesn't hit the wall
    {
        if(((map[PLAYER_POSITION + offset] == 'B') ||                   // if player hits the box
            (map[PLAYER_POSITION + offset] == 'O')) &&                  // or the box on 'x' cell
            (map[PLAYER_POSITION + offset * 2] != '#' ||                // and box doesn't hit a wall
             map[PLAYER_POSITION + offset * 2] != 'B' ||                // or another box
             map[PLAYER_POSITION + offset * 2] != 'O'))                 // or box on 'x' cell
        {
            map[PLAYER_POSITION] = ' ';                                 // clear previous player's position
            pos_x += offset;                                            // update player's coordinate

            if(map[PLAYER_POSITION + offset] == ' ')                    // if the square next to the box is empty
                map[PLAYER_POSITION + offset] = 'B';                    // push the box

            else if(map[PLAYER_POSITION + offset] == 'x')               // if the square next to the box is 'x'  
                map[PLAYER_POSITION + offset] = 'O';                    // mark the box is on it's place

            else
            {
                map[PLAYER_POSITION - offset] = '@';                    // if box hits the wall or another box
                return;                                                 // don't push it any further
            }

            map[PLAYER_POSITION] = '@';                                 // draw the player in the new position
        }

        else                                                            // if the square next to the player is empty                                                        
        {
            map[PLAYER_POSITION] = ' ';                                 // clear previous player position
            pos_x += offset;                                            // update player's coordinate
            map[PLAYER_POSITION] = '@';                                 // draw the player in the new position
        }
    }   
} 

int main()
{
    InitScreen();

    int key;                                                            // user input key
    int pos_x, pos_y;                                                   //  player's coordinates
    int dest_count;                                                     //  'x' cells counter

    int dest_num = GetDestSquares();

    int center_x = SCREEN_WIDTH / 2 - MAP_WIDTH / 2;
    int center_y = SCREEN_HEIGHT / 2 - MAP_HEIGHT / 2;

    PrintMap(center_x, center_y, MAP_WIDTH, MAP_HEIGHT, map);
    
    while(1)
    {
        if(key == 27) break;
        
        key = getch();
        GetPosition(&pos_x, &pos_y);

        switch(key)
        {
            case 'w': MoveCharacter(pos_x, pos_y, - MAP_WIDTH); break;
            case 's': MoveCharacter(pos_x, pos_y, MAP_WIDTH); break;
            case 'a': MoveCharacter(pos_x, pos_y, - 1); break;
            case 'd': MoveCharacter(pos_x, pos_y, 1); break;
        }

        dest_count = 0;                                                 // reset 'x' cells counter

        for(int i = 0; i < 10; i++)                                     // for all destination squares
        {            
            if(map[dest_squares[i]] == 'O') dest_count++;               // increase 'x' cells counter if box is on 'x' cell
        
            if(map[dest_squares[i]] == ' ')                             // if 'x' cell has been erased
                map[dest_squares[i]] = 'x';                             // restore it
        }
        
        PrintMap(center_x, center_y, MAP_WIDTH, MAP_HEIGHT, map);

        // if all boxes are on it's places break out of game loop
        if(dest_num == dest_count)
        {
            sprintf(screen + (SCREEN_WIDTH * SCREEN_HEIGHT / 2) - MAP_WIDTH / 2, "YOU WIN!");
            RefreshScreen();
            break;
        }       
    }

    Leave();
    
    return 0;
}