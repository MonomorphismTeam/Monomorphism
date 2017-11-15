#pragma once
#include<OWE.h>
#include"Creature.h"
enum class  CreatureRelation
{
    HOSTILE = 1,
    NEUTRAL,
    FRIENDLY
};

class NormalCreature : public Creature
{
public:
    enum class Status
    {
        DEAD = 1,
        FLOATING,//在空中
        STANDING,//特指站在地面上的状态

    };


    NormalCreature(float, glm::vec2, glm::vec2, std::string[3], float, float);


    //add constructor with all 
    ~NormalCreature(void);// May add kill here
    void SetHP(float);
    void MinusHP(float s) { hp_ -= s; delaytime = 400; };
    void Update(glm::vec2, double time);
    void Draw(const OWE::ScreenScale &screenscale);// temp Void need add 
    std::vector<OWE::BoundingArea> GetBoundingAreas(void) const;

    void Attack1(void);



    CreatureRelation RelationwithActor(void) const;


    void SetPosition(const glm::vec2);
    glm::vec2 GetPosition(void) const;
    glm::vec2 GetVelocity(void) const;

    void SetRelation(CreatureRelation);
    void SetVelocity(glm::vec2 vec)
    {
        velocity_ = vec;
        if(velocity_.y == 0 && status == NormalCreature::Status::FLOATING)
            status = NormalCreature::Status::STANDING;
    };
    //void deleteArea(OWE::BoundingArea);
    bool IsDead(void) const;
    //setPosition
    //

private:
    const double standtim = 1998;
    float hp_;
    double deadlasttime = 2500;
    double delaytime = 400;
    glm::vec2 lp_, rp_;
    glm::vec2 velocity_;
    OWE::Texture2D texture_, texdead, texattacked;
    CreatureRelation relation_;
    double attack1CoolDown_;
    Status status;

    float attackedspeedy = 0.0002f;
    float gravity = 0.00007f;
    float xspeed;
    float damage_;
    // update here;
    void _updateSTANDING(glm::vec2 playerPoi, double time);
    void _updateFLOATING(glm::vec2 playerPoi, double time);
    void _updateDEAD(double);
    double standingtime;

    //weapon
    //position_ + velocity_
    // using weapon to add attack mode

    //0.012f //0.025f
};