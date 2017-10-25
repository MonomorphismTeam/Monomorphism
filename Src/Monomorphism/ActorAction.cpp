/*================================================================
Filename: ActorAction.cpp
Date: 2017.10.24
Created by AirGuanZ
================================================================*/
#include <fstream>
#include <sstream>

#include "Include\ActorAction.h"

namespace
{
    bool _LoadActorActionFromFile(const std::string &filename, ActorActionData &rt)
    {
        rt.transSeq.clear();
        rt.kpSeq.clear();

        std::ifstream fin(filename, std::ifstream::in);
        if(!fin)
            return false;

        std::string line;
        float kp, x1, y1, z1, x2, y2, z2, x3, y3, z3;
        while(std::getline(fin, line) && fin)
        {
            if(line.empty())
                continue;
            std::stringstream sst(line);
            sst >> kp >> x1 >> y1 >> z1
                >> x2 >> y2 >> z2
                >> x3 >> y3 >> z3;
            if(!sst)
            {
                rt.transSeq.clear();
                rt.kpSeq.clear();
                return false;
            }
            rt.transSeq.push_back(glm::mat3(x1, y1, z1,
                x2, y2, z2,
                x3, y3, z3));
            rt.kpSeq.push_back(kp);
        }

        fin.close();
        return true;
    }
}

bool ActorActionData::LoadFromFile(const std::string &filename)
{
    return _LoadActorActionFromFile(filename, *this);
}

void ActorAction::Tick(float deltaTime)
{
    time += deltaTime;
    while(idx < static_cast<int>(transSeq.size()) - 1 && time >= kpSeq[idx])
        ++idx;
}

void ActorAction::Restart(void)
{
    time = 0.0f;
    idx = 0;
}

bool ActorAction::End(void) const
{
    return kpSeq.empty() || time >= kpSeq.back();
}
