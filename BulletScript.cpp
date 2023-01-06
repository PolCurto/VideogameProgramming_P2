#include "BulletScript.h"

void BulletScript::startScript() {
}

void BulletScript::setParameters(Entity* player, Entity* floor) {

	ComponentHandle<Camera> cam = player->get<Camera>();

	this->player = player;
	dir = cam->orientation;

	this->floor = floor;
	time = glfwGetTime();
}

void BulletScript::checkCollisions() {

	world->each<CubeCollider>([&](Entity* other_ent, ComponentHandle<CubeCollider> other_collider) {

		ComponentHandle<CubeCollider> collider = entity->get<CubeCollider>();

		if (other_ent->getEntityId() == player->getEntityId() || other_ent->getEntityId() == entity->getEntityId() || other_ent->getEntityId() == floor->getEntityId()) {
			return;
		}
		
		glm::vec3 pos = other_ent->get<Transform3D>()->position;
		ComponentHandle<Transform3D> posBullet = entity->get<Transform3D>();	
		
		if (posBullet->position.x  < pos.x + other_collider->width && posBullet->position.x  > pos.x - other_collider->width &&
			posBullet->position.y  < pos.y + other_collider->height && posBullet->position.y  > pos.y - other_collider->height &&
			posBullet->position.z  < pos.z + other_collider->length && posBullet->position.z > pos.z - other_collider->length) {

			other_collider->collidedWith = true;
			world->destroy(entity);

		}
		
		});
}

void BulletScript::move(float speedDelta) {
	
	ComponentHandle<Transform3D> transform = entity->get<Transform3D>();

	transform->position += dir * speedDelta;

}

void BulletScript::tickScript(float deltaTime) {

	float speedDelta = speed * deltaTime;

	if (glfwGetTime() - time > 3) {
		world->destroy(entity);
	}

	move(speedDelta);
	checkCollisions();

}