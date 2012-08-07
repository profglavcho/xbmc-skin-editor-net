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
#include <d3dx9tex.h>
#ifdef _DEBUG
//#pragma comment (lib,"libpngd.lib")
#endif
#include <d3d9.h>


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

template<int texcoords>
static void AdjustQuad(MYD3DVERTEX<texcoords>* v, double dx, double dy)
{
  double offset = 0.5;

  for(int i = 0; i < 4; i++)
  {
    v[i].x -= (float) offset;
    v[i].y -= (float) offset;
    
    for(int j = 0; j < max(texcoords-1, 1); j++)
    {
      v[i].t[j].u -= (float) (offset*dx);
      v[i].t[j].v -= (float) (offset*dy);
    }

    if(texcoords > 1)
    {
      v[i].t[texcoords-1].u -= (float) offset;
      v[i].t[texcoords-1].v -= (float) offset;
    }
  }
}

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

void CTextureRenderer::ResetViewPort()
{
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
}

void CTextureRenderer::Resize(int width, int height)
{
  if (!m_pD3DDevice)
    return;
  if (width>0 && height >0)
  {
    m_pD3DPP.BackBufferWidth = width;
    m_pD3DPP.BackBufferHeight = height;
  }
  m_nDeviceStatus = m_pD3DDevice->Reset(&m_pD3DPP);
  if (SUCCEEDED(m_nDeviceStatus))
    printf("yeah");
  else
    printf("damn");
  

}

void CTextureRenderer::SaveTexture(CBaseTexture* texture, CStdString dest)
{
  texture->LoadToGPU();
  texture->BindToUnit(0);
  
  HRESULT hr = D3DXSaveTextureToFile(dest.c_str(),D3DXIFF_PNG, texture->Get3DTexture(), NULL);
  if (SUCCEEDED(hr))
    wprintf(L"yeah");
  else
    wprintf(L"oh no");
}

void CTextureRenderer::RenderTexture(CBaseTexture* texture)
{
  if (!m_pD3DDevice)
    return;
  m_nDeviceStatus = m_pD3DDevice->TestCooperativeLevel();
  if (FAILED(m_nDeviceStatus))
  {
    Resize();
    return;
  }
  ResetViewPort();
  
  m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 212, 208, 200 ), 1.0f, 0 );// clear with the color of a dialog

  HRESULT hr = m_pD3DDevice->BeginScene();
  if (texture->GetWidth() > 0)
  {
    texture->LoadToGPU();
    texture->BindToUnit(0);
  }
  else
  {
    wprintf(L"not using the xbt file");
    
  }
  IDirect3DTexture9* pTexture = texture->Get3DTexture();
  hr = m_pD3DDevice->SetTexture( 0, pTexture );
  /**/
  
#if 0
  D3DSURFACE_DESC desc;
  if(!pTexture || FAILED(pTexture->GetLevelDesc(0, &desc)))
    return;
  float w = (float)desc.Width;
  float h = (float)desc.Height;
  RECT src;
  RECT dst;
  src.left = 0; src.top = 0;
  src.right = w;
  src.bottom = h;
  dst.left = 0; dst.top = 0;
  dst.right = m_pBufferWidth;
  dst.bottom = m_pBufferHeight;
  hr = AlphaBlt(&src, &dst, pTexture);
  hr = m_pD3DDevice->EndScene();
  hr = m_pD3DDevice->Present(NULL,NULL,NULL,NULL);
  texture->DestroyTextureObject();
  return;
  /**/
#else
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
#endif
}





HRESULT CTextureRenderer::AlphaBlt(RECT* pSrc, RECT* pDst, IDirect3DTexture9* pTexture)
{
  if(!pSrc || !pDst)
    return E_POINTER;

  CRect src(*pSrc), dst(*pDst);

  HRESULT hr;

  D3DSURFACE_DESC d3dsd;
  ZeroMemory(&d3dsd, sizeof(d3dsd));
  if(FAILED(pTexture->GetLevelDesc(0, &d3dsd)) /*|| d3dsd.Type != D3DRTYPE_TEXTURE*/)
    return E_FAIL;

  float w = (float)d3dsd.Width;
  float h = (float)d3dsd.Height;

  MYD3DVERTEX<1> pVertices[] =
  {
    {(float)dst.left, (float)dst.top, 0.5f, 2.0f, (float)src.left / w, (float)src.top / h},
    {(float)dst.right, (float)dst.top, 0.5f, 2.0f, (float)src.right / w, (float)src.top / h},
    {(float)dst.left, (float)dst.bottom, 0.5f, 2.0f, (float)src.left / w, (float)src.bottom / h},
    {(float)dst.right, (float)dst.bottom, 0.5f, 2.0f, (float)src.right / w, (float)src.bottom / h},
  };
  AdjustQuad(pVertices, 0, 0);

  hr = m_pD3DDevice->SetTexture(0, pTexture);
  hr = m_pD3DDevice->SetPixelShader(NULL);

  DWORD abe, sb, db;
  hr = m_pD3DDevice->GetRenderState(D3DRS_ALPHABLENDENABLE, &abe);
  hr = m_pD3DDevice->GetRenderState(D3DRS_SRCBLEND, &sb);
  hr = m_pD3DDevice->GetRenderState(D3DRS_DESTBLEND, &db);

  hr = m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  hr = m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
  hr = m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
  hr = m_pD3DDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
  hr = m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
  hr = m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE); 
  hr = m_pD3DDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE); 
  hr = m_pD3DDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA|D3DCOLORWRITEENABLE_BLUE|D3DCOLORWRITEENABLE_GREEN|D3DCOLORWRITEENABLE_RED); 
  hr = m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE); // pre-multiplied src and ...
  hr = m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA); // ... inverse alpha channel for dst

  hr = m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE );
  hr = m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
  hr = m_pD3DDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
  hr = m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE );
  hr = m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
  hr = m_pD3DDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );

  hr = m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
  hr = m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
  hr = m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
  hr = m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
  hr = m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

  hr = m_pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);

  MYD3DVERTEX<1> tmp = pVertices[2]; 
  pVertices[2] = pVertices[3]; 
  pVertices[3] = tmp;
  hr = m_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 2, pVertices, sizeof(pVertices[0]));

  m_pD3DDevice->SetTexture(0, NULL);

  m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, abe);
  m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, sb);
  m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, db);

  return S_OK;
}