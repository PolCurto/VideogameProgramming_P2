#pragma once
#include "Script.h"
#include "ScriptManager.h"

using namespace std;

class PlayerManagerScript : public Script
{

    using Script::Script;

public:

    void startScript() override;
    void setParameters(Entity* player, ScriptManager* scriptManager);

    Entity* CreateEntity3DWithMesh(glm::vec3 position, float scale, const char* meshFilepath, const char* texFilepath);

    void shoot();

    void tickScript(float deltaTime) override;

private:

    Entity* player;
    ScriptManager* scriptManager;

};

