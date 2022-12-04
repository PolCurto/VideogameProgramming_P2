#pragma once
#include "Script.h"

using namespace std;

class BossScript : public Script
{

    using Script::Script;

public:

    void startScript() override;

    void setTarget(Entity* player);

    void checkCollisions();

    void checkPhase();

    void move(float speedDelta);

    void tickScript(float deltaTime) override;

private:


    Entity* player;

    glm::vec3 currDir;

    float speed = 0.005f;
    int life = 15;
    int phase = 1;

};
