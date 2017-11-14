/*================================================================
Filename: TextureManager.cpp
Date: 2017.11.12
Created by AirGuanZ
================================================================*/
#include "Include\ConfigureFile.h"
#include "Include\TextureManager.h"

bool TextureManager::Initialize(const std::string &configFile)
{
    texs_.clear();

    ConfigureFile conf;
    if(!conf.Load(configFile))
        throw OWE::FatalError("Failed to load configure file: " + configFile);

    auto loadTex = [&](const std::string &texName, const std::string &filename)
    {
        OWE::Texture2D tex;
        if(!OWE::LoadTexture2DFromFile(filename, OWE::Texture2D::Desc(), tex))
            throw OWE::FatalError("Failed to load texture from file: " + filename);
        texs_.insert(std::make_pair(texName, std::move(tex)));
    };
    conf.ForEach(loadTex, "Land");
    conf.ForEach(loadTex, "Creature");
    conf.ForEach(loadTex, "Light");
    conf.ForEach(loadTex, "Other");
    conf.ForEach(loadTex, "GUI");

    return true;
}

OWE::Texture2DView TextureManager::GetTexture(const std::string &name) const
{
    auto it = texs_.find(name);
    if(it == texs_.end())
        return OWE::Texture2DView();
    return it->second.GetTextureView();
}

void TextureManager::Clear(void)
{
    texs_.clear();
}
