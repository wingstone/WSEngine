#ifndef FPS_H
#define FPS_H

#include <GLFW/glfw3.h>

//se
class FPS
{
private:
    float lastTime;
    float currentTime;
    float fps;
    float deltaTime;

    FPS();

public:

     static FPS& Instance(){
        static FPS instance;
        return instance;

    }

    void Update();
    float GetFps();
    float GetDeltTime();
    
};

#endif