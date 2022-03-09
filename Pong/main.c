#include "raylib.h"
#include <stdlib.h>
#include <math.h>

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
    ball->velo.x = (int)pow(-1, rand()) /* -1 (left) or 1 (right)*/ * (rand()%3 + 5.5f); //value between 5.5 and 7.5
    ball->velo.y = (int)pow(-1, rand()) * (rand()%3 + 5.5f);
}

int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 800;
  
    int scoreL = 0;
    int scoreR = 0;
    
    float playerHeight = 120;
    float playerWidth = 10;
    
    bool tutorial = false;
    bool winner = false;
    bool paused = false;
    
    int framesCounter = 0;
    
    InitWindow(screenWidth, screenHeight, "Another Pong");
    
    t_player playerL = { 0 };
    playerL.pos = (Vector2) { 50, (screenHeight/2 - playerHeight/2)};
    playerL.velo = (Vector2) { 0, 12 };
    
    t_player playerR = { 0 };
    playerR.pos = (Vector2) { (screenWidth - 50 - playerWidth), (screenHeight/2 - playerHeight/2)};
    playerR.velo = (Vector2) { 0, 12 };
    
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
                {
                    currScreen = GAMEPLAY;
                    ServeBall(&ball);
                }
            } break;
            case GAMEPLAY:
            {
                if (!paused)
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

                    //scoring points
                    if ((ball.pos.x >= screenWidth) && !winner)
                    {
                        if (!tutorial)
                            scoreL++;
                        if (scoreL < 10 && scoreR < 10)
                            ServeBall(&ball);
                    }
                    else if ((ball.pos.x < 0) && !winner)
                    {
                        if (!tutorial)
                            scoreR++;
                        if (scoreL < 10 && scoreR < 10)
                            ServeBall(&ball);
                    }
                    
                    //winning condition
                    if (scoreL == 10 || scoreR == 10)
                    {
                        winner = true;
                        if (framesCounter++ > 180)
                            currScreen = END;
                    }
                }

                //pause game
                if (IsKeyPressed(KEY_SPACE))
                    paused = (!paused);

                if (tutorial && IsKeyPressed(KEY_ENTER))
                {
                    currScreen = TITLE;
                    //deselect tutorial option
                    tutorial = false;
                }
            } break;
            case END:
            {
                if (IsKeyPressed(KEY_ENTER))
                {
                    currScreen = GAMEPLAY;
                    winner = false;
                    scoreL = 0;
                    scoreR = 0;
                    ServeBall(&ball);
                }
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
            
                    if (tutorial)
                    {
                        DrawText(TextFormat("Move the left paddle with F, D"), 150, 100, 20, GREEN);
                        DrawText(TextFormat("Move the right paddle with J, K"), 150, 150, 20, GREEN);
                        DrawText(TextFormat("Press Enter to end the tutorial"), 150, 200, 20, GRAY);
                    }
                    else
                    {
                        DrawText(TextFormat("Score: %d", scoreL), 200, 120, 30, GREEN);
                        DrawText(TextFormat("Score: %d", scoreR), 850, 120, 30, GREEN);
                        
                        if (winner && (scoreL > scoreR))
                            DrawText(TextFormat("The left player has won!"), screenWidth/2 - 200, screenHeight/2, 30, YELLOW);
                        else if (winner && (scoreL < scoreR))
                            DrawText(TextFormat("The right player has won!"), screenWidth/2 - 200, screenHeight/2, 30, YELLOW);
                    }
                    
                    if (paused)
                    {
                        DrawRectangle(300, screenHeight/2 - 300, 600, 500, RAYWHITE);
                        DrawRectangle(315, screenHeight/2 - 285, 570, 470, BLACK);
                        DrawText(TextFormat("PAUSED"), screenWidth/2 - MeasureText(TextFormat("PAUSED"), 40)/2 , screenHeight/2 - 200, 40, RAYWHITE);
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
