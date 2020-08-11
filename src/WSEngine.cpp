#include <WSEngine.h>

WSEngine::WSEngine() : renderManager(this), entityManager(this), resourceManager(this)
{
	_width = 1280;
	_height = 720;
	_title = "WSEngine";
	_window = nullptr;
	renderTexture = nullptr;

	quadShader = nullptr;
	quadMesh = nullptr;
}

WSEngine::~WSEngine()
{
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

bool WSEngine::Init()
{
	//window init
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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

	int display_w, display_h;
	glfwGetFramebufferSize(_window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);

	renderManager.SetAmbientColor(vec4(0.4f, 0.4f, 0.4f, 1.0f));

	//postprocess
	quadShader = resourceManager.LoadShader("../../assets/quadVert.txt", "../../assets/quadFrag.txt", "quad");
	vector<Vertex> quadverts;
	vector<unsigned int> quadindes;
	GeometryGenerator::GenerateQuad(quadverts, quadindes);
	quadMesh = resourceManager.CreateMesh(quadverts, quadindes, "quad");

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

	//skybox
	ShaderClass* shader = resourceManager.LoadShader("../../assets/skyboxVert.txt", "../../assets/skyboxFrag.txt", "skybox");
	vector<Vertex> skyboxverts;
	vector<unsigned int> skyboxindes;
	GeometryGenerator::GenerateSkyBox(skyboxverts, skyboxindes);
	Mesh* mesh = resourceManager.CreateMesh(skyboxverts, skyboxindes, "skybox");
	renderManager.Init(shader, mesh);

	return true;
}

void WSEngine::Run(UiCallBack uiCallBack)
{
	while (!glfwWindowShouldClose(_window))
	{
		processInput(_window);
		FPS::Instance().Update();

		if (renderTexture != nullptr)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, renderTexture->framebuffer);
			glEnable(GL_DEPTH_TEST);
			glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (!renderManager.Render())
				break;

			//postprocess
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glDisable(GL_DEPTH_TEST);
			glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			quadShader->use();
			quadShader->setTexture("screenTexture", renderTexture->textureColorbuffer, 0);
			quadMesh->DrawMesh();
		}
		else
		{
			glEnable(GL_DEPTH_TEST);
			glClearColor(_clearColor.x, _clearColor.y, _clearColor.z, _clearColor.w);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			if (!renderManager.Render())
				break;
		}

		//imgui Start
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		uiCallBack(this);

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

	renderManager.Quit();
	GlobalLog::Log("render manager quit complete");
	entityManager.Quit();
	GlobalLog::Log("entity manager quit complete");
	resourceManager.Quit();
	GlobalLog::Log("resource manager quit complete");
}