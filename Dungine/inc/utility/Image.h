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
 *   EasyX 20220610                                                           *
 ******************************************************************************/

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <easyx.h>


/********************************************************************
** Basize resize.
*/
void ResizeImage(IMAGE* pDestImage, IMAGE* pSrcImage, double scale);

void ResizeImage(
	IMAGE* pDestImage,
	IMAGE* pSrcImage,
	unsigned long destWidth,
	unsigned long destHeight);

void ResizeImage(
	IMAGE* pDestImage,
	IMAGE* pSrcImage,
	unsigned long destWidth,
	unsigned long destHeight,
	unsigned long srcWidth,
	unsigned long srcHeight);


/********************************************************************
** Resize and then copy to it self. 
*/
void ResizeImage(IMAGE* pImage, double scale);

void ResizeImage(
	IMAGE* pImage,
	unsigned long destWidth,
	unsigned long destHeight);

void ResizeImage(
	IMAGE* pImage,
	unsigned long destWidth,
	unsigned long destHeight,
	unsigned long srcWidth,
	unsigned long srcHeight);

#endif