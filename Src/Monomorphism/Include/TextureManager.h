/*================================================================
Filename: TextureManager.h
Date: 2017.11.12
Created by AirGuanZ
================================================================*/
#ifndef __TEXTURE_MANAGER_H__
#define __TEXTURE_MANAGER_H__

#include <map>
#include <string>

#include <OWE.h>

class TextureManager
{
public:
    bool Initialize(const std::string &configFile);

    OWE::Texture2DView GetTexture(const std::string &name) const;

    void Clear(void);

private:
    std::map<std::string, OWE::Texture2D> texs_;
};

#endif //__TEXTURE_MANAGER_H__
