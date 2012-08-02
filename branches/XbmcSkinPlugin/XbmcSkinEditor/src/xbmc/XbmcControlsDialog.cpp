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

#ifndef UNITY_BUILD_SINGLE_INCLUDE
#include "stdafx.h"
#include "XbmcControlsDialog.h"
#include "resource.h"
#include "NativeLang_def.h"
#include "MultiClipboardSettings.h"
#include "MultiClipboardSettingsDialog.h"
#include "XbmcPluginEditor.h"
#include "xbmc/lib/xbmc_communicator.h"
#include "xbmc/lib/ScintillaHelper.h"
#include "lib/SADirRead.h"
#endif


extern HINSTANCE g_hInstance;
extern NppData g_NppData;
extern MultiClipboardSettingsDialog OptionsDlg;


CXbmcControlsDialog::CXbmcControlsDialog()
: DockingDlgInterface(IDD_DOCK_CONTROLS_XBMC_DLG)
, IsShown( false )
, IsLoadingControls( false )
, AsModifiedTextBox( false )
, DragListMessage( 0 )
, pDataObject( NULL )
, pDropSource( NULL )
{
  m_pXbmcControlsFactory = new CXbmcControlsFactory();
  m_pXbmcIncludesFactory = new CXbmcIncludesFactory();
  m_pStrCurrent = L"";
  m_pStrCurrentInclude = L"";
  m_pCurrentLine = 0;
  m_pCurrentStartLinePos = -1;
  m_pCurrentEndLinePos = -1;
}


CXbmcControlsDialog::~CXbmcControlsDialog()
{
  if (m_pXbmcControlsFactory)
    delete m_pXbmcControlsFactory;
  
  if (m_pXbmcIncludesFactory)
    delete m_pXbmcIncludesFactory;
}


void CXbmcControlsDialog::Init( IModel * pNewModel, MultiClipboardProxy * pClipboardProxy, LoonySettingsManager * pSettings )
{
  DockingDlgInterface::init( g_hInstance, g_NppData._nppHandle );
  IController::Init( pNewModel, pClipboardProxy, pSettings );
  DragListMessage = ::RegisterWindowMessage( DRAGLISTMSGSTRING );

  MultiClipOLEDataObject::CreateDataObject( &pDataObject );
  MultiClipOLEDropSource::CreateDropSource( &pDropSource );
}


void CXbmcControlsDialog::Shutdown()
{
  pDataObject->Release();
  pDropSource->Release();
}


void CXbmcControlsDialog::ShowDialog( bool Show )
{
  if ( !isCreated() )
  {
    create( &TBData );

    // define the default docking behaviour
    if ( !NLGetText( g_hInstance, g_NppData._nppHandle, TEXT("Xbmc Controls"), TBData.pszName, MAX_PATH) )
    {
      lstrcpy( TBData.pszName, TEXT("Xbmc Controls") );
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

BOOL CXbmcControlsDialog::OnDragListMessage( LPDRAGLISTINFO pDragListInfo )
{

  if ( pDragListInfo->uNotification == DL_BEGINDRAG )
  {
    CStdString xmlcontrol;
    m_pDuoTextBox.GetXmlControl(xmlcontrol);
    if (xmlcontrol.size()>0)
    {
      //xmlcontrol.Insert(0,L"dragndrop");
      pDataObject->SetMultiClipDragData( xmlcontrol.c_str(), (xmlcontrol.size()+1)*2, false);
      DWORD dwEffect = 0;
      DoDragDrop( pDataObject, pDropSource, DROPEFFECT_COPY, &dwEffect );
    }
    // Store the return value in DWL_MSGRESULT. Set to false so we don't receive the rest of the drag messages
    // http://support.microsoft.com/kb/183115
    SetWindowLong( getHSelf(), DWL_MSGRESULT, FALSE );
  }
  return TRUE;
}

void CXbmcControlsDialog::OnFixDragAndDrop(SCNotification *notifyCode, CStdStringA text)
{
  //g_Scintilla.deleteRange(notifyCode->position,notifyCode->position+ notifyCode->length);
  g_Scintilla.setSelectionStart(notifyCode->position);
  g_Scintilla.setSelectionEnd(notifyCode->position+ notifyCode->length);
  //g_Scintilla.setSelText(text.c_str());
  //g_Scintilla.insertText(notifyCode->position, text);
  //int lineinserted = g_Scintilla.getLineFromPosition(notifyCode->position);


}

BOOL CALLBACK CXbmcControlsDialog::run_dlgProc( HWND hWnd, UINT msg, WPARAM wp, LPARAM lp )
{
  if ( msg == DragListMessage )
  {
    return OnDragListMessage( (LPDRAGLISTINFO)lp );
  }

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
      m_pComboBox.Resize(rc);
      m_pDuoTextBox.Resize(rc);
      break;
    }

  case WM_COMMAND:
    if ( (HWND)lp == m_pComboBox.getHSelf() )
    {
      switch ( HIWORD(wp) )
      {
      case CBN_SELENDOK:
        OnListSelectionChanged();
        return 0;
      }
    }
    else if ( (HWND)lp == m_pDuoTextBox.getHSelf())
    {
      switch ( HIWORD(wp) )
      {
      case CBN_EDITCHANGE:
      case CBN_SELCHANGE:
      case EN_CHANGE:
        OnEditBoxChange();
        return 0;
      
      }
    }
    break;

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


void CXbmcControlsDialog::InitialiseDialog()
{
  //::SendMessage( _hSelf, CB_ADDSTRING, 0, (LPARAM)item.c_str() );

  m_pComboBox.init(_hInst, _hSelf);
  m_pDuoTextBox.init(_hInst, _hSelf);
}


void CXbmcControlsDialog::OnModelModified()
{
  
  ShowXbmcControls();
}


void CXbmcControlsDialog::ShowXbmcControls()
{
  if ( !IsShown )
    return;
  
    std::vector<CStdString> thelist = m_pXbmcControlsFactory->GetControlsList();
  m_pComboBox.ClearAll();
  for( std::vector<CStdString>::iterator it = thelist.begin(); it != thelist.end(); it++)
  {
    m_pComboBox.AddItem(*it);
  }
}

std::vector<CStdString> CXbmcControlsDialog::GetTexture()
{
  std::vector<CStdString> returnvec;
  
  returnvec = m_pXbmcIncludesFactory->m_pTextures;
  
  
  if (returnvec.size() == 0)
  {
    m_pXbmcIncludesFactory->LoadTextures(L"");
    returnvec = m_pXbmcIncludesFactory->m_pTextures;
  }
  //returnvec = m_pXbmcIncludesFactory->GetTextures();
  return returnvec;
}

std::vector<CStdString> CXbmcControlsDialog::GetIncludes()
{
  std::vector<CStdString> returnvec;
  returnvec = m_pXbmcIncludesFactory->m_pIncludedNames;//GetIncludes();
  return returnvec;
  
}

void CXbmcControlsDialog::OnListSelectionChanged()
{
  if (!m_pStrCurrent.Equals(m_pComboBox.GetCurrentSelectionText()))
    m_pStrCurrent = m_pComboBox.GetCurrentSelectionText();
  else
    return;
  IsLoadingControls = true;
  m_pDuoTextBox.ClearAttributes();
  DestroyWindow(m_pDuoTextBox.getHSelf());
  m_pDuoTextBox.init(_hInst, _hSelf);
  Attributes attrib = m_pXbmcControlsFactory->GetAttributes(m_pStrCurrent);
  for (AttributesIt it = attrib.begin(); it != attrib.end(); it++)
  {
    if (it->first.Find(L"texture") != -1 && m_bLoadImages)
    {
      std::vector<CStdString> text = GetTexture();
      m_pDuoTextBox.AddAttribute((*it).first, text, it->second);//(*it).second);
    }
    else if (it->first.Find(L"include") != -1 && m_bLoadIncludes)
    {
      std::vector<CStdString> text = GetIncludes();
      m_pDuoTextBox.AddAttribute((*it).first, text, it->second);//(*it).second);
    }
    else
      m_pDuoTextBox.AddAttribute((*it).first, (*it).second);

  }
  //update the size of the container it might have changed
  SendMessage(getHSelf(), WM_SIZE, 0, 0);
  IsLoadingControls = false;
  
}

void CXbmcControlsDialog::OnBufferActivated()
{
  if (!m_bLoadIncludes)
    return;
  TCHAR dirpath[ MAX_PATH];
  SendMessage(g_NppData._nppHandle, NPPM_GETCURRENTDIRECTORY , MAX_PATH,(LPARAM)dirpath);
  CStdString include;
  include = dirpath;
  if (include.size() == 0)
    return;
  if (!m_pStrCurrentInclude.Equals(include))
    m_pStrCurrentInclude = include;
  else
    return;
  include.insert(include.size(),L"\\includes.xml");
  m_pXbmcIncludesFactory->LoadIncludes(include);
}

void CXbmcControlsDialog::OnNotepadChange()
{
  if ( !IsShown )
    return;
  if (AsModifiedTextBox)
  {
    AsModifiedTextBox = false;
    return;
  }
  int cur = g_Scintilla.getCurrentLine();
  if (cur != m_pCurrentLine)
    m_pCurrentLine = cur;
  else
    return;
  CStdString pStrCurrentLine;
  int control_start_line = -1;
  int control_end_line = -1;
  //get the start line of the control
  for (int i = cur; i > -1; i--)
  {
    pStrCurrentLine = g_Scintilla.getTextLine(i);
    //if we find a control end before the start of one we dont keep seeking for the start of this one since we are not in one actually
    if (pStrCurrentLine.ToLower().Find(L"</control>",0)> -1 || pStrCurrentLine.ToLower().Find(L"/>",0)> -1)
      break;
    //see if we have control and type= in the same line only getting control type would fuck when the its written control id="" type=""
    if (pStrCurrentLine.ToLower().Find(L"control",0)>-1 && pStrCurrentLine.ToLower().Find(L"type=",0)>-1)
    {
      control_start_line = i;
      break;
    }
  }
  if (control_start_line == -1)
  {
    m_pCurrentStartLinePos = -1;
    m_pCurrentEndLinePos = -1;
    return;
  }
  //get the end line of the control
  for (int i = control_start_line; i < g_Scintilla.getLineCount(); i++)
  {
    pStrCurrentLine = g_Scintilla.getTextLine(i);
    if (pStrCurrentLine.ToLower().Find(L"</control>",0)>-1|| pStrCurrentLine.ToLower().Find(L"/>",0)> -1)
    {
      control_end_line = i;
      break;
    }
  }
  if (control_end_line == -1)
  {
    m_pCurrentStartLinePos = -1;
    m_pCurrentEndLinePos = -1;
    return;
  }

  m_pCurrentStartLinePos = control_start_line;
  m_pCurrentEndLinePos = control_end_line;

  char* strControlText = (char*) new CHAR[ g_Scintilla.getLineEndPosition(control_end_line) - g_Scintilla.getLineStartPosition(control_start_line)+1];
  
  g_Scintilla.getTextRange(g_Scintilla.getLineStartPosition(control_start_line), g_Scintilla.getLineEndPosition(control_end_line), strControlText);
  m_pStrCurrentControl = strControlText;
  //check if we have 2 control end on last line
  if (m_pStrCurrentControl.Find(L"/>",0) > -1 && m_pStrCurrentControl.Find(L"</control>",0) > -1)
  {
    int first = m_pStrCurrentControl.Find(L"/>",0);
    int second = m_pStrCurrentControl.Find(L"</control>",0);
    if (first < second)
    {
      m_pStrCurrentControl.Delete(first,m_pStrCurrentControl.size()-first-2);
    }
    else
    {
      m_pStrCurrentControl.Delete(second,m_pStrCurrentControl.size()-second-10);
    }

  }

  
  m_pXbmcControlsFactory->LoadCurrentControl(m_pStrCurrentControl);

}

void CXbmcControlsDialog::SetCurrentControl(CStdString control)
{
  int control_index = SendMessage(m_pComboBox.getHSelf(), CB_FINDSTRING, -1, (LPARAM)control.c_str());
  if (control_index != CB_ERR)
  {
    SendMessage(m_pComboBox.getHSelf(), CB_SETCURSEL, control_index, NULL);
    OnListSelectionChanged();
    
  }
}

void CXbmcControlsDialog::OnEditBoxFocus()
{
  CStdString pLabel;
  m_pDuoTextBox.GetLabel(pLabel);
  if (pLabel.size()>3)
    pLabel.Insert(0,L"<");
  else
    return;
    
  
  int foundattribute = m_pStrCurrentControl.Find(pLabel.c_str());
  int position_to_edit = 0;
  if (foundattribute > -1)
  {
    position_to_edit = g_Scintilla.getLineFromPosition(m_pCurrentStartLinePos + foundattribute);
    CStdString pStrLineToModify; 
    pStrLineToModify = g_Scintilla.getTextLine(position_to_edit);
    int b_erase = pStrLineToModify.find_first_of(L">");
    int e_erase = pStrLineToModify.find_last_of(L"<");
    pStrLineToModify = pStrLineToModify.erase(e_erase-b_erase,b_erase);

    printf("yah");
    //found the line pos where is the attribute
  }
  
}

void CXbmcControlsDialog::OnEditBoxChange()
{
  if (IsLoadingControls)
    return;
  CStdString pTextbox;
  CStdString pLabel;

  m_pDuoTextBox.GetLabel(pLabel);
  bool iscombobox = m_pDuoTextBox.GetTextBox(pTextbox);
  if (iscombobox)
  {
    theApp.clipXbmcImage.ShowImage(pTextbox);
  }
  if (m_pCurrentStartLinePos == -1)
    return;
  if (m_pCurrentEndLinePos == -1)
    return;
  // Get length of text in edit box
  bool insertnewline = false;
  if (pLabel.size()>0)
  {
    m_pXbmcControlsFactory->SetAttribute(pLabel,pTextbox);
    newLineString returned_line = m_pXbmcControlsFactory->GetLineModification(m_pStrCurrentControl, pLabel, pTextbox);
    
    int space = g_Scintilla.getLineIndentation(m_pCurrentEndLinePos-1);
    //insert the new line
    if (returned_line.newline)
    {
      int pos_in_text = g_Scintilla.insertLine(m_pCurrentEndLinePos-1);
      m_pCurrentEndLinePos = m_pCurrentEndLinePos+1;
    }
    
    int linetomodif = m_pCurrentStartLinePos + returned_line.index;
    //select line to modif
    g_Scintilla.setSelectionStart(g_Scintilla.getLineStartPosition(linetomodif));
    g_Scintilla.setSelectionEnd(g_Scintilla.getLineEndPosition(linetomodif));
    
    

    CStdStringA nl;
    for (int i = 0; i < space; i++)
      nl.append(" ");
    nl.append(g_Scintilla.W_to_A(returned_line.strNewLine).c_str());
    AsModifiedTextBox = true;

    g_Scintilla.setSelText(nl.c_str());

    char* strControlText = (char*) new CHAR[ g_Scintilla.getLineEndPosition(m_pCurrentEndLinePos) - g_Scintilla.getLineStartPosition(m_pCurrentStartLinePos)+1];
  
    g_Scintilla.getTextRange(g_Scintilla.getLineStartPosition(m_pCurrentStartLinePos), g_Scintilla.getLineEndPosition(m_pCurrentEndLinePos), strControlText);
    m_pStrCurrentControl = strControlText;
    //check if we have 2 control end on last line
    if (m_pStrCurrentControl.Find(L"/>",0) > -1 && m_pStrCurrentControl.Find(L"</control>",0) > -1)
    {
      int first = m_pStrCurrentControl.Find(L"/>",0);
      int second = m_pStrCurrentControl.Find(L"</control>",0);
      if (first < second)
      {
        m_pStrCurrentControl.Delete(first,m_pStrCurrentControl.size()-first-2);
      }
      else
      {
        m_pStrCurrentControl.Delete(second,m_pStrCurrentControl.size()-second-10);
      }

    }
    //m_pCurrentLine = linetomodif-1;
    //Reload the strcurrentcontrol
    //OnNotepadChange();

    

  }
  
}


void CXbmcControlsDialog::OnObserverAdded( LoonySettingsManager * SettingsManager )
{
  SettingsObserver::OnObserverAdded( SettingsManager );
  
  // Add default settings if it doesn't exists
  SET_SETTINGS_BOOL( SETTINGS_GROUP_XBMC, SETTINGS_LOAD_INCLUDES, m_bLoadIncludes )
  SET_SETTINGS_BOOL( SETTINGS_GROUP_XBMC, SETTINGS_LOAD_IMAGES, m_bLoadImages )
    
}


void CXbmcControlsDialog::OnSettingsChanged( const stringType & GroupName, const stringType & SettingName )
{
  if ( GroupName != SETTINGS_GROUP_XBMC )
    return;

  IF_SETTING_CHANGED_BOOL( SETTINGS_GROUP_XBMC, SETTINGS_LOAD_INCLUDES, m_bLoadIncludes )
  else IF_SETTING_CHANGED_BOOL( SETTINGS_GROUP_XBMC, SETTINGS_LOAD_IMAGES, m_bLoadImages )
}