#include "BossScript.h"
#include "BossBulletScript.h"
#include "ScriptManager.h"



void BossScript::startScript() {
}

Entity* BossScript::CreateEntity3DWithMesh(glm::vec3 position, float scale, const char* meshFilepath, const char* texFilepath) {
	Entity* ent = world->create();
	ent->assign<Transform3D>(position, scale);
	ent->assign<MeshComponent>(texFilepath, meshFilepath);

	return ent;
}

void BossScript::setParameters(Entity* player, ScriptManager* scriptManager) {
	this->player = player;
	this->scriptManager = scriptManager;
	previousShot = glfwGetTime();
}

void BossScript::checkCollisions() {

	world->each<CubeCollider>([&](Entity* other_ent, ComponentHandle<CubeCollider> other_collider) {

		if (other_ent->getEntityId() != player->getEntityId()) {
			return;
		}

		glm::vec3 pos = other_ent->get<Camera>()->position;
		ComponentHandle<Transform3D> posBoss = entity->get<Transform3D>();

		if (posBoss->position.x < pos.x + other_collider->width && posBoss->position.x > pos.x - other_collider->width &&
			posBoss->position.y < pos.y + other_collider->height && posBoss->position.y > pos.y - other_collider->height &&
			posBoss->position.z < pos.z + other_collider->length && posBoss->position.z > pos.z - other_collider->length) {

			other_collider->collidedWith = true;

		}

		});
}

void BossScript::checkPhase() {

	ComponentHandle<MeshComponent> texture = entity->get<MeshComponent>();

	if (phase == 2) {
		texture->textureFilepath = "Textures/wall.png";
		delay = 2;
	}

	if (phase == 3) {
		texture->textureFilepath = "Textures/.png";
		speed = 0.02;
	}

}

void BossScript::move(float speedDelta) {

	ComponentHandle<Transform3D> boss = entity->get<Transform3D>();
	ComponentHandle<Camera> cam = player->get<Camera>();
	ComponentHandle<CubeCollider> collider = entity->get<CubeCollider>();


	currDir = glm::normalize(boss->position - cam->position);

	glm::vec3 currentPosition = boss->position;
	glm::vec3 desiredPosition = boss->position;

	
	desiredPosition -= currDir * speedDelta;
	

	world->each<CubeCollider>([&](Entity* ent, ComponentHandle<CubeCollider> cubeColl) {

		if (!(ent->getEntityId() == entity->getEntityId()) && !(ent->getEntityId() == player->getEntityId())) {
			glm::vec3 pos = ent->get<Transform3D>()->position;

			//Desired position inside cube
			if (desiredPosition.x < pos.x + cubeColl->width && desiredPosition.x > pos.x - cubeColl->width &&
				desiredPosition.y < pos.y + cubeColl->height && desiredPosition.y > pos.y - cubeColl->height &&
				desiredPosition.z < pos.z + cubeColl->length && desiredPosition.z > pos.z - cubeColl->length) {

				time_t result = time(NULL);

				char str[26];
				ctime_s(str, sizeof str, &result);

				if (currentPosition.x <= pos.x - cubeColl->width) desiredPosition.x = pos.x - cubeColl->width;
				if (currentPosition.x >= pos.x + cubeColl->width) desiredPosition.x = pos.x + cubeColl->width;
				if (currentPosition.z <= pos.z - cubeColl->length) desiredPosition.z = pos.z - cubeColl->length;
				if (currentPosition.z >= pos.z + cubeColl->length) desiredPosition.z = pos.z + cubeColl->length;
				if (currentPosition.y <= pos.y - cubeColl->height) desiredPosition.y = pos.y - cubeColl->height;
				if (currentPosition.y >= pos.y + cubeColl->height) {
					desiredPosition.y = pos.y + cubeColl->height;
				}
			}
		}

		});

	if (phase != 3) {
		boss->position.x = desiredPosition.x;
		boss->position.z = desiredPosition.z;
	}
	if (phase == 3) {
		boss->position = desiredPosition;
	}
	

}

void BossScript::shoot() {

	ComponentHandle<Transform3D> boss = entity->get<Transform3D>();

	if (glfwGetTime() - previousShot > delay && phase != 3) {
		Entity* bullet = CreateEntity3DWithMesh(glm::vec3(boss->position), 1, "Meshes/bala.obj", "Textures/wall2.png");
		BossBulletScript* bulletScript = new BossBulletScript(window, world, bullet);
		bullet->assign<ScriptComponent>(scriptManager->AddScript(bulletScript));
		bulletScript->setParameters(entity, player);
		bullet->assign<CubeCollider>(0.25, 0.25, 0.25);

		previousShot = glfwGetTime();
	}
}

void BossScript::tickScript(float deltaTime) {

	ComponentHandle<CubeCollider> collider = entity->get<CubeCollider>();

	float speedDelta = speed * deltaTime;
	move(speedDelta);
	checkCollisions();

	if (collider->collidedWith) {
		collider->collidedWith = false;
		life--;
	}

	if (life == 10) {
		phase = 2;
	}

	if (life == 5) {
		phase = 3;
	}

	if (life <= 0) {
		world->destroy(entity);
	}

	checkPhase();
	shoot();
}