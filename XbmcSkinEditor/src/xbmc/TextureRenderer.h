/*
 *      Copyright (C) 2005-2009 Team XBMC
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
#ifndef BITMAP_CREATOR_H
#define BITMAP_CREATOR_H

#include <vector>
#include <stdint.h>
#include "system.h"
#include <d3d9.h>

#include "TextureBundleXBT.h"
#include "Texture.h"
#include "lib/libsquish/squish.h"


class CTextureRenderer
{
public:
  CTextureRenderer();
  ~CTextureRenderer();
  bool InitD3d(HWND hWnd);
  void Resize(int width = 0, int height = 0);
  int GetMaxHeight() { return m_pMaxHeight; }
  int GetMaxWidth() { return m_pMaxWidth; }
  bool SupportsDXT() { return true; }
  void RenderTexture(CBaseTexture* texture);
  IDirect3DDevice9* Get3DDevice() { return m_pD3DDevice; }
  void SaveTexture(CBaseTexture* texture, CStdString dest);
  //Different type of rendering
  HRESULT AlphaBlt(RECT* pSrc, RECT* pDst, IDirect3DTexture9* pTexture);
protected:
  int m_pMaxWidth;
  int m_pMaxHeight;
  int m_pBufferWidth;
  int m_pBufferHeight;
  IDirect3D9* m_pD3D; // Used to create the D3DDevice
  IDirect3DDevice9* m_pD3DDevice; // Our rendering device
  D3DPRESENT_PARAMETERS m_pD3DPP;
  HRESULT m_nDeviceStatus;
  void ResetViewPort();
};

extern CTextureRenderer g_pBitmapCreator;


#endif
