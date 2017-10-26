/*================================================================
Filename: TestApp4_TextureAnimation.h
Date: 2017.10.25
Created by AirGuanZ
================================================================*/
#ifndef __TEST_APP_4_TEXTURE_ANIMATION_H__
#define __TEST_APP_4_TEXTURE_ANIMATION_H__

#include <glm\glm.hpp>
#include <OWE.h>

namespace Test
{
    using namespace std;
    using namespace glm;
    using namespace OWE;

    class TestApp4_TextureAnimation : public KeyboardListener
    {
        static const int TEX_CNT = 8;

        TextureAnimation texAni_;
        Texture2D texs_[TEX_CNT];

        ScreenScale scale_;
        Clock clock_;

        bool done_;

    public:
        void KeyDown(KEY_CODE kc)
        {
            if(kc == KEY_CODE::KEY_ESCAPE)
                done_ = true;
        }

        void Run(void)
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
            im.AddKeyboardListener(this);

            //加载动画纹理
            for(int i = 0; i != TEX_CNT; ++i)
            {
                if(!LoadTexture2DFromFile("TestApp4_TextureAnimation/walk" + to_string(i) + ".png", Texture2D::Desc(), texs_[i]))
                {
                    cout << "Failed to load texture from file" << endl;
                    return;
                }
            }

            TextureAnimation::Data data;
            for(int i = 0; i != TEX_CNT; ++i)
                data.texSeq.push_back(texs_[i]);
            data.kpSeq.push_back(5);
            for(int i = 1; i != TEX_CNT - 1; ++i)
                data.kpSeq.push_back(data.kpSeq[i - 1] + 100);

            texAni_ = TextureAnimation(&data);
            texAni_.SetLBPosition(vec2(0.0f, 0.0f));
            texAni_.SetRTPosition(vec2(0.86f, 2.22f));
            
            rc.SetClearColor(0.0f, 1.0f, 1.0f, 1.0f);

            scale_.Reinit(80.0f, 80.0f);
            clock_.Restart();
            texAni_.Restart();
            done_ = false;
            while(!done_)
            {
                rc.ClearColorAndDepth();

                clock_.Tick();
                if(im.IsKeyPressed(KEY_CODE::KEY_D))
                {
                    texAni_.Tick(clock_.ElapsedTime());
                    if(texAni_.End())
                    {
                        texAni_.Restart();
                        texAni_.Tick(10.0);
                    }
                    texAni_.Move(vec2(0.03f * (1.0f - abs(texAni_.GetIdx() - data.kpSeq.size() / 2.0f) / data.texSeq.size()), 0.0f));
                }
                else
                    texAni_.Restart();

                texAni_.Draw(scale_);

                rc.DoEvents();
                rc.Present();
            }
        }
    };
}

#endif //__TEST_APP_4_TEXTURE_ANIMATION_H__
