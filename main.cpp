#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <math.h>
#include <ctime>
#include <chrono>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"
#include "Texture.h"
#include "Renderer.h"
#include "RenderSystem.h"
#include "ScriptSystem.h"
#include "ScriptManager.h"
#include "FirstPersonCameraScript.h"
#include "Script.h"
#include "LifeBarScript.h"
#include "PlayerManagerScript.h"
#include "PowerUpScript.h"

#include "ECS.h"
#include "SpawnerScript.h"
#include "SkyboxScript.h"

using std::cout; 
using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;
using namespace ECS;

GLFWwindow* window; // Game window
const unsigned int width = 800;
const unsigned int height = 800;

float t = 0;
time_t current_time;

World* world;

void SetupGLFW() {

	glfwInit();

	// Tell GLFW we are using OpenGL 3.3 and the CORE profile (only the modern functions)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

}

bool SetupWindow() {
	//Create a GLFWwindow with size 800x800
	window = glfwCreateWindow(width, height, "ProgramacioVideojocs", NULL, NULL);
	if (window == NULL) {

		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Make window part of the current context
	glfwMakeContextCurrent(window);

	glfwSetWindowPos(window, 100, 100);

	//Load GLAD and specify the viewport
	gladLoadGL();
	glViewport(0, 0, 800, 800);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

Entity* CreateEntity2D(glm::vec2 position, float rotation, float scale, const char* filepath, glm::vec3 color, 
	bool autoSize = true, glm::vec2 size = glm::vec2(1.0, 1.0), const char* shaderName = "default") {
	Entity* ent = world->create();
	ent->assign<Transform2D>(position, rotation, scale);
	ent->assign<Sprite>(filepath, color, autoSize, size, shaderName);

	return ent;
}

Entity* CreateEntity3DWithMesh(glm::vec3 position, float scale, const char* meshFilepath, const char* texFilepath) {
	Entity* ent = world->create();
	ent->assign<Transform3D>(position, scale);
	ent->assign<MeshComponent>(texFilepath, meshFilepath);

	return ent;
}

Entity* CreateEntity3DEmpty() {
	Entity* ent = world->create();

	return ent;
}

Entity* CreateCamera(glm::vec3 position) {
	Entity* ent = world->create();
	ent->assign<Camera>(position, glm::vec3(0., 0., -1.), glm::vec3(0., 1., 0.));

	return ent;
}

Entity* CreateSkybox(const char* meshFilepath, const char* texFilepath) {
	Entity* ent = world->create();
	ent->assign<Skybox>(texFilepath, meshFilepath);

	return ent;
}

void SetupWorld() {

	RenderSystem* rs = new RenderSystem(width, height);

	world = World::createWorld();
	world->registerSystem(rs);
	ScriptSystem* scriptSystem = new ScriptSystem();
	world->registerSystem(scriptSystem);

	ScriptManager* scriptManager = scriptSystem->getScriptManager();

	Entity* player = CreateCamera(glm::vec3(52.0f, 2.f, 58.0f));
	FirstPersonCameraScript* fps = new FirstPersonCameraScript(window, world, player);
	
	player->assign<ScriptComponent>(scriptManager->AddScript(fps));

	rs->setCamera(player);

	player->assign<CubeCollider>(1, 1, 1);

	Entity* lifeBar = CreateEntity2D(glm::vec2(50, 20), 0.f, 1.f, "Textures/3_hearts.png", glm::vec3(1., 1., 1.), true);
	LifeBarScript* lifeBarScript = new LifeBarScript(window, world, lifeBar);
	lifeBar->assign<ScriptComponent>(scriptManager->AddScript(lifeBarScript));

	Entity* floor = CreateEntity3DWithMesh(glm::vec3(52, -4, 52), 75, "Meshes/plane.obj", "Textures/wall.png");
	floor->assign<CubeCollider>(75, 1, 75);

	Entity* powerUp = CreateEntity3DWithMesh(glm::vec3(52, -3, 52), 1, "Meshes/teapot2.obj", "Textures/skybox.png");
	PowerUpScript* powerUpScript = new PowerUpScript(window, world, powerUp);
	powerUp->assign<ScriptComponent>(scriptManager->AddScript(powerUpScript));
	powerUp->assign<CubeCollider>(1, 1, 1);

	fps->setParameters(lifeBar, powerUp);

	Entity* playerManager = CreateEntity3DEmpty();
	PlayerManagerScript* playerManagerScript = new PlayerManagerScript(window, world, playerManager);
	playerManager->assign<ScriptComponent>(scriptManager->AddScript(playerManagerScript));
	playerManagerScript->setParameters(player, floor, scriptManager);

	Entity* spawner = CreateEntity3DEmpty();
	SpawnerScript* spawner_script = new SpawnerScript(window, world, spawner);
	spawner->assign<ScriptComponent>(scriptManager->AddScript(spawner_script));
	spawner_script->setParameters(scriptManager, player);

	

		
	Entity* skybox = CreateSkybox("Meshes/flipped_sphere.obj", "Textures/sky.png");


	
	
	string map[] = { 
		"##################", 
		"#----------------#", 
		"#----------------#",
		"#----------------#",
		"#----------------#",
		"#----------------#",
		"#----------------#",
		"!----------------!",
		"!----------------!"
		"!----------------!",
		"#----------------#",
		"#----------------#",
		"#----------------#",
		"#----------------#",
		"#----------------#",
		"#----------------#",
		"##################",

	};

	for (int k = 0; k < 3; k++) {
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 18; j++) {
				
				if (map[i][j] == '!' && k < 1) {
					Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 8, k * 8, j * 8), 4, "Meshes/cube2.obj", "");
					wall->assign<CubeCollider>(5, 5, 5);
				}
				else {
					if (map[i][j] == '#' || map[i][j] == '!') {
						Entity* wall = CreateEntity3DWithMesh(glm::vec3(i * 8, k * 8, j * 8), 4, "Meshes/cube2.obj", "Textures/wall2.png");
						wall->assign<CubeCollider>(5, 5, 5);
					}
				}
			}
		}
	}
	
	

	
	
}

int main() {
	
	SetupGLFW();

	if (!SetupWindow()) {
		return -1;
	}

	SetupWorld();

	float dt = 0;
	float time = clock();

	//Program core loop
	while (!glfwWindowShouldClose(window)) {

		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		dt = clock() - time;
		time = clock();
		if (dt < 50) {
			world->tick(dt);
		}

		glfwSwapBuffers(window); //Swap buffers

		// Take care of GLFW events
		glfwPollEvents();


		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			break;
		}

	}

	// Cleanup

	glfwDestroyWindow(window);
	glfwTerminate();

	world->destroyWorld();

	return 0;
}