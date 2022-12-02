#include "PlayerManagerScript.h"
#include "BulletScript.h"
#include "ScriptManager.h"

void PlayerManagerScript::startScript()
{
}

Entity* PlayerManagerScript::CreateEntity3DWithMesh(glm::vec3 position, float scale, const char* meshFilepath, const char* texFilepath) {
	Entity* ent = world->create();
	ent->assign<Transform3D>(position, scale);
	ent->assign<MeshComponent>(texFilepath, meshFilepath);

	return ent;
}

void PlayerManagerScript::setParameters(Entity* player, ScriptManager* scriptManager) {
	this->player = player;
	this->scriptManager = scriptManager;
}

void PlayerManagerScript::shoot() {

	Entity* bullet = CreateEntity3DWithMesh(glm::vec3(0, 2, 0), 0.125, "Meshes/prova.obj", "Textures/wall.png");
	BulletScript* bulletScript = new BulletScript(window, world, bullet);
	bullet->assign<ScriptComponent>(scriptManager->AddScript(bulletScript));

}



void PlayerManagerScript::tickScript(float deltaTime)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		shoot();
		cout << "shoot" << endl;
	}

}