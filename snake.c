/**
 * @file snake2.c
 * @brief A simple snake game implementation in C.
 * 
 * This program implements a simple snake game using the terminal. The snake moves
 * around the screen and eats fruits to grow longer. The game ends when the snake
 * runs into the boundaries of the screen or into itself.
 * 
 * The snake can be controlled using the arrow keys. The game also displays the current
 * score and provides instructions to quit the game.
 * 
 * @note This program uses ANSI escape codes to control the cursor position and color.
 *      It may not work on all terminals or operating systems.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include "constants.h"
#include "draw.h"
#include "point.h"


/**
 * @brief Generates random coordinates for a fruit within the game boundaries.
 *
 * This function assigns random x and y coordinates to the fruit within the
 * specified game area, ensuring that the coordinates are within the valid
 * range defined by WIDTH and HEIGHT.
 *
 * @param fruitx Pointer to an integer where the x-coordinate of the fruit will be stored.
 * @param fruity Pointer to an integer where the y-coordinate of the fruit will be stored.
 */
void fruits(int* fruitx, int* fruity)
{
    *fruitx = rand() % (WIDTH-2) + 1;
    *fruity = rand() % (HEIGHT-2) + 1;
}


/**
 * @brief Handles user input for controlling the snake's direction.
 *
 * This function reads a character from the standard input and updates the 
 * direction flag based on the arrow key pressed. It also handles the 'x' or 'X' 
 * key to set the flag to -1, which can be used to signal an exit condition.
 *
 * @param flag Pointer to an integer representing the current direction of the snake.
 *             The direction is updated based on the arrow key pressed:
 *             - 1: Down
 *             - 2: Left
 *             - 3: Up
 *             - 4: Right
 *             - -1: Exit signal (when 'x' or 'X' is pressed)
 *
 * @note This function modifies the terminal settings to enable immediate reading 
 *       of characters without waiting for a newline. It uses the `system` function 
 *       to execute the `stty` command, which may not be portable across all systems.
 */
void input(int* flag)
{
    system("stty -icanon -echo");  
    int ch = getchar();  

    if (ch == 27) {
        ch = getchar();  
        if (ch == 91) {
            ch = getchar();  
            switch (ch)
            {
                case 65:  // Up arrow
                    if(*flag != 1)
                        *flag = 3;
                    break;
                case 66:  // Down arrow
                    if(*flag != 3)
                        *flag = 1;
                    break;
                case 67:  // Right arrow
                    if(*flag != 2)
                        *flag = 4;
                    break;
                case 68:  // Left arrow
                    if(*flag != 4)
                        *flag = 2;
                    break;
                default:
                    break;
            }
        }
    }
    else {
        if(ch == 'x' || ch == 'X'){
            *flag = -1;
        }
    }
}


/**
 * @brief Moves the snake in the specified direction.
 *
 * This function updates the position of the snake based on the given direction flag.
 * The snake is represented as an array of Points, where each Point contains x and y coordinates.
 *
 * @param snake An array of Points representing the snake.
 * @param length The length of the snake.
 * @param flag The direction in which to move the snake:
 *             - 1: down
 *             - 2: left
 *             - 3: up
 *             - 4: right
 */
void move_snake(Point snake[], int length, int flag)
{
    Point temp = snake[length-1];
    for(int i = 0; i < length; i++){
        snake[i] = snake[i+1];
    }
    switch (flag)
    {
        case 1:  // down
            snake[length-1].x = temp.x + 1;
            snake[length-1].y = temp.y;
            break;
        case 2:  // left
            snake[length-1].x = temp.x;
            snake[length-1].y = temp.y - 1;
            break;
        case 3:  // up
            snake[length-1].x = temp.x - 1;
            snake[length-1].y = temp.y;
            break;
        case 4:  // right
            snake[length-1].x = temp.x;
            snake[length-1].y = temp.y + 1;
            break;
        default:
            break;
    }
}

/**
 * @brief Checks for collisions of the snake with boundaries or itself.
 *
 * This function checks if the snake has collided with the boundaries of the game area
 * or with itself. It returns a specific code based on the type of collision detected.
 *
 * @param snake An array of Point structures representing the snake's body.
 * @param length The current length of the snake.
 * @return int Returns 2 if there is a boundary collision, 1 if there is a self collision, 
 * and 0 if there is no collision.
 */
int check_collision(const Point snake[], int length)
{
    if (snake[length-1].x <= 0 || snake[length-1].x >= HEIGHT-1 || snake[length-1].y <= 0 || snake[length-1].y >= WIDTH-1){
        return 2;  // Boundary collision
    }
    for(int i = 0; i < length - 1; i++){
        if(snake[length-1].x == snake[i].x && snake[length-1].y == snake[i].y){
            return 1;  // Self collision
        }
    }
    return 0;  // No collision
}


/**
 * @brief Grows the snake by one segment, updates the score, and generates a new fruit.
 *
 * This function handles the logic for growing the snake when it eats a fruit. It performs the following steps:
 * 1. Stores the current head of the snake.
 * 2. Erases the old fruit from the screen.
 * 3. Generates a new fruit at a random position.
 * 4. Draws the new fruit on the screen.
 * 5. Increases the score by 5 points.
 * 6. Updates the score display.
 * 7. Shifts all segments of the snake to make room for the new head.
 * 8. Sets the new head of the snake to the old head position.
 * 9. Increases the length of the snake by one segment.
 *
 * @param snake Array of Points representing the snake's body.
 * @param length Pointer to the current length of the snake.
 * @param score Pointer to the current score.
 * @param fruitx Pointer to the x-coordinate of the fruit.
 * @param fruity Pointer to the y-coordinate of the fruit.
 */
void grow_snake(Point snake[], int* length, int* score, int* fruitx, int* fruity)
{
    Point old = snake[0];
    erase_old_fruit(*fruitx, *fruity);
    fruits(fruitx, fruity);
    draw_fruit(*fruitx, *fruity);
    *score += 5;
    draw_score(*score);
    for(int i = *length; i > 0; i--){
        snake[i] = snake[i-1];
    }
    snake[0] = old;
    (*length)++;
}

/**
 * @brief Controls the game logic for the snake game.
 *
 * This function handles the main game logic, including moving the snake,
 * checking for collisions, and handling fruit consumption.
 *
 * @param flag An integer flag indicating the direction of the snake's movement.
 *             If the flag is 0, the function returns immediately.
 * @param snake An array of Point structures representing the snake's body.
 * @param length A pointer to an integer representing the current length of the snake.
 * @param gameover A pointer to an integer that will be set to a non-zero value if the game is over.
 * @param score A pointer to an integer representing the player's current score.
 * @param fruitx A pointer to an integer representing the x-coordinate of the fruit.
 * @param fruity A pointer to an integer representing the y-coordinate of the fruit.
 */
void logic(int flag, Point snake[], int* length, int* gameover, int* score, int* fruitx, int* fruity)
{
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 250000000L;  // 250 milliseconds
    if(*score >= 25){
        ts.tv_nsec = 200000000L;  // 200 milliseconds
    }
    else if(*score >= 50){
        ts.tv_nsec = 150000000L;  // 150 milliseconds
    }
    else if(*score >= 75){
        ts.tv_nsec = 100000000L;  // 100 milliseconds
    }
    nanosleep(&ts, NULL);  // Pause to slow down the game
    if(flag == 0){
        return;
    }
    move_snake(snake, *length, flag);
    *gameover = check_collision(snake, *length);
    if (*gameover != 0) {
        return;
    }
    if (snake[*length-1].x == *fruitx && snake[*length-1].y == *fruity)
    {
        grow_snake(snake, length, score, fruitx, fruity);
    }
    if(*score == 100){
        *gameover = -1;
    }
}


/**
 * @file snake.c
 * @brief Main file for the Snake game implementation.
 *
 * This file contains the main function which initializes and runs the Snake game.
 * The game logic includes setting up the game environment, handling user input,
 * updating the game state, and rendering the game on the console.
 *
 * The game ends when the snake hits itself, hits the boundary, or the user decides to quit.
 * The final score is displayed at the end of the game.
 *
 * The game uses non-blocking input to allow real-time interaction with the user.
 * The console cursor is hidden during the game and restored at the end.
 *
 * Functions:
 * - main: Entry point of the Snake game.
 *
 * Usage:
 * Compile and run the program. Use the keyboard to control the snake.
 * Press 'X' to quit the game.
 *
 * Dependencies:
 * - Standard C libraries: stdio.h, stdlib.h, time.h, unistd.h, fcntl.h
 * - Custom headers: Point, LENGTH_SNAKE, WIDTH, HEIGHT, fruits, initialize, draw, input, erase_old_snake, logic
 *
 * @note Ensure the terminal supports ANSI escape codes for proper rendering.
 */
int main()
{
    srand((unsigned int)time(NULL));
    int length;
    int* length_ptr = &length;  
    int gameover = 0;
    int score = 0;
    int fruitx;
    int fruity;
    int flag = 0;
    Point snake[LENGTH_SNAKE] = {0};
    fruits(&fruitx,&fruity);
    snake[0].x = WIDTH / 2;
    snake[0].y = HEIGHT / 2;
    length = 1;

    initialize(fruitx,fruity);
    fflush(stdout);
    system("stty -icanon -echo"); // Shut off canonical mode and echo mode
    system("tput civis"); // Hide the cursor
    int flags;

    flags = fcntl(STDIN_FILENO, F_GETFL, 0);

    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK); // Set non-blocking input

    while (gameover == 0)
    {
        draw(length,snake);
        fflush(stdout);
        input(&flag);
        if(flag == -1){
            break;
        }
        erase_old_snake(snake,length);
        logic(flag,snake,length_ptr,&gameover,&score,&fruitx,&fruity);
    }
    struct timespec ts;
    
    system("clear");
    printf("\033[1;1H");
    printf("\033[0;31m"); // Set text color to red
    switch (gameover) {
        case 1:
            printf("You hit yourself\n");
            break;
        case 2:
            printf("You hit the boundary\n");
            break;
        case -1:
            printf("\033[0;32m"); // Set text color to green
            printf("You are a winner\n");
            break;
        default:
            printf("Game Over\n");
            break;
    }
    printf("Score = %d\n",score);
    printf("Press 'X' to quit the game");
    fflush(stdout);
    while(1){
        input(&flag);
        if(flag == -1){
            break;
        }
        ts.tv_sec = 1;
        ts.tv_nsec = 0;
        nanosleep(&ts, NULL);
    }
    system("clear");
    system("tput cnorm");
    system("stty icanon echo");
    return 0;
}
