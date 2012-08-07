#pragma once

/*
 *      Copyright (C) 2005-2008 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "system.h"
#include <map>
#include "XBTFReader.h"

class CBaseTexture;

struct ImageFrame
{
  void* memory;
  int size;
};

struct ImageRawFrame
{
  ImageFrame image_frame;
  CXBTFFrame xbtf_frame;
};

class CTextureBundleXBT
{
public:
  CTextureBundleXBT(void);
  ~CTextureBundleXBT(void);

  void Cleanup();
  void SetThemeBundle(bool themeBundle);
  bool HasFile(const CStdString& Filename);
  void GetTexturesFromPath(const CStdString &path, std::vector<CStdString> &textures);
  static CStdString Normalize(const CStdString &name);
  bool ConvertFrameToTexture(const CStdString& name, CXBTFFrame& frame, CBaseTexture** ppTexture);

  bool LoadTextureForSaving(const CStdString& Filename, CBaseTexture** ppTexture);
  bool LoadTexture(const CStdString& Filename, CBaseTexture** ppTexture,
                       int &width, int &height);
  #if 0
  int LoadAnim(const CStdString& Filename, CBaseTexture*** ppTextures,
                int &width, int &height, int& nLoops, int** ppDelays);
#endif
  bool OpenBundle( CStdString Filename);//was private
private:
  
#if 0 
  bool ConvertFrameToTexture(const CStdString& name, CXBTFFrame& frame, CBaseTexture** ppTexture);
#endif

  time_t m_TimeStamp;

  bool m_themeBundle;
  CXBTFReader m_XBTFReader;
  CStdString strCurrentPath;
};


