/*================================================================
Filename: Main.cpp
Date: 2017.10.16
Created by AirGuanZ
================================================================*/
#include "Include\TestApp1.h"
#include "Include\TestApp2.h"
#include "Include\TestApp3.h"
#include "Include\TestApp4_TextureAnimation.h"

int main(void)
{
    try
    {
        Test::TestApp4_TextureAnimation app;
        app.Run();
    }
    catch(const OWE::FatalError &err)
    {
        std::cout << err.What() << std::endl;
    }
    catch(const std::exception &err)
    {
        std::cout << err.what() << std::endl;
    }
}
