/*
This file is part of Xbmc skin editor for notepad++
Copyright (C) 2009 LoonyChewy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#ifndef UNITY_BUILD_SINGLE_INCLUDE
#include "stdafx.h"
#include "XbmcImagePreviewer.h"
#include "resource.h"
#include "NativeLang_def.h"
#include "MultiClipboardSettings.h"
#include "MultiClipboardSettingsDialog.h"
#include "xbmc/lib/xbmc_communicator.h"
#include "lib/Scintillahelper.h"
#include "XbmcPluginEditor.h"
#endif


//************************ define here your toolbar layout *********************

extern HINSTANCE g_hInstance;
extern NppData g_NppData;
extern MultiClipboardSettingsDialog OptionsDlg;

XbmcImagePreviewer::XbmcImagePreviewer()
: DockingDlgInterface(IDD_DOCK_IMAGE_DLG)
, IsShown( false )
, IsLoading(false)
, m_pCurrentLine(0)
{
  
}


XbmcImagePreviewer::~XbmcImagePreviewer()
{
}


void XbmcImagePreviewer::Init( IModel * pNewModel, MultiClipboardProxy * pClipboardProxy, LoonySettingsManager * pSettings )
{
  DockingDlgInterface::init( g_hInstance, g_NppData._nppHandle );
  IController::Init( pNewModel, pClipboardProxy, pSettings );
}


void XbmcImagePreviewer::Shutdown()
{
}


void XbmcImagePreviewer::ShowDialog( bool Show )
{
  if ( !isCreated() )
  {
    create( &TBData );

    // define the default docking behaviour
    if ( !NLGetText( g_hInstance, g_NppData._nppHandle, TEXT("Xbmc Image Previewer"), TBData.pszName, MAX_PATH) )
    {
      lstrcpy( TBData.pszName, TEXT("Xbmc Image Previewer") );
    }
    TBData.uMask      = DWS_DF_CONT_LEFT | DWS_ICONTAB;
    TBData.hIconTab    = (HICON)::LoadImage(_hInst, MAKEINTRESOURCE(IDI_MULTICLIPBOARD), IMAGE_ICON, 0, 0, LR_LOADMAP3DCOLORS | LR_LOADTRANSPARENT);
    TBData.pszModuleName  = getPluginFileName();
    TBData.dlgID      = DIALOG_IMAGE_PREVIEWER;
    ::SendMessage( _hParent, NPPM_DMMREGASDCKDLG, 0, (LPARAM)&TBData );
    bool res = g_pBitmapCreator.InitD3d(getHSelf());
    if (!res)
      wprintf(L"damn");
  }

  display( Show );
  IsShown = Show;
}


BOOL CALLBACK XbmcImagePreviewer::run_dlgProc( HWND hWnd, UINT msg, WPARAM wp, LPARAM lp )
{

  switch ( msg )
  {
  case WM_INITDIALOG:
    InitialiseDialog();
    break;
  case WM_NCHITTEST:
    ShowImage();
    break;
  case WM_SIZE:
    {
      m_pCurrentLine = -1;
      ShowImage();
    }
  case WM_MOVE:
    {
      RECT rc;
      getClientRect(rc);
      g_pBitmapCreator.Resize(rc.right-rc.left,rc.bottom-rc.top);
      break;
    }

  case WM_NOTIFY:
    {
      LPNMHDR nmhdr = (LPNMHDR) lp;
      if ( nmhdr->hwndFrom == _hParent )
      {
        switch ( LOWORD( nmhdr->code ) )
        {
        case DMN_FLOAT:
        case DMN_DOCK:
          {
            if ( LOWORD( nmhdr->code ) == DMN_FLOAT )
            {
              _isFloating = true;
            }
            else
            {
              _isFloating = false;
              _iDockedPos = HIWORD( nmhdr->code );
            }
            break;
          }
        default:
          // Parse all other notifications to docking dialog interface
          return DockingDlgInterface::run_dlgProc( _hSelf, msg, wp, lp );
        }
      }
      else
      {
        // Parse all other notifications to docking dialog interface
        return DockingDlgInterface::run_dlgProc( _hSelf, msg, wp, lp );
      }
      break;
    }

  case WM_DESTROY:
    // Destroy icon of tab
    ::DestroyIcon( TBData.hIconTab );
    break;

  default:
    return DockingDlgInterface::run_dlgProc( _hSelf, msg, wp, lp );
  }

  return FALSE;
}


void XbmcImagePreviewer::InitialiseDialog()
{
  
}

void XbmcImagePreviewer::OnModelModified()
{
  
}

std::wstring A_to_W (const char* p)
{
    std::wstring   ws ;
    if (p)
    {
        setlocale (LC_CTYPE, "") ;

        std::vector<wchar_t>  buf (strlen(p)+1, 0) ;
        mbstowcs (&buf[0], p, buf.size()-1) ;
        ws = &buf[0] ;
    }
    return ws ;
}

void XbmcImagePreviewer::UpdateImage()
{
  if (IsShown)
    ShowImage();
}

void XbmcImagePreviewer::ClearWindow()
{
  bool result = m_pPicture.LoadBitmapW(IDB_BACKGROUND,g_hInstance);
  if (result)
  {
    if (m_pPicture.ColorBits() == 32)
      m_pPicture.ApplyEffect( FCEffectPremultipleAlpha());
    RECT    rcWindow;
    HDC hdc;
    ::GetClientRect(getHSelf(), &rcWindow);
    SIZE   img_size = {m_pPicture.Width(), m_pPicture.Height()} ;
     
    RECT   rc = FCObjImage::CalcFitWindowSize(img_size, rcWindow) ;
    hdc = GetWindowDC(getHSelf());
    m_pPicture.Draw (hdc, rcWindow) ;
  
  }
}

void XbmcImagePreviewer::ShowImage(CStdString image)
{
  if ( !IsShown && IsLoading )
  {
    return;
  }
  
  IsLoading=true;
  
  TCHAR dirpath[ MAX_PATH];
  SendMessage(g_NppData._nppHandle, NPPM_GETCURRENTDIRECTORY , MAX_PATH,(LPARAM)dirpath);
   
  std::wstring wpath = dirpath;
  std::wstring convertedstring = image;
  for (;;)
  {
    int last = wpath.find_last_of(L"\\");
    if (wpath.size()-1 == last)
    break;
    else
    wpath.pop_back();
  }
  wpath.insert(wpath.size(),L"media\\");
  convertedstring.insert(0,wpath);
  //reset background to normal
  ClearWindow();
  CStdString conv = convertedstring;
  conv.Replace(L"/",L"\\");
  bool res =m_pPicture.Load(conv.c_str());

  if (!res)
  {
#if 0
    res = m_pPicture.Load(image);
    printf("yeah");
#endif
    //g_XbmcIncludeFactory->
  
  }
  if (res)
  {
    
    if (m_pPicture.ColorBits() == 32)
    m_pPicture.ApplyEffect( FCEffectPremultipleAlpha());
    RECT  rcWindow;
    HDC hdc;
    ::GetClientRect(getHSelf(), &rcWindow);
    SIZE   img_size = {m_pPicture.Width(), m_pPicture.Height()} ;
    
    RECT   rc = FCObjImage::CalcFitWindowSize(img_size, rcWindow) ;
    hdc = GetWindowDC(getHSelf());
    
    ::GetClientRect(getHSelf(), &rcWindow);
    m_pPicture.Draw (hdc, rc) ;
    printf("image printed");
  }
  else
    printf("failed to load");


  
  IsLoading = false;

}
#define RETURNLOADING IsLoading = false; return;
void XbmcImagePreviewer::ShowImage()
{
  if ( !IsShown || IsLoading )
  {
    return;
  }
  
  IsLoading=true;
  TCHAR currentmsg[MAX_PATH];
  int current_line = ::SendMessage(g_NppData._nppHandle, NPPM_GETCURRENTLINE , 0,0);//MAX_PATH, (LPARAM)currentmsg);
  if (current_line != m_pCurrentLine)
  {
    m_pCurrentLine = current_line;
  }
  else
  {
    IsLoading = false;
    return;
  }

  std::wstring strCurrentLine = currentmsg;
  INT line_start = (INT)g_Scintilla.ScintillaMsg(SCI_POSITIONFROMLINE, current_line);
  INT line_end = (INT)g_Scintilla.ScintillaMsg(SCI_GETLINEENDPOSITION, current_line);
  int buffer_size = line_end-line_start+1;
  LPSTR strLine = (LPSTR) new CHAR[buffer_size];

  g_Scintilla.ScintillaGetText(strLine, line_start,line_end);

  CStdString line = strLine;
  /*if (!(line.Find(L"png",0)>0 || line.Find(L"jpg",0)>0))
    return;*/
  g_Scintilla.ScintillaMsg(SCI_SETSEARCHFLAGS, SCFIND_REGEXP|SCFIND_POSIX);
  g_Scintilla.ScintillaMsg(SCI_SETTARGETSTART, line_start);
  g_Scintilla.ScintillaMsg(SCI_SETTARGETEND, line_end);
  const char *regex = ">(.+?)<";

  std::string strLineStripped;

  int posFound = g_Scintilla.ScintillaMsg(SCI_SEARCHINTARGET, strlen(regex), (LPARAM)regex);
  if (posFound>-1)
  {
    int imagestart,imageend;
    imagestart = g_Scintilla.ScintillaMsg(SCI_GETTARGETSTART);
    imageend = g_Scintilla.ScintillaMsg(SCI_GETTARGETEND);
    LPSTR image_str = (LPSTR) new CHAR[imageend-imagestart+1];
    g_Scintilla.ScintillaGetText(strLine, imagestart,imageend);
    strLineStripped = strLine;
    strLineStripped.erase(0,1);
    
    strLineStripped.pop_back();
    TCHAR dirpath[ MAX_PATH];
    SendMessage(g_NppData._nppHandle, NPPM_GETCURRENTDIRECTORY , MAX_PATH,(LPARAM)dirpath);
     
    std::wstring wpath = dirpath;
    std::wstring convertedstring = A_to_W(strLineStripped.c_str());
    for (;;)
    {
      int last = wpath.find_last_of(L"\\");
      if (wpath.size()-1 == last)
        break;
      else
        wpath.pop_back();
    }
    wpath.insert(wpath.size(),L"media\\");
    convertedstring.insert(0,wpath);
    //reset background to normal
    ClearWindow();
    CStdString conv = convertedstring;
    conv.Replace(L"/",L"\\");
    CStdStringA convA = conv;
    bool res = false;
    if(fopen(convA.c_str(),"r")!=0)
    {
      CBaseTexture* text;
      text = new CTexture();
      res = text->LoadFromFile2(conv);
      if (res)
      {
        g_pBitmapCreator.RenderTexture(text);
        RETURNLOADING;
      }

    } 
    if (!res)
    {
      CBaseTexture* text;
      CXBTFFrame frame;
      if (!g_XbmcIncludeFactory->GetFrame(conv, frame, &text))
      {
        RETURNLOADING;
      }
      g_pBitmapCreator.RenderTexture(text);
      
      RETURNLOADING;
    }
  }
  IsLoading = false;

}

void XbmcImagePreviewer::OnObserverAdded( LoonySettingsManager * SettingsManager )
{
  SettingsObserver::OnObserverAdded( SettingsManager );
}


void XbmcImagePreviewer::OnSettingsChanged( const stringType & GroupName, const stringType & SettingName )
{
  if ( GroupName != SETTINGS_GROUP_XBMC )
    return;
}