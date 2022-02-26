#include "raylib.h"
#include <math.h>

typedef struct s_circle{
    float x;
    float y;
    float radius;
}t_circle;

int main(void)
{
    //initialize
    const int screenWidth = 1000;
    const int screenHeight = 700;
    
    InitWindow(screenWidth, screenHeight, "Circles Collision");
    
    //starting position
    t_circle moveCirc = { GetScreenWidth()/2, GetScreenHeight()/2, 40 };
    t_circle staticCirc = { 300, 400, 80 };
    
    bool collision = false;
    
    SetTargetFPS(60);
    
    //main game loop
    while (!WindowShouldClose())
    {
        //move circle with mouse
        moveCirc.x = GetMouseX();
        moveCirc.y = GetMouseY();
        
        //collision means distance between circles is smaller than the sum of the radii
        collision = sqrt(pow(staticCirc.x - moveCirc.x, 2) + pow(staticCirc.y - moveCirc.y, 2)) <= (staticCirc.radius + moveCirc.radius);
        
        BeginDrawing();
        
        ClearBackground(BLACK);
        
        DrawCircle(staticCirc.x, staticCirc.y, staticCirc.radius, WHITE);
        DrawCircle(moveCirc.x, moveCirc.y, moveCirc.radius, collision ? RED : SKYBLUE);
        
        EndDrawing();
    }
    
    CloseWindow();
    
    return(0);
}
