#pragma once

#include <UniDx.h>
#include <UniDx/Rigidbody.h>

using namespace UniDx;

class Player : public UniDx::Behaviour
{
public:
    virtual void OnEnable() override;
    virtual void Update() override;
    virtual void OnTriggerEnter(Collider* other) override;
    virtual void OnTriggerStay(Collider* other) override;
    virtual void OnTriggerExit(Collider* other) override;
    virtual void OnCollisionEnter(const Collision& collision) override;
    virtual void OnCollisionExit(const Collision& collision) override;

    UniDx::Rigidbody* rb = nullptr;

private:
    std::vector<UniDx::Transform*>bones;
    std::vector<UniDx::Quaternion>initialRotate;
    float animFrame=0.0f;


	bool isGrounded = false; 

    const float jumpPower = 7.0f;
	const float wallJumpPowerY = 7.0f;
	const float wallJumpPowerX = 6.0f;
    const float wallCheckDistance = 1.8f;
};
