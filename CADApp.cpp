// CADApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#define OLC_PGE_APPLICATION
#include <iostream>
#include "olcPixelGameEngine.h"


//static variables

float sShape::worldScale = 1.0f;



class CADApplication : public olc::PixelGameEngine {

public:
    CADApplication() {
    
    //what
        sAppName = "CAD Application";
    }

private:
    olc::vf2d vOffset = { 0.0f,0.0f };
    olc::vf2d vStartPan = { 0.0f, 0.0f };


    float fScale = 10.0f;
    float fGrid = 1.0f;
    
    //convert coordinates from world space to screenspace

    void WorldtoScreen(const olc::vf2d &v, int &nScreenX, int &nScreenY) 
    {
        nScreenX = (int)((v.x - vOffset.x) * fScale);
        nScreenY = (int)((v.y - vOffset.y) * fScale);
    }

    // Convert coordinates from Screen Space --> World Space
    void ScreenToWorld(int nScreenX, int nScreenY, olc::vf2d& v)
    {
        v.x = (float)(nScreenX) / fScale + vOffset.x;
        v.y = (float)(nScreenY) / fScale + vOffset.y;
    }




public:

    bool OnUserCreate() override 
    {
        //set the intial coordinates
        //configure world space to be middle of screen
        vOffset = { (float)(-ScreenWidth() / 2) / fScale , (float)(-ScreenHeight() / 2) / fScale };
        //vOffset = { (float)(-ScreenWidth() / 2) / fScale, (float)(-ScreenHeight() / 2) / fScale };
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        
        //get the mouse coordinates
        olc::vf2d vMouse = {(float)GetMouseX(), (float)GetMouseY() };

        //handle pan and zoom
        if (GetMouse(2).bPressed)
        {
            vStartPan = vMouse;

        }
        if (GetMouse(2).bHeld) {
            vOffset -= (vMouse - vStartPan) / fScale;
            vStartPan = vMouse;
        }

        olc::vf2d vMouseBeforeZoom;

    
    }

};






int main()
{
    std::cout << "Hello World!\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
