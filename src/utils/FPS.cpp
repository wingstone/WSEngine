#include <utils/FPS.h>

FPS::FPS()
{
    lastTime = 0.f;
    currentTime = 0.f;
    fps = 0.f;
    deltaTime = 0.f;
}

void FPS::Update()
{
    currentTime = (float)glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    fps = 1/deltaTime;
}

float FPS::GetDeltTime()
{
    return deltaTime;
}

float FPS::GetFps()
{
    return fps;
}