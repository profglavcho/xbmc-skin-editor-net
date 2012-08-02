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
#include "MultiClipboardSettingsDialog.h"
#include "LoonySettingsManager.h"
#include "MultiClipboardSettings.h"
#include "PluginInterface.h"
#include "NativeLang_def.h"
#include "resource.h"
#include <windowsx.h>
#include <sstream>
#include <vector>
#include <ShlObj.h>//shgetmalloc
#endif

extern HINSTANCE g_hInstance;
extern LoonySettingsManager g_SettingsManager;
extern NppData        g_NppData;
#define NM_MOUSE_OVER_CONTROL 1000

static int __stdcall BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM, LPARAM pData)
{
  if (uMsg == BFFM_INITIALIZED)
    ::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, pData);
  return 0;
};

// This is the subclassed wnd proc for the children control of the settings dialog.
// It is used to trap the mouse move before the message is being sent to the child controls
LRESULT CALLBACK MCBSettingsChildCtrlDlgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  switch ( msg )
  {
  case WM_NCHITTEST:
    HWND hDlgParent = ::GetParent( hwnd );
    int OwnID = ::GetDlgCtrlID( hwnd );

    ::SendMessage( hDlgParent, WM_COMMAND, MAKEWPARAM( OwnID, NM_MOUSE_OVER_CONTROL ), (LPARAM)hwnd );
    break;
  }
  WNDPROC OwnWndProc = reinterpret_cast<WNDPROC>( GetWindowLongPtr( hwnd, GWLP_USERDATA ) );
  return ::CallWindowProc( OwnWndProc, hwnd, msg, wParam, lParam );
}


LRESULT CALLBACK StaticTextChildCtrlDlgDlgProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
  switch ( msg )
  {
  case WM_NCHITTEST:
    HWND hDlgParent = ::GetParent( hwnd );
    int OwnID = ::GetDlgCtrlID( hwnd );

    ::SendMessage( hDlgParent, WM_COMMAND, MAKEWPARAM( OwnID, NM_MOUSE_OVER_CONTROL ), (LPARAM)hwnd );
    // Static text needs to return this or Windows will return HTTRANSPARENT and pass this message away
    return HTCLIENT;
  }
  WNDPROC OwnWndProc = reinterpret_cast<WNDPROC>( GetWindowLongPtr( hwnd, GWLP_USERDATA ) );
  return ::CallWindowProc( OwnWndProc, hwnd, msg, wParam, lParam );
}


void MultiClipboardSettingsDialog::Init( HINSTANCE hInst, HWND hNpp )
{
  Window::init( hInst, hNpp );
  CurrentMouseOverID = 0;
}


void MultiClipboardSettingsDialog::ShowDialog( bool Show )
{
  if ( !isCreated() )
  {
    create( IDD_OPTIONS_DLG );
    LoadSettingsControlMap();
    SubclassAllChildControls();
  }
  if ( Show )
  {
    LoadMultiClipboardSettings();
  }
  display( Show );

  goToCenter();
}

void MultiClipboardSettingsDialog::ShowFolderBrowser()
{
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
    info.lpszTitle      = _T("Select a folder:");
    info.ulFlags      = BIF_RETURNONLYFSDIRS;
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
        // Set edit control to the directory path.
        ::SetWindowText(::GetDlgItem(_hSelf, IDC_EDIT_PATH_WGET), _pLink);
      }
      pShellMalloc->Free(pidl);
    }
    pShellMalloc->Release();
    delete [] info.pszDisplayName;
  }
}
BOOL CALLBACK MultiClipboardSettingsDialog::run_dlgProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
  switch ( message )
  {
  case WM_INITDIALOG:
    // Change language
    NLChangeDialog( _hInst, g_NppData._nppHandle, _hSelf, TEXT("Options") );
    return TRUE;

  case WM_COMMAND:
    if ( ( HIWORD(wParam) == NM_MOUSE_OVER_CONTROL ) && ( LOWORD(wParam) != 0 ) )
    {
      if ( CurrentMouseOverID != LOWORD(wParam) )
      {
        CurrentMouseOverID = LOWORD(wParam);
        DisplayMouseOverIDHelp( CurrentMouseOverID );
      }
      break;
    }
      
    switch ( wParam )
    {
    case IDOK:
      SaveMultiClipboardSettings();
      // fall through
    case IDCANCEL:
      display(FALSE);
      return TRUE;
    case IDC_OPEN_FOLDER:
      ShowFolderBrowser();

    default :
      break;
    }


  case WM_MOUSEMOVE:
    if ( CurrentMouseOverID != 0 )
    {
      CurrentMouseOverID = 0;
      DisplayMouseOverIDHelp( CurrentMouseOverID );
    }
    break;
  }
  return FALSE;
}


void MultiClipboardSettingsDialog::SetIntValueToDialog( const std::wstring & GroupName, const std::wstring & SettingName, const int DlgItemID )
{
  int intValue = g_SettingsManager.GetIntSetting( GroupName, SettingName );
  ::SetDlgItemInt( _hSelf, DlgItemID, intValue, FALSE );
}

void MultiClipboardSettingsDialog::SetBoolValueToDialog( const std::wstring & GroupName, const std::wstring & SettingName, const int DlgItemID )
{
  bool boolValue = g_SettingsManager.GetBoolSetting( GroupName, SettingName );
  ::CheckDlgButton( _hSelf, DlgItemID, boolValue ? BST_CHECKED : BST_UNCHECKED );
}

void MultiClipboardSettingsDialog::SetStringValueToDialog( const std::wstring & GroupName, const std::wstring & SettingName, const int DlgItemID )
{
  
  std::wstring strValue = g_SettingsManager.GetStringSetting( GroupName, SettingName );
  SetDlgItemTextW(_hSelf, DlgItemID, strValue.c_str() );
  //::SetWindowText(::GetDlgItem(_hSelf, DlgItemID), strValue.c_str());
  //::SetDlgItemInt( _hSelf, DlgItemID, intValue, FALSE );
}

void MultiClipboardSettingsDialog::GetIntValueFromDialog( const std::wstring & GroupName, const std::wstring & SettingName, const int DlgItemID )
{
  int intValue = ::GetDlgItemInt( _hSelf, DlgItemID, NULL, FALSE );
  g_SettingsManager.SetIntSetting( GroupName, SettingName, intValue );
}


void MultiClipboardSettingsDialog::GetBoolValueFromDialog( const std::wstring & GroupName, const std::wstring & SettingName, const int DlgItemID )
{
  bool boolValue = BST_CHECKED == ::IsDlgButtonChecked( _hSelf, DlgItemID );
  g_SettingsManager.SetBoolSetting( GroupName, SettingName, boolValue );
}

void MultiClipboardSettingsDialog::GetStringValueFromDialog( const std::wstring & GroupName, const std::wstring & SettingName, const int DlgItemID )
{
  LPTSTR value = (LPTSTR)new TCHAR[MAX_PATH];
  UINT result = ::GetDlgItemTextW( _hSelf, DlgItemID, value, MAX_PATH );
  if (result>0)
    g_SettingsManager.SetStringSetting( GroupName, SettingName, value );
}

void MultiClipboardSettingsDialog::LoadMultiClipboardSettings()
{
  for ( unsigned int i = 0; i < SettingsControlMap.size(); ++i )
  {
    switch ( SettingsControlMap[i].SettingType )
    {
    case SCTE_BOOL:
      SetBoolValueToDialog( SettingsControlMap[i].GroupName, SettingsControlMap[i].SettingName, SettingsControlMap[i].ControlID );
      break;

    case SCTE_INT:
      SetIntValueToDialog( SettingsControlMap[i].GroupName, SettingsControlMap[i].SettingName, SettingsControlMap[i].ControlID );
      break;

    case SCTE_STRING:
      SetStringValueToDialog( SettingsControlMap[i].GroupName, SettingsControlMap[i].SettingName, SettingsControlMap[i].ControlID );
      break;
    default:
      break;;
    }
  }
}


void MultiClipboardSettingsDialog::SaveMultiClipboardSettings()
{
  for ( unsigned int i = 0; i < SettingsControlMap.size(); ++i )
  {
    switch ( SettingsControlMap[i].SettingType )
    {
    case SCTE_BOOL:
      GetBoolValueFromDialog( SettingsControlMap[i].GroupName, SettingsControlMap[i].SettingName, SettingsControlMap[i].ControlID );
      break;

    case SCTE_INT:
      GetIntValueFromDialog( SettingsControlMap[i].GroupName, SettingsControlMap[i].SettingName, SettingsControlMap[i].ControlID );
      break;
    case SCTE_STRING:
      GetStringValueFromDialog( SettingsControlMap[i].GroupName, SettingsControlMap[i].SettingName, SettingsControlMap[i].ControlID );
      break;
    default:
      break;;
    }
  }
}


void MultiClipboardSettingsDialog::DisplayMouseOverIDHelp( int ControlID )
{
  if ( ControlID == 0 )
  {
    ::SetDlgItemText( _hSelf, IDC_OPTION_EXPLANATION, TEXT("") );
    return;
  }

  std::wostringstream HelpNativeLangIndex;
  HelpNativeLangIndex << ControlID << TEXT("_HELP");
  std::vector< TCHAR > HelpText(512);
  int len = NLGetText( g_hInstance, g_NppData._nppHandle, HelpNativeLangIndex.str().c_str(), &HelpText[0], HelpText.capacity() );
  if ( len == 0 )
  {
    ::SetWindowText( ::GetDlgItem( _hSelf, IDC_OPTION_EXPLANATION ), GetControlHelpText( ControlID ) );
  }
  else
  {
    ::SetWindowText( ::GetDlgItem( _hSelf, IDC_OPTION_EXPLANATION ), &HelpText[0] );
  }
}


void MultiClipboardSettingsDialog::SubclassChildControl( const int ControlID )
{
  HWND hChild = GetDlgItem( _hSelf, ControlID );
  WNDPROC ChildWndProc = (WNDPROC) SetWindowLong( hChild, GWL_WNDPROC, (LONG) MCBSettingsChildCtrlDlgProc );
  ::SetWindowLongPtr( hChild, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(ChildWndProc) );
}


void MultiClipboardSettingsDialog::SubclassStaticTextChildControl( const int ControlID )
{
  HWND hChild = GetDlgItem( _hSelf, ControlID );
  WNDPROC ChildWndProc = (WNDPROC) SetWindowLong( hChild, GWL_WNDPROC, (LONG) StaticTextChildCtrlDlgDlgProc );
  ::SetWindowLongPtr( hChild, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(ChildWndProc) );
}


void MultiClipboardSettingsDialog::SubclassAllChildControls()
{
  for ( unsigned int i = 0; i < SettingsControlMap.size(); ++i )
  {
    SubclassChildControl( SettingsControlMap[i].ControlID );
    if ( SettingsControlMap[i].ControlStaticTextID > 0 )
    {
      SubclassStaticTextChildControl( SettingsControlMap[i].ControlStaticTextID );
    }
  }
}


void MultiClipboardSettingsDialog::GetSettingsGroupAndName( const int Control, std::wstring & GroupName, std::wstring & SettingName )
{
  for ( unsigned int i = 0; i < SettingsControlMap.size(); ++i )
  {
    if ( Control == SettingsControlMap[i].ControlID ||
       Control == SettingsControlMap[i].ControlStaticTextID )
    {
      GroupName = SettingsControlMap[i].GroupName;
      SettingName = SettingsControlMap[i].SettingName;
    }
  }
}


LPCTSTR MultiClipboardSettingsDialog::GetControlHelpText( int ControlID )
{
  for ( unsigned int i = 0; i < SettingsControlMap.size(); ++i )
  {
    if ( ControlID == SettingsControlMap[i].ControlID ||
       ControlID == SettingsControlMap[i].ControlStaticTextID )
    {
      return SettingsControlMap[i].SettingHelp.c_str();
    }
  }
  return TEXT("");
}


// All settings to be defined here, and the rest of the functions will take care of the rest
void MultiClipboardSettingsDialog::LoadSettingsControlMap()
{
  SettingsControlMap.push_back( SettingsControlMapStruct(
    IDC_EDIT_XBMC_IP, SCTE_STRING,
    SETTINGS_GROUP_XBMC, SETTINGS_XBMC_IP,
    TEXT("Ip of xbmc for the test dialog") ) );

  SettingsControlMap.push_back( SettingsControlMapStruct(
    IDC_EDIT_PATH_WGET, SCTE_STRING,
    SETTINGS_GROUP_XBMC,SETTINGS_WGET_PATH,
    TEXT("Path of wget for http query to xbmc") ) );

  SettingsControlMap.push_back( SettingsControlMapStruct(
    IDC_EDIT_LOGIN, SCTE_STRING,
    SETTINGS_GROUP_XBMC,SETTINGS_XBMC_LOGIN,
    TEXT("Login") ) );

  SettingsControlMap.push_back( SettingsControlMapStruct(
    IDC_EDIT_PASSWORD, SCTE_STRING,
    SETTINGS_GROUP_XBMC,SETTINGS_XBMC_PASS,
    TEXT("Password") ) );

  SettingsControlMap.push_back( SettingsControlMapStruct(
    IDC_CHECK_LOAD_INCLUDES, SCTE_BOOL,
    SETTINGS_GROUP_XBMC,SETTINGS_LOAD_INCLUDES,
    TEXT("Load includes") ) );

  SettingsControlMap.push_back( SettingsControlMapStruct(
    IDC_CHECK_LOAD_IMAGES, SCTE_BOOL,
    SETTINGS_GROUP_XBMC,SETTINGS_LOAD_IMAGES,
    TEXT("Load images") ) );
  /*SettingsControlMap.push_back( SettingsControlMapStruct(
    IDC_CHECK_COPY_FROM_OTHER_PROGRAMS, SCTE_BOOL,
    SETTINGS_GROUP_OSCLIPBOARD, SETTINGS_COPY_FROM_OTHER_PROGRAMS,
    TEXT("Get text that are copied from other programs") ) );*/

}