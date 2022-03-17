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

typedef enum SwitchScreen {
    START = 0,
    INSTRUCTIONS,
    MENU,
    GAMEPLAY,
    END
} SwitchScreen;

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
    ball->velo.x = (int)pow(-1, rand()) /* -1 (left) or 1 (right)*/ * GetRandomValue(6, 7);
    ball->velo.y = (int)pow(-1, rand()) * GetRandomValue(6, 7);
}

void DrawTextCentered(char *text, int posY, int fontSize, Color color)
{
    int textWidth = MeasureText(text, fontSize);
    DrawText(text, GetScreenWidth()/2.0f - textWidth/2.0f, posY, fontSize, color);
}

int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 800;
  
    int scoreL = 0;
    int scoreR = 0;
    
    float playerHeight = 120;
    float playerWidth = 10;
    
    bool winner = false;
    bool paused = false;
    bool beginner = false;
    bool intermediate = true;
    bool hard = false;
    bool extreme;
    
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
                    currScreen = INSTRUCTIONS;
            } break;
            case INSTRUCTIONS:
            {
                if (IsKeyPressed(KEY_ENTER))
                    currScreen = MENU;
            } break;
            case MENU:
            {
                if (IsKeyPressed(KEY_ENTER))
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
                        playerL.pos = my_sub_vectors2(playerL.pos, playerL.velo);
                    else if (IsKeyDown(KEY_D) && (playerL.pos.y < (screenHeight - playerHeight)))
                        playerL.pos = my_add_vectors2(playerL.pos, playerL.velo);
                    if (IsKeyDown(KEY_J) && (playerR.pos.y > 0))
                        playerR.pos = my_sub_vectors2(playerR.pos, playerL.velo);
                    else if (IsKeyDown(KEY_K) && (playerR.pos.y < (screenHeight - playerHeight)))
                        playerR.pos = my_add_vectors2(playerR.pos, playerR.velo);
            
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
                        scoreL++;
                        if (scoreL < 10 && scoreR < 10)
                            ServeBall(&ball);
                    }
                    else if ((ball.pos.x < 0) && !winner)
                    {
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
                    DrawTextCentered(TextFormat("Let's play Pong!"), screenHeight/2 - 100, 50, RAYWHITE);
                    DrawTextCentered(TextFormat("Press Enter to continue"), screenHeight/2 + 200, 25, BLUE);
                } break;
                case INSTRUCTIONS:
                {
                    DrawTextCentered(TextFormat("How to play"), 100, 40, WHITE);
                    DrawTextCentered(TextFormat("Move the left paddle with F, D"), 200, 30, LIGHTGRAY);
                    DrawTextCentered(TextFormat("Move the right paddle with J, K"), 250, 30, LIGHTGRAY);
                    DrawTextCentered(TextFormat("Score 10 points to win the round"), 300, 30, LIGHTGRAY);
                    DrawTextCentered(TextFormat("Press space to pause/resume the game"), 400, 30, LIGHTGRAY);
                    DrawTextCentered(TextFormat("Press Enter to continue"), 600, 25, BLUE);
                } break;
                case MENU:
                {
                    DrawTextCentered(TextFormat("Menu"), 100, 40, WHITE);
                    DrawTextCentered(TextFormat("DIFFICULTY LEVEL"), 200, 30, LIGHTGRAY);
                    DrawTextCentered(TextFormat("Beginner"), 280, 30, beginner ? GREEN : LIGHTGRAY);
                    DrawTextCentered(TextFormat("Intermediate"), 330, 30, intermediate ? GREEN : LIGHTGRAY);
                    DrawTextCentered(TextFormat("Hard"), 380, 30, hard ? GREEN : LIGHTGRAY);
                    DrawTextCentered(TextFormat("Extreme"), 430, 30, extreme ? GREEN : LIGHTGRAY);
                    DrawTextCentered(TextFormat("Press Enter to start the game"), 600, 25, BLUE);
                } break;
                case GAMEPLAY:
                {
                    DrawCircleV(ball.pos, ball.rad, PURPLE);
                    DrawRectangle(playerL.pos.x, playerL.pos.y, playerWidth, playerHeight, WHITE);
                    DrawRectangle(playerR.pos.x, playerR.pos.y, playerWidth, playerHeight, WHITE);
            
                    DrawText(TextFormat("%02d", scoreL), screenWidth/2 - 200 - MeasureText("00", 50), 50, 50, GREEN);
                    DrawText(TextFormat("%02d", scoreR), screenWidth/2 + 200, 50, 50, GREEN);
                    DrawTextCentered(TextFormat(":"), 50, 50, GREEN);
                        
                    if (winner && (scoreL > scoreR))
                        DrawTextCentered(TextFormat("The left player has won!"), screenHeight/2, 30, YELLOW);
                    else if (winner && (scoreL < scoreR))
                        DrawTextCentered(TextFormat("The right player has won!"), screenHeight/2, 30, YELLOW);
                    
                    if (paused)
                    {
                        //draw a window with white border
                        DrawRectangleLinesEx((Rectangle) {300, screenHeight/2 - 200, 600, 500}, 10.0f, RAYWHITE);
                        DrawTextCentered(TextFormat("PAUSED"), screenHeight/2 - 150, 40, RAYWHITE);
                    }
                    DrawFPS(1100, 10);
                } break;
                case END:
                {
                    DrawTextCentered(TextFormat("Press Enter to play again"), screenHeight/2 + 200, 25, BLUE);
                } break;
                default: break;
            }
            
        EndDrawing();
    }
    CloseWindow();

    return(0);
}
