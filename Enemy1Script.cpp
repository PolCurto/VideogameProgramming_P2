#include "Enemy1Script.h"

void Enemy1Script::startScript() {
}

void Enemy1Script::setTarget(Entity* player) {
	this->player = player;
}

void Enemy1Script::checkCollisions() {

	world->each<CubeCollider>([&](Entity* other_ent, ComponentHandle<CubeCollider> other_collider) {

		if (other_ent->getEntityId() != player->getEntityId()) {
			return;
		}

		glm::vec3 pos = other_ent->get<Camera>()->position;
		ComponentHandle<Transform3D> posEnemy = entity->get<Transform3D>();

		if (posEnemy->position.x < pos.x + other_collider->width && posEnemy->position.x > pos.x - other_collider->width &&
			posEnemy->position.y < pos.y + other_collider->height && posEnemy->position.y > pos.y - other_collider->height &&
			posEnemy->position.z < pos.z + other_collider->length && posEnemy->position.z > pos.z - other_collider->length) {

			other_collider->collidedWith = true;

		}

		});
}

void Enemy1Script::move(float speedDelta) {

	ComponentHandle<Transform3D> enemy = entity->get<Transform3D>();
	ComponentHandle<Camera> cam = player->get<Camera>();

	currDir = glm::normalize(enemy->position - cam->position);

	glm::vec3 currentPosition = enemy->position;
	glm::vec3 desiredPosition = enemy->position;

	switch (life) {
	case 3:
		desiredPosition -= currDir * speedDelta * 0.75f;
		break;
	case 2:
		desiredPosition -= currDir * speedDelta;
		break;
	case 1:
		desiredPosition -= currDir * speedDelta * 1.5f;
		break;
	}

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

	enemy->rotation.x = -atan2(currDir.z, currDir.x);



}

void Enemy1Script::tickScript(float deltaTime) {

	ComponentHandle<CubeCollider> collider = entity->get<CubeCollider>();
	ComponentHandle<MeshComponent> texture = entity->get<MeshComponent>();

	float speedDelta = speed * deltaTime;
	move(speedDelta);
	checkCollisions();

	if (collider->collidedWith) {
		collider->collidedWith = false;
		life--;
	}

	if (life == 2) {
		texture->textureFilepath = "Textures/NormalEnemy2.png";
	}


	if (life == 1) {
		texture->textureFilepath = "Textures/AngryEnemy.png";
	}

	if (life <= 0) {
		world->destroy(entity);
	}

}