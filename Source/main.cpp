/*
The Glide3D Game Engine by Samuel Rasquinha
*/

/* Contributors
kiroma 
hazurl
*/

#include "Core/GlideEngine.h"

using namespace Glide3D;

FPSCamera camera(70, 800.0f / 600.0f, 0.2f, 1000.0f, 0.25f);
bool wireframe = false;
bool cursor_locked = true;
float exposure = 0.1f;
float camera_speed = 0.02f;
std::stringstream camera_props;

class MyApp : public Application
{
public:
	void OnUserCreate(double ts) override
	{

	}

	void OnUserUpdate(double ts) override
	{

	}

	void OnImguiRender(double ts) override
	{
		ImGuiWindowFlags window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_NoScrollbar;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoBackground;

		if (ImGui::Begin("Settings", false, window_flags))
		{
			const glm::vec3& position = camera.GetPosition();
			const glm::vec3& front = camera.GetFront();

			ImGui::SliderFloat("Exposure", &exposure, 0.0f, 5.0f);
			ImGui::SliderFloat("Speed", &camera_speed, 0.01f, 2.0f);
			ImGui::Text("Camera Position : %f, %f, %f", position.x, position.y, position.z);
			ImGui::Text("Camera Front : %f, %f, %f", front.x, front.y, front.z);
			ImGui::End();
		}   
	}

	void OnEvent(Event e) override
	{
		if (e.type == EventTypes::MouseMove)
		{
			camera.UpdateOnMouseMovement(e.mx, e.my);
		}

		else if (e.type == EventTypes::WindowResize)
		{
			if (e.wx > 1 && e.wy > 1) // To prevent division by zero
			{
				camera.SetAspect((float)e.wx / (float)e.wy); // No integer division!
			}
		}

		else if (e.type == EventTypes::KeyPress && e.key == GLFW_KEY_V)
		{
			static bool vsync = true;
			vsync = !vsync;
			if (vsync)
			{
				glfwSwapInterval(1);
			}

			else
			{
				glfwSwapInterval(0);
			}
		}

		else if (e.type == EventTypes::KeyPress && e.key == GLFW_KEY_ESCAPE)
		{
			cursor_locked = !cursor_locked;
		}

		else if (e.type == EventTypes::KeyPress && e.key == GLFW_KEY_M)
		{
			wireframe = !wireframe;
		}
	}

};

MyApp app;

int main()
{
	app.Initialize();
	glfwSwapInterval(1);

	Glide3D::Renderer &renderer = app.GetRenderer();
	GLFWwindow* window = app.GetWindow();
	GLClasses::Framebuffer FBO(800, 600, true);

	CubeObject cube;

	Entity entity(&cube);
	Entity entity1(&cube);
	Entity entity2(&cube);
	Entity entity3(&cube);

	entity.GetTransform().Translate(glm::vec3(4, 0, 0));

	camera.SetPosition(glm::vec3(0, 0, -2));

	Object object_1;
	Object object_2;
	Object object_3;
	CubeObject object_4;
	CubeObject floor_obj;
	CubeObject block;
	CubeObject block_1;
	Object object_5;

	// Load all the OBJ files
	//FileLoader::LoadOBJFile(&object_1, "Resources/teapot.model");

	FileLoader::LoadOBJFile(&object_1, "Resources/models/sponza/quintessentials.model");
	FileLoader::LoadOBJFile(&object_2, "Resources/suzanne.model");
	FileLoader::LoadOBJFile(&object_3, "Resources/12305_backpack_v2_l3.model");
	FileLoader::LoadOBJFile(&object_5, "Resources/globe-sphere.model");
	floor_obj.AddTextureMapToMesh("Resources/marble.jpg", TextureType::Albedo);

	object_1.p_CanFacecull = true;
	object_4.AddTextureMapToMesh("Resources/brickwall.jpg", TextureType::Albedo, true);
	object_4.AddTextureMapToMesh("Resources/brickwall_normal.jpg", TextureType::Normal, true);

	block.AddTextureMapToMesh("Resources/end_bricks.png", TextureType::Albedo, true);
	block.AddTextureMapToMesh("Resources/end_bricks_n.png", TextureType::Normal, true);
	block_1.AddTextureMapToMesh("Resources/bricks.png", TextureType::Albedo, true);
	block_1.AddTextureMapToMesh("Resources/bricks_n.png", TextureType::Normal, true);

	Entity sponza(&object_1);
	Entity suzanne(&object_2);
	Entity backpack(&object_3);
	Entity brickwall(&object_4);
	Entity sphere(&object_5);
	Entity floor_entity(&floor_obj);
	Entity block0_entity(&block);
	Entity block1_entity(&block_1);

	// Pot Object
	sponza.GetTransform().Translate(glm::vec3(15, 0, 0));
	sponza.GetTransform().Scale(glm::vec3(0.1f));

	// Suzanne Object
	suzanne.GetTransform().Translate(glm::vec3(20, 0, 0));

	// Backpack
	backpack.GetTransform().Translate(glm::vec3(24, 0, 0));
	backpack.GetTransform().Scale(glm::vec3(0.1f, 0.1f, 0.1f));
	backpack.GetTransform().Rotate(-90.0f);
	backpack.GetTransform().Rotate(-55.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	// The sphere
	sphere.GetTransform().Translate(glm::vec3(1, 0, 0));
	sphere.GetTransform().Scale(glm::vec3(2, 2, 2));

	// Brick Wall
	brickwall.GetTransform().Translate(glm::vec3(27, 0, 0));
	brickwall.GetTransform().Scale(glm::vec3(10, 10, 0.5f));

	// The floor
	floor_entity.GetTransform().Translate(glm::vec3(-100, -4, -100));
	floor_entity.GetTransform().Scale(glm::vec3(200, 2, 200));

	// The blocks
	block0_entity.GetTransform().Translate(glm::vec3(40, 0, 0));
	block0_entity.GetTransform().Scale(glm::vec3(2, 2, 2));
	block1_entity.GetTransform().Translate(glm::vec3(45, 0, 0));
	block1_entity.GetTransform().Scale(glm::vec3(2, 2, 2));

	glm::vec3 light_dir = glm::vec3(0.00349f, -0.59832f, -0.80124f);

	DirectionalLight d_light({ -300.0f, 300.0f, -300.0f, 300.0f, 0.1f, 300.0f }, {4096,4096 });

	d_light.m_Direction = light_dir;
	d_light.m_ShadowPosition = glm::vec3(90, 193, 65); // 10, 70, 10
	d_light.m_SpecularStrength = 2.0f;
	d_light.m_SpecularExponent = 32;
	d_light.m_IsBlinn = true;
	d_light.m_UpdateRate = 0;

	/*DirectionalLight d_light2;

	d_light2.m_Direction = glm::vec3(-0.0066f, -0.6427f, 0.7660f);
	d_light2.m_ShadowPosition = glm::vec3(16.0f, 16.0f, -19.0f);
	d_light2.m_SpecularStrength = 0.0f;
	d_light2.m_SpecularExponent = 0;
	d_light2.m_IsBlinn = true;*/

	PointLight p_light; // -73 8 81 
	p_light.m_Position = glm::vec3(-73, 8, 81);
	p_light.m_SpecularStrength = 0.01f;

	p_light.m_SpecularExponent = 1;
	p_light.m_IsBlinn = true;
	p_light.m_Linear = 0.09f;
	p_light.m_Quadratic = 0.0032f;
	p_light.m_Constant = 0.1f;


	Skybox skybox({
		"Resources/skybox/px.png",
		"Resources/skybox/nx.png",
		"Resources/skybox/py.png",
		"Resources/skybox/ny.png",
		"Resources/skybox/pz.png",
		"Resources/skybox/nz.png"
		});

	renderer.SetEnvironmentMap(skybox);
	renderer.AddDirectionalLight(&d_light);
	renderer.AddPointLight(p_light);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	renderer.AddEntity(&sponza);

	//renderer.AddEntities({ &entity });
	//renderer.AddEntities({ &suzanne });
	//renderer.AddEntities({ &backpack });
	//renderer.AddEntities({ &sphere });
	//renderer.AddEntities({ &brickwall });
	//renderer.AddEntities({ &block0_entity });
	//renderer.AddEntities({ &block1_entity });
	//renderer.AddEntities({ &floor_entity });

	while (!glfwWindowShouldClose(app.GetWindow()))
	{
		FBO.SetExposure(exposure);

		app.OnUpdate();
		app.SetCursorLocked(cursor_locked);

		// Clear the framebuffer
		FBO.Bind();
		FBO.OnUpdate(app.GetWidth(), app.GetHeight(), wireframe);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			glm::vec3 front = -glm::cross(camera.GetRight(), camera.GetUp());
			camera.ApplyAcceleration(front * camera_speed);
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			glm::vec3 back = glm::cross(camera.GetRight(), camera.GetUp());
			camera.ApplyAcceleration(back * camera_speed);
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.ApplyAcceleration(-(camera.GetRight() * camera_speed));
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.ApplyAcceleration(camera.GetRight() * camera_speed);
		}

		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			camera.ApplyAcceleration(camera.GetUp() * camera_speed);
		}

		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			camera.ApplyAcceleration(-(camera.GetUp() * camera_speed)); 
		}

		renderer.Render(&camera, FBO);  
		renderer.RenderFBO(FBO);
		camera.OnUpdate();
		camera.ResetAcceleration();
		app.FinishFrame(); 

		GLClasses::DisplayFrameRate(app.GetWindow(), "Glide3D ");
	}
} 