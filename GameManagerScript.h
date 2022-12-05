#pragma once
#include "Script.h"
#include "FirstPersonCameraScript.h"
#include "SpawnerScript.h"


using namespace std;

class GameManagerScript : public Script
{

    using Script::Script;

public:

    void startScript() override;

    void youWin();

    void youLose();

    void setParameters(FirstPersonCameraScript* playerScript, SpawnerScript* spawnerScript, Entity* lifeBar, Entity* mirilla);

    bool isOver();

    void tickScript(float deltaTime) override;

private:

    FirstPersonCameraScript* playerScript;
    SpawnerScript* spawnerScript;

    Entity* lifeBar;
    Entity* mirilla;

    bool over = false;

};
