#pragma once
#include "Script.h"

using namespace std;

class PlayerManagerScript : public Script
{

    using Script::Script;

public:

    void startScript() override;

    void shoot();

    void tickScript(float deltaTime) override;

private:

};

