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
#include "D3DResource.h"
#include "TextureRenderer.h"
#include <d3dx9tex.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"D3dx9.lib")

using namespace std;

CD3DTexture::CD3DTexture()
{
  m_width = 0;
  m_height = 0;
  m_mipLevels = 0;
  m_usage = 0;
  m_format = D3DFMT_A8R8G8B8;
  m_pool = D3DPOOL_DEFAULT;
  m_texture = NULL;
  m_data = NULL;
  m_pitch = 0;
}

CD3DTexture::~CD3DTexture()
{
  Release();
  delete[] m_data;
}

bool CD3DTexture::Create(UINT width, UINT height, UINT mipLevels, DWORD usage, D3DFORMAT format, D3DPOOL pool)
{
  m_width = width;
  m_height = height;
  m_mipLevels = mipLevels;
  m_usage = usage;
  m_format = format;
  m_pool = pool;
  // create the texture
  Release();
  HRESULT hr = D3DXCreateTexture(g_pBitmapCreator.Get3DDevice(), m_width, m_height, m_mipLevels, m_usage, m_format, m_pool, &m_texture);
  if (FAILED(hr))
  {
    wprintf(L"yeah");
    //CLog::Log(LOGERROR, __FUNCTION__" - failed 0x%08X", hr);
  }
  else
  {
    D3DSURFACE_DESC desc;
    if( D3D_OK == m_texture->GetLevelDesc(0, &desc))
    {
      if(desc.Format != m_format)
      {
        wprintf(L"warning");
      }
        //CLog::Log(LOGWARNING, "CD3DTexture::Create - format changed from %d to %d", m_format, desc.Format);
      if(desc.Height != m_height || desc.Width != m_width)
        wprintf(L"warning");
        //CLog::Log(LOGWARNING, "CD3DTexture::Create - size changed from %ux%u to %ux%u", m_width, m_height, desc.Width, desc.Height);
    }

    //g_Windowing.Register(this);
    return true;
  }
  return false;
}

void CD3DTexture::Release()
{
  //g_Windowing.Unregister(this);
  SAFE_RELEASE(m_texture);
}

bool CD3DTexture::LockRect(UINT level, D3DLOCKED_RECT *lr, const RECT *rect, DWORD flags)
{
  if (m_texture)
  {
    if ((flags & D3DLOCK_DISCARD) && !(m_usage & D3DUSAGE_DYNAMIC))
      flags &= ~D3DLOCK_DISCARD;
    return (D3D_OK == m_texture->LockRect(level, lr, rect, flags));
  }
  return false;
}

bool CD3DTexture::UnlockRect(UINT level)
{
  if (m_texture)
    return (D3D_OK == m_texture->UnlockRect(level));
  return false;
}

bool CD3DTexture::GetLevelDesc(UINT level, D3DSURFACE_DESC *desc)
{
  if (m_texture)
    return (D3D_OK == m_texture->GetLevelDesc(level, desc));
  return false;
}

bool CD3DTexture::GetSurfaceLevel(UINT level, LPDIRECT3DSURFACE9 *surface)
{
  if (m_texture)
    return (D3D_OK == m_texture->GetSurfaceLevel(level, surface));
  return false;
}

void CD3DTexture::SaveTexture()
{
  if (m_texture)
  {
    delete[] m_data;
    m_data = NULL;
    if(!(m_usage & D3DUSAGE_RENDERTARGET)
    && !(m_usage & D3DUSAGE_DEPTHSTENCIL)
    && !(m_pool == D3DPOOL_DEFAULT && (m_usage & D3DUSAGE_DYNAMIC) == 0))
    {
      D3DLOCKED_RECT lr;
      if (LockRect( 0, &lr, NULL, D3DLOCK_READONLY ))
      {
        m_pitch = lr.Pitch;
        unsigned int memUsage = GetMemoryUsage(lr.Pitch);
        m_data = new unsigned char[memUsage];
        memcpy(m_data, lr.pBits, memUsage);
        UnlockRect(0);
      }
    }
  }
  SAFE_RELEASE(m_texture);
}

void CD3DTexture::OnDestroyDevice()
{
  SaveTexture();
}

void CD3DTexture::OnLostDevice()
{
  if (m_pool == D3DPOOL_DEFAULT)
    SaveTexture();
}

void CD3DTexture::RestoreTexture()
{
  // yay, we're back - make a new copy of the texture
  if (!m_texture)
  {
    HRESULT hr = D3DXCreateTexture(g_pBitmapCreator.Get3DDevice(), m_width, m_height, m_mipLevels, m_usage, m_format, m_pool, &m_texture);
    if (FAILED(hr))
    {
      //CLog::Log(LOGERROR, __FUNCTION__": D3DXCreateTexture failed 0x%08X", hr);
    }
    else
    {
      // copy the data to the texture
      D3DLOCKED_RECT lr;
      if (m_texture && m_data && LockRect(0, &lr, NULL, D3DLOCK_DISCARD ))
      {
        if (lr.Pitch == m_pitch)
          memcpy(lr.pBits, m_data, GetMemoryUsage(lr.Pitch));
        else
        {
          UINT minpitch = ((UINT)lr.Pitch < m_pitch) ? lr.Pitch : m_pitch;
        
          for(UINT i = 0; i < m_height; ++i)
          {
            // Get pointers to the "rows" of pixels in texture
            BYTE* pBits = (BYTE*)lr.pBits + i*lr.Pitch;
            BYTE* pData = m_data + i*m_pitch;
            memcpy(pBits, pData, minpitch);
          }
        }
        UnlockRect(0);
      }
    }

    delete[] m_data;
    m_data = NULL;
    m_pitch = 0;
  }
}

void CD3DTexture::OnCreateDevice()
{
  RestoreTexture();
}

void CD3DTexture::OnResetDevice()
{
  if (m_pool == D3DPOOL_DEFAULT)
    RestoreTexture();
}


unsigned int CD3DTexture::GetMemoryUsage(unsigned int pitch) const
{
  switch (m_format)
  {
  case D3DFMT_DXT1:
  case D3DFMT_DXT3:
  case D3DFMT_DXT5:
    return pitch * m_height / 4;
  default:
    return pitch * m_height;
  }
}
