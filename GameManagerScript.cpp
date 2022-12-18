#include "GameManagerScript.h"


void GameManagerScript::startScript() {

}

void GameManagerScript::setParameters(FirstPersonCameraScript* playerScript, SpawnerScript* spawnerScript, Entity* lifeBar, Entity* mirilla) {
	this->playerScript = playerScript;
	this->spawnerScript = spawnerScript;
	this->lifeBar = lifeBar;
	this->mirilla = mirilla;
}

void GameManagerScript::youWin() {

	ComponentHandle<Sprite>spr1 = lifeBar->get<Sprite>();
	ComponentHandle<Sprite>spr2 = mirilla->get<Sprite>();

	Entity* ent = world->create();
	ent->assign<Transform2D>(glm::vec2(400, 400), 0, 1);
	ent->assign<Sprite>("Textures/you_win.png", glm::vec3(1., 1., 1.), true);
	spr1->filepath = "";
	spr2->filepath = "";
	over = true;
}

void GameManagerScript::youLose() {

	ComponentHandle<Sprite>spr1 = lifeBar->get<Sprite>();
	ComponentHandle<Sprite>spr2 = mirilla->get<Sprite>();

	Entity* ent = world->create();
	ent->assign<Transform2D>(glm::vec2(400, 400), 0, 1);
	ent->assign<Sprite>("Textures/game_over.png", glm::vec3(1., 1., 1.), true);
	spr1->filepath = "";
	spr2->filepath = "";
	over = true;
}

bool GameManagerScript::isOver() {
	return over;
}

void GameManagerScript::tickScript(float deltaTime) {

	if (playerScript->isDead()) {
		youLose();
	}

	if (glfwGetTime() >= 15) {
		if (spawnerScript->bossIsDead()) {
			youWin();
		}
	}
	

}