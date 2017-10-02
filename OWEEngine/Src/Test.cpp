#include <iostream>

#include "Include\ASCIIFile.h"
#include "Include\GLHeaders.h"
#include "Include\InputManager.h"
#include "Include\RenderContext.h"
#include "Include\Shader.h"

#include "Include\UniformVariable.h"
#include "Include\VertexBuffer.h"

using namespace std;
using namespace OWE;

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
        Utility::ASCIIFile::Read("test.vs", vtx);
        Utility::ASCIIFile::Read("test.fs", frag);
        if(shader.Initialize(err, VSSrc(vtx), FSSrc(frag)) != Shader::InitState::Success)
        {
            cout << err << endl;
            return -1;
        }

        auto mgr = shader.CreateUniformMgr();
        auto offset = mgr.GetUniform<glm::vec3>("offset");
        offset.SetVals(glm::vec3{ 0.0f, 2.0f, 3.0f });

        VertexBuffer<glm::vec2> vec2Buf;
        const glm::vec2 vec2BufData[] =
        {
            { -1.0f, -1.0f },
            { 0.0f, 1.0f },
            { 1.0f, -1.0f }
        };
        vec2Buf.Initialize(3, vec2BufData);

        //主循环
        while(!closed_)
        {
            //glViewport(0, 0, rc.ClientWidth(), rc.ClientHeight());
            glClearColor((abs(glm::sin(t_ += 0.08f)) + 1.0f) / 2.0f, 0.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shader.Bind();
            mgr.Bind();
            //Draw something
            shader.Unbind();

            rc.DoEvents();
            rc.Present();

            if(im.IsKeyPressed(KEY_CODE::KEY_ESCAPE))
                closed_ = true;
        }

        vec2Buf.Destroy();
        shader.Destroy();
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

//#include <gl_includes\glew.h>
//#include <gl_includes\GL.h>
//#include <GLFW\glfw3.h>
//
//int main(void)
//{
//    glfwInit();
//    glfwWindowHint(GLFW_VERSION_MAJOR, 4);
//    glfwWindowHint(GLFW_VERSION_MINOR, 3);
//    GLFWwindow *win = glfwCreateWindow(640, 480, "Test", nullptr, nullptr);
//    glfwMakeContextCurrent(win);
//    glewExperimental = GL_TRUE;
//    glewInit();
//    glfwSwapInterval(1);
//
//    while(!glfwWindowShouldClose(win))
//    {
//        glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);
//        glBegin(GL_TRIANGLES);
//
//        glColor3f(1.0, 0.0, 0.0);    // Red
//        glVertex3f(0.0, 1.0, 0.0);
//
//        glColor3f(0.0, 1.0, 0.0);    // Green
//        glVertex3f(-1.0, -1.0, 0.0);
//
//        glColor3f(0.0, 0.0, 1.0);    // Blue
//        glVertex3f(1.0, -1.0, 0.0);
//
//        glEnd();
//        glfwSwapBuffers(win);
//        glfwPollEvents();
//    }
//
//    glfwTerminate();
//}