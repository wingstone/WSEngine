#include <WSEngine.h>

using namespace std;
using namespace glm;

unsigned int width = 1280;
unsigned int height = 720;

struct UiPeremeters
{
	bool show_demo_window;
	bool show_tips_window;
	bool useDepthTest;
	bool useHDR;
	bool useMSAA;
	bool cullback;
	bool oldDepthTest;
	bool oldHDR;
	bool oldMSAA;
	bool oldcullback;
	Transform *modelTrans;
	Light *light;
	vec4 ambientcolor;
	RenderTexture *rt;
	float autoExplosion;
};
UiPeremeters gPeremeters;

void uicallback()
{

	if (gPeremeters.show_demo_window)
		ImGui::ShowDemoWindow(&gPeremeters.show_demo_window);

	if (gPeremeters.show_tips_window)
	{
		ImGui::Begin("Operation Tips!", &gPeremeters.show_tips_window);

		ImGui::Text("Press Alt and move Left Button to rotate view");
		ImGui::Separator();
		ImGui::Text("Roll the scroll wheel to zoom view");
		ImGui::Separator();
		ImGui::Text("Move Right Button to move view");

		ImGui::End();
	}

	ImGui::Begin("WSEngine Ui!");

	ImGui::Checkbox("Demo Window", &gPeremeters.show_demo_window);
	ImGui::Checkbox("Tips Window", &gPeremeters.show_tips_window);
	ImGui::Separator();

	ImGui::Checkbox("Use Depth Test", &gPeremeters.useDepthTest);
	if (gPeremeters.useDepthTest != gPeremeters.oldDepthTest)
	{
		RenderManager::Instance().SetDepthTest(gPeremeters.useDepthTest);
		gPeremeters.oldDepthTest = gPeremeters.useDepthTest;
	}
	ImGui::Checkbox("Use HDR", &gPeremeters.useHDR);
	if (gPeremeters.useHDR != gPeremeters.oldHDR)
	{
		RenderManager::Instance().SetHDR(gPeremeters.useHDR);
		gPeremeters.oldHDR = gPeremeters.useHDR;
	}
	ImGui::Checkbox("Use MSAA", &gPeremeters.useMSAA);
	if (gPeremeters.useMSAA != gPeremeters.oldMSAA)
	{
		RenderManager::Instance().SetMSAA(gPeremeters.useMSAA);
		gPeremeters.oldMSAA = gPeremeters.useMSAA;
	}
	ImGui::Checkbox("Cull Back", &gPeremeters.cullback);
	RenderManager::Instance().SetCullBack(gPeremeters.cullback);
	ImGui::Separator();

	ImGui::Text("tmp RT");
	ImGui::Image((void *)(intptr_t)(gPeremeters.rt->textureColorbuffer), ImVec2(256, 180), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::Separator();

	ImGui::ColorEdit4("ambient color", (float *)&gPeremeters.ambientcolor);
	RenderManager::Instance().SetAmbientColor(gPeremeters.ambientcolor);
	ImGui::ColorEdit4("light color", (float *)&gPeremeters.light->lightColor);
	ImGui::SliderFloat("light intensity", (float *)&gPeremeters.light->lightIntensity, 0.0f, 5.0f);
	ImGui::SliderFloat("Auto Explosioin", (float *)&gPeremeters.autoExplosion, 0.1f, 5.0f);
	RenderManager::Instance().SetAutoExplosion(gPeremeters.autoExplosion);
	ImGui::Separator();

	ImGui::Text("TransForm");
	ImGui::SliderFloat3("Position", (float *)&gPeremeters.modelTrans->position, -2.0f, 2.0f);
	ImGui::SliderFloat3("Rotation", (float *)&gPeremeters.modelTrans->rotation, -180.0f, 180.0f);
	ImGui::SliderFloat3("Scale", (float *)&gPeremeters.modelTrans->scale, 0.0f, 2.0f);
	ImGui::Separator();

	float fps = FPS::Instance().GetFps();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / fps, fps);

	ImGui::End();
}

int main()
{
	WSEngine engine;
	if (!engine.Init())
	{
		return -1;
	}
	cout << "complete init" << endl;

	//resource loading
	vector<string> outnames;
	vector<Mesh *> meshs = ResourcesManager::Instance().LoadMesh("../../assets/house.fbx", &outnames);
	ShaderClass *shader = ResourcesManager::Instance().LoadShader("../../assets/pbrVert.txt", "../../assets/pbrFrag.txt", "pbr");
	PBRMaterial *mat = ResourcesManager::Instance().CreateMaterial<PBRMaterial>(shader, "pbrmat");
	// Texture *mask = ResourcesManager::Instance().LoadTexture("../../assets/mask.tga", "mask");
	// mat->textures.push_back(mask);

	//mesh entity
	Entity *monkey = EntityManager::Instance().CreateEntity();
	MeshRender *meshRender = monkey->AddComponent<MeshRender>();
	meshRender->SetMaterial(mat);
	meshRender->SetMesh(meshs[0]);

	//light entity
	Entity *sun = EntityManager::Instance().CreateEntity();
	Light *lightcom = sun->AddComponent<Light>();
	lightcom->lightIntensity = 0.7;

	//gperameters setting
	gPeremeters.show_demo_window = false;
	gPeremeters.show_tips_window = true;
	gPeremeters.useDepthTest = true;
	gPeremeters.useHDR = true;
	gPeremeters.useMSAA = true;
	gPeremeters.oldDepthTest = true;
	gPeremeters.oldHDR = true;
	gPeremeters.oldMSAA = true;
	gPeremeters.cullback = true;
	gPeremeters.ambientcolor = vec4(0.5, 0.5, 0.4, 1.0);
	gPeremeters.autoExplosion = 1.0f;
	gPeremeters.modelTrans = monkey->GetComponent<Transform>();
	gPeremeters.light = sun->GetComponent<Light>();
	gPeremeters.rt = RenderManager::Instance().GetRenderTexture();

	engine.Run(uicallback);
	cout << "complete run" << endl;

	engine.Quit();
	cout << "complete quit" << endl;

	return 0;
}