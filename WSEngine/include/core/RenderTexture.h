#pragma once

class RenderTexture
{
public:
    unsigned int framebuffer;
    unsigned int textureColorbuffer;
    unsigned int rbo;

public:
    RenderTexture()
    {
    }
    ~RenderTexture()
    {
    }
};
