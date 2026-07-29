#pragma once

#include <UniDx.h>
#include <UniDx/Behaviour.h>


class Player;
class CameraController : public UniDx::Behaviour
{
public:
    const Player* player = nullptr;

    virtual void OnEnable() override;
    virtual void Update() override;

private:
    float pitch=0.0f;
    float yaw=0.0f;
    float length=0.0f;
};