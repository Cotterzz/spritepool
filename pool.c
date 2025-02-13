#include "pool.h"
#include <stdlib.h>
#include "raylib.h"

Pool NewPool(int size, const char *vert, const char *frag) {
    Pool newPool;
    newPool.size = size;
    newPool.pos = (Vector3){ 0.0f, 0.0f, 0.0f };
    newPool.shader = LoadShader(vert, frag);
    newPool.parts = (Part *)calloc(size, sizeof(Part));
    newPool.mesh = (Mesh){ 0 };
    newPool.mesh.triangleCount = size * 2;
    int vc = newPool.mesh.triangleCount * 3;
    newPool.mesh.vertexCount = vc;
    newPool.mesh.vertices = (float *)MemAlloc(vc * sizeof(float) * 3);
    newPool.mesh.texcoords = (float *)MemAlloc(vc * sizeof(float) * 2);
    newPool.mesh.colors = (unsigned char *)MemAlloc(vc * sizeof(unsigned char) * 4);
    return newPool;
}

Camera PoolCamera() {
    Camera camera = { 0 };
    camera.position = (Vector3){ 0.0f, 0.0f, 5.0f };
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
    camera.fovy = 50.0f;
    camera.projection = CAMERA_ORTHOGRAPHIC;
    return camera;
}

void DrawPool(Pool *pool) {
    DrawModel(pool->model, pool->pos, 1.0f, WHITE);
}

void SetTexcoordsToMesh(Pool *pool) {
    int tc[] = {0,0,1,0,0,1,1,1,0,1,1,0};
    for (int i = 0; i < pool->size; i++) {
        int di = i * 12;
        for (int j = 0; j < 12; j++) { pool->mesh.texcoords[di + j] = tc[j]; }
    }
}

void CopyVerticesToMesh(Pool *pool) {
    for (int i = 0; i < pool->size; i++) {
        float ts = pool->parts[i].size;
        float tx = pool->parts[i].pos.x;
        float ty = pool->parts[i].pos.y;
        float txx = tx + ts;
        float tyy = ty + ts;
        float vc[] = { tx,ty,0,  txx,ty,0,  tx,tyy,0,  txx,tyy,0,  tx,tyy,0,  txx,ty,0 };
        int tri = i * 18;
        for (int j = 0; j < 18; j++) { pool->mesh.vertices[tri + j] = vc[j]; }
    }
}

void CopyColorsToMesh(Pool *pool) {
    for (int i = 0; i < pool->size; i++) {
        int ci = i * 24;
        Colour color = pool->parts[i].color;
        unsigned char vc[] = { color.r, color.g, color.b, color.a };
        for (int j = 0; j < 24; j++) { pool->mesh.colors[ci + j] = vc[j % 4]; }
    }
}

void setPart(Pool *pool, int p, float x, float y, float xv, float yv, float size, Colour color) {
    pool->parts[p].pos.x = x;
    pool->parts[p].pos.y = y;
    pool->parts[p].vel.x = xv;
    pool->parts[p].vel.y = yv;
    pool->parts[p].size = size;
    pool->parts[p].visible = true;
    pool->parts[p].color = color;
}

void SetUpPool(Pool *pool) {
    CopyColorsToMesh(pool);
    CopyVerticesToMesh(pool);
    SetTexcoordsToMesh(pool);
    UploadMesh(&(pool->mesh), true);
    pool->model = LoadModelFromMesh(pool->mesh);
    pool->model.materials[0].shader = pool->shader;
}

void UpdatePool(Pool *pool) {
    CopyColorsToMesh(pool);
    CopyVerticesToMesh(pool);
    SetTexcoordsToMesh(pool);
    UpdateMeshBuffer(pool->mesh, 0, pool->mesh.vertices, sizeof(float) * pool->mesh.vertexCount * 3, 0);
    UpdateMeshBuffer(pool->mesh, 3, pool->mesh.colors, pool->mesh.vertexCount * sizeof(unsigned char) * 4, 0);
}

void FreePool(Pool *pool) {
    free(pool->parts);
    UnloadModel(pool->model);
    UnloadShader(pool->shader);
}