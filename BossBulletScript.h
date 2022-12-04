#pragma once
#include "Script.h"

using namespace std;

class BossBulletScript : public Script
{

    using Script::Script;

public:

    void startScript() override;

    void setParameters(Entity* boss, Entity* player);

    void checkCollisions();

    void move(float speedDelta);

    void tickScript(float deltaTime) override;

private:

    float speed = 0.03;
    float time = 0;

    glm::vec3 dir;

    Entity* boss;
    Entity* player;
};
