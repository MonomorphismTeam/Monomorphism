/*================================================================
Filename: TestApp1.h
Date: 2017.10.16
Created by AirGuanZ
================================================================*/

/*
    关于Shader，Texture，FrameBuffer的基本测试
*/

#ifndef __TEST_APP_1_H__
#define __TEST_APP_1_H__

#include <iostream>
#include <string>

#include <OWE.h>

namespace Test
{
    using namespace std;
    using namespace OWE;

    class TestApp1 : public WindowListener
    {
    public:
        int Run(void)
        {
            RenderContext::Desc desc;
            desc.bordered = true;
            desc.resizable = false;
            desc.fullscreen = false;
            desc.winWidth = 1440;
            desc.winHeight = 768;
            desc.title = "Monomorphism";
            desc.glVersion = "4.3";

            //初始化RenderContext
            RenderContext::InitInstance(desc);
            RenderContext &rc = RenderContext::GetInstance();
            InputManager &im = InputManager::GetInstance();

            rc.AddWindowListener(this);

            //加载shader文件
            Shader shader;
            string vtx, frag, err;
            Utility::ASCIIFile::Read("TestApp1/test.vs", vtx);
            Utility::ASCIIFile::Read("TestApp1/test.fs", frag);
            if(shader.Initialize(err, VSSrc(vtx), FSSrc(frag)) != Shader::InitState::Success)
            {
                cout << err << endl;
                return -1;
            }

            //准备vertex buffer
            VertexBuffer<glm::vec4> vec4Buf;
            const glm::vec4 vec4BufData[] =
            {
                { -0.5f, -0.5f, 0.0f, 1.0f },
                { 0.0f, 0.5f, 0.0f, 1.0f },
                { 0.5f, -0.5f, 0.0f, 1.0f }
            };
            vec4Buf.Initialize(3, vec4BufData);

            VertexBuffer<glm::vec2> uvBuf;
            const glm::vec2 uvBufData[] =
            {
                { 0.0, 0.0 },
                { 0.5, 1.0 },
                { 1.0, 0.0 }
            };
            uvBuf.Initialize(3, uvBufData);

            Texture2D tex;
            if(!LoadTexture2DFromFile("TestApp1/testTex.png", Texture2D::Desc(), tex))
            {
                cout << "Failed to load testTex.png" << endl;
                return -1;
            }

            //准备uniform variable
            Shader::UniformMgrPtr uniformMgr = shader.GetUniformMgrPtr();
            UniformVariable<Texture2DBase> texSam = uniformMgr->GetUniform<Texture2DBase>("tex");
            UniformVariable<glm::vec2> offset = uniformMgr->GetUniform<glm::vec2>("offset");

            glm::vec2 offsetVal(0.0f, 0.0f);

            //准备顶点属性
            Shader::AttribMgrPtr attribMgr = shader.GetAttribMgrPtr();
            AttribVariable<glm::vec4> pos = attribMgr->GetAttrib<glm::vec4>("position");
            AttribVariable<glm::vec2> uv = attribMgr->GetAttrib<glm::vec2>("uv_in");
            pos.SetBuffer(vec4Buf);
            uv.SetBuffer(uvBuf);

            FrameBuffer fb;
            fb.Initialize(100, 100, 1);
            fb.AddTex(0, Texture2D::Desc());
            fb.AddDepth();
            if(!fb.IsAvailable())
            {
                cout << "Failed to initialize framebuffer" << endl;
                return -1;
            }

            //主循环
            while(!closed_)
            {
                glClearColor((abs(glm::sin(t_ += 0.08f)) + 1.0f) / 2.0f, 0.0f, 1.0f, 1.0f);
                rc.ClearColorAndDepth();

                constexpr float SPEED = 0.01f;
                InputManager &im = InputManager::GetInstance();
                if(im.IsKeyPressed(KEY_CODE::KEY_W))
                    offsetVal += glm::vec2(0.0f, SPEED);
                if(im.IsKeyPressed(KEY_CODE::KEY_A))
                    offsetVal += glm::vec2(-SPEED, 0.0f);
                if(im.IsKeyPressed(KEY_CODE::KEY_S))
                    offsetVal += glm::vec2(0.0f, -SPEED);
                if(im.IsKeyPressed(KEY_CODE::KEY_D))
                    offsetVal += glm::vec2(SPEED, 0.0f);
                offset.SetVal(offsetVal);

                //渲染到纹理
                fb.Begin();
                {
                    glClearColor((abs(glm::sin(t_)) + 1.0f) / 2.0f, 1.0f, 0.0f, 1.0f);
                    rc.ClearColorAndDepth();

                    texSam.SetVal(tex);

                    shader.Bind();
                    attribMgr->Bind();

                    uniformMgr->Apply();
                    rc.DrawTriangles(3);

                    attribMgr->Unbind();
                    shader.Unbind();
                }
                fb.End();

                //渲染主场景
                texSam.SetVal(fb.GetTex(0));

                shader.Bind();
                attribMgr->Bind();

                uniformMgr->Apply();
                rc.DrawTriangles(3);

                attribMgr->Unbind();
                shader.Unbind();

                rc.DoEvents();
                rc.Present();

                if(im.IsKeyPressed(KEY_CODE::KEY_ESCAPE))
                    closed_ = true;
            }

            return 0;
        }

    protected:
        bool closed_ = false;
        float t_ = 0.0;

        void WindowClosed(void)
        {
            closed_ = true;
        }
    };
}

#endif //__TEST_APP_1_H__
