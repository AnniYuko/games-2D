#include "raylib.h"

typedef struct s_ball{
    float x;
    float y;
    float rad;
    float speedX;
    float speedY;
    //in cpp: add DrawCircle() here as method, in c: function pointer -useful?
}t_ball;

const int SCREENWIDTH = 1200;
const int SCREENHEIGHT = 800;

int main(void)
{
    t_ball *ball;
    
    ball = malloc(sizeof(t_ball));
    ball->x = SCREENWIDTH/2;
    ball->y = SCREENHEIGHT/2;
    ball->speedX = 300;
    ball->speedY = 300;
    ball->rad = 8;
    
    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Ping Pong!");
    SetWindowState(FLAG_VSYNC_HINT);
      
    while (!WindowShouldClose())
    {
        //add speed to ball
        ball->x += ball->speedX * GetFrameTime(); //otherwise game runs faster on monitor with smaller FrameTime/higher FPS and slower on lower FPS
        ball->y += ball->speedY * GetFrameTime();
        
        if (ball->x > SCREENWIDTH - 5)
        {
            ball->x = SCREENWIDTH - 5;
            ball->speedX *= -1;
        }
        else if (ball->x < 5)
        {
            ball->x = 5;
            ball->speedX *= -1;
        }
        //bounce off top and bottom of screen
        else if (ball->y > SCREENHEIGHT - 5)
        {
            ball->y = SCREENHEIGHT - 5;
            ball->speedY *= -1;
        }
        else if (ball->y < 5)
        {
            ball->y = 5;
            ball->speedY *= -1;
        }
        
        BeginDrawing();
            ClearBackground(BLACK);
            
            DrawCircle((int)ball->x, (int)ball->y, ball->rad, PURPLE);
            DrawRectangle(50, SCREENHEIGHT/2 - 100, 10, 200, WHITE);
            DrawRectangle(SCREENWIDTH - 50 - 10, SCREENHEIGHT/2 - 100, 10, 200, WHITE);
            

            DrawFPS(1100, 10);
        EndDrawing();
    }

    free(ball);
    CloseWindow();

    return(0);
}