/*================================================================
Filename: ResourceNames.h
Date: 2017.10.26
Created by AirGuanZ
================================================================*/

/*
    ×ÊÔ´Â·¾¶×¢²á
*/

#ifndef __RESOURCE_NAMES_H__
#define __RESOURCE_NAMES_H__

#define MM_FILENAME(X) (_MM_FILENAME(X))
#define _MM_FILENAME(X) "Bin\\"##X

#define ACTOR_ACTION_RESOURCE MM_FILENAME("Actor\\actionRsc.conf")
#define GLOBAL_TEXTURE_RESOURCE MM_FILENAME("texs.conf")

#define SCENE_LIGHT_VERTEX_SHADER MM_FILENAME("Shader\\SceneLight.vs")
#define SCENE_LIGHT_FRAGMENT_SHADER MM_FILENAME("Shader\\SceneLight.fs")

#endif //__RESOURCE_NAMES_H__
