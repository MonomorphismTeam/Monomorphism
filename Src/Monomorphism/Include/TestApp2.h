/*================================================================
Filename: TestApp2.h
Date: 2017.10.16
Created by AirGuanZ
================================================================*/

/*
    关于Scale System和Clock
*/

#ifndef __TEST_APP_2_H__
#define __TEST_APP_2_H__

#include <glm\glm.hpp>
#include <OWE.h>

namespace Test
{
    using namespace OWE;
    using namespace glm;
    using namespace std;

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
            desc.winWidth = 1440;
            desc.winHeight = 768;
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
            const vec2 vtxPosData[] =
            {
                { -1.0f, -1.0f },
                { -1.0f, +1.0f },
                { +1.0f, +1.0f },

                { -1.0f, -1.0f },
                { +1.0f, +1.0f },
                { +1.0f, -1.0f }
            };

            const vec3 vtxColorData[] =
            {
                { 1.0f, 0.0f, 0.0f },
                { 0.0f, 1.0f, 0.0f },
                { 0.0f, 0.0f, 1.0f },

                { 1.0f, 0.0f, 0.0f },
                { 0.0f, 0.0f, 1.0f },
                { 0.0f, 1.0f, 1.0f }
            };

            vtxPos_.Initialize(6, vtxPosData);
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
        }

        void Run(void)
        {
            InitGLContext();
            scale_.Reinit(100.0f, 100.0f);
            InitVertices();
            InitShader();

            attribs_->GetAttrib<vec2>("pos").SetBuffer(vtxPos_);
            attribs_->GetAttrib<vec3>("color").SetBuffer(vtxColor_);

            pos_ = vec2(0.5f * scale_.ScreenWidth(), 0.5f * scale_.ScreenHeight());
            done_ = false;
            clock_.Restart();

            while(!done_)
            {
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                uniforms_->GetUniform<glm::mat3>("transMat").SetVal(scale_.ProjMatrix() * Transform::Translate(pos_));
                shader_.Bind();
                attribs_->Bind();

                uniforms_->Apply();
                glDrawArrays(GL_TRIANGLES, 0, 6);

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
