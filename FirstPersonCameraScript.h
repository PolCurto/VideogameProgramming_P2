#pragma once

#include "Script.h"

using namespace std;

class FirstPersonCameraScript : public Script
{

    using Script::Script;

public:

    void startScript() override;

    void move(float speedDelta);

    glm::vec3 jump(glm::vec3 desiredPosition, float speedDelta);

    glm::vec3 dash(glm::vec3 desiredPosition, float speedDelta);

    void tickScript(float deltaTime) override;

private:

    glm::vec3 direction = glm::vec3(1., 0., 0.);

    bool firstClick = true;
    float speed = 0.01f;
    float sensitivity = 100.0f;
    bool isJumping = false;
    float jumpStart = 0;
    float dashStart = 0;
    bool isOnGround = true;
    float previousY = 0;
    float checkGround = 0.05;
    int numJumps = 2;
    int dir = 1;
    bool canDash = true;
    int numDashes = 10;

};