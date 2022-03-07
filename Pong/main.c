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

typedef enum SwitchScreen { START = 0, TITLE, GAMEPLAY, END } SwitchScreen;

Vector2 my_add_vectors2(Vector2 v1, Vector2 v2)
{
    return((Vector2) { v1.x + v2.x, v1.y + v2.y });
}

Vector2 my_sub_vectors2(Vector2 v1, Vector2 v2)
{
    return((Vector2) { v1.x - v2.x, v1.y - v2.y });
}

void ServeBall(t_ball *ball)
{
    ball->pos = (Vector2) { GetScreenWidth()/2, GetScreenHeight()/2 };
    ball->velo = (Vector2) { 7, 7 };     //make random values between (-)7 and (-)6
}

int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 800;
  
    int scoreL = 0;
    int scoreR = 0;
    
    float playerHeight = 120;
    float playerWidth = 10;
    
    int tutorial = 0;
    
    InitWindow(screenWidth, screenHeight, "Another Pong");
    
    t_player playerL = { 0 };
    playerL.pos = (Vector2) { 50, (screenHeight/2 - playerHeight/2)};
    playerL.velo = (Vector2) { 0, 10 };
    
    t_player playerR = { 0 };
    playerR.pos = (Vector2) { (screenWidth - 50 - playerWidth), (screenHeight/2 - playerHeight/2)};
    playerR.velo = (Vector2) { 0, 10 };
    
    t_ball ball = { 0 };
    ball.rad = 8;
    
    SwitchScreen currScreen = START;
    
    SetTargetFPS(60);
      
    //main game loop
    while (!WindowShouldClose())
    {
                
        switch(currScreen)
        {
            case START:
            {
              if (IsKeyPressed(KEY_ENTER))
                    currScreen = TITLE;
            } break;
            case TITLE:
            {
                if (IsKeyPressed(KEY_T))
                    tutorial = (!tutorial);
                else if (IsKeyPressed(KEY_ENTER))
                    currScreen = GAMEPLAY;
            } break;
            case GAMEPLAY:
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
             
                //move players up(-) and down(+)
                if (IsKeyDown(KEY_F) && (playerL.pos.y > 0))
                {
                    playerL.pos = my_sub_vectors2(playerL.pos, playerL.velo);
                }
                else if (IsKeyDown(KEY_D) && (playerL.pos.y < (screenHeight - playerHeight)))
                {
                    playerL.pos = my_add_vectors2(playerL.pos, playerL.velo);
                }
                if (IsKeyDown(KEY_J) && (playerR.pos.y > 0))
                {
                    playerR.pos = my_sub_vectors2(playerR.pos, playerL.velo);
                }
                else if (IsKeyDown(KEY_K) && (playerR.pos.y < (screenHeight - playerHeight)))
                {
                    playerR.pos = my_add_vectors2(playerR.pos, playerR.velo);
                }
        
                //ball player collision & speed up game over time
                if (CheckCollisionCircleRec(ball.pos, ball.rad, (Rectangle) { playerR.pos.x, playerR.pos.y, playerWidth, playerHeight}))
                {    
                    if (ball.velo.x > 0)
                    {
                        ball.velo.x *= -1.05;
                        ball.velo.y *=  1.05;
                    }
                }
                else if (CheckCollisionCircleRec(ball.pos, ball.rad, (Rectangle) { playerL.pos.x, playerL.pos.y, playerWidth, playerHeight}))
                {
                    if (ball.velo.x < 0)
                    {
                        ball.velo.x *= -1.05;
                        ball.velo.y *=  1.05;
                    }
                }

                if (ball.pos.x >= screenWidth)
                {
                    if (!tutorial)
                        scoreL++;
                    ServeBall(&ball);
                }
                else if (ball.pos.x <= 0)
                {
                    if (!tutorial)
                        scoreR++;
                    ServeBall(&ball);
                }
                
                if (IsKeyPressed(KEY_ENTER))
                    currScreen = TITLE;
            } break;
            case END:
            {
                if (IsKeyPressed(KEY_ENTER))
                    currScreen = GAMEPLAY;
            } break;
            default: break;
        }
        //--------------------- Drawing ----------------------------------------
        BeginDrawing();
            ClearBackground(BLACK);
            
            switch(currScreen)
            {
                case START:
                {
                    DrawText(TextFormat("Let's play Pong!"), screenWidth/2 - 150, screenHeight/2 - 100, 40, RAYWHITE);
                    DrawText(TextFormat("Press Enter to continue"), screenWidth/2 - 150, screenHeight/2 + 200, 25, BLUE);
                } break;
                case TITLE:
                {
                    DrawText(TextFormat("Press 'T' to try the tutorial first"), screenWidth/2 - 200, screenHeight/2, 25, tutorial ? GREEN : RAYWHITE);
                    DrawText(TextFormat("Press Enter to continue"), screenWidth/2 - 150, screenHeight/2 + 200, 25, BLUE);
                } break;
                case GAMEPLAY:
                {
                    DrawCircleV(ball.pos, ball.rad, PURPLE);
                    DrawRectangle(playerL.pos.x, playerL.pos.y, playerWidth, playerHeight, WHITE);
                    DrawRectangle(playerR.pos.x, playerR.pos.y, playerWidth, playerHeight, WHITE);
            
                    if (!tutorial)
                    {
                        DrawText(TextFormat("Score: %d", scoreL), 150, 120, 25, GREEN);
                        DrawText(TextFormat("Score: %d", scoreR), 900, 120, 25, GREEN);
                    }
                    else
                    {
                        DrawText(TextFormat("Move the left paddle with F, D"), 150, 100, 20, GREEN);
                        DrawText(TextFormat("Move the right paddle with J, K"), 150, 150, 20, GREEN);
                        DrawText(TextFormat("Press Enter to end the tutorial"), 150, 200, 20, GRAY);
                    }

                    DrawFPS(1100, 10);
                } break;
                case END:
                {
                    DrawText(TextFormat("Press Enter to play again"), screenWidth/2 - 150, screenHeight/2 + 200, 25, BLUE);
                } break;
                default: break;
            }
            
        EndDrawing();
    }
    CloseWindow();

    return(0);
}
