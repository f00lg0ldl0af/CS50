#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //declare height variable
    int height ;
    do
    {
      height = get_int("Height: ");
    } while (height < 1 || height > 8 );

    //prints out (height) rows
    for (int row = 0; row < height; row++)
    {
        //prints out __ spaces
        //row 0, 7 spaces
        //row 1, 6 spaces
        //... row 7, 0 spaces
        for (int space = (height - row - 1); space > 0; space--)
        {
           printf(" ");
        }

        //prints out bricks
        //row 0, 1 brick
        //row 1, 2 bricks
        //... row 7, 8 bricks
        for (int brick = 0; brick < row + 1; brick++)
        {
           printf("#");
        }

        //prints out gap for middle
        printf("  ");

        for (int brick = 0; brick < row + 1; brick++)
        {
           printf("#");
        }
       printf("\n");

    }

}