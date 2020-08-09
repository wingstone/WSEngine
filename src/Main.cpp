#include <WSEngine.h>

using namespace std;
using namespace glm;

struct UiPeremeters
{
	bool show_demo_window;
	vec4 clear_color;
	Transform *trans;
	Light *light;
};
UiPeremeters gPeremeters;

void uicallback(WSEngine *engine)
{

	if (gPeremeters.show_demo_window)
		ImGui::ShowDemoWindow(&gPeremeters.show_demo_window);

	ImGui::Begin("WSEngine Ui!");

	ImGui::Checkbox("Demo Window", &gPeremeters.show_demo_window);
	ImGui::Separator();
	ImGui::ColorEdit4("clear color", (float *)&gPeremeters.clear_color);
	engine->SetClearColor(gPeremeters.clear_color);
	ImGui::Separator();
	ImGui::ColorEdit4("light color", (float *)&gPeremeters.light->lightColor);
	ImGui::Separator();
	ImGui::Text("TransForm");
	ImGui::InputFloat3("Position", (float *)&gPeremeters.trans->position);
	ImGui::InputFloat3("Rotation", (float *)&gPeremeters.trans->rotation);
	ImGui::InputFloat3("Scale", (float *)&gPeremeters.trans->scale);
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

	gPeremeters.clear_color = vec4(0.2f, 0.3f, 0.3f, 1.0f);
	gPeremeters.show_demo_window = true;
	gPeremeters.trans = monkey->GetComponent<Transform>();
	gPeremeters.light = sun->GetComponent<Light>();

	engine.Run(uicallback);
	cout << "complete run" << endl;

	engine.Quit();
	cout << "complete quit" << endl;

	return 0;
}