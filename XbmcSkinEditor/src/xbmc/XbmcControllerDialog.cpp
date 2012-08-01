/*
This file is part of MultiClipboard Plugin for Notepad++
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
    if ( !NLGetText( g_hInstance, g_NppData._nppHandle, TEXT("Xbmc Tester"), TBData.pszName, MAX_PATH) )
    {
      lstrcpy( TBData.pszName, TEXT("Xbmc Tester") );
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
  {
    return;
  }
  MultiClipViewerListbox.ClearAll();
  std::wstring textToAdd = L"Test window";
  if ( textToAdd.size() > 100 )
    textToAdd.resize( 100 );
  MultiClipViewerListbox.AddItem( textToAdd );
}


void XbmcControllerDialog::OnListSelectionChanged()
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
  MultiClipViewerEditBox.SetText(L"");

}


void XbmcControllerDialog::OnListDoubleClicked()
{
  TCHAR currentFile[MAX_PATH];
  ::SendMessage(g_NppData._nppHandle, NPPM_GETFILENAME , MAX_PATH, (LPARAM)currentFile);

  
  //gogogogo
  INT Index = MultiClipViewerListbox.GetCurrentSelectionIndex();
  if ( Index == LB_ERR )
    return;
  
  //MultiClipViewerListbox.AddItem( textToAdd );
  std::wstring thetext = MultiClipViewerListbox.GetCurrentSelectionText();
  
  XbmcCommunicator* com = new XbmcCommunicator();
  std::wstring window = currentFile;
  for(int i=0; i<window.size();++i)
   window[i] = tolower(window[i]);
  if (window.compare(window.size()-4,4,L".xml")==0)
  {
    com->SendCommand(m_pXbmcIp,m_pXbmcLogin,m_pXbmcPassword,m_pWgetPath, window);
  }
  /*
  MultiClipViewerEditBox.EnableEditBox();
  ClipboardList * pClipboardList = (ClipboardList *)GetModel();
  g_ClipboardProxy.PasteTextToNpp( pClipboardList->GetText( Index ) );
  g_ClipboardProxy.SetFocusToDocument();*/

  
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