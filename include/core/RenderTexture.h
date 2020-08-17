#pragma once

class RenderTexture
{
public:
    unsigned int framebuffer;
    unsigned int textureColorbuffer;
    unsigned int rbo;
    unsigned int width;
    unsigned int height;

public:
    RenderTexture()
    {
    }
    ~RenderTexture()
    {
    }
};
