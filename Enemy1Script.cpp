#include "Enemy1Script.h"

void Enemy1Script::startScript() {
}

void Enemy1Script::setTarget(Entity* player) {
	this->player = player;
}

void Enemy1Script::move(float speedDelta) {

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

	enemy->position.x = desiredPosition.x;
	enemy->position.z = desiredPosition.z;

}

void Enemy1Script::tickScript(float deltaTime) {

	float speedDelta = speed * deltaTime;
	move(speedDelta);

}