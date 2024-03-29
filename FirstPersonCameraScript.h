#pragma once

#include "Script.h"

using namespace std;

class FirstPersonCameraScript : public Script
{

    using Script::Script;

public:

    void startScript() override;

    void shoot();

    void move(float speedDelta);

    void jump(glm::vec3* desiredPosition, float speedDelta);

    void dash(glm::vec3* desiredPosition, float speedDelta);

    void moveView();

    void tickScript(float deltaTime) override;

private:

    glm::vec3 direction = glm::vec3(1., 0., 0.);
    
    float speed = 0.01f;
    float sensitivity = 100.0f;
    float jumpStart = 0;
    float dashStart = 0;
    
    float previousY = 0;
    float checkGround = 0.05;

    bool startGame = true;
    bool isJumping = false;
    bool isOnGround = true;
    bool canDash = true;
    bool canShoot = true;

    int dir = 1;
    int numJumps = 2;
    int numDashes = 10;
};