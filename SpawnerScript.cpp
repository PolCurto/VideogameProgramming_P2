#include "SpawnerScript.h"
#include "Enemy1Script.h"
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

	t += deltaTime;

	if (t > delay) {
		cout << "spawn" << endl;
		t = 0;

		Entity* enemy = CreateEntity3DWithMesh(glm::vec3(20., 0., 0.), 0.5, "Meshes/bala.obj", "Textures/a.png");
		Enemy1Script* enemyScript = new Enemy1Script(window, world, enemy);
		enemy->assign<ScriptComponent>(scriptManager->AddScript(enemyScript));
		enemyScript->setTarget(target);
		enemy->assign<CubeCollider>(1, 1, 1);

	}
}
