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

#pragma once


#include "system.h"
#include <d3d9.h>
#include <d3d9types.h>
#include <map>

class ID3DResource
{
public:
  virtual ~ID3DResource() {};

  virtual void OnDestroyDevice()=0;
  virtual void OnCreateDevice()=0;
  virtual void OnLostDevice() {};
  virtual void OnResetDevice() {};
};

class CD3DTexture : public ID3DResource
{
public:
  CD3DTexture();
  ~CD3DTexture();

  bool Create(UINT width, UINT height, UINT mipLevels, DWORD usage, D3DFORMAT format, D3DPOOL pool);
  void Release();
  bool LockRect(UINT level, D3DLOCKED_RECT *lr, const RECT *rect, DWORD flags);
  bool UnlockRect(UINT level);
  bool GetLevelDesc(UINT level, D3DSURFACE_DESC *desc);
  bool GetSurfaceLevel(UINT level, LPDIRECT3DSURFACE9 *surface);

  // Accessors
  LPDIRECT3DTEXTURE9 Get() const { return m_texture; };
  UINT GetWidth()  { return m_width; }
  UINT GetHeight() { return m_height; }

  virtual void OnDestroyDevice();
  virtual void OnCreateDevice();
  virtual void OnLostDevice();
  virtual void OnResetDevice();


private:
  unsigned int GetMemoryUsage(unsigned int pitch) const;

  void SaveTexture();
  void RestoreTexture();

  // creation parameters
  UINT      m_width;
  UINT      m_height;
  UINT      m_mipLevels;
  DWORD     m_usage;
  D3DFORMAT m_format;
  D3DPOOL   m_pool;
  UINT      m_pitch;

  // created texture
  LPDIRECT3DTEXTURE9 m_texture;

  // saved data
  BYTE*     m_data;
};