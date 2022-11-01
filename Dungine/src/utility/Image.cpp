/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Image.h                                   *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : June 12, 2022                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Provide some image operations.                                           *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220901                                                           *
 ******************************************************************************/

#include "../../inc/utility/Image.h"

/******************************************************************************
 * ResizeImage -- Resize image.                                               *
 *                                                                            *
 *    Hmm... It must first resize to another image and then copy to itself.   *
 *                                                                            *
 * INPUT:   pDestImage -- Destination image.                                  *
 *          pSrcImage  -- Source Image.                                       *
 *          scale      -- Scale of resize.                                    *
 *          destWidth                                                         *
 *          destHeight                                                        *
 *          srcWidth                                                          *
 *          srcHeight                                                         *
 *                                                                            *
 * OUTPUT:  none                                                              *
 *                                                                            *
 * REFERENCE:                                                                 *
 * https://wenku.baidu.com/view/eef29e501411cc7931b765ce050876323012745c.html *
 *                                                                            *
 * WARNINGS:  Do not pass invalid scales.                                     *
 *                                                                            *
 * HISTORY:                                                                   *
 *   2022/06/12 Tony : Created.                                               *
 *============================================================================*/
void ResizeImage(IMAGE* pDestImage, IMAGE* pSrcImage, double scale)
{
	unsigned long srcWidth = (unsigned long)pSrcImage->getwidth();
	unsigned long srcHeight = (unsigned long)pSrcImage->getheight();
	unsigned long destWidth = (unsigned long)(srcWidth * scale) + 1ul;
	unsigned long destHeight = (unsigned long)(srcHeight * scale) + 1ul;

	ResizeImage(pDestImage, pSrcImage, destWidth, destHeight, srcWidth, srcHeight);
}

void ResizeImage(
	IMAGE* pDestImage,
	IMAGE* pSrcImage,
	unsigned long destWidth,
	unsigned long destHeight)
{
	ResizeImage(
		pDestImage,
		pSrcImage,
		destWidth,
		destHeight,
		(unsigned long)pSrcImage->getwidth(),
		(unsigned long)pSrcImage->getheight());
}

void ResizeImage(
	IMAGE* pDestImage,
	IMAGE* pSrcImage,
	unsigned long destWidth,
	unsigned long destHeight,
	unsigned long srcWidth,
	unsigned long srcHeight)
{
	if ((destWidth == srcWidth) && (destHeight == srcHeight))
	{
		*pDestImage = *pSrcImage;
		return;
	}
	destWidth = max(destWidth, 1);
	destHeight = max(destHeight, 1);

	unsigned long rx_f16 = (srcWidth << 16ul) / destWidth + 1ul;
	unsigned long ry_f16 = (srcHeight << 16ul) / destHeight + 1ul;
	unsigned long srcX_f16, srcY_f16;

	pDestImage->Resize(destWidth, destHeight);
	DWORD* pSrcBuffer = GetImageBuffer(pSrcImage);
	DWORD* pDestRow = GetImageBuffer(pDestImage);
	DWORD* pSrcRow;

	srcY_f16 = 0ul;
	for (unsigned long y = 0; y < destHeight; ++y)
	{
		pSrcRow = pSrcBuffer + srcWidth * (srcY_f16 >> 16);
		srcX_f16 = 0;
		for (unsigned long x = 0; x < destWidth; ++x)
		{
			pDestRow[x] = pSrcRow[srcX_f16 >> 16];
			srcX_f16 += rx_f16;
		}
		srcY_f16 += ry_f16;
		pDestRow += destWidth;
	}
}


static IMAGE tempImage;

void ResizeImage(IMAGE* pImage, double scale)
{
	ResizeImage(&tempImage, pImage, scale);
	*pImage = tempImage;
}

void ResizeImage(
	IMAGE* pImage,
	unsigned long destWidth,
	unsigned long destHeight)
{
	ResizeImage(pImage, destWidth, destHeight, pImage->getwidth(), pImage->getheight());
}

void ResizeImage(
	IMAGE* pImage,
	unsigned long destWidth,
	unsigned long destHeight,
	unsigned long srcWidth,
	unsigned long srcHeight)
{
	if ((destWidth == srcWidth) && (destHeight == srcHeight))
		return;

	ResizeImage(&tempImage, pImage, destWidth, destHeight, srcWidth, srcHeight);
	*pImage = tempImage;
}