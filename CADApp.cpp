// CADApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//


#define OLC_PGE_APPLICATION
#include <iostream>
#include "olcPixelGameEngine.h"


//static variables

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


    olc::vf2d vCursor = { 0,0 };
    
    //convert coordinates from world space to screenspace

    void WorldToScreen(const olc::vf2d &v, int &nScreenX, int &nScreenY) 
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
        olc::vf2d vMouse = { (float)GetMouseX(), (float)GetMouseY() };


        // Handle Pan & Zoom
        if (GetMouse(2).bPressed)
        {
            vStartPan = vMouse;
        }

        if (GetMouse(2).bHeld)
        {
            vOffset -= (vMouse - vStartPan) / fScale;
            vStartPan = vMouse;
        }

        olc::vf2d vMouseBeforeZoom;
        ScreenToWorld((int)vMouse.x, (int)vMouse.y, vMouseBeforeZoom);

        if (GetKey(olc::Key::Q).bHeld || GetMouseWheel() > 0)
        {
            fScale *= 1.1f;
        }

        if (GetKey(olc::Key::A).bHeld || GetMouseWheel() < 0)
        {
            fScale *= 0.9f;
        }

        olc::vf2d vMouseAfterZoom;
        ScreenToWorld((int)vMouse.x, (int)vMouse.y, vMouseAfterZoom);
        vOffset += (vMouseBeforeZoom - vMouseAfterZoom);


        //snap cursor to the screen, basically make it so the cursor value can only be a whole number so it will 
        //be on a grid line
        // Snap mouse cursor to nearest grid interval
        vCursor.x = floorf((vMouseAfterZoom.x + 0.5f) * fGrid);
        vCursor.y = floorf((vMouseAfterZoom.y + 0.5f) * fGrid);



        // Clear Screen
        Clear(olc::WHITE);

        int sx, sy;
        int ex, ey;

        // Get visible world
        olc::vf2d vWorldTopLeft, vWorldBottomRight;
        ScreenToWorld(0, 0, vWorldTopLeft);
        ScreenToWorld(ScreenWidth(), ScreenHeight(), vWorldBottomRight);

        // Get values just beyond screen boundaries
        vWorldTopLeft.x = floor(vWorldTopLeft.x);
        vWorldTopLeft.y = floor(vWorldTopLeft.y);
        vWorldBottomRight.x = ceil(vWorldBottomRight.x);
        vWorldBottomRight.y = ceil(vWorldBottomRight.y);

        // Draw Grid dots
        for (float x = vWorldTopLeft.x; x < vWorldBottomRight.x; x += fGrid)
        {
            for (float y = vWorldTopLeft.y; y < vWorldBottomRight.y; y += fGrid)
            {
                WorldToScreen({ x, y }, sx, sy);
                Draw(sx, sy, olc::BLUE);
            }
        }




        // Draw World Axis
        WorldToScreen({ 0,vWorldTopLeft.y }, sx, sy);
        WorldToScreen({ 0,vWorldBottomRight.y }, ex, ey);
        DrawLine(sx, sy, ex, ey, olc::GREY, 0xF0F0F0F0);
        WorldToScreen({ vWorldTopLeft.x,0 }, sx, sy);
        WorldToScreen({ vWorldBottomRight.x,0 }, ex, ey);
        DrawLine(sx, sy, ex, ey, olc::GREY, 0xF0F0F0F0);

        //draw cursor

        WorldToScreen(vCursor, sx, sy);
        DrawCircle(sx, sy, 3, olc::YELLOW);

        return true;



    
    }

};






int main()
{
    
    CADApplication bob;
    if (bob.Construct(1600, 960, 1, 1, false))
        bob.Start();
    return 0;
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
