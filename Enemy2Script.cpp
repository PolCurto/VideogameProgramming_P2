#include "Enemy2Script.h"

void Enemy2Script::startScript() {
}

void Enemy2Script::setTarget(Entity* player) {
	this->player = player;
}

void Enemy2Script::checkCollisions() {

	world->each<CubeCollider>([&](Entity* other_ent, ComponentHandle<CubeCollider> other_collider) {

		if (other_ent->getEntityId() != player->getEntityId()) {
			return;
		}

		glm::vec3 pos = other_ent->get<Camera>()->position;
		ComponentHandle<Transform3D> posPlayer = entity->get<Transform3D>();

		if (posPlayer->position.x < pos.x + other_collider->width && posPlayer->position.x > pos.x - other_collider->width &&
			posPlayer->position.y < pos.y + other_collider->height && posPlayer->position.y > pos.y - other_collider->height &&
			posPlayer->position.z < pos.z + other_collider->length && posPlayer->position.z > pos.z - other_collider->length) {

			other_collider->collidedWith = true;

		}

		});
}

void Enemy2Script::move(float speedDelta) {

	ComponentHandle<Transform3D> enemy = entity->get<Transform3D>();
	ComponentHandle<Camera> cam = player->get<Camera>();

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

	enemy->position = desiredPosition;

	enemy->rotation.x = -atan2(currDir.z, currDir.x) + glm::radians(75.f);
	//enemy->rotation.y = -atan2(currDir.y, currDir.x); //+ glm::radians(75.f);
	//enemy->rotation.z = -atan2(currDir.y, currDir.z); //+ glm::radians(75.f);

}

void Enemy2Script::tickScript(float deltaTime) {

	ComponentHandle<CubeCollider> collider = entity->get<CubeCollider>();
	ComponentHandle<MeshComponent> texture = entity->get<MeshComponent>();

	float speedDelta = speed * deltaTime;
	move(speedDelta);
	checkCollisions();

	if (collider->collidedWith) {
		collider->collidedWith = false;
		world->destroy(entity);
	}

}