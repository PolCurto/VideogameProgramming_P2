#pragma once

#include "Script.h"

using namespace std;

class FirstPersonCameraScript : public Script
{

    using Script::Script;

public:

    void startScript() override;

    void setParameters(Entity* lifeBar, Entity* powerUp);

    bool isPoweredUp();

    void checkPowerUp();

    void move(float speedDelta);

    void jump(glm::vec3* desiredPosition, float speedDelta);

    void dash(glm::vec3* desiredPosition, float speedDelta);

    void checkHits();

    void moveView();

    bool isDead();

    void tickScript(float deltaTime) override;

private:

    Entity* lifeBar;
    Entity* powerUp;

    glm::vec3 direction = glm::vec3(1., 0., 0.);
    
    float speed = 0.015f;
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
    bool vulnerable = true;
    bool alive = true;
    bool poweredUp = false;

    int dir = 1;
    int numJumps = 2;
    int numDashes = 10;
    int life = 3;

    float lastX = 400;
    float lastY = 400;
    float yaw = 0.0f;
    float pitch = 0.0f;
    float lastVulnerable = 0;
    float lastPowered = 0;
};