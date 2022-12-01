#include "PlayerManagerScript.h"

void PlayerManagerScript::startScript()
{
}

void PlayerManagerScript::shoot() {

	Entity* ent = world->create();
	ent->assign<Transform3D>(glm::vec3(0., 0., 0.), 1000);
	ent->assign<MeshComponent>("Meshes/prova.obj", "Textures/wall.png");


}



void PlayerManagerScript::tickScript(float deltaTime)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		shoot();
		cout << "shoot" << endl;
	}

}