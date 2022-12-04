#include "PowerUpScript.h"


void PowerUpScript::startScript() {

}

void PowerUpScript::tickScript(float deltaTime) {

	ComponentHandle<CubeCollider> collider = entity->get<CubeCollider>();

	if (collider->collidedWith) {
		world->destroy(entity);
	}
}