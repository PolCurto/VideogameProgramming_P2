#include "BulletScript.h"

void BulletScript::startScript() {
}

void BulletScript::move(float speedDelta) {
	
	ComponentHandle<Transform3D> transform = entity->get<Transform3D>();

	transform->position.x += speed * speedDelta;
}

void BulletScript::tickScript(float deltaTime) {

	float speedDelta = speed * deltaTime;

	move(speedDelta);

}