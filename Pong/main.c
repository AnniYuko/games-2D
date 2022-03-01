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

void ServeBall(t_ball *ball)
{
    ball->x = GetScreenWidth()/2.0f;
    ball->y = GetScreenHeight()/2.0f;
    ball->speedX = 400;
    ball->speedY = 400;
}

int main(void)
{
    const int SCREENWIDTH = 1200;
    const int SCREENHEIGHT = 800;

    int scoreL = 0;
    int scoreR = 0;

    t_ball *ball;

    ball = malloc(sizeof(t_ball));
    if (ball == NULL)
        return(-1);
    //starting position ball: middle of the screen
    ball->x = SCREENWIDTH/2.0f;
    ball->y = SCREENHEIGHT/2.0f;
    ball->speedX = 400;
    ball->speedY = 400;
    ball->rad = 8;

    //Vector2 ballPosition = {ball->x, ball->y};

    t_player *playerL;
    playerL = malloc(sizeof(t_player));
    if (playerL == NULL)
        return(-1);
    //player width, height and speed better as constants or part of struct?
    playerL->width = 10;
    playerL->height = 150;
    playerL->x = 50;
    playerL->y = SCREENHEIGHT/2 - playerL->height/2;
    playerL->speed = 550;

    t_player *playerR;
    playerR = malloc(sizeof(t_player));
    if (playerR == NULL)
        return(-1);
    playerR->width = 10;
    playerR->height = 150;
    playerR->x = SCREENWIDTH - 50 - playerR->width;
    playerR->y = SCREENHEIGHT/2 - playerR->height/2;
    playerR->speed = 550;

    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Ping Pong!");
    SetWindowState(FLAG_VSYNC_HINT);

    //main game loop
    while (!WindowShouldClose())
    {
        ball->x += ball->speedX * GetFrameTime(); //otherwise game runs faster on monitor with smaller FrameTime/higher FPS and slower on lower FPS
        ball->y += ball->speedY * GetFrameTime();

        //bounce off top and bottom of screen
        if (ball->y > (SCREENHEIGHT - 5))
        {
            ball->y = SCREENHEIGHT - 5;
            ball->speedY *= -1;
        }
        else if (ball->y < 5)
        {
            ball->y = 5;
            ball->speedY *= -1;
        }
        //for testing purposes
        /*if (ball->x < 5)
        {
            ball->x = 5;
            ball->speedX *= -1;
        }
        else if (ball->x > (SCREENWIDTH - 5))
        {
            ball->x = SCREENWIDTH - 5;
            ball->speedX *= -1;
        }*/

        //move players up(-) and down(+)
        if (IsKeyDown(KEY_F))
            playerL->y -= playerL->speed * GetFrameTime();
        else if (IsKeyDown(KEY_D))
            playerL->y += playerL->speed * GetFrameTime();
        if (IsKeyDown(KEY_J))
            playerR->y -= playerR->speed * GetFrameTime();
        else if (IsKeyDown(KEY_K))
            playerR->y += playerR->speed * GetFrameTime();
        //add: players can't move outside of screen

        //ball-player collision, make game faster over time
        if (CheckCollisionCircleRec((Vector2) { ball->x, ball->y }, ball->rad, (Rectangle) { playerR->x, playerR->y, playerR->width, playerR->height }))
        {
            if (ball->speedX > 0)
                ball->speedX *= -1.05;
        }
        else if (CheckCollisionCircleRec((Vector2) { ball->x, ball->y }, ball->rad, (Rectangle) { playerL->x, playerL->y, playerL->width, playerL->height }))
        {
            if (ball->speedX < 0)
            ball->speedX *= -1.05;
        }

        if (ball->x >= SCREENWIDTH)
        {
            scoreL++;
            ServeBall(ball);
        }
        else if (ball->x <= 0)
        {
            scoreR++;
            ServeBall(ball);
        }

        BeginDrawing();
            ClearBackground(BLACK);

            DrawCircle((int)ball->x, (int)ball->y, ball->rad, PURPLE);
            DrawRectangle(playerL->x, playerL->y, playerL->width, playerL->height, WHITE);
            DrawRectangle(playerR->x, playerR->y, playerR->width, playerR->height, WHITE);

            DrawText(TextFormat("Score: %d", scoreL), 150, 120, 20, GREEN);
            DrawText(TextFormat("Score: %d", scoreR), 950, 120, 20, GREEN);

            DrawFPS(1100, 10);
        EndDrawing();
    }

    free(ball);
    free(playerL);
    free(playerR);
    CloseWindow();

    return(0);
}
