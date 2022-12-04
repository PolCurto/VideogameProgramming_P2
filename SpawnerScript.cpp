#include "SpawnerScript.h"
#include "Enemy1Script.h"
#include "Enemy2Script.h"
#include "ScriptManager.h"

void SpawnerScript::startScript()
{
}

void SpawnerScript::setParameters(ScriptManager* scriptManager, Entity* target) {
	this->scriptManager = scriptManager;
	this->target = target;
}

Entity* SpawnerScript::CreateEntity3DWithMesh(glm::vec3 position, float scale, const char* meshFilepath, const char* texFilepath) {
	Entity* ent = world->create();
	ent->assign<Transform3D>(position, scale);
	ent->assign<MeshComponent>(texFilepath, meshFilepath);

	return ent;
}


void SpawnerScript::tickScript(float deltaTime)
{

	t1 += deltaTime;
	t2 += deltaTime;

	if (t1 > delay1) {
		t1 = 0;

		Entity* enemy = CreateEntity3DWithMesh(glm::vec3(52, -3., 5.), 0.5, "Meshes/bala.obj", "Textures/skybox.png");
		Enemy1Script* enemyScript = new Enemy1Script(window, world, enemy);
		enemy->assign<ScriptComponent>(scriptManager->AddScript(enemyScript));
		enemyScript->setTarget(target);
		enemy->assign<CubeCollider>(1, 1, 1);
	}

	if (t2 > delay2) {
		t2 = 0;

		Entity* enemy = CreateEntity3DWithMesh(glm::vec3(52, 10., 99.), 0.5, "Meshes/character_medium.obj", "Textures/skybox.png");
		Enemy2Script* enemyScript = new Enemy2Script(window, world, enemy);
		enemy->assign<ScriptComponent>(scriptManager->AddScript(enemyScript));
		enemyScript->setTarget(target);
		enemy->assign<CubeCollider>(1, 1, 1);
	}
}
