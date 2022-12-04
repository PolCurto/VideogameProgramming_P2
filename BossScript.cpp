#include "BossScript.h"

void BossScript::startScript() {
}

void BossScript::setTarget(Entity* player) {
	this->player = player;
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
	}

	if (phase == 3) {
		texture->textureFilepath = "Textures/.png";
		speed = 0.015;
	}

}

void BossScript::move(float speedDelta) {

	ComponentHandle<Transform3D> enemy = entity->get<Transform3D>();
	ComponentHandle<Camera> cam = player->get<Camera>();
	ComponentHandle<CubeCollider> collider = entity->get<CubeCollider>();


	currDir = glm::normalize(enemy->position - cam->position);

	glm::vec3 currentPosition = enemy->position;
	glm::vec3 desiredPosition = enemy->position;

	
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

	enemy->position.x = desiredPosition.x;
	enemy->position.z = desiredPosition.z;

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

}