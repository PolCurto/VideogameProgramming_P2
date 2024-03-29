#pragma once
#include "Script.h"

using namespace std;

class Enemy1Script : public Script
{

    using Script::Script;

public:

    void startScript() override;

    void move(float speedDelta);

    void tickScript(float deltaTime) override;

private:

    float speed = 0.01f;
};
