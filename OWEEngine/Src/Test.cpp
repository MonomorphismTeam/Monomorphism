#include <iostream>

#include "Include\OWE.h"

using namespace std;
using namespace OWE;
using Utility::ASCIIFile;

class App : public WindowListener
{
public:
    int run(void)
    {
        RenderContext::Desc desc;
        desc.bordered   = true;
        desc.resizable  = false;
        desc.fullscreen = false;
        desc.winWidth   = 800;
        desc.winHeight  = 600;
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
        ASCIIFile::Read("test.vs", vtx);
        ASCIIFile::Read("test.fs", frag);
        if(shader.Initialize(err, VSSrc(vtx), FSSrc(frag)) != Shader::InitState::Success)
        {
            cout << err << endl;
            return -1;
        }

        //准备vertex buffer
        VertexBuffer<glm::vec4, true> vec4Buf;
        const glm::vec4 vec4BufData[] =
        {
            { -0.5f, -0.5f, 0.0f, 1.0f },
            { 0.0f, 0.5f, 0.0f, 1.0f },
            { 0.5f, -0.5f, 0.0f, 1.0f }
        };
        vec4Buf.Initialize(3, vec4BufData);

        //准备uniform variable
        auto uniformMgr = shader.CreateUniformMgr();
        auto color = uniformMgr.GetUniform<glm::vec4>("color_");
        color.SetVals(glm::vec4{ 1.0f, 0.5f, 0.2f, 1.0f });

        //准备顶点属性
        auto attribMgr = shader.CreateAttribMgr();
        auto pos = attribMgr.GetAttrib<glm::vec4>("position");
        pos.SetBuffer(vec4Buf);

        //主循环
        while(!closed_)
        {
            glClearColor((abs(glm::sin(t_ += 0.04f)) + 1.0f) / 2.0f, 0.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //场景更新
            color.SetVals(glm::vec4{ 1.0f, (1.0f - abs(glm::sin(3.14159 * t_)) + 1.0f) / 2.0f, 0.4f, 1.0f });

            //场景绘制
            shader.Bind();
            uniformMgr.Bind();
            attribMgr.Bind();

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

int main(void)
{
    App app;
    return app.run();
}
