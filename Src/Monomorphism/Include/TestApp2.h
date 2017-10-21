/*================================================================
Filename: TestApp2.h
Date: 2017.10.16
Created by AirGuanZ
================================================================*/

/*
    关于Scale System和Clock的测试
*/

#ifndef __TEST_APP_2_H__
#define __TEST_APP_2_H__

#include <glm\glm.hpp>
#include <OWE.h>

namespace Test
{
    using namespace std;
    using namespace glm;
    using namespace OWE;

    class TestApp2 : public WindowListener
    {
    public:
        void WindowClosed(void)
        {
            done_ = true;
        }

        void InitGLContext(void)
        {
            RenderContext::Desc desc;
            desc.bordered = true;
            desc.resizable = false;
            desc.fullscreen = false;
            desc.winWidth = 800;
            desc.winHeight = 600;
            desc.title = "Monomorphism";
            desc.glVersion = "4.3";

            //初始化RenderContext
            RenderContext::InitInstance(desc);
            RenderContext &rc = RenderContext::GetInstance();
            InputManager &im = InputManager::GetInstance();

            rc.AddWindowListener(this);
        }

        void InitVertices(void)
        {
            vec2 vtxPosData[6];
            Utility::GenBoxVertices(vec2(-1.0f), vec2(1.0f), vtxPosData);
            vtxPos_.Initialize(6, vtxPosData);

            const vec3 vtxColorData[] =
            {
                { 1.0f, 0.0f, 0.0f },
                { 0.0f, 1.0f, 0.0f },
                { 0.0f, 0.0f, 1.0f },

                { 1.0f, 0.0f, 0.0f },
                { 0.0f, 0.0f, 1.0f },
                { 0.0f, 1.0f, 1.0f }
            };
            vtxColor_.Initialize(6, vtxColorData);
        }

        void InitShader(void)
        {
            std::string err;
            std::string vsSrc, fsSrc;
            Utility::ASCIIFile::Read("TestApp2/test.vs", vsSrc);
            Utility::ASCIIFile::Read("TestApp2/test.fs", fsSrc);
            if(shader_.Initialize(err, VSSrc(vsSrc), FSSrc(fsSrc)) != Shader::InitState::Success)
            {
                cout << err;
                exit(-1);
            }
            uniforms_ = shader_.GetUniformMgrPtr();
            attribs_ = shader_.GetAttribMgrPtr();

            attribs_->GetAttrib<vec2>("pos").SetBuffer(vtxPos_);
            attribs_->GetAttrib<vec3>("color").SetBuffer(vtxColor_);
        }

        void Run(void)
        {
            InitGLContext();
            scale_.Reinit(100.0f, 100.0f);
            InitVertices();
            InitShader();

            pos_ = vec2(0.5f * scale_.ScreenWidth(), 0.5f * scale_.ScreenHeight());
            done_ = false;
            clock_.Restart();

            while(!done_)
            {
                clock_.Tick();

                constexpr float SPEED = 5.0f / 1000.0f;
                if(InputManager::GetInstance().IsKeyPressed(KEY_CODE::KEY_W))
                    pos_ += vec2(0.0f, SPEED * clock_.ElapsedTime());
                if(InputManager::GetInstance().IsKeyPressed(KEY_CODE::KEY_A))
                    pos_ += vec2(-SPEED * clock_.ElapsedTime(), 0.0f);
                if(InputManager::GetInstance().IsKeyPressed(KEY_CODE::KEY_S))
                    pos_ += vec2(0.0f, -SPEED * clock_.ElapsedTime());
                if(InputManager::GetInstance().IsKeyPressed(KEY_CODE::KEY_D))
                    pos_ += vec2(SPEED * clock_.ElapsedTime(), 0.0f);

                RenderContext::GetInstance().ClearColorAndDepth();

                uniforms_->GetUniform<glm::mat3>("transMat").SetVal(scale_.ProjMatrix() * Transform::Translate(pos_));
                shader_.Bind();
                attribs_->Bind();

                uniforms_->Apply();
                RenderContext::GetInstance().DrawTriangles(6);

                attribs_->Bind();
                shader_.Unbind();

                RenderContext::GetInstance().DoEvents();
                RenderContext::GetInstance().Present();

                if(InputManager::GetInstance().IsKeyPressed(KEY_CODE::KEY_ESCAPE))
                    done_ = true;
            }
        }

    private:
        VertexBuffer<vec2> vtxPos_;
        VertexBuffer<vec3> vtxColor_;

        Shader shader_;
        Shader::UniformMgrPtr uniforms_;
        Shader::AttribMgrPtr attribs_;

        vec2 pos_;

        Clock clock_;
        ScreenScale scale_;

        bool done_;
    };
}

#endif //__TEST_APP_2_H__
