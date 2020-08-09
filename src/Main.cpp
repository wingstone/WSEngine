#include <WSEngine.h>

using namespace std;
using namespace glm;

unsigned int width = 1280;
unsigned int height = 720;

struct UiPeremeters
{
	bool show_demo_window;
	vec4 clearColor;
	Transform *trans;
	Light *light;
	vec4 ambientcolor;
	RenderTexture *rt;
};
UiPeremeters gPeremeters;

void uicallback(WSEngine *engine)
{

	if (gPeremeters.show_demo_window)
		ImGui::ShowDemoWindow(&gPeremeters.show_demo_window);

	ImGui::Begin("WSEngine Ui!");

	ImGui::Checkbox("Demo Window", &gPeremeters.show_demo_window);
	ImGui::Separator();
	ImGui::Text("tmp RT");
	ImGui::Image((void *)(intptr_t)(gPeremeters.rt->textureColorbuffer), ImVec2(256, 180), ImVec2(1, 1), ImVec2(0, 0));
	ImGui::Separator();
	ImGui::ColorEdit4("clear color", (float *)&gPeremeters.clearColor);

	engine->SetClearColor(gPeremeters.clearColor);
	ImGui::ColorEdit4("ambient color", (float *)&gPeremeters.ambientcolor);
	engine->renderManager.SetAmbientColor(gPeremeters.ambientcolor);
	ImGui::Separator();
	ImGui::ColorEdit4("light color", (float *)&gPeremeters.light->lightColor);
	ImGui::Separator();
	ImGui::Text("TransForm");
	ImGui::SliderFloat3("Position", (float *)&gPeremeters.trans->position, -2.0f, 2.0f);
	ImGui::SliderFloat3("Rotation", (float *)&gPeremeters.trans->rotation, -180.0f, 180.0f);
	ImGui::SliderFloat3("Scale", (float *)&gPeremeters.trans->scale, 0.0f, 2.0f);
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
	vector<Mesh *> meshs = engine.resourceManager.LoadMesh("../../assets/monkey.fbx", &outnames);
	ShaderClass *shader = engine.resourceManager.LoadShader("../../assets/vert.txt", "../../assets/frag.txt", "pbr");
	PBRMaterial *mat = engine.resourceManager.CreateMaterial<PBRMaterial>(shader, "pbrmat");
	RenderTexture *rt = engine.resourceManager.CreateRenderTexture(width, height, "copyrt");
	engine.SetRenderTexture(rt);

	//mesh entity
	Entity *monkey = engine.entityManager.CreateEntity();
	MeshRender *meshRender = monkey->AddComponent<MeshRender>();
	meshRender->SetMaterial(mat);
	meshRender->SetMesh(meshs[0]);

	//light entity
	Entity *sun = engine.entityManager.CreateEntity();
	sun->AddComponent<Light>();

	//camera entity
	Entity *maincam = engine.entityManager.CreateEntity();
	Transform *trans = maincam->GetComponent<Transform>();
	trans->position = glm::vec3(0.0f, 0.0f, 5.0f);
	trans->rotation.y = 180.0f;
	maincam->AddComponent<Camera>();

	gPeremeters.clearColor = vec4(0.2f, 0.3f, 0.3f, 1.0f);
	gPeremeters.ambientcolor = vec4(0.5, 0.5, 0.5, 1.0);
	gPeremeters.show_demo_window = true;
	gPeremeters.trans = monkey->GetComponent<Transform>();
	gPeremeters.light = sun->GetComponent<Light>();
	gPeremeters.rt = rt;

	engine.Run(uicallback);
	cout << "complete run" << endl;

	engine.Quit();
	cout << "complete quit" << endl;

	return 0;
}