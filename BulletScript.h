#pragma once
#include "Script.h"

using namespace std;

class BulletScript : public Script
{

    using Script::Script;

public:

    void startScript() override;

    void setParameters(Entity* player, Entity* floor);

    void checkCollisions();

    void move(float speedDelta);

    void tickScript(float deltaTime) override;

private:

    float speed = 0.25;
    float time = 0;

    glm::vec3 dir;

    Entity* player;
    Entity* floor;
};
