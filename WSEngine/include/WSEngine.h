#ifndef WSENGINE_H
#define WSENGINE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <functional>

#include "component/Camera.h"
#include "component/Light.h"
#include "component/MeshRender.h"
#include "component/Transform.h"

#include "core/Component.h"
#include "core/Entity.h"
#include "core/Material.h"
#include "core/Mesh.h"
#include "core/ShaderClass.h"
#include "core/Texture.h"

#include "utils/FPS.h"
#include "utils/GlobalLog.h"
#include "utils/imgui_impl_glfw.h"
#include "utils/imgui_impl_opengl3.h"
#include "utils/stb_image.h"
#include "utils/ResourcesManager.h"
#include "utils/RenderManager.h"
#include "utils/EntityManager.h"

using namespace std;
using namespace glm;

class WSEngine
{
public:
    typedef function<void()> UiCallBack;

private:
    unsigned int _width, _height;
    string _title;
    GLFWwindow *_window;
    UiCallBack _uiCallBack;


public:
    WSEngine();
    ~WSEngine();

    //main
    bool Init(int width = 1280, int height = 720, const char* title = "WSEngine");
    void Run(UiCallBack uiCallBack);
    void Quit();

    //other
    int GetWidth()
    {
        return _width;
    }

    int GetHeight()
    {
        return _height;
    }

    void SetUiCallBack(UiCallBack uicallback)
    {
        _uiCallBack = uicallback;
    }

};

#endif