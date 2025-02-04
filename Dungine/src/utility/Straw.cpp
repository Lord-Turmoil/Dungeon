/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Straw.h                                   *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 11, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Overview:                                                                 *
 *   Load external resources.                                                 *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include <easyx.h>

#include "../../inc/common/Logger.h"
#include "../../inc/device/Device.h"
#include "../../inc/utility/Image.h"
#include "../../inc/utility/Straw.h"
#include "../../inc/utility/fmodcore.h"

/******************************************************************************
 * FetchImage -- Load external image resource.                                *
 *                                                                            *
 *    There are some overloads for different requirements.                    *
 *                                                                            *
 * INPUT:   pDestImage -- Pointer to destination image.                       *
 *          destWidth  -- Width of dest image.                                *
 *          destHeight -- Height of dest image.                               *
 *          origin     -- Origin in the src image.                            *
 *          srcWidth   -- Width in source image.                              *
 *          srcHeight  -- Height in source image.                             *
 *          filename   -- Filename of source image.                           *
 *          pSrcImage  -- Used if source image is already loaded.             *
 *                                                                            *
 * OUTPUT:  Return whether succeeded or not.                                  *
 *                                                                            *
 * WARNINGS:  Just be careful.                                                *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/11 Tony : Created.                                               *
 *============================================================================*/
static IMAGE tempImage;

bool FetchImage(IMAGE* pDestImage, const char* filename)
{
    if (!IsValidDirectory(filename))
    {
        LOG_ERROR(CANNOT_ACCESS_DIR, filename);
        return false;
    }

    loadimage(pDestImage, widen(filename));

    return true;
}

bool FetchImage(IMAGE* pDestImage, const wchar_t* filename)
{
    if (!IsValidDirectory(filename))
    {
        LOG_ERROR(CANNOT_ACCESS_DIR, narrow(filename));
        return false;
    }

    loadimage(pDestImage, filename);

    return true;
}

bool FetchImage(IMAGE* pDestImage, const char* filename, int destWidth, int destHeight)
{
    return FetchImage(pDestImage, widen(filename), destWidth, destHeight);
}

bool FetchImage(IMAGE* pDestImage, const wchar_t* filename, int destWidth, int destHeight)
{
    if (!IsValidDirectory(filename))
    {
        LOG_ERROR(CANNOT_ACCESS_DIR, narrow(filename));
        return false;
    }

    loadimage(pDestImage, filename, destWidth, destHeight);

    return true;
}

bool FetchImage(IMAGE* pDestImage, const char* filename, int srcX, int srcY, int srcWidth, int srcHeight)
{
    return FetchImage(pDestImage, widen(filename), srcX, srcY, srcWidth, srcHeight);
}

bool FetchImage(IMAGE* pDestImage, const wchar_t* filename, int srcX, int srcY, int srcWidth, int srcHeight)
{
    if (!IsValidDirectory(filename))
    {
        LOG_ERROR(CANNOT_ACCESS_DIR, narrow(filename));
        return false;
    }

    loadimage(&tempImage, filename, 0, 0, true);

    Device::GetInstance()->SetTargetImage(&tempImage);
    getimage(pDestImage, srcX, srcY, srcWidth, srcHeight);
    Device::GetInstance()->SetTargetImage();

    return true;
}

bool FetchImage(IMAGE* pDestImage, const char* filename, int destWidth, int destHeight, int srcX, int srcY,
                int srcWidth, int srcHeight)
{
    return FetchImage(pDestImage, widen(filename), destWidth, destHeight, srcX, srcY, srcWidth, srcHeight);
}

bool FetchImage(IMAGE* pDestImage, const wchar_t* filename, int destWidth, int destHeight, int srcX, int srcY,
                int srcWidth, int srcHeight)
{
    if (!IsValidDirectory(filename))
    {
        LOG_ERROR(FAILED_TO_LOAD_RESOURCE, narrow(filename));
        return false;
    }

    loadimage(&tempImage, filename, 0, 0, true);

    Device::GetInstance()->SetTargetImage(&tempImage);
    getimage(pDestImage, srcX, srcY, srcWidth, srcHeight);
    Device::GetInstance()->SetTargetImage();

    ResizeImage(pDestImage, destWidth, destHeight, srcWidth, srcHeight);

    return true;
}

bool FetchImage(IMAGE* pDestImage, IMAGE* pSrcImage)
{
    (*pDestImage) = (*pSrcImage);
    return true;
}

bool FetchImage(IMAGE* pDestImage, IMAGE* pSrcImage, int srcX, int srcY, int srcWidth, int srcHeight)
{
    Device::GetInstance()->SetTargetImage(pSrcImage);
    getimage(pDestImage, srcX, srcY, srcWidth, srcHeight);
    Device::GetInstance()->SetTargetImage();

    return true;
}

bool FetchImage(IMAGE* pDestImage, IMAGE* pSrcImage, int destWidth, int destHeight, int srcX, int srcY, int srcWidth,
                int srcHeight)
{
    Device::GetInstance()->SetTargetImage(pSrcImage);
    getimage(pDestImage, srcX, srcY, srcWidth, srcHeight);
    Device::GetInstance()->SetTargetImage();

    ResizeImage(pDestImage, destWidth, destHeight, srcWidth, srcHeight);

    return true;
}

/******************************************************************************
 * FetchSound -- Load an external sound.                                      *
 *                                                                            *
 *    The sound file should be small.                                         *
 *                                                                            *
 * INPUT:   sound                                                             *
 *          filename                                                          *
 *                                                                            *
 * OUTPUT:  Return succeeded or not.                                          *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/11 Tony : Created.                                               *
 *============================================================================*/
bool FetchSound(DSound* sound, const char* filename)
{
    return true;
}

/******************************************************************************
 * FetchStream -- Load an external stream.                                    *
 *                                                                            *
 *    The stream file is larger, such as bgm.                                 *
 *                                                                            *
 * INPUT:   sound                                                             *
 *          filename                                                          *
 *                                                                            *
 * OUTPUT:  Return succeeded or not.                                          *
 *                                                                            *
 * WARNINGS:  none                                                            *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/11 Tony : Created.                                               *
 *============================================================================*/
bool FetchStream(DSound* stream, const char* filename)
{
    return true;
}
