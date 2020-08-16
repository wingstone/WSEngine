#include <WSEngine.h>

WSEngine::WSEngine()
{
	_width = 0;
	_height = 0;
	_title = "";
	_window = nullptr;
}

WSEngine::~WSEngine()
{
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
	static bool firstMouse = true;
	static float lastX = 0;
	static float lastY = 0;
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = (float)ypos - lastY;
	yoffset = -yoffset; //reverse

	lastX = (float)xpos;
	lastY = (float)ypos;

	// if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	// {
	// 	Camera* camera = RenderManager::Instance().GetDefaultCamera()->GetComponent<Camera>();
	// 	camera->transform->rotation.y -= xoffset;
	// 	camera->transform->rotation.x -= yoffset;
	// }
	// else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS
	// 	&&glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
	// {
	// 	Camera* camera = RenderManager::Instance().GetDefaultCamera()->GetComponent<Camera>();
	// 	vec3 focuspos = camera->transform->position + camera->transform->Front()*camera->focusDist;
	// 	camera->transform->rotation.y -= xoffset;
	// 	camera->transform->rotation.x -= yoffset;
	// 	camera->transform->position =focuspos - camera->transform->Front()*camera->focusDist;
	// }

	//another interection
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		//right button
		Camera *camera = RenderManager::Instance().GetDefaultCamera()->GetComponent<Camera>();
		camera->transform->position += camera->transform->Right() * xoffset * 0.02f;
		camera->transform->position -= camera->transform->Up() * yoffset * 0.02f;
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
	{
		//left button
		Camera *camera = RenderManager::Instance().GetDefaultCamera()->GetComponent<Camera>();
		vec3 focuspos = camera->transform->position + camera->transform->Front() * camera->focusDist;

		static float totalRotY = 0;
		totalRotY += yoffset;

		camera->transform->rotation.y -= xoffset;
		if (abs(totalRotY) < 90)
		{
			camera->transform->rotation.x -= yoffset;
		}
		camera->transform->position = focuspos - camera->transform->Front() * camera->focusDist;
	}
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
	Camera *camera = RenderManager::Instance().GetDefaultCamera()->GetComponent<Camera>();
	camera->transform->position += camera->transform->Front() * (float)yoffset * 0.1f;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

bool WSEngine::Init(int width, int height, const char *title)
{
	_width = width;
	_height = height;
	_title = title;

	//window init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	_window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);
	if (_window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(_window);
	glfwSwapInterval(1); // Enable vsync

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return false;
	}

	glfwSetFramebufferSizeCallback(_window, framebuffer_size_callback);
	glfwSetCursorPosCallback(_window, mouse_callback);
	glfwSetScrollCallback(_window, scroll_callback);

	int display_w, display_h;
	glfwGetFramebufferSize(_window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	(void)io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(_window, true);
	const char *glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);

	//manager init
	ResourcesManager::Instance().Init();
	RenderManager::Instance().Init();
	EntityManager::Instance().Init();

	return true;
}

void WSEngine::Run(UiCallBack uiCallBack)
{
	while (!glfwWindowShouldClose(_window))
	{
		processInput(_window);
		FPS::Instance().Update();

		RenderManager::Instance().Render();

		//imgui Start
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		uiCallBack();

		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		//imgui end

		glfwSwapBuffers(_window);
		glfwPollEvents();
	}
}

void WSEngine::Quit()
{
	//imgui Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	RenderManager::Instance().Quit();
	GlobalLog::Log("render manager quit complete");
	EntityManager::Instance().Quit();
	GlobalLog::Log("entity manager quit complete");
	ResourcesManager::Instance().Quit();
	GlobalLog::Log("resource manager quit complete");
}