/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Device.h                                  *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 2, 2022                             *
 *                                                                            *
 *                    Last Update : August 16, 2022                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   The canvas of the game is the screen, on which all symbols are finally   *
 *   rendered. It will automatically fit the user's computer screen, and it   *
 *   will handle the rendering process.                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <Windows.h>
#include <easyx.h>
#include <queue>

#include "Symbol.h"

#include "../common/Common.h"

#include "../template/Singleton.h"

/********************************************************************
** This is uesd to convert subscription for the image buffer.
*/
#define MAPPING(r, c, col) ((r) * (col) + (c))

// Default device size.
const int DEVICE_WIDTH = 640;
const int DEVICE_HEIGHT = 480;

/********************************************************************
** The renderer renders the image and put it to the screen, or the
** assigned buffer.
*/
class Renderer
{
public:
    Renderer() : m_symbol(nullptr), m_width(0), m_height(0), m_pTargetImage(nullptr)
    {
    }

    ~Renderer()
    {
    }

    void SetSize(int width, int height)
    {
        m_width = width;
        m_height = height;
    }

    void Render(Symbol* symbol);

    void SetTargetImage(IMAGE* pTargetImage)
    {
        m_pTargetImage = pTargetImage;
    }

private:
    void _PutImage(IMAGE* pSrcImage);
    void _PutAlphaImage(IMAGE* pSrcImage);

    Symbol* m_symbol;
    Coordinate m_offset; // Needed if scale occurs.

    int m_width;
    int m_height;

    IMAGE* m_pTargetImage;

    // Two buffer images.
    IMAGE m_betaImage;
    IMAGE m_alphaImage;
};

/********************************************************************
** In this version, the option for small screen is abandoned. Here
** the singleton pattern is used. And to auto release the instance,
** a private garbo is used.
*/
class Device : public Singleton<Device>
{
    friend class Singleton<Device>;

public:
    int GetWidth() const
    {
        return m_width;
    }

    int GetHeight() const
    {
        return m_height;
    }

    /*
    ** Initialize the device and fit the user's screen.
    */
    void Create(bool fullscreen);

    /*
    ** Close the device graph.
    */
    void Destroy();

    void AddSymbol(Symbol* symbol);
    Device* RenderSymbol(Symbol* symbol);

    /*
    ** This will clear the screen.
    */
    void Clear();

    void Flush();

    /*
    ** Render all symbols. The screen will be cleared as the
    ** first step of rendering.
    */
    void Render();
    void Render(IMAGE* pDestImage);
    void RenderWithoutFlush();
    void Abandon();

    void PrintScreen();

    /*
    ** Get current image on the screen.
    */
    IMAGE* GetImage();

    /*
    ** Set the target to draw all symbols.
    */
    void SetTargetImage(IMAGE* pDestImage = nullptr);

private:
    Device() : m_width(SCREEN_WIDTH), m_height(SCREEN_HEIGHT), m_isOpen(false)
    {
    }

    ~Device() override;

    /*
    ** This will close the device.
    */
    void _Destroy();

    std::priority_queue<Symbol*, std::vector<Symbol*>, CompareSymbol> m_queue;

    Renderer m_renderer;

    int m_width;
    int m_height;

    bool m_isOpen;
};

/********************************************************************
** This is the client device's info.
*/
struct DeviceInfo
{
    // size of the client screen
    int clientWidth;
    int clientHeight;

    // padding to add in x and y directions
    Coordinate padding;

    // ratio to convert 640 * 480 to client window size
    float aspectRatio;

    Rect GetDeviceRect() const
    {
        return Rect(0, 0, clientWidth, clientHeight);
    }
};

// global device info
extern DeviceInfo deviceInfo;

#endif
