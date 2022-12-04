#include "BossBulletScript.h"

void BossBulletScript::startScript() {
}

void BossBulletScript::setParameters(Entity* boss, Entity* player) {

	glm::vec3 posBoss = boss->get<Transform3D>()->position;
	glm::vec3 posPlayer = player->get<Camera>()->position;

	this->boss = boss;
	this->player = player;

	dir = glm::normalize(posBoss - posPlayer);

	time = glfwGetTime();
}

void BossBulletScript::checkCollisions() {

	world->each<CubeCollider>([&](Entity* other_ent, ComponentHandle<CubeCollider> other_collider) {

		if (other_ent->getEntityId() != player->getEntityId()) {
			return;
		}

		glm::vec3 pos = other_ent->get<Camera>()->position;
		ComponentHandle<Transform3D> posBullet = entity->get<Transform3D>();

		if (posBullet->position.x < pos.x + other_collider->width && posBullet->position.x > pos.x - other_collider->width &&
			posBullet->position.y < pos.y + other_collider->height && posBullet->position.y > pos.y - other_collider->height &&
			posBullet->position.z < pos.z + other_collider->length && posBullet->position.z > pos.z - other_collider->length) {

			other_collider->collidedWith = true;
			world->destroy(entity);

		}

		});
}

void BossBulletScript::move(float speedDelta) {

	ComponentHandle<Transform3D> transform = entity->get<Transform3D>();

	transform->position -= dir * speedDelta;

}

void BossBulletScript::tickScript(float deltaTime) {

	float speedDelta = speed * deltaTime;

	if (glfwGetTime() - time > 6) {
		world->destroy(entity);
	}

	move(speedDelta);
	checkCollisions();

}