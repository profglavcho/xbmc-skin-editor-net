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

#ifndef XBMC_CONTROLS_DIALOG_H
#define XBMC_CONTROLS_DIALOG_H


#include "ModelViewController.h"
#include "DockingDlgInterface.h"
#include "DialogControls/ComboBox.h"
#include "DialogControls/DuoTextBoxContainer.h"
#include "MultiClipOLEDragDrop.h"
#include "XbmcControlsFactory.h"
#include "XbmcIncludesFactory.h"

// ID for dockable window
#define  MULTICLIPBOARD_DOCKABLE_WINDOW_INDEX 0


class CXbmcControlsDialog : public DockingDlgInterface, public IController
{
public:
  CXbmcControlsDialog();
  ~CXbmcControlsDialog();
  // Not inherited from DockingDlgInterface
  virtual void Init( IModel * pNewModel, MultiClipboardProxy * pClipboardProxy, LoonySettingsManager * pSettings );
  virtual void Shutdown();
  // Inherited from DockingDlgInterface
  virtual void destroy() {}

  void ShowDialog( bool Show = TRUE );
  void SetCurrentControl(CStdString control);

  virtual void OnModelModified();
  void OnNotepadChange();
  void OnBufferActivated();
  void ResetCurrentControl() { m_pStrCurrent = L""; }
  void SetHidden() { IsShown = false; }
  bool IsShowed() { return IsShown; }
  void OnFixDragAndDrop(SCNotification *notifyCode, CStdStringA text);
protected:
  CStdString m_pStrCurrentInclude;
  // Overload DockingDlgInterface's dialog procedure
  virtual BOOL CALLBACK run_dlgProc( HWND hWnd, UINT msg, WPARAM wp, LPARAM lp );

  // Initialise dialog here
  void InitialiseDialog();
  std::vector<CStdString> GetTexture();
  std::vector<CStdString> GetIncludes();
  // Toolbar commands
  void tb_cmd(UINT message);
private:
  tTbData TBData;
  bool IsShown;
  bool IsLoadingControls;
  bool AsModifiedTextBox;
  UINT DragListMessage;
  MultiClipOLEDataObject * pDataObject;
  MultiClipOLEDropSource * pDropSource;

  CStdString m_pStrCurrent;
  CStdString m_pStrCurrentControl;
  int m_pCurrentStartLinePos;
  int m_pCurrentEndLinePos;
  int m_pCurrentLine;
  CXbmcControlsFactory* m_pXbmcControlsFactory;
  CXBMCComboBox m_pComboBox;
  CDuoTextBoxContainer m_pDuoTextBox;
  void ShowXbmcControls();
  void OnListSelectionChanged();
 
  void OnEditBoxFocus();
  void OnEditBoxChange();
  BOOL OnDragListMessage( LPDRAGLISTINFO pDragListInfo );

  //settings
  bool m_bLoadIncludes;
  bool m_bLoadImages;
  virtual void OnObserverAdded( LoonySettingsManager * SettingsManager );
  virtual void OnSettingsChanged( const stringType & GroupName, const stringType & SettingName );
};


#endif