#include "draw.h"
#include "constants.h"
#include <stdio.h>

/**
 * @brief Initializes the terminal screen for the game.
 *
 * This function clears the terminal screen, hides the cursor, and positions the cursor at the top-left corner.
 * 
 * The following ANSI escape codes are used:
 * - \033[2J: Clears the entire screen.
 * - \033[?25l: Hides the cursor.
 * - \033[%d;%dH: Moves the cursor to the specified position (row, column).
 *
 * @param fruitx The x-coordinate of the fruit to be drawn.
 * @param fruity The y-coordinate of the fruit to be drawn.
 */
void initialize(int fruitx, int fruity)
{
    printf("\033[2J"); 
    printf("\033[?25l"); 
    printf("\033[%d;%dH", 1, 1); 
    draw_border();
    draw_fruit(fruitx, fruity);
    draw_score(0);
    draw_footer();
}

/**
 * @brief Draws the border of the game area.
 *
 * This function prints the border of the game area using the '#' character.
 * The border is drawn along the edges of the area defined by HEIGHT and WIDTH.
 * The interior of the game area is filled with spaces.
 *
 * The border is drawn as follows:
 * - The top and bottom edges are filled with '#'.
 * - The left and right edges are filled with '#'.
 * - The interior is filled with spaces.
 *
 * @note Ensure that HEIGHT and WIDTH are defined before calling this function.
 */
void draw_border()
{
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (i == 0 || i == WIDTH - 1 || j == 0 || j == HEIGHT - 1)
            {
                printf("#");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
}

/**
 * @brief Erases the old snake from the console.
 *
 * This function moves the cursor to each point of the snake's body and prints a space character
 * to effectively erase the snake from the console.
 *
 * @param snake A pointer to an array of Point structures representing the snake's body.
 * @param length The length of the snake (number of points in the snake's body).
 */
void erase_old_snake(const Point* snake, int length)
{
    for (int k = 0; k < length; k++)
    {
        printf("\033[%d;%dH ", (snake[k].x+1),(snake[k].y+1));
    }
}

/**
 * @brief Draws the snake on the console.
 *
 * This function iterates through the snake's body points and prints each segment
 * at the corresponding coordinates on the console using ANSI escape codes.
 *
 * @param snake Pointer to an array of Point structures representing the snake's body.
 * @param length The number of segments in the snake's body.
 */
void draw_snake(const Point* snake, int length)
{
    for (int k = 0; k < length; k++)
    {
        printf("\033[%d;%dH0", (snake[k].x+1),(snake[k].y+1));
    }
}

/**
 * @brief Draws a fruit at the specified coordinates on the console.
 *
 * This function positions the cursor at the given (fruitx, fruity) coordinates
 * and prints a '*' character to represent a fruit.
 *
 * @param fruitx The x-coordinate (row) where the fruit should be drawn.
 * @param fruity The y-coordinate (column) where the fruit should be drawn.
 */
void draw_fruit(int fruitx, int fruity)
{
    printf("\033[%d;%dH*", fruitx+1, fruity+1);
}

/**
 * @brief Erases the fruit at the specified coordinates on the console.
 *
 * This function positions the cursor at the given (fruitx, fruity) coordinates
 * and prints a space character to remove the previously drawn fruit.
 *
 * @param fruitx The x-coordinate (row) where the fruit should be erased.
 * @param fruity The y-coordinate (column) where the fruit should be erased.
 */
void erase_old_fruit(int fruitx, int fruity)
{
    printf("\033[%d;%dH ", fruitx+1, fruity+1);
}

/**
 * @brief Draws the score and a quit message on the console.
 *
 * This function uses ANSI escape codes to position the cursor and print the
 * current score and a message to quit the game on the console.
 *
 * @param score The current score to be displayed.
 */
void draw_score(int score)
{
    printf("\033[%d;%dHScore = %d", HEIGHT + 2, 1, score);
    printf("\033[%d;%dHPress 'X' to quit the game", HEIGHT + 3, 1);
}

/**
 * @brief Draws the footer section of the Snake game interface.
 *
 * This function prints the footer messages at the bottom of the game screen,
 * providing instructions and credits to the player. The messages include:
 * - A welcome message.
 * - Instructions on how to move the snake using arrow keys.
 * - Information on how to score points by eating the fruit.
 * - A warning to avoid running into walls or the snake itself.
 * - Developer credits.
 *
 * The messages are printed starting from a position below the game area,
 * determined by the constant HEIGHT.
 */
void draw_footer()
{
    printf("\033[%d;%dHWelcome to the Snake Game!", HEIGHT + 5, 1);
    printf("\033[%d;%dHUse the arrow keys to move the snake.", HEIGHT + 6, 1);
    printf("\033[%d;%dHEat the fruit (*) to grow and score points.", HEIGHT + 7, 1);
    printf("\033[%d;%dHAvoid running into the walls or the snake itself.", HEIGHT + 8, 1);
    printf("\033[%d;%dHDeveloped by Florent Delalande.", HEIGHT + 9, 1);
}


/**
 * @brief Draws the snake on the screen.
 *
 * This function takes the length of the snake and an array of Points representing the snake's body,
 * and calls the draw_snake function to render the snake on the screen.
 *
 * @param length The length of the snake.
 * @param snake A pointer to an array of Points representing the snake's body.
 */
void draw(int length, const Point* snake)
{
    draw_snake(snake, length);
}