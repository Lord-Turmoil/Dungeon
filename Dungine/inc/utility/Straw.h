/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : Dungine(Dungeon Engine)                   *
 *                                                                            *
 *                      File Name : Straw.h                                   *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : May 14, 2022                              *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   Load external resources.                                                 *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#ifndef _STRAW_H_
#define _STRAW_H_

#include <easyx.h>

#include "Auxilliary.h"


bool FetchImage(IMAGE* pDestImage, const char* filename);
bool FetchImage(IMAGE* pDestImage, const wchar_t* filename);

bool FetchImage(
	IMAGE* pDestImage,
	const char* filename,
	int destWidth,
	int destHeight);

bool FetchImage(
	IMAGE* pDestImage,
	const wchar_t* filename,
	int destWidth,
	int destHeight);

bool FetchImage(
	IMAGE* pDestImage,
	const char* filename,
	int srcX,
	int srcY,
	int srcWidth,
	int srcHeight);

bool FetchImage(
	IMAGE* pDestImage,
	const wchar_t* filename,
	int srcX,
	int srcY,
	int srcWidth,
	int srcHeight);

bool FetchImage(
	IMAGE* pDestImage,
	const char* filename,
	int destWidth,
	int destHeight,
	int srcX,
	int srcY,
	int srcWidth,
	int srcHeight);

bool FetchImage(
	IMAGE* pDestImage,
	const wchar_t* filename,
	int destWidth,
	int destHeight,
	int srcX,
	int srcY,
	int srcWidth,
	int srcHeight);

bool FetchImage(IMAGE* pDestImage, IMAGE* pSrcImage);

bool FetchImage(
	IMAGE* pDestImage,
	IMAGE* pSrcImage,
	int srcX,
	int srcY,
	int srcWidth,
	int srcHeight);

bool FetchImage(
	IMAGE* pDestImage,
	IMAGE* pSrcImage,
	int destWidth,
	int destHeight,
	int srcX,
	int srcY,
	int srcWidth,
	int srcHeight);

struct DSound;

bool FetchSound(DSound* sound, const char* filename);
bool FetchStream(DSound* stream, const char* filename);

#endif