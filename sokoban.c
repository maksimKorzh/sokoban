#include <stdio.h>
#include <string.h>

#define MAP_WIDTH 8
#define MAP_HEIGHT 9

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

int dest_squares[10];

int GetDestSquares()
{
    int count;

    for(int height = 0; height < MAP_HEIGHT; height++)
    {
        for(int width = 0; width < MAP_WIDTH; width++)
        {
            if(map[height * MAP_WIDTH + width] == 'x' ||
               map[height * MAP_WIDTH + width] == 'O')
            {
                dest_squares[count++] = height * MAP_WIDTH + width;
            }
        }
    }

    return count;
}

void GetPosition(int *pos_x, int *pos_y)
{
    for(int height = 0; height < MAP_HEIGHT; height++)
    {
        for(int width = 0; width < MAP_WIDTH; width++)
        {
            if(map[height * MAP_WIDTH + width] == '@')
            {
                *pos_x = width;
                *pos_y = height;
            }
        }
    }
}

void MoveCharacter(int pos_x, int pos_y, int offset)
{
    if(map[pos_y * MAP_WIDTH + pos_x + offset] != '#')
    {
        if(((map[pos_y * MAP_WIDTH + pos_x + offset] == 'B') ||
            (map[pos_y * MAP_WIDTH + pos_x + offset] == 'O')) &&
            (map[pos_y * MAP_WIDTH + pos_x + offset * 2] != '#' ||
             map[pos_y * MAP_WIDTH + pos_x + offset * 2] != 'B' ||
             map[pos_y * MAP_WIDTH + pos_x + offset * 2] != 'O'))
        {
            map[pos_y * MAP_WIDTH + pos_x] = ' ';
            pos_x += offset;

            if(map[pos_y * MAP_WIDTH + pos_x + offset] == ' ')
                map[pos_y * MAP_WIDTH + pos_x + offset] = 'B';

            else if(map[pos_y * MAP_WIDTH + pos_x + offset] == 'x')
                map[pos_y * MAP_WIDTH + pos_x + offset] = 'O';

            else
            {
                map[pos_y * MAP_WIDTH + pos_x - offset] = '@';
                return;
            }

            map[pos_y * MAP_WIDTH + pos_x] = '@';
        }

        else
        {
            map[pos_y * MAP_WIDTH + pos_x] = ' ';
            pos_x += offset;
            map[pos_y * MAP_WIDTH + pos_x] = '@';
        }
    }   
}

int main()
{
    int key, pos_x, pos_y, dest_count;

    int dest_num = GetDestSquares();

    printf("%s\n", map);

    while(key != 27)
    {  
        GetPosition(&pos_x, &pos_y);
        
        key = getchar();

        switch(key)
        {
            case 'w':
                MoveCharacter(pos_x, pos_y, - MAP_WIDTH - 1);
                break;

            case 's':
                MoveCharacter(pos_x, pos_y, MAP_WIDTH + 1);
                break; 

            case 'a':
                MoveCharacter(pos_x, pos_y, -1);
                break; 

            case 'd':
                MoveCharacter(pos_x, pos_y, 1);
                break; 

        }

        dest_count = 0;

        for(int i = 0; i < 10; i++)
        {            
            if(map[dest_squares[i]] == 'O') dest_count++;
        
            if(map[dest_squares[i]] == ' ')
                map[dest_squares[i]] = 'x';
        }

        printf("%s\n", map);

        if(dest_num == dest_count)
        {
            printf("You win!\n");
            key = 27;
        }        
    }
        
    return 0;
}
