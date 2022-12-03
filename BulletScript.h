#pragma once
#include "Script.h"

using namespace std;

class BulletScript : public Script
{

    using Script::Script;

public:

    void startScript() override;

    void setPlayer(Entity* player);

    void checkCollisions();

    void move(float speedDelta);

    void tickScript(float deltaTime) override;

private:

    float speed = 0.25;

    glm::vec3 dir;

    Entity* player;
};
