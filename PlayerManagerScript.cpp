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

	ComponentHandle<Camera> cam = player->get<Camera>();

	Entity* bullet = CreateEntity3DWithMesh(glm::vec3(cam->position + (cam->orientation * 1.5f)), 0.125, "Meshes/bala.obj", "Textures/wall.png");
	BulletScript* bulletScript = new BulletScript(window, world, bullet);
	bullet->assign<ScriptComponent>(scriptManager->AddScript(bulletScript));
	bulletScript->setPlayer(player);
	bullet->assign<CubeCollider>(0.25, 0.25, 0.25);

}



void PlayerManagerScript::tickScript(float deltaTime)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && glfwGetTime() - lastBullet > bulletCd && bullets > 0) {
		lastBullet = glfwGetTime();
		bullets--;
		shoot();
	}

	if (bullets <= 0 && ammo) {
		ammo = false;
		noAmmo = world->create();
		noAmmo->assign<Transform2D>(glm::vec2(705, 775), 0.f, 20.f);
		noAmmo->assign<Sprite>("Textures/no_ammo.png", glm::vec3(1., 1., 1.), true);
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		ammo = true;
		world->destroy(noAmmo);
		bullets = 20;
	}
}