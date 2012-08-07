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

#include "TextureRenderer.h"
#include "stdafx.h"
#ifdef _DEBUG
//#pragma comment (lib,"libpngd.lib")
#endif
#include <d3d9.h>
#include <d3dx9tex.h>

#define countof(array) (sizeof(array)/sizeof(array[0]))

template<int texcoords>
struct MYD3DVERTEX
{
  float x, y, z, rhw;
  struct
  {
    float u, v;
  } t[texcoords];
};

CTextureRenderer g_pBitmapCreator;
CTextureRenderer::CTextureRenderer()
{
  m_pD3D = NULL;
  m_pD3DDevice = NULL;
}

CTextureRenderer::~CTextureRenderer()
{
  SAFE_RELEASE(m_pD3D);
  SAFE_RELEASE(m_pD3DDevice);
}

bool CTextureRenderer::InitD3d(HWND hWnd)
{
  // Create the D3D object.
  if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
    return false;

  // Set up the structure used to create the D3DDevice. Since we are now
  // using more complex geometry, we will create a device with a zbuffer.
  D3DPRESENT_PARAMETERS d3dpp;
  ZeroMemory( &d3dpp, sizeof( d3dpp ) );
  d3dpp.Windowed = TRUE;
  d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
  d3dpp.EnableAutoDepthStencil = TRUE;
  d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
  d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
  // Create the D3DDevice
  if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &m_pD3DDevice ) ) )
  {
    return false;
  }
  ZeroMemory( &m_pD3DPP, sizeof( m_pD3DPP ) );
  m_pD3DPP = d3dpp;
  // Turn off culling
  m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
  
  // Turn off D3D lighting
  m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

  // Turn on the zbuffer
  m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
  D3DCAPS9 caps;
  memset(&caps, 0, sizeof(caps));
  m_pD3DDevice->GetDeviceCaps(&caps);
  m_pMaxWidth = caps.MaxTextureWidth;
  m_pMaxHeight = caps.MaxTextureHeight;

  LPDIRECT3DSURFACE9 pBackBuffer = NULL;
  D3DSURFACE_DESC d3dsd;
  m_pD3DDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
  pBackBuffer->GetDesc( &d3dsd );
  pBackBuffer->Release();
  m_pBufferWidth  = d3dsd.Width;
  m_pBufferHeight = d3dsd.Height;

  return true;
}

void CTextureRenderer::Resize(int width, int height)
{
  if (!m_pD3DDevice)
    return;
  m_pD3DPP.BackBufferWidth = width;
  m_pD3DPP.BackBufferHeight = height;
  
  HRESULT hr = m_pD3DDevice->Reset(&m_pD3DPP);
  if (SUCCEEDED(hr))
    printf("yeah");
  else
    printf("damn");
  

}

void CTextureRenderer::RenderTexture(CBaseTexture* texture)
{
  if (!m_pD3DDevice)
  {
    printf("yeah");
  }
  LPDIRECT3DSURFACE9 pBackBuffer = NULL;
  D3DSURFACE_DESC d3dsd;
  m_pD3DDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
  pBackBuffer->GetDesc( &d3dsd );
  pBackBuffer->Release();
  m_pBufferWidth  = d3dsd.Width;
  m_pBufferHeight = d3dsd.Height;
  D3DVIEWPORT9 leftViewPort;

    leftViewPort.X      = 0;
    leftViewPort.Y      = 0;
    leftViewPort.Width  = m_pBufferWidth;
    leftViewPort.Height = m_pBufferHeight;
    leftViewPort.MinZ   = 0.0f;
    leftViewPort.MaxZ   = 1.0f;

    m_pD3DDevice->SetViewport( &leftViewPort );

  int orientation = 0;
  int m_info_orientation = 0;
  float x[5];
  float y[5];
  float z[5];
  uint32_t m_col = 4294967295;
  for (int i = 0; i < 5; i++)
  {
    x[i] = 0.0f;
    y[i] = 0.0f;
    z[i] = 0.0f;
  }
  m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET ,
                         D3DCOLOR_XRGB( 212, 208, 200 ), 1.0f, 0 );

  HRESULT hr = m_pD3DDevice->BeginScene();
  

  texture->LoadToGPU();
  texture->BindToUnit(0);
  IDirect3DTexture9* pTexture = texture->Get3DTexture();
  hr = m_pD3DDevice->SetTexture( 0, pTexture );
  /*IDirect3DTexture9* g_pTexture;
  if( FAILED( D3DXCreateTextureFromFile( m_pD3DDevice, L"J:\\SDK\\DXSDK\\Samples\\C++\\Direct3D\\Tutorials\\Tut05_Textures\\banana.bmp", &g_pTexture ) ) )
    {
        // If texture is not in current folder, try parent folder
        if( FAILED( D3DXCreateTextureFromFile( m_pD3DDevice, L"..\\banana.bmp", &g_pTexture ) ) )
        {
            MessageBox( NULL, L"Could not find banana.bmp", L"Textures.exe", MB_OK );
            return;
        }
    }
  hr = m_pD3DDevice->SetTexture( 0, g_pTexture );*/

  hr = m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
  hr = m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  hr = m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
  hr = m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
  hr = m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
  hr = m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
  
  hr = m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  hr = m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
  hr = m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
  hr = m_pD3DDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
  hr = m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
  hr = m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE); 
  hr = m_pD3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE); 
  hr = m_pD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA|D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_RED); 
  
  hr = m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
  hr = m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
  hr = m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
  hr = m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
  hr = m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
  
  hr = m_pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);

  

  D3DSURFACE_DESC desc;
  if(!pTexture || FAILED(pTexture->GetLevelDesc(0, &desc)))
    return;
  
  float w = (float)desc.Width;
  float h = (float)desc.Height;

  MYD3DVERTEX<1> v[] =
  {
    {0, 0, 0.5f, 2.0f, 0, 0},
    {w, 0, 0.5f, 2.0f, 1, 0},
    {0, h, 0.5f, 2.0f, 0, 1},
    {w, h, 0.5f, 2.0f, 1, 1},
  };

  for(int i = 0; i < (countof(v)); i++)
  {
    v[i].x -= 0.5;
    v[i].y -= 0.5;
  }

  MYD3DVERTEX<1> tmp = v[2]; 
  v[2] = v[3]; 
  v[3] = tmp;

  hr = m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, v, sizeof(v[0]));
  m_pD3DDevice->SetTexture(0, NULL);
  hr = m_pD3DDevice->EndScene();
  hr = m_pD3DDevice->Present(NULL,NULL,NULL,NULL);
  texture->DestroyTextureObject();
}