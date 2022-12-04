#pragma once
#include "Script.h"
#include "ScriptManager.h"


class SpawnerScript : public Script
{

    using Script::Script;

public:

    void startScript() override;
    void setParameters(ScriptManager* scriptManager, Entity* target);

    Entity* CreateEntity3DWithMesh(glm::vec3 position, float scale, const char* meshFilepath, const char* texFilepath);

    void tickScript(float deltaTime) override;

private:

    float delay1 = 3000.f;
    float delay2 = 6000.f;
    float t1 = 0;
    float t2 = 0;
    int counter = 0;

    bool bossSpawned = false;

    ScriptManager* scriptManager;
    Entity* target;

};
