#pragma once

#include "Player.h"

#include <UniDx/Input.h>
#include <UniDx/Collider.h>
#include <UniDx/Time.h>
#include <UniDx/PrimitiveRenderer.h>
#include <UniDx/Physics.h>

#include "MainGame.h"

using namespace DirectX;
using namespace UniDx;

namespace
{
    const StringId CoinName = StringId::intern("Coin");

    const StringId BoneName[] =
    {
        StringId::intern("LeftUpperArm"),
        StringId::intern("RightUpperArm"),
        StringId::intern("LeftUpperLeg"),
        StringId::intern("RightUpperLeg"),
        StringId::intern("Tail")
    };
    // アニメーションさせる角度の範囲（pitch, yaw, roll）
    const Vector3 Range[] =
    {
        Vector3(80,  0,  0),
        Vector3(-80,  0,  0),
        Vector3(30,  0, 45),
        Vector3(-30,  0, 45),
        Vector3(30,  0,  0),
    };
    // アニメーションさせる角度のオフセット（pitch, yaw, roll）
    const Vector3 Offset[] =
    {
        Vector3(0,  0, 30),
        Vector3(0,  0,-30),
        Vector3(0,  5,  0),
        Vector3(0, -5,  0),
        Vector3(20,  0,  0),
    };
    constexpr size_t BoneMax = sizeof(BoneName) / sizeof(StringId);
    constexpr float animSpeed = 0.07f;
}


void Player::OnEnable()
{
    rb = GetComponent<Rigidbody>(true);
    assert(rb != nullptr);

    GetComponent<Collider>(true)->bounciness = 0.0f;
}

void Player::Update()
{
    const float moveSpeed = 5;

    // 操作方向
    Vector3 cont;
    if (Input::GetKey(Keyboard::A)) cont.x = -1.0f;
    else if (Input::GetKey(Keyboard::D)) cont.x = 1.0f;

    if (Input::GetKey(Keyboard::S)) cont.z = -1.0f;
    else if (Input::GetKey(Keyboard::W)) cont.z = 1.0f;

    cont = cont.normalized();

    // カメラ方向を考慮して水平の移動速度を計算
    Vector3 camF = Camera::main->transform->forward;
    float camAngle = std::atan2(camF.x, camF.z) * UniDx::Rad2Deg;
    Vector3 moveVelocity = cont * moveSpeed * Quaternion::AngleAxis(camAngle, Vector3::up);

    Vector3 currentVel = rb->linearVelocity;
    currentVel.x = moveVelocity.x;
    currentVel.z = moveVelocity.z;

    //壁があるか判定
    Vector3 pPos = transform->position;
    
    bool isNearWall = (pPos.z >= 1.8f && pPos.z <= 2.8f) && (std::abs(pPos.x) <= 3.0f);


    //壁ジャン処理です
    if (Input::GetKeyDown(Keyboard::Space))
    {
        if (!isGrounded && isNearWall)
        {
            //壁ジャン
            currentVel.y = wallJumpPowerY;
            currentVel.z = -wallJumpPowerX;

            isGrounded = false;
        }
        //通常ジャンプ
        else if (isGrounded || std::abs(rb->linearVelocity.y) < 0.1f)
        {
            currentVel.y = jumpPower;
            isGrounded = false;
        }
    }
    rb->linearVelocity = currentVel;

    // プレイヤーの向きを変更
    if (cont != Vector3::zero)
    {
        float vAngle = std::atan2(moveVelocity.x, moveVelocity.z) * UniDx::Rad2Deg;
        rb->rotation = Quaternion::Euler(0, vAngle, 0);
    }

    // プログラムアニメーション
    animFrame += cont.magnitude();
    for (size_t i = 0; i < bones.size(); ++i)
    {
        auto& bone = bones[i];
        if (bone == nullptr) continue;

        float sn = std::sin(animFrame * animSpeed);
        Quaternion r = Quaternion::Euler(
            sn * Range[i].x + Offset[i].x,
            sn * Range[i].y + Offset[i].y,
            sn * Range[i].z + Offset[i].z);
        bone->localRotation = r * initialRotate[i];
    }
}


void Player::OnTriggerEnter(Collider* other)
{
}


void Player::OnTriggerStay(Collider* other)
{
}


void Player::OnTriggerExit(Collider* other)
{
}


void Player::OnCollisionEnter(const Collision& collision)
{
    if (collision.collider->name == CoinName)
    {
        MainGame::getInstance()->AddScore(1);
        Destroy(collision.collider->gameObject);
        return;
    }

    if (collision.collider->name == StringId::intern("床"))
    {
        isGrounded = true;
    }
}

void Player::OnCollisionExit(const Collision& collision)
{
    if (collision.collider->name == StringId::intern("床"))
    {
        isGrounded = false;
    }
}

