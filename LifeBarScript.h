#pragma once
#include "Script.h"

using namespace std;

class LifeBarScript : public Script
{

    using Script::Script;

public:

    void startScript() override;

    void tickScript(float deltaTime) override;

private:

    int hearts = 5;
};

