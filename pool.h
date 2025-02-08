#include <stdio.h>
#include <stdlib.h>
#include "raylib.h"

typedef struct Colour { // alternative to raylibs Color, uses british spelling and floats
    float r;
    float g;
    float b;
    float a;
} Colour;

typedef struct Particle {
    Vector2 position;
    Vector2 velocity;
    Colour color;
    float size;
    float distance;
    bool visible;
} Particle;

typedef struct Pool {
    int size;
    Mesh mesh;
    Model model;
    Shader shader;
    Particle *particles;
    bool visible;
} Pool;

Pool NewPool(int size, char *vert, char *frag){
    Pool newPool;
    newPool.size = size;
    newPool.shader = LoadShader(vert, frag);
    newPool.particles = malloc(size * sizeof(Particle));
    newPool.mesh = (Mesh){ 0 };
    newPool.mesh.triangleCount = size * 2;
    int vc = newPool.mesh.triangleCount * 3;
    newPool.mesh.vertexCount = vc;
    newPool.mesh.vertices = (float *)MemAlloc(vc * 3 * sizeof(float));
    newPool.mesh.texcoords = (float *)MemAlloc(vc * 2 * sizeof(float));
    newPool.mesh.colors = (unsigned char *)MemAlloc(vc * 4);
    return newPool;
}

void SetTexcoordsToMesh(Pool *pool)
{
    int tc[] = {0,0,1,0,0,1,1,1,0,1,1,0};
    for (int i = 0; i < (*pool).size; i++)
    {
        int di = i * 12;
        for (int j = 0; j < 12; j++){ (*pool).mesh.texcoords[di + j] = tc[j]; }
    }
}

void CopyVerticesToMesh(Pool *pool){
    for (int i = 0; i < (*pool).size; i++)
    {
        float ts = (*pool).particles[i].size;
        float tx = (*pool).particles[i].position.x;
        float ty = (*pool).particles[i].position.y;
        float txx = tx + ts;
        float tyy = ty + ts;
        float vc[] = { tx,ty,0,  txx,ty,0,  tx,tyy,0,  txx,tyy,0,  tx,tyy,0,  txx,ty,0 };
        int tri = i * 18;
        for (int j = 0; j < 18; j++){ (*pool).mesh.vertices[tri + j] = vc[j]; }
    }
}

void CopyColorsToMesh(Pool *pool){
    for (int i = 0; i < (*pool).size; i++)
    {
        int ci = i * 24;
        Colour color = (*pool).particles[i].color;
        char vc[] = { color.r,  color.g,  color.b,  color.a};
        for (int j = 0; j < 24; j++){ (*pool).mesh.colors[ci + j] = vc[j % 4]; }
    }
}

void setParticle(Pool *pool, int p, float x, float y, float xv, float yv, float size, Colour color){
    (*pool).particles[p].position.x = x;
    (*pool).particles[p].position.y = y;
    (*pool).particles[p].velocity.x = xv;
    (*pool).particles[p].velocity.y = yv;
    (*pool).particles[p].size = size;
    (*pool).particles[p].visible = true;
    (*pool).particles[p].color = color;
}

void SetUpPool(Pool *pool){
    CopyColorsToMesh(pool);
    CopyVerticesToMesh(pool);
    SetTexcoordsToMesh(pool);
    UploadMesh(&(pool->mesh), true); // Alt - UploadMesh(&((*pool).mesh), true);
    (*pool).model = LoadModelFromMesh((*pool).mesh);
    (*pool).model.materials[0].shader = (*pool).shader;
}

void UpdatePool(Pool *pool){
    CopyColorsToMesh(pool);
    CopyVerticesToMesh(pool);
    SetTexcoordsToMesh(pool);
    UpdateMeshBuffer((*pool).mesh, 0, (*pool).mesh.vertices, sizeof(float) * (*pool).mesh.vertexCount * 3, 0);
    UpdateMeshBuffer((*pool).mesh, 3, (*pool).mesh.colors, (*pool).mesh.vertexCount * 4, 0);
}