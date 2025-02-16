#pragma once

#include "raylib.h"

typedef struct Colour { float r, g, b, a; } Colour;

typedef struct Part {
    Vector2 pos;
    Vector2 vel;
    Colour color;
    float size;
    float distance;
    bool visible;
} Part;

typedef struct Pool {
    int size;
    Vector3 pos;
    Mesh mesh;
    Model model;
    Shader shader;
    Part *parts;
    bool visible;
} Pool;

Pool NewPool(int size, const char *vert, const char *frag);
Camera PoolCamera();
void DrawPool(Pool *pool);
void SetTexcoordsToMesh(Pool *pool);
void CopyVerticesToMesh(Pool *pool);
void CopyColorsToMesh(Pool *pool);
void setPart(Pool *pool, int p, float x, float y, float xv, float yv, float size, Colour color);
void SetUpPool(Pool *pool);
void UpdatePool(Pool *pool);
void FreePool(Pool *pool);