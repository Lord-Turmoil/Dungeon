/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungeon                                   *
 *                                                                            *
 *                      File Name : Startup.cpp                               *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : July 30, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Load all resources and make all preperations.                            *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/common/Defines.h"

#include "../../inc/game/Startup.h"
#include "../../inc/game/Settings.h"

#include "../../inc/ui/ErrorInterface.h"
#include "../../inc/ui/Cursor.h"

/******************************************************************************
 * Initialize -- Initialize all things.                                       *
 *                                                                            *
 *    It is divided into some small procedures.                               *
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
bool Initialize()
{
	Settings::GetInstance()->Link(SETTINGS_XML);
	Settings::GetInstance()->Load();

	if (!_InitDevice())
	{
		LOG_ERROR("Failed to initialize Device");
		return false;
	}

	if (!_InitApplication())
	{
		_UnInitApplication();
		if (!_InitErrorApplication())
			return false;
	}

	srand((unsigned)time(NULL));

	return true;
}

bool _InitDevice()
{
	// Speaker
	if (!Speaker::GetInstance()->Create())
		return false;
	Speaker::GetInstance()->SetSoundVolume(Settings::GetInstance()->SoundVolume());
	Speaker::GetInstance()->SetMusicVolume(Settings::GetInstance()->MusicVolume());

	// Device
	Device::GetInstance()->Create(Settings::GetInstance()->IsFullscreen());

	// Explorer
	if (!Explorer::GetInstance()->Load(RESOURCE_XML))
		return false;
	IMAGE* pSkyImage = new IMAGE(deviceInfo.clientWidth, deviceInfo.clientHeight);
	GradiantFill(pSkyImage, SRC_COLOR, DEST_COLOR);
	Explorer::GetInstance()->AssignInternalResource(
		ResourceType::RES_IMAGE, "sky", pSkyImage);

	// Additional font
	if (AddFontResourceEx(FONT_PATH, FR_PRIVATE, NULL) == 0)
	{
		LOG_ERROR(R"(Missing essential font "%s")", narrow(FONT_NAME));
		return false;
	}

	// Custom cursor.
	LoadCursorStyle();
	SetCursorStyle(CUR_ARROW);

	return true;
}

bool _InitApplication()
{
	int intfNum = (int)(sizeof(UI_XML) / sizeof(UI_XML[0]));
	Application* app = Application::GetInstance();

	for (int i = 0; i < intfNum; i++)
	{
		if (!app->Load(splice(UI_DIRECTORY, UI_XML[i])))
			return false;
	}

	auto& pool = app->GetInterfacePool();
	for (auto it = pool.begin(); it != pool.end(); it++)
	{
		static_cast<PlainInterface*>(it->second)->AddEvents();
		it->second->Update(nullptr);	// Hmm... Initialize some image?
	}

	return true;
}

void _UnInitApplication()
{
	Application::GetInstance()->UnLoad();
}

bool _InitErrorApplication()
{
	ErrorInterface* intf = new ErrorInterface();

	Logger::ClearGlobalState();

	intf->Load(nullptr);
	intf->AddEvents();

	Application::GetInstance()->AddInterface(intf);


	return true;
}


/******************************************************************************
 * Run -- Run the application.                                                *
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
void Run()
{
	Application* app = Application::GetInstance();
	app->Initialize();
	app->Run();

	if (Logger::Error())
		_RunError();
}

void _RunError()
{
	Application* app = Application::GetInstance();

	app->UnLoad();
	_InitErrorApplication();
	app->Initialize();

	app->Run();
}

/******************************************************************************
 * ClearUp -- Clearup all resources.                                          *
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
 *   2022/07/31 Tony : Created.                                               *
 *============================================================================*/
void ClearUp()
{
	Application::GetInstance()->UnLoad();
	Explorer::GetInstance()->UnLoad();
	RemoveFontResourceEx(FONT_PATH, FR_PRIVATE, NULL);
}


/******************************************************************************
 * GradiantFill -- Fill the image with gradiant color.                        *
 *                                                                            *
 *    Just the literal meaning.                                               *
 *                                                                            *
 * INPUT:   pImage                                                            *
 *          srcColor                                                          *
 *          destColor                                                         *
 *                                                                            *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/07/30 Tony : Created.                                               *
 *============================================================================*/
void GradiantFill(IMAGE* pImage, COLORREF srcColor, COLORREF destColor)
{
	int width = pImage->getwidth();
	int height = pImage->getheight();

	int sr = PR(srcColor);
	int sg = PG(srcColor);
	int sb = PB(srcColor);
	int dr = PR(destColor);
	int dg = PG(destColor);
	int db = PB(destColor);

	int r, g, b;
	double ratio;
	COLORREF color;
	DWORD* pBuffer = GetImageBuffer(pImage);
	for (int i = 0; i < height; i++)
	{
		ratio = (double)i / (double)height;
		r = BlendValue(sr, dr, ratio);
		g = BlendValue(sg, dg, ratio);
		b = BlendValue(sb, db, ratio);
		color = RGB(b, g, r);	// R and B in buffer is inverted?
		for (int j = 0; j < width; j++)
			pBuffer[j] = color;
		pBuffer += width;
	}
}