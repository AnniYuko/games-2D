#include "raylib.h"

Vector2 my_add_vectors(Vector2 v1, Vector2 v2)
{ 
    return ((Vector2) { v1.x + v2.x, v1.y + v2.y });
}

int main(void)
{
    const int screenWidth = 1000;
    const int screenHeight = 700;
    
    InitWindow(screenWidth, screenHeight, "Vectors, Velocity, Gravity");
    
    float circRad = 35;
    Vector2 circPos = { 400, 200 };
    Vector2 veloc = { 0.5, 5 };
    Vector2 grav = { 0, 0.2 };
    
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        //add velocity to position of the circle
        circPos = my_add_vectors(circPos, veloc);
        
        //add gravity to velocity
        if (veloc.y != 0)
            veloc = my_add_vectors(veloc, grav);
        
        if (circPos.y > (screenHeight - circRad))
        {
            circPos.y = screenHeight - circRad;
            veloc.y *= -0.9;
        }
        else if (circPos.x > (screenWidth - circRad))
            veloc.x *= -1;
        
        BeginDrawing();
        
        ClearBackground(BLACK);
        DrawCircle(circPos.x, circPos.y, circRad, LIME);

        EndDrawing();
    }
    
    CloseWindow();
    
    return(0);
}
