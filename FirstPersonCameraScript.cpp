#include "FirstPersonCameraScript.h"
#include <chrono>

void FirstPersonCameraScript::startScript() {

}

glm::vec3 FirstPersonCameraScript::jump(glm::vec3 desiredPosition, float speedDelta) {
	
	ComponentHandle<Camera> cam = entity->get<Camera>();
	
	if (isJumping) {

		if (glfwGetTime() - jumpStart < 0.5f) {

			desiredPosition += speedDelta * cam->up * (float)((0.5 - (glfwGetTime() - jumpStart)) * 2);
		}
		else {
			if (glfwGetTime() - jumpStart < 1.f) {
				desiredPosition += speedDelta * -cam->up * (float)(((glfwGetTime() - jumpStart) - 0.5) * 2);
			}
		}
		if (glfwGetTime() - jumpStart >= 1.f) {
			isJumping = false;
		}
	}

	return desiredPosition;
}

glm::vec3 FirstPersonCameraScript::dash(glm::vec3 desiredPosition, float speedDelta) {

	ComponentHandle<Camera> cam = entity->get<Camera>();

	float dashspeed = 20;

	if (numDashes < 1) {
		return desiredPosition;
	}

	numDashes--;


	cout<<"ola"<<endl;

	switch (dir) {
	case 1:
		desiredPosition += speedDelta * cam->orientation * dashspeed;
		break;
	case 2:
		desiredPosition += speedDelta * glm::normalize(glm::cross(cam->orientation, cam->up)) * dashspeed;
		break;
	case 3:
		desiredPosition += speedDelta * -cam->orientation * dashspeed;
		break;
	case 4:
		desiredPosition += speedDelta * -glm::normalize(glm::cross(cam->orientation, cam->up)) * dashspeed;
		break;
	}

	dashStart = glfwGetTime();

	return desiredPosition;

}


void FirstPersonCameraScript::move(float speedDelta) {

	ComponentHandle<Camera> cam = entity->get<Camera>();

	glm::vec3 currentPosition = cam->position;
	glm::vec3 desiredPosition = cam->position;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		desiredPosition += speedDelta * cam->orientation;
		dir = 1;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		desiredPosition += speedDelta * -glm::normalize(glm::cross(cam->orientation, cam->up));
		dir = 4;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		desiredPosition += speedDelta * -cam->orientation;
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
		desiredPosition = dash(desiredPosition, speedDelta);
	}

	desiredPosition = jump(desiredPosition, speedDelta);

	if (!isJumping) {
		desiredPosition += speedDelta * -cam->up;
	}

	if (glfwGetTime() > dashStart + 2.f) {
		numDashes = 10;
	}

	world->each<CubeCollider>([&](Entity* ent, ComponentHandle<CubeCollider> cubeColl) {

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

void FirstPersonCameraScript::tickScript(float deltaTime) {

	float speedDelta = speed * deltaTime;

	float width = 800;
	float height = 800;

	ComponentHandle<Camera> cam = entity->get<Camera>();

	move(speedDelta);

	// Handles mouse inputs
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		// Hides mouse cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		// Prevents camera from jumping on the first click
		if (firstClick)
		{
			glfwSetCursorPos(window, (width / 2), (height / 2));
			firstClick = false;
		}

		// Stores the coordinates of the cursor
		double mouseX;
		double mouseY;
		// Fetches the coordinates of the cursor
		glfwGetCursorPos(window, &mouseX, &mouseY);

		// Normalizes and shifts the coordinates of the cursor such that they begin in the middle of the screen
		// and then "transforms" them into degrees 
		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		// Calculates upcoming vertical change in the Orientation
		glm::mat4 m = glm::mat4(1.0f);

		m = glm::rotate(m, glm::radians(-rotX), glm::normalize(glm::cross(cam->orientation, cam->up)));

		// Rotates the Orientation left and right
		glm::mat4 m2 = glm::mat4(1.0f);

		m2 = glm::rotate(m2, glm::radians(-rotY), cam->up);

		cam->orientation = m2 * glm::vec4(cam->orientation, 1.);

		// Sets mouse cursor to the middle of the screen so that it doesn't end up roaming around
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		// Unhides cursor since camera is not looking around anymore
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		// Makes sure the next time the camera looks around it doesn't jump
		firstClick = true;
	}

}