/*================================================================
Filename: Actor.h
Date: 2017.10.21
Created by AirGuanZ
================================================================*/
#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <glm\glm.hpp>
#include <OWE.h>

class Actor : public OWE::Utility::Uncopyable
{
public:
    Actor(void);
    ~Actor(void);

    glm::vec2 GetPosition(void) const;
    void SetPosition(const glm::vec2 &pos);

    glm::vec2 GetVelocity(void) const;
    void SetVelocity(const glm::vec2 &vel);
    
    glm::vec2 GetAcceleratedVelocity(void) const;
    void SetAcceleratedVelocity(const glm::vec2 &pos);

private:
    glm::vec2 position_;
    glm::vec2 velocity_;
    glm::vec2 acceleratedVelocity_;

    float HP_;
    bool onGround_;
};

#endif //__ACTOR_H__
