/*
This file is part of Xbmc skin editor for notepad++
Copyright (C) 2012 Ti-BEN

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


#include "stdafx.h"
#include "XbmcControlsListDialog.h"
#include "resource.h"
#include "NativeLang_def.h"
#include "MultiClipboardSettings.h"
#include "MultiClipboardSettingsDialog.h"
#include "XbmcPluginEditor.h"
#include "xbmc/lib/xbmc_communicator.h"
#include "xbmc/lib/ScintillaHelper.h"

extern HINSTANCE g_hInstance;
extern NppData g_NppData;
extern MultiClipboardSettingsDialog OptionsDlg;


CXbmcControlsListDialog::CXbmcControlsListDialog()
: DockingDlgInterface(IDD_DOCK_CONTROLS_XBMC_DLG)
, IsShown( false )
{
}

CXbmcControlsListDialog::~CXbmcControlsListDialog()
{
}

void CXbmcControlsListDialog::Init( IModel * pNewModel, MultiClipboardProxy * pClipboardProxy, LoonySettingsManager * pSettings )
{
  DockingDlgInterface::init( g_hInstance, g_NppData._nppHandle );
  IController::Init( pNewModel, pClipboardProxy, pSettings );
}

void CXbmcControlsListDialog::Shutdown()
{
}

void CXbmcControlsListDialog::ShowDialog( bool Show )
{
  if ( !isCreated() )
  {
    create( &TBData );

    // define the default docking behaviour
    if ( !NLGetText( g_hInstance, g_NppData._nppHandle, TEXT("Xbmc Controls List"), TBData.pszName, MAX_PATH) )
    {
      lstrcpy( TBData.pszName, TEXT("Xbmc Controls List") );
    }
    TBData.uMask      = DWS_DF_CONT_LEFT | DWS_ICONTAB;
    TBData.hIconTab    = (HICON)::LoadImage(_hInst, MAKEINTRESOURCE(IDI_MULTICLIPBOARD), IMAGE_ICON, 0, 0, LR_LOADMAP3DCOLORS | LR_LOADTRANSPARENT);
    TBData.pszModuleName  = getPluginFileName();
    TBData.dlgID      = DIALOG_CONTROLS;
    ::SendMessage( _hParent, NPPM_DMMREGASDCKDLG, 0, (LPARAM)&TBData );
  }

  
  display( Show );
  IsShown = Show;
  ShowXbmcControls();
}

BOOL CALLBACK CXbmcControlsListDialog::run_dlgProc( HWND hWnd, UINT msg, WPARAM wp, LPARAM lp )
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
      m_pListBox.reSizeTo(rc);
      break;
    }

  case WM_COMMAND:
    if ( (HWND)lp == m_pListBox.getHSelf() )
    {
      switch ( HIWORD(wp) )
      {
      case LBN_SELCHANGE:
        OnListSelectionChanged();
        return 0;
      }
    }
    break;

  case WM_NOTIFY:
    {
      //pass it by the the base class
      return DockingDlgInterface::run_dlgProc( _hSelf, msg, wp, lp );
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


void CXbmcControlsListDialog::InitialiseDialog()
{
  //::SendMessage( _hSelf, CB_ADDSTRING, 0, (LPARAM)item.c_str() );

  m_pListBox.init(_hInst, _hSelf);
}


void CXbmcControlsListDialog::OnModelModified()
{
  ShowXbmcControls();
}


void CXbmcControlsListDialog::ShowXbmcControls()
{
  if ( !IsShown )
    return;
  g_XbmcControlsFactory->LoadCurrentControls();
  std::vector<Attributes> currentlist = g_XbmcControlsFactory->GetCurrentControlsList();
  m_pListBox.ClearAll();
  for( std::vector<Attributes>::iterator it = currentlist.begin(); it != currentlist.end(); it++)
  {
    CStdString boxtext ="";
    if (it->find("type") != it->end())
    {
      boxtext.append(L"type=");
      boxtext.append(it->find("type")->second);
      
    }
    if (it->find("id") != it->end())
    {
      if (boxtext.size()>0)
        boxtext.append(L" ");
      boxtext.append(L"id=");
      boxtext.append(it->find("id")->second);
      
    }
    if (it->find("line") != it->end())
    {
      if (boxtext.size()>0)
        boxtext.append(L" ");
      boxtext.append(L"line=");
      boxtext.append(it->find("line")->second);
    }
    if (boxtext.size()>0)
      m_pListBox.AddItem(boxtext);
  }
}

void CXbmcControlsListDialog::OnListSelectionChanged()
{
  
  CStdStringA currenttext = m_pListBox.GetCurrentSelectionText();
  int index = currenttext.find_last_of("=");
  currenttext.Delete(0,index+1);

  int line = atoi(currenttext.c_str());
  int pos = g_Scintilla.getLineStartPosition(line-1);
  g_Scintilla.setSel(pos,pos);
}

void CXbmcControlsListDialog::OnObserverAdded( LoonySettingsManager * SettingsManager )
{
  SettingsObserver::OnObserverAdded( SettingsManager );
}


void CXbmcControlsListDialog::OnSettingsChanged( const stringType & GroupName, const stringType & SettingName )
{
  if ( GroupName != SETTINGS_GROUP_XBMC )
    return;
}