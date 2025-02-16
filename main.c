#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#include "pool.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Window Title");

    float interval = 0.001;
    float lastTick = GetTime();
    int active = 0;
    int poolsize = 1500;

    Pool fb = NewPool(poolsize, 0, "fireballs.frag");
    Pool *fptr = &fb;
    Camera camera = PoolCamera();

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
               DrawPool(fptr);
            EndMode3D();
            DrawFPS(10, 10);
        EndDrawing();

        for (int i = 0; i < active; i++)
        {
            fb.parts[i].pos.x += fb.parts[i].vel.x;
            fb.parts[i].pos.y += fb.parts[i].vel.y;
            fb.parts[i].vel.y -=0.005;
            if(fb.parts[i].size<5){fb.parts[i].size += 0.005;}
            if(fb.parts[i].color.g<200){fb.parts[i].color.g += 0.5;}
            if(fb.parts[i].color.b<55){fb.parts[i].color.b += 0.5;}
            if ((fb.parts[i].pos.x + fb.parts[i].size) > 22) {fb.parts[i].vel.x *= -1;fb.parts[i].pos.x=22-(fb.parts[i].size);}
            if ((fb.parts[i].pos.x ) < -22) {fb.parts[i].vel.x *= -1;fb.parts[i].pos.x=-22;}
            if ((fb.parts[i].pos.y + fb.parts[i].size) > 22) {fb.parts[i].vel.y *= -1;fb.parts[i].pos.y=22-(fb.parts[i].size);}
            if ((fb.parts[i].pos.y ) < -22) {fb.parts[i].vel.y *= -0.9;fb.parts[i].pos.y=-22;}
        }
        UpdatePool(fptr);
    }

    FreePool(fptr);
    CloseWindow();
    return 0;
}

