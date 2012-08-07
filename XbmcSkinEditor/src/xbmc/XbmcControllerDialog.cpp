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
#include "XbmcControllerDialog.h"
#include "resource.h"
#include "NativeLang_def.h"
#include "MultiClipboardSettings.h"
#include "MultiClipboardSettingsDialog.h"
#include "xbmc/lib/xbmc_communicator.h"
#include "XbmcPluginEditor.h"
#endif


//************************ define here your toolbar layout *********************

extern HINSTANCE g_hInstance;
extern NppData g_NppData;
extern MultiClipboardProxy g_ClipboardProxy;
extern MultiClipboardSettingsDialog OptionsDlg;


XbmcControllerDialog::XbmcControllerDialog()
: DockingDlgInterface(IDD_DOCK_XBMC_DLG)
, IsShown( false )
, DragListMessage( 0 )
, pDataObject( NULL )
, pDropSource( NULL )
{
}


XbmcControllerDialog::~XbmcControllerDialog()
{
}


void XbmcControllerDialog::Init( IModel * pNewModel, MultiClipboardProxy * pClipboardProxy, LoonySettingsManager * pSettings )
{
  DockingDlgInterface::init( g_hInstance, g_NppData._nppHandle );
  IController::Init( pNewModel, pClipboardProxy, pSettings );
  DragListMessage = ::RegisterWindowMessage( DRAGLISTMSGSTRING );

  MultiClipOLEDataObject::CreateDataObject( &pDataObject );
  MultiClipOLEDropSource::CreateDropSource( &pDropSource );
}


void XbmcControllerDialog::Shutdown()
{
  pDataObject->Release();
  pDropSource->Release();
}


void XbmcControllerDialog::ShowDialog( bool Show )
{
  if ( !isCreated() )
  {
    create( &TBData );

    // define the default docking behaviour
    if ( !NLGetText( g_hInstance, g_NppData._nppHandle, TEXT("Xbmc Commands"), TBData.pszName, MAX_PATH) )
    {
      lstrcpy( TBData.pszName, TEXT("Xbmc Commands") );
    }
    TBData.uMask      = DWS_DF_CONT_LEFT | DWS_ICONTAB;
    TBData.hIconTab    = (HICON)::LoadImage(_hInst, MAKEINTRESOURCE(IDI_MULTICLIPBOARD), IMAGE_ICON, 0, 0, LR_LOADMAP3DCOLORS | LR_LOADTRANSPARENT);
    TBData.pszModuleName  = getPluginFileName();
    TBData.dlgID      = DIALOG_TESTER;
    ::SendMessage( _hParent, NPPM_DMMREGASDCKDLG, 0, (LPARAM)&TBData );
  }

  display( Show );
  IsShown = Show;
  ShowClipText();
}


BOOL CALLBACK XbmcControllerDialog::run_dlgProc( HWND hWnd, UINT msg, WPARAM wp, LPARAM lp )
{

  switch ( msg )
  {
  case WM_INITDIALOG:
    InitialiseDialog();
    break;

  case WM_SIZE:
  case WM_MOVE:
    {
      RECT rc;
      getClientRect(rc);
      SetSplitterOrientation();
      MultiClipViewerPanel.reSizeTo(rc);
      break;
    }

  case WM_COMMAND:
    if ( (HWND)lp == MultiClipViewerListbox.getHSelf() )
    {
      switch ( HIWORD(wp) )
      {
      case LBN_SELCHANGE:
        OnListSelectionChanged();
        return 0;

      case LBN_DBLCLK:
        OnListDoubleClicked();
        return 0;

      case LBN_DELETEITEM:
        DeleteSelectedItem();
        return 0;
      }
    }
    else if ( (HWND)lp == MultiClipViewerEditBox.getHSelf() )
    {
      switch ( HIWORD(wp) )
      {
      case EN_UPDATE:
        OnEditBoxUpdated();
        return 0;
      }
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
            SetSplitterOrientation();
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


void XbmcControllerDialog::InitialiseDialog()
{
  MultiClipViewerPanel.init( _hInst, _hSelf );

  ListBoxPanel.init( _hInst, MultiClipViewerPanel.getHSelf() );
  MultiClipViewerListbox.init( _hInst, ListBoxPanel.getHSelf() );
  MultiClipViewerPanel.pChildWin1 = &ListBoxPanel;
  ListBoxPanel.SetChildWindow( &MultiClipViewerListbox );
  /*ListBoxToolBar.init( _hInst, ListBoxPanel.getHSelf(), TB_STANDARD, ListBoxToolBarXbmcButtons, ListBoxToolBarSize );
  ListBoxToolBar.display();
  ListBoxPanel.SetToolbar( &ListBoxToolBar );
  */
  EditBoxPanel.init( _hInst, MultiClipViewerPanel.getHSelf() );
  MultiClipViewerEditBox.init( _hInst, EditBoxPanel.getHSelf() );
  MultiClipViewerPanel.pChildWin2 = &EditBoxPanel;
  EditBoxPanel.SetChildWindow( &MultiClipViewerEditBox );
  MultiClipViewerEditBox.EnableEditBox( FALSE );
}


void XbmcControllerDialog::SetSplitterOrientation()
{
  RECT rc = {0};

  getClientRect(rc);
  if ( _isFloating )
  {
    if ( (rc.bottom-rc.top) >= (rc.right-rc.left) )
    {
      MultiClipViewerPanel.SetSplitterPanelOrientation( ESPO_VERTICAL );
    }
    else
    {
      MultiClipViewerPanel.SetSplitterPanelOrientation( ESPO_HORIZONTAL );
    }
  }
  else
  {
    if ( _iDockedPos == CONT_LEFT || _iDockedPos == CONT_RIGHT )
    {
      MultiClipViewerPanel.SetSplitterPanelOrientation( ESPO_VERTICAL );
    }
    else
    {
      MultiClipViewerPanel.SetSplitterPanelOrientation( ESPO_HORIZONTAL );
    }
  }
}


void XbmcControllerDialog::OnModelModified()
{
  ShowClipText();
}


void XbmcControllerDialog::ShowClipText()
{
  if ( !IsShown )
    return;

  MultiClipViewerListbox.ClearAll();
  MultiClipViewerListbox.AddItem(L"Test window");
  MultiClipViewerListbox.AddItem(L"Extract xbt");
}


void XbmcControllerDialog::OnListSelectionChanged()
{
  MultiClipViewerEditBox.SetText(L"");
  INT Index = MultiClipViewerListbox.GetCurrentSelectionIndex();
  if ( Index == LB_ERR )
    return;
  CStdString thetext = MultiClipViewerListbox.GetCurrentSelectionText();
  
  if (thetext.Equals(L"Test window"))
  {
    TCHAR currentFile[MAX_PATH];
    ::SendMessage(g_NppData._nppHandle, NPPM_GETFILENAME , MAX_PATH, (LPARAM)currentFile);
    std::wstring window = currentFile;
    XbmcCommunicator* com = new XbmcCommunicator();
    if (window.compare(window.size()-4,4,L".xml")==0)
    {
      std::wstring windowid = com->GetWindowId(window);
      if (windowid.compare(window)!=0)
      {
        windowid.insert(0,std::wstring(L"The window id that \r\nis going to be loaded: "));
        MultiClipViewerEditBox.SetText( windowid.c_str() );
        // And make it enabled, but read-only
        MultiClipViewerEditBox.EnableEditBox();
        MultiClipViewerEditBox.SetEditBoxReadOnly( TRUE );
        return;
      }
    }
    
  }
  else if (thetext.Equals(L"Extract xbt"))
  {
    std::wstring desc;
    desc = L"This command is used if you want to extract a compressed xbt file";
    MultiClipViewerEditBox.SetText( desc.c_str() );
    
    MultiClipViewerEditBox.EnableEditBox();
    MultiClipViewerEditBox.SetEditBoxReadOnly( TRUE );

  }
}

static int __stdcall BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM, LPARAM pData)
{
  if (uMsg == BFFM_INITIALIZED)
    ::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, pData);
  return 0;
};

CStdString XbmcControllerDialog::ShowFolderBrowser()
{
  CStdString pStrSelectedFolder;
  //Coming from the explorer plugin
  LPTSTR _pLink = (LPTSTR)new TCHAR[MAX_PATH];
  LPMALLOC pShellMalloc = 0;
  if (::SHGetMalloc(&pShellMalloc) == NO_ERROR)
  {
    // If we were able to get the shell malloc object,
    // then proceed by initializing the BROWSEINFO stuct
    BROWSEINFO info;
    ZeroMemory(&info, sizeof(info));
    info.hwndOwner      = _hParent;
    info.pidlRoot      = NULL;
    info.pszDisplayName    = (LPTSTR)new TCHAR[MAX_PATH];
    info.lpszTitle      = _T("Select a folder for extraction:");
    info.ulFlags      = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    info.lpfn        = BrowseCallbackProc;
    info.lParam        = (LPARAM)_pLink;

    // Execute the browsing dialog.
    LPITEMIDLIST pidl = ::SHBrowseForFolder(&info);

    // pidl will be null if they cancel the browse dialog.
    // pidl will be not null when they select a folder.
    if (pidl) 
    {
      // Try to convert the pidl to a display string.
      // Return is true if success.
      if (::SHGetPathFromIDList(pidl, _pLink))
      {
        pStrSelectedFolder = _pLink;
      }
      pShellMalloc->Free(pidl);
    }
    pShellMalloc->Release();
    delete [] info.pszDisplayName;
  }
  return pStrSelectedFolder;
}

CStdString XbmcControllerDialog::ShowFileBrowser()
{
  InitCommonControls();
  CStdString pStrSelectedFile;
  OPENFILENAME info;
  ZeroMemory(&info, sizeof(info));
  
  info.lStructSize = sizeof(info);
  info.hwndOwner = _hParent;
  info.hInstance = getHinst();
  info.lpstrFilter = L"Xbt files\0*.xbt\0\0";
  info.lpTemplateName = NULL;
  info.lpfnHook = NULL;
  info.lpstrCustomFilter = NULL;
  info.nMaxFile = MAX_PATH;
  
  LPTSTR fileout = (LPTSTR)new TCHAR[MAX_PATH];
  fileout[0] = NULL;
  info.lpstrFile = fileout;
  info.lpstrFileTitle = NULL;
  info.nMaxFileTitle = MAX_PATH;
  info.lpstrInitialDir = NULL;
  info.lpstrTitle = NULL;
  info.nFileOffset = 0;
  info.nFileExtension = 0;
  info.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
  BOOL res = GetOpenFileName(&info);
  DWORD err;
  if (res)
  {
    pStrSelectedFile = info.lpstrFile;
  }
  else
  {
  err = CommDlgExtendedError();
  
  
  }


  return pStrSelectedFile;
}


void XbmcControllerDialog::OnListDoubleClicked()
{
  INT Index = MultiClipViewerListbox.GetCurrentSelectionIndex();
  if ( Index == LB_ERR )
    return;
  
  
  CStdString thetext = MultiClipViewerListbox.GetCurrentSelectionText();
  
  if (thetext.Equals(L"Test window"))
  {
    TCHAR currentFile[MAX_PATH];
    ::SendMessage(g_NppData._nppHandle, NPPM_GETFILENAME , MAX_PATH, (LPARAM)currentFile);
    XbmcCommunicator* com = new XbmcCommunicator();
    std::wstring window = currentFile;
    for(size_t i=0; i<window.size();++i)
     window[i] = tolower(window[i]);
    if (window.compare(window.size()-4,4,L".xml")==0)
    {
      com->SendCommand(m_pXbmcIp,m_pXbmcLogin,m_pXbmcPassword,m_pWgetPath, window);
    }
  }
  else if (thetext.Equals(L"Extract xbt"))
  {
    CStdString xbtpath;
    CStdString destfolder;
    CStdString destFile;
    xbtpath = ShowFileBrowser();
    if (xbtpath.size() == 0)
    {
      ::MessageBoxW(getHParent(), L"You most choose a xbt file",L"no input file", MB_OK);
      return;
    }
    destfolder = ShowFolderBrowser();
    if (destfolder.size() == 0)
    {
      ::MessageBoxW(getHParent(), L"You most choose an output directory",L"no output dir", MB_OK);
      return;
    }
    if (!destfolder.Right(1).Equals(L"\\"))
      destfolder.append(L"\\");
    std::vector<CStdString> textures = g_XbmcIncludeFactory->GetTexturesFromXbt(xbtpath);
    for (size_t i = 0; i < textures.size(); i++)
    {
      CBaseTexture* text = NULL;
      CXBTFFrame frame;
      destFile = textures[i];
      
      destFile.Replace(L"/",L"\\");
      CStdStringA foldcreate;
      CStdStringW foldcreateW;
      if (destFile.Find(L"\\",0)>-1)
      {
        
        //folder might not exist
        foldcreateW = destFile;
        int index = foldcreateW.find_last_of(L"\\");
        foldcreateW.Delete(index,foldcreateW.size()-index);
        foldcreateW.Insert(0,destfolder.c_str());
        foldcreate = foldcreateW;
        if ((GetFileAttributes(foldcreateW.c_str())) == INVALID_FILE_ATTRIBUTES)
        {
          foldcreate.Insert(0,"mkdir ");
          int res = system(foldcreate.c_str());
        }

        
        
      
      }
      destFile.Insert(0,destfolder.c_str());
      if (g_XbmcIncludeFactory->GetBaseTexture(textures[i], &text))
      {
        //file don't already exist create it
        if ((GetFileAttributes(destFile.c_str())) == INVALID_FILE_ATTRIBUTES)
        {
          g_pBitmapCreator.SaveTexture(text, destFile);
        }
        delete text;
      }
      else
      {
        wprintf(L"wtf");
      }
    }

  }

  
}


void XbmcControllerDialog::OnEditBoxUpdated()
{
  
}


void XbmcControllerDialog::PasteSelectedItem()
{
}


void XbmcControllerDialog::PasteAllItems()
{
  
}


void XbmcControllerDialog::DeleteSelectedItem()
{
  
}


void XbmcControllerDialog::DeleteAllItems()
{
  
}


void XbmcControllerDialog::CopySelectedItemToClipboard()
{
  
}


void XbmcControllerDialog::OnObserverAdded( LoonySettingsManager * SettingsManager )
{
  SettingsObserver::OnObserverAdded( SettingsManager );
  // Add default settings if it doesn't exists
  SET_SETTINGS_STRING( SETTINGS_GROUP_XBMC, SETTINGS_XBMC_IP, m_pXbmcIp )
  SET_SETTINGS_STRING( SETTINGS_GROUP_XBMC, SETTINGS_WGET_PATH, m_pWgetPath )
  SET_SETTINGS_STRING( SETTINGS_GROUP_XBMC, SETTINGS_XBMC_LOGIN, m_pXbmcLogin )
  SET_SETTINGS_STRING( SETTINGS_GROUP_XBMC, SETTINGS_XBMC_PASS, m_pXbmcPassword )
}


void XbmcControllerDialog::OnSettingsChanged( const stringType & GroupName, const stringType & SettingName )
{
  if ( GroupName != SETTINGS_GROUP_XBMC )
  {
    return;
  }
  IF_SETTING_CHANGED_STRING( SETTINGS_GROUP_XBMC, SETTINGS_XBMC_IP, m_pXbmcIp )
  else IF_SETTING_CHANGED_STRING( SETTINGS_GROUP_XBMC, SETTINGS_WGET_PATH, m_pWgetPath )
  else IF_SETTING_CHANGED_STRING( SETTINGS_GROUP_XBMC, SETTINGS_XBMC_LOGIN, m_pXbmcLogin )
  else IF_SETTING_CHANGED_STRING( SETTINGS_GROUP_XBMC, SETTINGS_XBMC_PASS, m_pXbmcPassword )
  /*IF_SETTING_CHANGED_STRING(
  IF_SETTING_CHANGED_BOOL( SETTINGS_GROUP_MULTI_CLIP_VIEWER, SETTINGS_NO_EDIT_LARGE_TEXT, bNoEditLargeText )
  else IF_SETTING_CHANGED_INT( SETTINGS_GROUP_MULTI_CLIP_VIEWER, SETTINGS_NO_EDIT_LARGE_TEXT_SIZE, NoEditLargeTextSize )
  else IF_SETTING_CHANGED_INT( SETTINGS_GROUP_MULTI_CLIP_VIEWER, SETTINGS_LARGE_TEXT_DISPLAY_SIZE, LargeTextDisplaySize )
  else IF_SETTING_CHANGED_BOOL( SETTINGS_GROUP_MULTI_CLIP_VIEWER, SETTINGS_PASTE_ALL_REVERSE, bPasteAllReverseOrder )
  else IF_SETTING_CHANGED_BOOL( SETTINGS_GROUP_MULTI_CLIP_VIEWER, SETTINGS_PASTE_ALL_NEWLINE_BETWEEN, bPasteAllEOLBetweenItems )*/
}