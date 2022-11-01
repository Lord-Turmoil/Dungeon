/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : device.cpp                                *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 12, 2022                            *
 *                                                                            *
 *                    Last Update : August 16, 2022                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   The canvas of the game is the screen, on which all symbols are finally   *
 *   rendered. It will automatically fit the user's computer screen, and it   *
 *   will handle the rendering process.                                       *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include <algorithm>

#include "../../inc/device/Color.h"
#include "../../inc/device/Device.h"
#include "../../inc/utility/Auxilliary.h"
#include "../../inc/utility/DMath.h"
#include "../../inc/utility/Image.h"

/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Renderer Region
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

/******************************************************************************
 * Renderer::Render -- Render a symbol to the current EasyX device.           *
 *                                                                            *
 *    This is the common render method. Specific rendering will be performed  *
 *    in it. Hmm... too much calls of functions will reduce performance...    *
 *    significantly. Well... The MOST MOST MOST TOUGH works are done here. :( *
 *                                                                            *
 * INPUT:   symbol -- The symbol to render.                                   *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  This will make modification to the symbol. So the symbol should *
 *            be updated every time before it is rendered.                    *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/12 Tony : Created.                                               *
 *   2022/08/16 Tony : Invalid image check added. Function calls reduced.     *
 *============================================================================*/
void Renderer::Render(Symbol* symbol)
{
	// This shouldn't happen, but in case critical error occurs.
	if (!symbol->m_pImage)
		return;

	if (symbol->m_pSupSymbol)
		Render(symbol->m_pSupSymbol);

	m_symbol = symbol;
	m_offset.Init(m_symbol->m_offset);
	if (m_symbol->m_pAttribute)
	{
		SymbolAttribute& attr = *(m_symbol->m_pAttribute);
		IMAGE* pSrcImage;

		if (attr.alpha != 0)
		{
			// Resize?
			if (dabs(attr.scale - 1.0) < EPSILON)
			{
				// Rotate?
				if (dabs(attr.rotationAngle) < EPSILON)
					pSrcImage = m_symbol->m_pImage;
				else	// Just rotate.
				{
					rotateimage(&m_alphaImage, m_symbol->m_pImage, attr.rotationAngle, TRANSPARENT_COLOR, false, HIGH_QUALITY);
					pSrcImage = &m_alphaImage;
				}
			}
			else
			{
				// Resize image.
				int srcWidth = m_symbol->m_pImage->getwidth();
				int srcHeight = m_symbol->m_pImage->getheight();
				int destWidth = (int)(srcWidth * attr.scale);
				int destHeight = (int)(srcHeight * attr.scale);
				ResizeImage(&m_alphaImage, m_symbol->m_pImage, destWidth, destHeight, srcWidth, srcHeight);
				m_offset.x += (srcWidth - destWidth) >> 1;
				m_offset.y += (srcHeight - destHeight) >> 1;

				// Rotate?
				if (dabs(attr.rotationAngle) < EPSILON)
					pSrcImage = &m_alphaImage;
				else
				{
					rotateimage(&m_betaImage, &m_alphaImage, attr.rotationAngle, TRANSPARENT_COLOR, false, HIGH_QUALITY);
					pSrcImage = &m_betaImage;
				}
			}

			if (attr.alpha == 255)
				_PutImage(pSrcImage);
			else
				_PutAlphaImage(pSrcImage);
		}
	}
	else
		_PutImage(m_symbol->m_pImage);

	if (symbol->m_pSubSymbol)
		Render(symbol->m_pSubSymbol);
}


/******************************************************************************
 * Renderer::_PutImage -- Draw the symbol to the device.                      *
 *                                                                            *
 *    This will draw the image regardless of its alpha info. If pSrcImage is  *
 *    nullptr, then it will draw m_image instead.                             *
 *                                                                            *
 * INPUT:   pSrcImage -- The source image to draw.                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/12 Tony : Created.                                               *
 *============================================================================*/
void Renderer::_PutImage(IMAGE* pSrcImage)
{
	DWORD* pDestBuffer = GetImageBuffer(m_pTargetImage);
	DWORD* pSrcBuffer = GetImageBuffer(pSrcImage);
	int srcWidth = pSrcImage->getwidth();
	int srcHeight = pSrcImage->getheight();
	int destWidth = m_width;
	int destHeight = m_height;

	int destX = m_symbol->m_coord.x + m_offset.x;
	int destY = m_symbol->m_coord.y + m_offset.y;

	/*
	** Return if the whole image is out of the buffer
	*/
	if ((destX + srcWidth < 0) || (destY + srcHeight < 0) \
		|| (destX >= destWidth) || (destY >= destHeight))
		return;

	/*
	** Adjust image size and position.
	*/
	int width = dmin(destX + srcWidth, destWidth) - dmax(destX, 0);
	int height = dmin(destY + srcHeight, destHeight) - dmax(destY, 0);
	if (destX < 0)
	{
		pSrcBuffer -= destX;
		destX = 0;
	}
	if (destY < 0)
	{
		pSrcBuffer -= destY * srcWidth;
		destY = 0;
	}
	pDestBuffer += destWidth * destY + destX;

	// Put src image to the dest buffer.
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if ((pSrcBuffer[j] & 0x00ffffff) != TRANSPARENT_COLOR)
				pDestBuffer[j] = pSrcBuffer[j];
		}
		pDestBuffer += destWidth;
		pSrcBuffer += srcWidth;
	}
}


/******************************************************************************
 * Renderer::_PutAlphaImage -- Put image with alpha info.                     *
 *                                                                            *
 *    This means the symbol has attribute.                                    *
 *                                                                            *
 * INPUT:   pSrcImage -- The source image.                                    *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/12 Tony : Created.                                               *
 *============================================================================*/
void Renderer::_PutAlphaImage(IMAGE* pSrcImage)
{
	DWORD* pDestBuffer = GetImageBuffer(m_pTargetImage);
	DWORD* pSrcBuffer = GetImageBuffer(pSrcImage);
	int srcWidth = pSrcImage->getwidth();
	int srcHeight = pSrcImage->getheight();
	int destWidth = m_width;
	int destHeight = m_height;

	int destX = m_symbol->m_coord.x + m_offset.x;
	int destY = m_symbol->m_coord.y + m_offset.y;

	if ((destX + srcWidth < 0) || (destY + srcHeight < 0) \
		|| (destX >= destWidth) || (destY >= destHeight))
		return;

	int width = min(destX + srcWidth, destWidth) - max(destX, 0);
	int height = min(destY + srcHeight, destHeight) - max(destY, 0);
	if (destX < 0)
	{
		pSrcBuffer -= destX;
		destX = 0;
	}
	if (destY < 0)
	{
		pSrcBuffer -= destY * srcWidth;
		destY = 0;
	}
	pDestBuffer += destWidth * destY + destX;

	// Put src image to the dest buffer.
	int alpha = m_symbol->m_pAttribute->alpha;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if ((pSrcBuffer[j] & 0x00ffffff) != TRANSPARENT_COLOR)
				pDestBuffer[j] = ARGB(pSrcBuffer[j], pDestBuffer[j], alpha);
		}

		pDestBuffer += destWidth;
		pSrcBuffer += srcWidth;
	}
}



/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Device Region
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

DeviceInfo deviceInfo;


/******************************************************************************
 * Device::Create -- Initialize the canvas.                                   *
 *                                                                            *
 *    In this version, it will automatically full screen unless FULLSCREEN is *
 *    not defined. If the device is already open, then nothing will happen.   *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/13 Tony : Created.                                               *
 *============================================================================*/
void Device::Create(bool fullscreen)
{
	if (m_isOpen)
		return;
	m_isOpen = true;

	// Initialize the window and graph.
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (fullscreen)
	{
		m_width = screenWidth;
		m_height = screenHeight;
	}
	else
	{
		m_width = DEVICE_WIDTH;
		m_height = DEVICE_HEIGHT;
	}
	
	HWND hWnd;

	if (fullscreen)
	{
		hWnd = initgraph(m_width, m_height);
		SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & (~WS_CAPTION));
	}
	else
	{
#ifdef DGE_SHOW_CONSOLE
		hWnd = initgraph(m_width, m_height, EW_SHOWCONSOLE);
#else
		hWnd = initgraph(m_width, m_height);
#endif
	}

	SetWindowPos(hWnd, HWND_TOP, (screenWidth - m_width) >> 1, (screenHeight - m_height) >> 1, m_width, m_height, SWP_SHOWWINDOW);
	Resize(NULL, m_width, m_height);

	// Initialize device info
	if (fullscreen)
	{
		deviceInfo.clientWidth = screenWidth;
		deviceInfo.clientHeight = screenHeight;
		float xAsp = (float)screenWidth / (float)DEVICE_WIDTH;
		float yAsp = (float)screenHeight / (float)DEVICE_HEIGHT;
		if (xAsp < yAsp)
		{
			deviceInfo.padding.x = 0;
			deviceInfo.padding.y = (screenHeight - (int)(DEVICE_HEIGHT * xAsp)) >> 1;
			deviceInfo.aspectRatio = xAsp;
		}
		else
		{
			deviceInfo.padding.x = (screenWidth - (int)(DEVICE_WIDTH * yAsp)) >> 1;
			deviceInfo.padding.y = 0;
			deviceInfo.aspectRatio = yAsp;
		}
	}
	else
	{
		deviceInfo.clientWidth = DEVICE_WIDTH;
		deviceInfo.clientHeight = DEVICE_HEIGHT;
		deviceInfo.padding = COORD_ZERO;
		deviceInfo.aspectRatio = 1.0;
	}

	// Initialize renderer.
	m_renderer.SetSize(m_width, m_height);

	BeginBatchDraw();
}


/******************************************************************************
 * Device::Destroy -- Close the drawing graph.                                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/05/31 Tony : Created.                                               *
 *============================================================================*/
void Device::Destroy()
{
	_Destroy();
}


/******************************************************************************
 * Device::AddSymbol -- Add a symbol to the symbol queue.                     *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   symbol -- The symbol to add.                                      *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/13 Tony : Created.                                               *
 *============================================================================*/
void Device::AddSymbol(Symbol* symbol)
{
	m_queue.push(symbol);
}


/******************************************************************************
 * Device::RenderSymbol -- Render symbol immediately.                         *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   symbol                                                            *
 *                                                                            *
 * OUTPUT:  Return itself.                                                    *
 *                                                                            *
 * WARNINGS:  This will render the symbol to screen immediately, so it may    *
 *            disrupt the layer property of other symbols.                    *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/02 Tony : Created.                                               *
 *============================================================================*/
Device* Device::RenderSymbol(Symbol* symbol)
{
	m_renderer.Render(symbol);
	return this;
}


/******************************************************************************
 * Device::Clear -- Clear the device.                                         *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/13 Tony : Created.                                               *
 *============================================================================*/
void Device::Clear()
{
	cleardevice();
}


/******************************************************************************
 * Device::Flush -- Clear the symbol queue.                                   *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/13 Tony : Created.                                               *
 *============================================================================*/
void Device::Flush()
{
	FlushBatchDraw();
}


/******************************************************************************
 * Device::Render -- Render all symbols.                                      *
 *                                                                            *
 *    This will clear the symbol queue at the same time.                      *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/13 Tony : Created.                                               *
 *============================================================================*/
void Device::Render()
{
	cleardevice();

	while (!m_queue.empty())
	{
		m_renderer.Render(m_queue.top());
		m_queue.pop();
	}

	FlushBatchDraw();
}

void Device::Render(IMAGE* pDestImage)
{
	SetTargetImage(pDestImage);
	cleardevice();

	while (!m_queue.empty())
	{
		m_renderer.Render(m_queue.top());
		m_queue.pop();
	}

	SetTargetImage();
}


void Device::RenderWithoutFlush()
{
	cleardevice();

	while (!m_queue.empty())
	{
		m_renderer.Render(m_queue.top());
		m_queue.pop();
	}

#ifndef _NEW_DESIRE_STUDIOS_
	static RECT rect = {
		deviceInfo.clientWidth - 200,
		deviceInfo.clientHeight - 30,
		deviceInfo.clientWidth,
		deviceInfo.clientHeight };
	drawtext(L"MADE WITH DUNGINE", &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP);
#endif
}
/******************************************************************************
 * Device::Abandon -- Abandon current rendering queue.                        *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/30 Tony : Created.                                               *
 *============================================================================*/
void Device::Abandon()
{
	std::priority_queue<Symbol*, std::vector<Symbol*>, CompareSymbol> empty;

	std::swap(empty, m_queue);
}


/******************************************************************************
 * Device::PrintScreen -- Take a screen shot.                                 *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/09 Tony : Created.                                               *
 *============================================================================*/
void Device::PrintScreen()
{
	SYSTEMTIME sysTime;
	wchar_t str[32];
	std::wstring filename;

	GetLocalTime(&sysTime);

	//YYYYMMDDHHMMSS
	swprintf_s(str, L"%4hu%02hu%02hu%02hu%02hu%02hu.png", \
		sysTime.wYear, sysTime.wMonth, sysTime.wDay, \
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	filename = L"screenshots\\";
	if (!IsValidDirectory(filename.c_str()))
	{
		if (!CreateNewDirectory(filename.c_str()))
			return;
	}

	filename += str;

	// Here, the device should be the screen.
	saveimage(filename.c_str(), NULL);

#ifdef DUNGINE_DEBUG
	LOG_MESSAGE("Screenshot captured");
#endif
}


/******************************************************************************
 * Device::GetImage -- Get the current working image.                         *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/13 Tony : Created.                                               *
 *============================================================================*/
IMAGE* Device::GetImage()
{
	return GetWorkingImage();
}


/******************************************************************************
 * Device::SetTargetImage -- Set working image.                               *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   pDestImage -- The working image to be set.                        *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/13 Tony : Created.                                               *
 *============================================================================*/
void Device::SetTargetImage(IMAGE* pDestImage)
{
	SetWorkingImage(pDestImage);
	m_renderer.SetTargetImage(pDestImage);
}


/******************************************************************************
 * Device::~Device -- Destructor of the class.                                *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/13 Tony : Created.                                               *
 *============================================================================*/
Device::~Device()
{
	_Destroy();
}


/******************************************************************************
 * Device::_Destroy -- Destroy the canvas.                                    *
 *                                                                            *
 *    This is the end of the program, as it will close the EasyX graph.       *
 *                                                                            *
 * INPUT:   none                                                              *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/04/13 Tony : Created.                                               *
 *============================================================================*/
void Device::_Destroy()
{
	if (m_isOpen)
	{
		EndBatchDraw();
		closegraph();

		m_isOpen = false;
	}
}