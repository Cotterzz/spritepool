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

    Pool fb = NewPool(poolsize, 0, "fireballs.frag");
    Pool *fptr = &fb;
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
                if(fb.parts[active].visible == false)
                {
                    int ct = GetRandomValue(200, 600);
                    int cb = (ct>510) ? ct-510 : 0;
                    int cg = (ct>255) ? ct-255-cb : 0;
                    int cr = (ct<256) ? ct : 255;
                    float newSize = (float)GetRandomValue(50, 300)/500.0f;
                    float newVX = (float)GetRandomValue(18, 22)/100.0f;
                    float newVY = (float)GetRandomValue(8, 12)/100.0f;
                    setPart(fptr, active, -20.0, 0.0, newVX, newVY, newSize, (Colour){ cr,cg,cb,255 });
                }
                active+=1;
            }  
        }
        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(camera);
               DrawModel(fb.model, position, 1.0f, WHITE);
            EndMode3D();
            DrawFPS(10, 10);
        EndDrawing();

        for (int i = 0; i < active; i++)
        {
            fb.parts[i].position.x += fb.parts[i].velocity.x;
            fb.parts[i].position.y += fb.parts[i].velocity.y;
            fb.parts[i].velocity.y -=0.005;
            if(fb.parts[i].size<5){fb.parts[i].size += 0.005;}
            if(fb.parts[i].color.g<200){fb.parts[i].color.g += 0.5;}
            if(fb.parts[i].color.b<55){fb.parts[i].color.b += 0.5;}
            if ((fb.parts[i].position.x + fb.parts[i].size) > 22) {fb.parts[i].velocity.x *= -1;fb.parts[i].position.x=22-(fb.parts[i].size);}
            if ((fb.parts[i].position.x ) < -22) {fb.parts[i].velocity.x *= -1;fb.parts[i].position.x=-22;}
            if ((fb.parts[i].position.y + fb.parts[i].size) > 22) {fb.parts[i].velocity.y *= -1;fb.parts[i].position.y=22-(fb.parts[i].size);}
            if ((fb.parts[i].position.y ) < -22) {fb.parts[i].velocity.y *= -0.9;fb.parts[i].position.y=-22;}
        }
        UpdatePool(fptr);
    }

    FreePool(fptr);
    CloseWindow();
    return 0;
}

