#include "SpawnerScript.h"
#include "Enemy1Script.h"
#include "Enemy2Script.h"
#include "BossScript.h"
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

	if (glfwGetTime() < 8) {
		if (t1 > delay1) {
			t1 = 0;

			Entity* enemy = CreateEntity3DWithMesh(glm::vec3(52, -3., 5.), 1, "Meshes/bala.obj", "Textures/skybox.png");
			Enemy1Script* enemyScript = new Enemy1Script(window, world, enemy);
			enemy->assign<ScriptComponent>(scriptManager->AddScript(enemyScript));
			enemyScript->setTarget(target);
			enemy->assign<CubeCollider>(1, 1, 1);
		}

		if (t2 > delay2) {
			t2 = 0;

			Entity* enemy = CreateEntity3DWithMesh(glm::vec3(52, 12., 99.), 0.5, "Meshes/bala.obj", "Textures/wall.png");
			Enemy2Script* enemyScript = new Enemy2Script(window, world, enemy);
			enemy->assign<ScriptComponent>(scriptManager->AddScript(enemyScript));
			enemyScript->setTarget(target);
			enemy->assign<CubeCollider>(1, 1, 1);
		}
	}
	else {
		if (!bossSpawned && glfwGetTime() >= 12) {
			bossSpawned = true;

			Entity* boss = CreateEntity3DWithMesh(glm::vec3(52, 7, 90), 2, "Meshes/cube.obj", "Textures/skybox.png");
			BossScript* bossScript = new BossScript(window, world, boss);
			boss->assign<ScriptComponent>(scriptManager->AddScript(bossScript));
			bossScript->setTarget(target);
			boss->assign<CubeCollider>(2.5, 2.5, 2.5);
		}
	}
}
