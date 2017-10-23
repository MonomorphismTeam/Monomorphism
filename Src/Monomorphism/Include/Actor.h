/*================================================================
Filename: Actor.h
Date: 2017.10.21
Created by AirGuanZ
================================================================*/

/* actor、weapon大概这么设计：
    actor有本身的动画，也有根据weapon而变的动画
    把actor的动画交给weapon会让actor的扩展非常不方便
    所以应该是actor提供有限种可供选择的用weapon的动画，weapon告知actor应该用哪个
    对weapon的形态和位置变换可以提供仿射变换，变换矩阵是actor交给weapon来画的
*/

#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <OWE.h>

class Actor : public OWE::Utility::Uncopyable
{
public:
    enum class State
    {
        Stand,
        Walk
    };

    enum class Direction
    {
        Left, Right
    };

    Actor(void);
    ~Actor(void);

    void Initialize(void);
    bool IsAvailable(void);
    void Destroy(void);

    void Restart(void);

    glm::vec2 GetPosition(void) const;
    void SetPosition(const glm::vec2 &pos);

    glm::vec2 GetVelocity(void) const;
    void SetVelocity(const glm::vec2 &vel);
    
    glm::vec2 GetAcceleratedVelocity(void) const;
    void SetAcceleratedVelocity(const glm::vec2 &pos);

    bool IsOnGround(void) const;
    void OnGround(bool onGround);

    void Update(double deltaTime);
    void Draw(void);

    glm::mat3 GetWeaponTrans(void) const;

private:
    glm::vec2 position_;
    glm::vec2 velocity_;
    glm::vec2 acceleratedVelocity_;

    float HP_;
    bool onGround_;

    State state_;
    Direction dir_;
    OWE::Clock clock_;
};

#endif //__ACTOR_H__
