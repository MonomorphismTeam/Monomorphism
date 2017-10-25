/*================================================================
Filename: ActorAction.h
Date: 2017.10.24
Created by AirGuanZ
================================================================*/
#ifndef __ACTOR_ACTION_H__
#define __ACTOR_ACTION_H__

#include <string>
#include <vector>

#include <glm\glm.hpp>
#include <OWE.h>

//一个动作是一系列针对身体部件的仿射变换
//kpSeq[i]指出变换transSeq[i]的结束时间
//要求transSeq.size() = kpSeq.size()
struct ActorActionData
{
    using TransSeq = std::vector<glm::mat3>;
    using KeypntsSeq = std::vector<float>;

    TransSeq transSeq;
    KeypntsSeq kpSeq;

    /* 文件格式
        (float float^9 \n)+
       每行第一个float值为kp值，其后9个float值给出仿射变换矩阵的列主序表示
    */
    bool LoadFromFile(const std::string &filename);
};

//访问权限好烦 >_< 干脆struct吧
struct ActorAction : public ActorActionData
{
    void Tick(float deltaTime);
    void Restart(void);
    bool End(void) const;

    float time = 0.0f;
    int idx;
};

#endif //__ACTOR_ACTION_H__
