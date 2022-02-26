#include "raylib.h"
#include <stdlib.h>

typedef struct s_circle{
    float x;
    float y;
    float radius;
}t_circle;

int main(void)
{
    //initialize
    const int screenWidth = 1200;
    const int screenHeight = 800;
    
    InitWindow(screenWidth, screenHeight, "Circle Collision");
    
    t_circle moveCirc = { GetScreenWidth()/2, GetScreenHeight()/2, 40 };
    
    SetTargetFPS(60);
    
    //main game loop
    while (!WindowShouldClose())
    {
        //move circle with mouse
        moveCirc.x = GetMouseX();
        moveCirc.y = GetMouseY();
        
        BeginDrawing();
        
        ClearBackground(BLACK);
        
        DrawCircle(300, 500, 80, WHITE);
        DrawCircle(moveCirc.x, moveCirc.y, moveCirc.radius, SKYBLUE);
        
        EndDrawing();
    }
    
    CloseWindow();
    
    return(0);
}
