/*================================================================
Filename: Test1.h
Date: 2017.10.16
Created by AirGuanZ
================================================================*/
#include <iostream>
#include <string>

#include <glm\gtc\matrix_transform.hpp>

#include <OWE.h>

using namespace std;
using namespace OWE;

class TestApp1 : public WindowListener
{
public:
    int Run(void)
    {
        RenderContext::Desc desc;
        desc.bordered   = true;
        desc.resizable  = false;
        desc.fullscreen = false;
        desc.winWidth   = 1440;
        desc.winHeight  = 768;
        desc.title      = "Monomorphism";
        desc.glVersion  = "4.3";

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
        UniformVariableManager uniformMgr = shader.CreateUniformMgr();
        UniformVariable<GLint> texSam = uniformMgr.GetUniform<GLint>("tex");
        UniformVariable<glm::vec2> offset = uniformMgr.GetUniform<glm::vec2>("offset");

        glm::vec2 offsetVal(0.0f, 0.0f);

        //准备顶点属性
        AttribVariableManager attribMgr = shader.CreateAttribMgr();
        AttribVariable<glm::vec4> pos = attribMgr.GetAttrib<glm::vec4>("position");
        AttribVariable<glm::vec2> uv = attribMgr.GetAttrib<glm::vec2>("uv_in");
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
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                tex.Bind(0);

                shader.Bind();
                attribMgr.Bind();

                uniformMgr.Apply();
                glDrawArrays(GL_TRIANGLES, 0, 3);

                attribMgr.Unbind();
                shader.Unbind();
            }
            fb.End();

            //渲染主场景
            fb.GetTex(0).Bind(0);

            shader.Bind();
            attribMgr.Bind();

            uniformMgr.Apply();
            glDrawArrays(GL_TRIANGLES, 0, 3);

            attribMgr.Unbind();
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

class App2 : public WindowListener
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
            { { -BALL_RADIUS, -BALL_RADIUS }, { 0.0f, 0.0f } },
            { { -BALL_RADIUS, +BALL_RADIUS }, { 0.0f, 1.0f } },
            { { +BALL_RADIUS, +BALL_RADIUS }, { 1.0f, 1.0f } },

            { { -BALL_RADIUS, -BALL_RADIUS }, { 0.0f, 0.0f } },
            { { +BALL_RADIUS, +BALL_RADIUS }, { 1.0f, 1.0f } },
            { { +BALL_RADIUS, -BALL_RADIUS }, { 1.0f, 0.0f } }
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
