#define FP fireballs.particles

#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"
#include "pool.h"

int main(void)
{
    // Begin
    const int screenWidth = 800;
    const int screenHeight = 800;

    float interval = 0.001;
    float lastTick = GetTime();

    int active = 0;

    int poolsize = 1500;

    InitWindow(screenWidth, screenHeight, "Window Title");

    Pool fireballs = NewPool(poolsize, 0, "fireballs.frag");
    Pool *fptr = &fireballs;
    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 0.0f, 5.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 50.0f;
    camera.projection = CAMERA_ORTHOGRAPHIC;
    Vector3 position = { 0.0f, 0.0f, 0.0f };

    SetTargetFPS(120);

    SetUpPool(fptr);

    // Main loop
    while (!WindowShouldClose())
    {
        if(GetTime() - lastTick >= interval){
            lastTick = GetTime();
            if(active<poolsize)
            {
                if(FP[active].visible == false)
                {
                    int ct = GetRandomValue(200, 600);
                    int cb = (ct>510) ? ct-510 : 0;
                    int cg = (ct>255) ? ct-255-cb : 0;
                    int cr = (ct<256) ? ct : 255;
                    float newSize = (float)GetRandomValue(50, 300)/500.0f;
                    float newVX = (float)GetRandomValue(18, 22)/100.0f;
                    float newVY = (float)GetRandomValue(8, 12)/100.0f;
                    setParticle(fptr, active, -20.0, 0.0, newVX, newVY, newSize, (Colour){ cr,cg,cb,255 });
                }
                active+=1;
            }  
        }
        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(camera);
               DrawModel(fireballs.model, position, 1.0f, WHITE);
            EndMode3D();
            DrawFPS(10, 10);
        EndDrawing();

        for (int i = 0; i < active; i++)
        {
            FP[i].position.x += FP[i].velocity.x;
            FP[i].position.y += FP[i].velocity.y;
            //FP[i].velocity.x +=(float)GetRandomValue(-1, 1)/5000.0f;
            FP[i].velocity.y -=0.005;
            if(FP[i].size<5){FP[i].size += 0.005;}
            if(FP[i].color.g<200){FP[i].color.g += 0.5;}
            if(FP[i].color.b<55){FP[i].color.b += 0.5;}
            if ((FP[i].position.x + FP[i].size) > 22) {FP[i].velocity.x *= -1;FP[i].position.x=22-(FP[i].size);}
            if ((FP[i].position.x ) < -22) {FP[i].velocity.x *= -1;FP[i].position.x=-22;}
            if ((FP[i].position.y + FP[i].size) > 22) {FP[i].velocity.y *= -1;FP[i].position.y=22-(FP[i].size);}
            if ((FP[i].position.y ) < -22) {FP[i].velocity.y *= -0.9;FP[i].position.y=-22;}
        }
        UpdatePool(fptr);
    }

    free(FP);
    UnloadModel(fireballs.model);
    CloseWindow();
    return 0;
}

