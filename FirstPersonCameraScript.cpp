#include "FirstPersonCameraScript.h"
#include <chrono>

void FirstPersonCameraScript::startScript() {
}

void FirstPersonCameraScript::setParameters(Entity* lifeBar, Entity* powerUp) {
	this->lifeBar = lifeBar;
	this->powerUp = powerUp;
}

bool FirstPersonCameraScript::isPoweredUp() {
	return poweredUp;
}

void FirstPersonCameraScript::checkPowerUp() {

	ComponentHandle<CubeCollider> collider = entity->get<CubeCollider>();

	world->each<CubeCollider>([&](Entity* other_ent, ComponentHandle<CubeCollider> other_collider) {

		if (other_ent->getEntityId() != powerUp->getEntityId()) {
			return;
		}

		glm::vec3 pos = other_ent->get<Transform3D>()->position;
		ComponentHandle<Camera> posPlayer = entity->get<Camera>();

		if (posPlayer->position.x - collider < pos.x + other_collider->width && posPlayer->position.x + collider > pos.x - other_collider->width &&
			posPlayer->position.y - collider < pos.y + other_collider->height && posPlayer->position.y + collider > pos.y - other_collider->height &&
			posPlayer->position.z - collider < pos.z + other_collider->length && posPlayer->position.z + collider > pos.z - other_collider->length) {

			other_collider->collidedWith = true;
			poweredUp = true;
			lastPowered = glfwGetTime();

		}

		});

	if (poweredUp) {
		speed = 0.04;
	}

	if (poweredUp && glfwGetTime() - lastPowered > 10) {
		poweredUp = false;
		speed = 0.01;
	}
}

void FirstPersonCameraScript::jump(glm::vec3* desiredPosition, float speedDelta) {
	
	ComponentHandle<Camera> cam = entity->get<Camera>();
	
	if (isJumping) {

		if (glfwGetTime() - jumpStart < 0.5f) {

			*desiredPosition += speedDelta * cam->up * (float)((0.5 - (glfwGetTime() - jumpStart)) * 2);
		}
		else {
			if (glfwGetTime() - jumpStart < 1.f) {
				*desiredPosition += speedDelta * -cam->up * (float)(((glfwGetTime() - jumpStart) - 0.5) * 2);
			}
		}
		if (glfwGetTime() - jumpStart >= 1.f) {
			isJumping = false;
		}
	}
}

void FirstPersonCameraScript::dash(glm::vec3* desiredPosition, float speedDelta) {

	ComponentHandle<Camera> cam = entity->get<Camera>();

	float dashspeed = 20;

	if (numDashes < 1) {
		return;
	}

	numDashes--;

	switch (dir) {
	case 1:
		*desiredPosition += speedDelta * cam->orientation * dashspeed;
		break;
	case 2:
		*desiredPosition += speedDelta * glm::normalize(glm::cross(cam->orientation, cam->up)) * dashspeed;
		break;
	case 3:
		*desiredPosition += speedDelta * -cam->orientation * dashspeed;
		break;
	case 4:
		*desiredPosition += speedDelta * -glm::normalize(glm::cross(cam->orientation, cam->up)) * dashspeed;
		break;
	}

	dashStart = glfwGetTime();
}


void FirstPersonCameraScript::move(float speedDelta) {

	ComponentHandle<Camera> cam = entity->get<Camera>();

	glm::vec3 currentPosition = cam->position;
	glm::vec3 desiredPosition = cam->position;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		desiredPosition += speedDelta * glm::vec3 (cam->orientation.x, 0.f, cam->orientation.z);
		dir = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		desiredPosition += speedDelta * -glm::normalize(glm::cross(cam->orientation, cam->up));
		dir = 4;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		desiredPosition += speedDelta * -glm::vec3(cam->orientation.x, 0.f, cam->orientation.z);
		dir = 3;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		desiredPosition += speedDelta * glm::normalize(glm::cross(cam->orientation, cam->up));
		dir = 2;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (isOnGround) {
			isJumping = true;
			jumpStart = glfwGetTime();
			isOnGround = false;
			numJumps--;
		}
		else {
			if (numJumps > 0 && glfwGetTime() - jumpStart > 0.3f) {
				isJumping = true;
				jumpStart = glfwGetTime();
				numJumps = 0;
			}
		}
	}

	if (!(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) && !(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) && !(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)) {
		dir = 1;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		dash(&desiredPosition, speedDelta);
	}
	jump(&desiredPosition, speedDelta);

	if (!isJumping) {
		desiredPosition += speedDelta * -cam->up * 2.f;
	}

	if (glfwGetTime() > dashStart + 2.f) {
		numDashes = 10;
	}

	world->each<CubeCollider>([&](Entity* ent, ComponentHandle<CubeCollider> cubeColl) {

		if (!(ent->getEntityId() == entity->getEntityId())) {

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
					isOnGround = true;
					numJumps = 2;
				}
			}

		}

		});

	cam->position = desiredPosition;

	if (glfwGetTime() > checkGround) {
		if (previousY > cam->position.y + 0.05) {
			isOnGround = false;
		}
		checkGround += 0.05;
		previousY = cam->position.y;
	}

}

void FirstPersonCameraScript::moveView() {

	ComponentHandle<Camera> cam = entity->get<Camera>();

	// Stores the coordinates of the cursor
	double mouseX;
	double mouseY;

	// Fetches the coordinates of the cursor
	glfwGetCursorPos(window, &mouseX, &mouseY);


	float xoffset = mouseX - lastX;
	float yoffset = lastY - mouseY;
	lastX = mouseX;
	lastY = mouseY;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 70.0f)
		pitch = 70.0f;
	if (pitch < -70.0f)
		pitch = -70.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	cam->orientation = front;
}

void FirstPersonCameraScript::checkHits() {
	
	ComponentHandle<CubeCollider> collider = entity->get<CubeCollider>();

	ComponentHandle<Sprite> spr = lifeBar->get<Sprite>();

	
	if (collider->collidedWith) {
		collider->collidedWith = false;

		if (vulnerable) {
			lastVulnerable = glfwGetTime();
			life--;
			vulnerable = false;

			cout << "se acabo" << endl;


			if (life == 2) {
				spr->filepath = "Textures/2_hearts.png";
			}
			if (life == 1) {
				spr->filepath = "Textures/1_heart.png";
			}
			if (life <= 0) {
				alive = false;
			}

		}
	}

	if (!vulnerable && !poweredUp) {
		speed = 0.02;
	}

	if (vulnerable && !poweredUp) {
		speed = 0.01;
	}

	if (glfwGetTime() - lastVulnerable > 2) {
		vulnerable = true;
	}
}

void FirstPersonCameraScript::tickScript(float deltaTime) {

	float speedDelta = speed * deltaTime;

	float width = 800;
	float height = 800;

	ComponentHandle<Camera> cam = entity->get<Camera>();

	move(speedDelta);

	// Hides mouse cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Prevents camera from jumping on the first click
	if (startGame)
	{
		glfwSetCursorPos(window, (width / 2), (height / 2));
		startGame = false;
	}

	moveView();
	checkHits();
	checkPowerUp();	
}