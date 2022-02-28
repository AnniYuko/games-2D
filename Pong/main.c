#include "raylib.h"
#include <stdlib.h>

typedef struct s_ball{
    Vector2 pos;
    Vector2 velo;
    float rad;
}t_ball;

typedef struct s_player{
    Vector2 pos;
    Vector2 velo;
}t_player;

Vector2 my_add_vectors2(Vector2 v1, Vector2 v2)
{
    return((Vector2) { v1.x + v2.x, v1.y + v2.y });
}

Vector2 my_sub_vectors2(Vector2 v1, Vector2 v2)
{
    return((Vector2) { v1.x - v2.x, v1.y - v2.y });
}

int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 800;

    //starting position ball: middle of the screen
    t_ball ball = { { screenWidth/2, screenHeight/2 }, { 8, 8 }, 8};
    
    //initialize players
    float playerHeight = 150;
    float playerWidth = 10;
    
    t_player playerL = { { 50, (screenHeight/2 - playerHeight/2)}, { 0, 10 } };
    t_player playerR = { { (screenWidth - 50 - playerWidth), (screenHeight/2 - playerHeight/2)}, { 0, 10 } };
    
    InitWindow(screenWidth, screenHeight, "Ping Pong!");
    SetTargetFPS(60);
      
    //main game loop
    while (!WindowShouldClose())
    {
        //add velocity to ball position
        ball.pos = my_add_vectors2(ball.pos, ball.velo);
        
        //bounce off top and bottom of screen
        if (ball.pos.y > (screenHeight - 5))
        {
            ball.pos.y = screenHeight - 5;
            ball.velo.y *= -1;
        }
        else if (ball.pos.y < 5)
        {
            ball.pos.y = 5;
            ball.velo.y *= -1;
        }
        //for testing purposes
        if (ball.pos.x < 5)
        {
            ball.pos.x = 5;
            ball.velo.x *= -1;
        }
        else if (ball.pos.x > (screenWidth - 5))
        {
            ball.pos.x = screenWidth - 5;
            ball.velo.x *= -1;
        }
        
        //move players up(-) and down(+)
        if (IsKeyDown(KEY_F))
            playerL.pos = my_sub_vectors2(playerL.pos, playerL.velo);
        else if (IsKeyDown(KEY_D))
            playerL.pos = my_add_vectors2(playerL.pos, playerL.velo);
        if (IsKeyDown(KEY_J))
            playerR.pos = my_sub_vectors2(playerR.pos, playerL.velo);
        else if (IsKeyDown(KEY_K))
            playerR.pos = my_add_vectors2(playerR.pos, playerR.velo);
        //add: players can't move outside of screen
        
        //ball-player collision
        if (CheckCollisionCircleRec(ball.pos, ball.rad, (Rectangle) { playerR.pos.x, playerR.pos.y, playerWidth, playerHeight}))
        {    
            if (ball.velo.x > 0)
                ball.velo.x *= -1.05;
        }
        else if (CheckCollisionCircleRec(ball.pos, ball.rad, (Rectangle) { playerL.pos.x, playerL.pos.y, playerWidth, playerHeight}))
        {
            if (ball.velo.x < 0)
            ball.velo.x *= -1.05;
        }
        
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
            
            DrawCircleV(ball.pos, ball.rad, PURPLE);
            DrawRectangle(playerL.pos.x, playerL.pos.y, playerWidth, playerHeight, WHITE);
            DrawRectangle(playerR.pos.x, playerR.pos.y, playerWidth, playerHeight, WHITE);
            
            DrawFPS(1100, 10);
        EndDrawing();
    }

    CloseWindow();

    return(0);
}
