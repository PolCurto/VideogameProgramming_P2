#pragma once
#include "Script.h"

using namespace std;

class PlayerManagerScript : public Script
{

    using Script::Script;

public:

    void startScript() override;

    Entity* CreateEntity3DWithMesh(glm::vec3 position, float scale, const char* meshFilepath, const char* texFilepath);

    void shoot();

    void tickScript(float deltaTime) override;

private:

};

