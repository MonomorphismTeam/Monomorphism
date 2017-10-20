/*================================================================
Filename: TestApp4.h
Date: 2017.10.20
Created by AirGuanZ
================================================================*/

/*
    使用演示
*/

#ifndef __TEST_APP_4_H__
#define __TEST_APP_4_H__

#include <string>
#include <vector>

#include <glm\glm.hpp>
#include <OWE.h>

namespace Test
{
    using namespace std;
    using namespace glm;
    using namespace OWE;

    class TestApp4 : public KeyboardListener, public WindowListener
    {
    public:
        void WindowClosed(void)
        {
            done_ = true;
        }

        void KeyDown(KEY_CODE kc)
        {
            if(kc == KEY_CODE::KEY_ESCAPE)
                done_ = true;
        }

        bool Init(void)
        {
            //============== 初始化RenderContext ==============

            RenderContext::Desc desc;
            desc.bordered = true;
            desc.resizable = false;
            desc.fullscreen = false;
            desc.winWidth = 1200;
            desc.winHeight = 900;
            desc.title = "Monomorphism";
            desc.glVersion = "4.3";

            RenderContext::InitInstance(desc);
            RenderContext &rc = RenderContext::GetInstance();
            InputManager &im = InputManager::GetInstance();

            rc.AddWindowListener(this);
            im.AddKeyboardListener(this);

            //============== 初始化小球 ==============

            constexpr float BALL_RADIUS = 5.0f;
            vector<vec2> boxVtxData(6);

            Utility::GenBoxVertices(vec2(-BALL_RADIUS), vec2(BALL_RADIUS), boxVtxData.data());
            ballVtxPos_.Initialize(6, boxVtxData.data());

            Utility::GenBoxVertices(vec2(0.0f), vec2(1.0f), boxVtxData.data());
            balVtxUV_.Initialize(6, boxVtxData.data());

            string ballVtxSrc, ballVtxFrag, errMsg;
            Utility::ASCIIFile::Read("TestApp4/ball.vs", ballVtxSrc);
            Utility::ASCIIFile::Read("TestApp4/ball.fs", ballVtxFrag);
            if(ballShader_.Initialize(errMsg, VSSrc(ballVtxSrc), FSSrc(ballVtxFrag)) != Shader::InitState::Success)
            {
                cout << errMsg << endl;
                return false;
            }

            ballAttribs_ = ballShader_.GetAttribMgrPtr();
            ballUniforms_ = ballShader_.GetUniformMgrPtr();

            if(!LoadTexture2DFromFile("TestApp4/ball.png", Texture2D::Desc(), ballTex_))
            {
                cout << "Failed to load ball texture from file" << endl;
                return false;
            }

            ballPos_ = vec2(0.0f, BALL_RADIUS + 1e-5);
            ballOldPos_ = ballPos_;

            //============== 初始化地面 ==============

            if(!LoadTexture2DFromFile("TestApp4/ground.png", Texture2D::Desc(), gdTileTex_))
            {
                cout << "Failed to load ground texture from file" << endl;
                return false;
            }

            ground_.Initialize(100, 7, 10.0f, 10.0f);
            groundOffset_ = vec2(5000.0f, 700.0f);
            for(int y = 0; y != 7; ++y)
            {
                for(int x = 0; x != 100; ++x)
                    ground_.SetTile(x, y, vec2(0.0f), vec2(1.0f), gdTileTex_);
            }

            //============== 初始化背景 ==============

            if(!LoadTexture2DFromFile("TestApp4/background1.png", Texture2D::Desc(), bkgdTex_[0]) ||
               !LoadTexture2DFromFile("TestApp4/background2.png", Texture2D::Desc(), bkgdTex_[1]) ||
               !LoadTexture2DFromFile("TestApp4/background3.png", Texture2D::Desc(), bkgdTex_[2]))
            {
                cout << "Failed to load background texture from file" << endl;
                return false;
            }

            bkgd_.Initialize(21, 1, 100.0f, 500.0f);
            bkgdOffset_ = vec2(5000.0f, 0.0f);
        }

        void Run(void)
        {
            
        }

    private:
        //============== 小球 ==============
        VertexBuffer<vec2> ballVtxPos_;
        VertexBuffer<vec2> balVtxUV_;
        Shader ballShader_;
        Shader::AttribMgrPtr ballAttribs_;
        Shader::UniformMgrPtr ballUniforms_;
        Texture2D ballTex_;

        vec2 ballPos_;
        vec2 ballOldPos_;

        //============== 地面 ==============
        TiledTexture ground_;
        Texture2D gdTileTex_;

        glm::vec2 groundOffset_;

        //============== 背景 ==============
        TiledTexture bkgd_;
        Texture2D bkgdTex_[3];

        glm::vec2 bkgdOffset_;

        //============== 障碍物 ==============
        VertexBuffer<vec2> brickVtxPos_;
        VertexBuffer<vec2> brickVtxUV_;
        Shader brickShader_;
        Shader::AttribMgrPtr brickAttribs_;
        Shader::UniformMgrPtr uniformAttribs_;
        Texture2D brickTex_;

        //============== 其他 ==============
        Clock clock_;
        ScreenScale scale_;

        bool done_;
    };
}

#endif //__TEST_APP_4_H__
