#include <stdio.h>
#include <string.h>

#define MAP_WIDTH 8
#define MAP_HEIGHT 9
#define PLAYER_POSITION pos_y * MAP_WIDTH + pos_x

char map[] = {

    "  ##### \n"
    "###   # \n"
    "#x@B  # \n"
    "### Bx# \n"
    "#x##B # \n"
    "# # x ##\n"
    "#B OBBx#\n"
    "#   x  #\n"
    "########\n"

};

/*#define MAP_WIDTH 14
#define MAP_HEIGHT 10
#define PLAYER_POSITION pos_y * MAP_WIDTH + pos_x

char map[] = {

    "##############\n"
    "#      # xB  #\n"
    "#  x   # xB  #\n"
    "#   B  # xB  #\n"
    "#      ####  #\n"
    "#     @   #  #\n"
    "#         #  #\n"
    "#    B    #  #\n"
    "#     x      #\n"
    "##############\n"

};*/

int dest_squares[10];                                                   // array to store cell indexes for 'x' cells

int GetDestSquares()                                                    // init 'x' cells indexes
{
    int count, cell;                                                    // 'x' cell number, current cell index

    for(int row = 0; row < MAP_HEIGHT; row++)                           // loop ower map rows
    {
        for(int col = 0; col < MAP_WIDTH; col++)                        // loop ower map columns
        {
            cell = row * MAP_WIDTH + col;                               // init current cell index
        
            if(map[cell] == 'x' || map[cell] == 'O')                    // if 'x' cell is emty or with box on it
                dest_squares[count++] = cell;                           // store it in array
        }
    }

    return count - 1;                                                   // return number of 'x' cells
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
    int key;                                                            // user input key
    int pos_x, pos_y;                                                   //  player's coordinates
    int dest_count;                                                     //  'x' cells counter

    int dest_num = GetDestSquares();                                    // get number of 'x' cells

    printf("%s\n", map);                                                // print map

    while(key != 27)                                                    // game loop
    {  
        GetPosition(&pos_x, &pos_y);                                    // get player's coordinates
        
        key = getchar();                                                // get user input

        switch(key)
        {
            // move character up
            case 'w':
                MoveCharacter(pos_x, pos_y, - MAP_WIDTH - 1);
                break;

            // move character down
            case 's':
                MoveCharacter(pos_x, pos_y, MAP_WIDTH + 1);
                break; 

            // move character left
            case 'a':
                MoveCharacter(pos_x, pos_y, -1);
                break; 

            // move character right
            case 'd':
                MoveCharacter(pos_x, pos_y, 1);
                break; 

        }

        dest_count = 0;                                                 // reset 'x' cells counter

        for(int i = 0; i < 10; i++)                                     // for all destination squares
        {            
            if(map[dest_squares[i]] == 'O') dest_count++;               // increase 'x' cells counter if box is on 'x' cell
        
            if(map[dest_squares[i]] == ' ')                             // if 'x' cell has been erased
                map[dest_squares[i]] = 'x';                             // restore it
        }

        printf("%s\n", map);                                            // print map

        // if all boxes are on it's places break out of game loop
        if(dest_num == dest_count)
        {
            printf("You win!\n");
            key = 27;
        }        
    }
        
    return 0;
}
