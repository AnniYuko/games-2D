#include "raylib.h"
#include <stdlib.h>

typedef struct s_ball{
    float x;
    float y;
    float rad;
    float speedX;
    float speedY;
    //in cpp: add DrawCircle() here as method, in c: function pointer -useful?
}t_ball;

typedef struct s_player{
    float x;
    float y;
    float width;
    float height;
    float speed;
}t_player;


int main(void)
{
    const int SCREENWIDTH = 1200;
    const int SCREENHEIGHT = 800;

    t_ball *ball;
    
    ball = malloc(sizeof(t_ball));
    if (ball == NULL)
        return(-1);
    //starting position ball: middle of the screen
    ball->x = SCREENWIDTH/2.0f;
    ball->y = SCREENHEIGHT/2.0f;
    ball->speedX = 200;
    ball->speedY = 300;
    ball->rad = 8;
    
    //Vector2 ballPosition = {ball->x, ball->y};
    
    t_player *playerL;
    playerL = malloc(sizeof(t_player));
    if (playerL == NULL)
        return(-1);
    //player width, height and speed better constants or part of struct?
    playerL->width = 10;
    playerL->height = 150;
    playerL->x = 50;
    playerL->y = SCREENHEIGHT/2 - playerL->height/2;
    playerL->speed = 500;
    
    t_player *playerR;
    playerR = malloc(sizeof(t_player));
    if (playerR == NULL)
        return(-1);
    playerR->width = 10;
    playerR->height = 150;
    playerR->x = SCREENWIDTH - 50 - playerR->width;
    playerR->y = SCREENHEIGHT/2 - playerR->height/2;
    playerR->speed = 500;
    
    /*Rectangle recR = {playerR->x, playerR->y, playerR->width, playerR->height};
    Rectangle recL = {playerL->x, playerL->y, playerL->width, playerL->height};*/
    
    
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Ping Pong!");
    SetWindowState(FLAG_VSYNC_HINT);
      
    //main game loop
    while (!WindowShouldClose())
    {
        ball->x += ball->speedX * GetFrameTime(); //otherwise game runs faster on monitor with smaller FrameTime/higher FPS and slower on lower FPS
        ball->y += ball->speedY * GetFrameTime();
        
        //bounce off top and bottom of screen
        if (ball->y > SCREENHEIGHT - 5)
        {
            ball->y = SCREENHEIGHT - 5;
            ball->speedY *= -1;
        }
        else if (ball->y < 5)
        {
            ball->y = 5;
            ball->speedY *= -1;
        }
        
        //move players up(-) and down(+)
        if (IsKeyDown(KEY_D))
            playerL->y -= playerL->speed * GetFrameTime();
        else if (IsKeyDown(KEY_S))
            playerL->y += playerL->speed * GetFrameTime();
        if (IsKeyDown(KEY_K))
            playerR->y -= playerR->speed * GetFrameTime();
        else if (IsKeyDown(KEY_L))
            playerR->y += playerR->speed * GetFrameTime();
        //add: players can't move outside of screen
        
        //ball-player collision
        /*if (CheckCollisionCircleRec(ballPosition, ball->rad, recR))
            ball->speedX *= -1;
        else if (CheckCollisionCircleRec(ballPosition, ball->rad, recL))
            ball->speedX *= -1;*/
        
        /* if (ball->x >= SCREENWIDTH)
        {
            point for playerL;
        }
        else if (ball->x <= 0)
        {
            point for playerR;
        } */
    
        
        BeginDrawing();
            ClearBackground(BLACK);
            
            DrawCircle((int)ball->x, (int)ball->y, ball->rad, PURPLE);
            DrawRectangle(playerL->x, playerL->y, playerL->width, playerL->height, WHITE);
            DrawRectangle(playerR->x, playerR->y, playerR->width, playerR->height, WHITE);
            

            DrawFPS(1100, 10);
        EndDrawing();
    }

    free(ball);
    free(playerL);
    free(playerR);
    CloseWindow();

    return(0);
}