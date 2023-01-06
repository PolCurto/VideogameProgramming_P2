#include "PlayerManagerScript.h"
#include "BulletScript.h"
#include "PowerUpScript.h"
#include "ScriptManager.h"

void PlayerManagerScript::startScript()
{
}

Entity* PlayerManagerScript::CreateEntity3DWithMesh(glm::vec3 position, float scale, const char* meshFilepath, const char* texFilepath) {
	Entity* ent = world->create();
	ent->assign<Transform3D>(position, glm::vec3(0, 0, 0), scale);
	ent->assign<MeshComponent>(texFilepath, meshFilepath);

	return ent;
}

void PlayerManagerScript::setParameters(Entity* player, Entity* floor, FirstPersonCameraScript* fps, ScriptManager* scriptManager) {
	this->player = player;
	this->scriptManager = scriptManager;
	this->floor = floor;
	this->fps = fps;
}

void PlayerManagerScript::showMessages() {

	if (bullets <= 0 && ammo) {
		ammo = false;
		noAmmo = world->create();
		noAmmo->assign<Transform2D>(glm::vec2(705, 775), 0.f, 20.f);
		noAmmo->assign<Sprite>("Textures/no_ammo.png", glm::vec3(1., 1., 1.), true);
	}

	if (fps->isPoweredUp() && !powerUp) {
		powerUp = true;
		poweredUp = world->create();
		poweredUp->assign<Transform2D>(glm::vec2(705, 30), 0.f, 20.f);
		poweredUp->assign<Sprite>("Textures/power_up.png", glm::vec3(1., 1., 1.), true);
	}

	if (!fps->isPoweredUp() && powerUp) {
		world->destroy(poweredUp);
	}


}


void PlayerManagerScript::shoot() {

	ComponentHandle<Camera> cam = player->get<Camera>();

	Entity* bullet = CreateEntity3DWithMesh(glm::vec3(cam->position + (cam->orientation * 1.f)), 0.02, "Meshes/ball.obj", "Textures/bullet.png");
	BulletScript* bulletScript = new BulletScript(window, world, bullet);
	bullet->assign<ScriptComponent>(scriptManager->AddScript(bulletScript));
	bulletScript->setParameters(player, floor);
	bullet->assign<CubeCollider>(0.05, 0.05, 0.05);
}

void PlayerManagerScript::tickScript(float deltaTime)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && glfwGetTime() - lastBullet > bulletCd && bullets > 0) {
		lastBullet = glfwGetTime();
		bullets--;
		shoot();
	}

	showMessages();

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		ammo = true;
		world->destroy(noAmmo);
		bullets = 30;
	}

	if (fps->isPoweredUp()) {
		bulletCd = 0.1;
	}
	else {
		bulletCd = 0.5;
	}

}