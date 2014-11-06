#pragma once
#include "cgg/cgg.h"

void custom_gl_draw(gl::Device* device);
void init();
void loadAssets();
void freeAssets();
void kill();
void update(float dt);
void draw3D();
void draw2D();
void mousePress(int32_t button, int32_t x, int32_t y);
void mouseRelease(int32_t button, int32_t x, int32_t y);
void mouseMove(int32_t x, int32_t y);
