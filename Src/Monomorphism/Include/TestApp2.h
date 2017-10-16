/*================================================================
Filename: TestApp2.h
Date: 2017.10.16
Created by AirGuanZ
================================================================*/
#include <iostream>
#include <string>

#include <glm\gtc\matrix_transform.hpp>

#include <OWE.h>

using namespace std;
using namespace OWE;

class TestApp2 : public WindowListener
{
public:
    struct BallVertex
    {
        glm::vec2 pos;
        glm::vec2 uv;
    };

    bool FatalError(const std::string &msg)
    {
        cout << msg << endl;
        return false;
    }

    bool InitContext(void)
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
        return true;
    }

    bool InitBall(void)
    {
        //加载球体着色器
        std::string vtxSrc, fragSrc;
        if(!Utility::ASCIIFile::Read("TestApp2/ball.vs", vtxSrc) ||
            !Utility::ASCIIFile::Read("TestApp2/ball.fs", fragSrc))
            return FatalError("Failed to load shader source from file");
        std::string shaderErr;
        if(ballShader_.Initialize(shaderErr, VSSrc(vtxSrc), FSSrc(fragSrc)) != Shader::InitState::Success)
            return FatalError(shaderErr);

        //准备顶点buffer
        constexpr float BALL_RADIUS = 10.0f;
        BallVertex ballVtxData[] =
        {
            { { -BALL_RADIUS, -BALL_RADIUS },{ 0.0f, 0.0f } },
            { { -BALL_RADIUS, +BALL_RADIUS },{ 0.0f, 1.0f } },
            { { +BALL_RADIUS, +BALL_RADIUS },{ 1.0f, 1.0f } },

            { { -BALL_RADIUS, -BALL_RADIUS },{ 0.0f, 0.0f } },
            { { +BALL_RADIUS, +BALL_RADIUS },{ 1.0f, 1.0f } },
            { { +BALL_RADIUS, -BALL_RADIUS },{ 1.0f, 0.0f } }
        };
        ballVtx_.Initialize(6, ballVtxData);

        //加载球体纹理
        if(!LoadTexture2DFromFile("TestApp2/ball.png", Texture2D::Desc(), ballTex_))
            return FatalError("Failed to load texture from file");

        ballPos_ = glm::vec2(0.5f * scaleSys_.ScreenWidth(), 0.5f * scaleSys_.ScreenHeight());

        attribMgr_ = std::make_shared<AttribVariableManager>(ballShader_.CreateAttribMgr());
        uniformMgr_ = std::make_shared<UniformVariableManager>(ballShader_.CreateUniformMgr());

        attribMgr_->GetAttrib<glm::vec2>("position").SetBuffer(ballVtx_, &BallVertex::pos);
        attribMgr_->GetAttrib<glm::vec2>("uv").SetBuffer(ballVtx_, &BallVertex::uv);

        return true;
    }

    bool Init(void)
    {
        if(!InitContext())
            return false;
        scaleSys_.Reinit(10.0f, 10.0f);
        if(!InitBall())
            return false;
        glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
        return true;
    }

    int Run(void)
    {
        if(Init())
        {
            clock_.Restart();
            while(!done_)
                MainLoop();
            return 0;
        }
        return -1;
    }

    void MainLoop(void)
    {
        clock_.Tick();
        float deltaT = static_cast<float>(clock_.ElapsedTime());

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        constexpr float SPEED = 10.0f / 1000.0f;
        InputManager &im = InputManager::GetInstance();
        if(im.IsKeyPressed(KEY_CODE::KEY_W))
            ballPos_ += glm::vec2(0.0f, SPEED * deltaT);
        if(im.IsKeyPressed(KEY_CODE::KEY_A))
            ballPos_ += glm::vec2(-SPEED * deltaT, 0.0f);
        if(im.IsKeyPressed(KEY_CODE::KEY_S))
            ballPos_ += glm::vec2(0.0f, -SPEED * deltaT);
        if(im.IsKeyPressed(KEY_CODE::KEY_D))
            ballPos_ += glm::vec2(SPEED * deltaT, 0.0f);

        ballPos_.x = glm::clamp(ballPos_.x, 0.0f, scaleSys_.ScreenWidth());
        ballPos_.y = glm::clamp(ballPos_.y, 0.0f, scaleSys_.ScreenHeight());

        //uniformMgr_->GetUniform<glm::mat3>("modelMat").SetVal(Transform::Translate(ballPos_));
        //uniformMgr_->GetUniform<glm::mat3>("projMat").SetVal(scaleSys_.ProjMatrix());
        uniformMgr_->GetUniform<GLint>("tex").SetVal(0);
        ballTex_.Bind(0);

        ballShader_.Bind();
        attribMgr_->Bind();

        uniformMgr_->Apply();
        glDrawArrays(GL_TRIANGLES, 0, 6);

        attribMgr_->Unbind();
        ballShader_.Unbind();

        RenderContext::GetInstance().DoEvents();
        RenderContext::GetInstance().Present();

        if(im.IsKeyPressed(KEY_CODE::KEY_ESCAPE))
            done_ = true;
    }

private:
    VertexBuffer<BallVertex> ballVtx_;
    Shader ballShader_;
    Texture2D ballTex_;

    glm::vec2 ballPos_;

    std::shared_ptr<AttribVariableManager> attribMgr_;
    std::shared_ptr<UniformVariableManager> uniformMgr_;

    ScreenScale scaleSys_;
    Clock clock_;

    bool done_;

    void WindowClosed(void)
    {
        done_ = true;
    }
};
