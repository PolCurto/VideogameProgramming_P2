#pragma once
#include "Script.h"
#include "ScriptManager.h"

using namespace std;

class BossScript : public Script
{

    using Script::Script;

public:

    void startScript() override;

    Entity* CreateEntity3DWithMesh(glm::vec3 position, float scale, const char* meshFilepath, const char* texFilepath);

    void setParameters(Entity* player, ScriptManager* scriptManager);

    void checkCollisions();

    void checkPhase();

    void move(float speedDelta);

    void shoot();

    bool isDead();

    void tickScript(float deltaTime) override;

private:


    Entity* player;
    ScriptManager* scriptManager;

    glm::vec3 currDir;

    float speed = 0.005f;
    float delay = 4;
    float previousShot = 0;

    int life = 15;
    int phase = 1;

    bool alive = true;

};
