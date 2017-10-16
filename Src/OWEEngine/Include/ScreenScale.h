/*================================================================
Filename: ScreenScale.h
Date: 2017.10.15
Created by AirGuanZ
================================================================*/

//窗口尺寸和OpenGL屏幕坐标是割裂的，直接用两者中的任何一个都太僵硬
//Scale系统用于建立一个新的坐标系统将二者联系起来

#ifndef __OWE_SCREEN_SCALE_H__
#define __OWE_SCREEN_SCALE_H__

#include <cassert>
#include <glm\glm.hpp>

#include "Common.h"
#include "RenderContext.h"

__OWE_BEGIN_NAMESPACE__(OWE)

//Screen坐标系：左下角为(0, 0)，刻度由user提供的xpp和ypp指定
//GL坐标系：OpenGL屏幕坐标系，左下角为(-1, -1)，右上角为(1, 1)
//Client坐标系：窗口坐标系，左上角为(0, 0)，y轴正方向向下，刻度为像素

class ScreenScale
{
public:
    ScreenScale(void);
    ScreenScale(float xpp, float ypp);
    ~ScreenScale(void);

    //窗口大小改变后需重新初始化该系统

    void Reinit(float xpp, float ypp);

    float GetXPerPixel(void) const;
    float GetYPerPixel(void) const;

    float ScreenWidth(void) const;
    float ScreenHeight(void) const;

    float ScreenToGLX(float x) const;
    float ScreenToClientX(float x) const;

    float GLToScreenX(float x) const;
    float ClientToScreenX(float x) const;

    float ScreenToGLY(float y) const;
    float ScreenToClientY(float y) const;

    float GLToScreenY(float y) const;
    float ClientToScreenY(float y) const;

    glm::vec2 ScreenToGL(const glm::vec2 &screenCoord) const;
    glm::vec2 ScreenToClient(const glm::vec2 &screenCoord) const;

    glm::vec2 GLToScreen(const glm::vec2 &GLCoord) const;
    glm::vec2 ClientToScreen(const glm::vec2 &clientCoord) const;

    glm::mat3 ProjMatrix(void) const;

private:
    float xpp_;
    float ypp_;

    int clientWidth_;
    int clientHeight_;
    float screenWidth_;
    float screenHeight_;
};

__OWE_END_NAMESPACE__(OWE)

#include "ScreenScale.inl"

#endif //__OWE_SCREEN_SCALE_H__
