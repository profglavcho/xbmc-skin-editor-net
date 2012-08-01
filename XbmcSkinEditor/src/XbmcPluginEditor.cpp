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

// include files
#ifndef UNITY_BUILD_SINGLE_INCLUDE
#include "stdafx.h"

#include "XbmcPluginEditor.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRA_LEAN



#endif


BEGIN_MESSAGE_MAP(CXbmcPluginEditor, CWinApp)
END_MESSAGE_MAP()

// information for notepad++


CONST TCHAR  PLUGIN_NAME[] = TEXT("&XbmcSkinPlugin");
LoonySettingsManager g_SettingsManager( TEXT("XbmcSkinPluginSettings") );
CONST TCHAR configFileName[] = TEXT( "\\XbmcSkinPlugin.xml" );
NppData        g_NppData;
WNDPROC        g_NppWndProc;
SciSubClassWrp    g_ScintillaMain, g_ScintillaSecond;
HINSTANCE g_hInstance;
TCHAR configPath[MAX_PATH];
MultiClipboardProxy  g_ClipboardProxy;
HWND        g_HSource;
MultiClipboardSettingsDialog OptionsDlg;
CStdString g_configDir;

// menu functions
void ToggleView();
void ToggleXbmcView();
void ToggleXbmcImage();
void ToggleXbmcControls();
void ShowClipPasteMenu();
void ShowAboutDlg();
void ShowOptionsDlg();

CXbmcPluginEditor::CXbmcPluginEditor()
  :
nbFunc(NUMBER_OF_FUNCTIONS)
{
  currentSCI = MAIN_VIEW;
  g_hInstance = NULL;
  
}

CXbmcPluginEditor theApp;

FuncItem      funcItem[NUMBER_OF_FUNCTIONS];

BOOL CXbmcPluginEditor::InitInstance()
{
  CWinApp::InitInstance();
  
  // Set function pointers
  modulestate = AfxGetModuleState();
  g_hInstance = m_hInstance;
  return TRUE;
}

int CXbmcPluginEditor::ExitInstance()
{
  delete funcItem[0]._pShKey;
  //delete funcItem[1]._pShKey;
  delete funcItem[2]._pShKey;
  delete funcItem[3]._pShKey;
  delete funcItem[4]._pShKey;

  // save settings
  SaveSettings();

  ShutDownPlugin();
  return 0;
}// return app exit code

extern "C" __declspec(dllexport) void setInfo(NppData notpadPlusData)
{ 
  // stores notepad data
  g_NppData = notpadPlusData;
  
  g_ClipboardProxy.Init();

  // Subclass the Notepad++ windows procedure
  g_NppWndProc = (WNDPROC) SetWindowLong( g_NppData._nppHandle, GWL_WNDPROC, (LONG) MCSubClassNppWndProc );
  // As well as the 2 scintilla windows'
  g_ScintillaMain.Init( g_NppData._scintillaMainHandle, MCSubClassSciWndProc );
  g_ScintillaSecond.Init( g_NppData._scintillaSecondHandle, MCSubClassSciWndProc );

  // load data of plugin
  theApp.LoadSettings();

  // initial dialogs
  theApp.AboutDlg.Init( theApp.m_hInstance, g_NppData );
  OptionsDlg.Init( theApp.m_hInstance, g_NppData._nppHandle );
  
  // Init xbmc components
  g_SettingsManager.AddSettingsObserver( &theApp.controlsList );
  theApp.controlsList.LoadControls();

  // Initialisation of MVC components
  g_SettingsManager.AddSettingsObserver( &theApp.clipboardList );

  theApp.clipboardList.LoadClipboardSession();
  
  theApp.clipXbmcImage.Init(&theApp.clipboardList, &g_ClipboardProxy, &g_SettingsManager );
  theApp.clipXbmcDialog.Init(&theApp.clipboardList, &g_ClipboardProxy, &g_SettingsManager );
  theApp.clipXbmcControls.Init(&theApp.clipboardList, &g_ClipboardProxy, &g_SettingsManager );
  theApp.OSClipboard.Init( &theApp.clipboardList, &g_ClipboardProxy, &g_SettingsManager );
  theApp.clipViewerDialog.Init( &theApp.clipboardList, &g_ClipboardProxy, &g_SettingsManager );
  theApp.clipPasteMenu.Init( &theApp.clipboardList, &g_ClipboardProxy, &g_SettingsManager );
  theApp.cyclicPaste.Init( &theApp.clipboardList, &g_ClipboardProxy, &g_SettingsManager );
  theApp.autoCopier.Init( &theApp.clipboardList, &g_ClipboardProxy, &g_SettingsManager );
}

extern "C" __declspec(dllexport) const TCHAR * getName()
{
  return PLUGIN_NAME;
}

extern "C" __declspec(dllexport) FuncItem * getFuncsArray(INT *nbF)
{
  funcItem[0]._pFunc = ToggleView;//ToggleView;
  funcItem[1]._pFunc = ToggleXbmcView;
  funcItem[2]._pFunc = ToggleXbmcImage;
  funcItem[3]._pFunc = ToggleXbmcControls;
  funcItem[4]._pFunc = ShowClipPasteMenu;
  funcItem[4]._pFunc = ShowOptionsDlg;
  funcItem[5]._pFunc = ShowAboutDlg;

  //clipXbmcImage
  // Fill menu names
  lstrcpy( funcItem[0]._itemName, TEXT("&Xbmc Skin Controls...") );
  lstrcpy( funcItem[1]._itemName, TEXT("&Xbmc Tester") );
  lstrcpy( funcItem[2]._itemName, TEXT("&Xbmc Image Previewer") );
  lstrcpy( funcItem[3]._itemName, TEXT("&Xbmc Controls") );
  lstrcpy( funcItem[4]._itemName, TEXT("&Options...") );
  lstrcpy( funcItem[5]._itemName, TEXT("&About...") );

  // Set shortcuts
  funcItem[0]._pShKey = new ShortcutKey;
  funcItem[0]._pShKey->_isAlt    = true;
  funcItem[0]._pShKey->_isCtrl  = true;
  funcItem[0]._pShKey->_isShift  = false;
  funcItem[0]._pShKey->_key    = 'V';
  /*funcItem[1]._pShKey = new ShortcutKey;
  funcItem[1]._pShKey->_isAlt    = true;
  funcItem[1]._pShKey->_isCtrl  = true;
  funcItem[1]._pShKey->_isShift  = false;
  funcItem[1]._pShKey->_key    = 'B';*/
  funcItem[2]._pShKey = new ShortcutKey;
  funcItem[2]._pShKey->_isAlt    = true;
  funcItem[2]._pShKey->_isCtrl  = true;
  funcItem[2]._pShKey->_isShift  = false;
  funcItem[2]._pShKey->_key    = 'N';
  funcItem[3]._pShKey = new ShortcutKey;
  funcItem[3]._pShKey->_isAlt    = true;
  funcItem[3]._pShKey->_isCtrl  = true;
  funcItem[3]._pShKey->_isShift  = false;
  funcItem[3]._pShKey->_key    = 'B';
  funcItem[4]._pShKey = new ShortcutKey;
  funcItem[4]._pShKey->_isAlt    = false;
  funcItem[4]._pShKey->_isCtrl  = true;
  funcItem[4]._pShKey->_isShift  = true;
  funcItem[4]._pShKey->_key    = 'V';
  funcItem[4]._pShKey = NULL;
  funcItem[5]._pShKey = NULL;
  *nbF = NUMBER_OF_FUNCTIONS;
  return funcItem;
}

/***
 *  beNotification()
 *
 *  This function is called, if a notification in Scantilla/Notepad++ occurs
 */
extern "C" __declspec(dllexport) void beNotified(SCNotification *notifyCode)
{
  if (notifyCode->nmhdr.code == NPPN_BUFFERACTIVATED)
  {
    
    theApp.clipXbmcControls.OnBufferActivated();
  }
  else if (notifyCode->nmhdr.code == SCN_UPDATEUI)
  {
    HMENU hMenu = ::GetMenu(g_NppData._nppHandle);
    UINT state = ::GetMenuState(hMenu, funcItem[DIALOG_IMAGE_PREVIEWER]._cmdID, MF_BYCOMMAND);
    if (!(state & MF_CHECKED))
      theApp.clipXbmcImage.SetHidden();
    theApp.clipXbmcImage.UpdateImage();
    
    state = ::GetMenuState(hMenu, funcItem[DIALOG_CONTROLS]._cmdID, MF_BYCOMMAND);
    if (!(state & MF_CHECKED))
      theApp.clipXbmcControls.SetHidden();
    theApp.clipXbmcControls.OnNotepadChange();
  }
  else if (notifyCode->nmhdr.hwndFrom == g_NppData._nppHandle)
  {
    // Change menu language
    NLChangeNppMenu( (HINSTANCE)theApp.m_hInstance, g_NppData._nppHandle, (LPTSTR)PLUGIN_NAME, funcItem, NUMBER_OF_FUNCTIONS );
  }
}

/***
 *  messageProc()
 *
 *  This function is called, if a notification from Notepad occurs
 */
extern "C" __declspec(dllexport) LRESULT messageProc(UINT Message, WPARAM wParam, LPARAM lParam)
{
  return TRUE;
}


#ifdef UNICODE
extern "C" __declspec(dllexport) BOOL isUnicode()
{
  return TRUE;
}
#endif //UNICODE


/***
 *  LoadSettings()
 *
 *  Load the parameters of plugin
 */
void CXbmcPluginEditor::LoadSettings(void)
{
  // initialize the config directory
  ::SendMessage( g_NppData._nppHandle, NPPM_GETPLUGINSCONFIGDIR, MAX_PATH, (LPARAM)configPath );

  // Test if config path exist
  if ( ::PathFileExists( configPath ) == FALSE )
  {
    ::CreateDirectory( configPath, NULL );
  }
  g_configDir = configPath;
  lstrcpy( SettingsFilePath, configPath );
  lstrcat( SettingsFilePath, configFileName );
  if ( ::PathFileExists(SettingsFilePath) == FALSE )
  {
    ::CloseHandle( ::CreateFile( SettingsFilePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL ) );
  }

  g_SettingsManager.LoadSettings( SettingsFilePath );
}

/***
 *  SaveSettings()
 *
 *  Saves the parameters of plugin
 */
void CXbmcPluginEditor::SaveSettings(void)
{
  g_SettingsManager.SaveSettings( SettingsFilePath );

  clipboardList.SaveClipboardSession();
}

void CXbmcPluginEditor::ShutDownPlugin()
{
  clipXbmcDialog.Shutdown();
  clipViewerDialog.Shutdown();
  g_ClipboardProxy.Destroy();
  // Shutdown COM for OLE drag drop
  OleUninitialize();
}

/**************************************************************************
 *  Interface functions
 */
void ToggleView(void)
{
  // get menu and test if dockable dialog is open
  HMENU hMenu = ::GetMenu(g_NppData._nppHandle);
  UINT state = ::GetMenuState(hMenu, funcItem[0]._cmdID, MF_BYCOMMAND);

  theApp.clipViewerDialog.ShowDialog( state & MF_CHECKED ? false : true );
}

void ToggleXbmcView(void)
{
  // get menu and test if dockable dialog is open
  HMENU hMenu = ::GetMenu(g_NppData._nppHandle);
  UINT state = ::GetMenuState(hMenu, funcItem[1]._cmdID, MF_BYCOMMAND);

  theApp.clipXbmcDialog.ShowDialog( state & MF_CHECKED ? false : true );
}

void ToggleXbmcImage(void)
{
  // Set function pointers
  

  theApp.UpdateHSCI();
  // get menu and test if dockable dialog is open
  HMENU hMenu = ::GetMenu(g_NppData._nppHandle);
  UINT state = ::GetMenuState(hMenu, funcItem[2]._cmdID, MF_BYCOMMAND);

  theApp.clipXbmcImage.ShowDialog( state & MF_CHECKED ? false : true );
}

void ToggleXbmcControls(void)
{
  theApp.UpdateHSCI();
  HMENU hMenu = ::GetMenu(g_NppData._nppHandle);
  UINT state = ::GetMenuState(hMenu, funcItem[3]._cmdID, MF_BYCOMMAND);
  theApp.clipXbmcControls.ShowDialog( state & MF_CHECKED ? false : true );
}

void ShowAboutDlg(void)
{
  theApp.AboutDlg.doDialog();
}


void ShowOptionsDlg()
{
  OptionsDlg.ShowDialog();
}


void ShowClipPasteMenu()
{
  if ( theApp.clipPasteMenu.IsUsePasteMenu() )
  {
    theApp.clipPasteMenu.ShowPasteMenu();
  }
  else
  {
    theApp.cyclicPaste.DoCyclicPaste();
  }
}

/***
 *  getCurrentHScintilla()
 *
 *  Get the handle of the current scintilla
 */
void CXbmcPluginEditor::UpdateHSCI(void)
{
  ::SendMessage(g_NppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&currentSCI);
  g_HSource = (currentSCI == MAIN_VIEW)?g_NppData._scintillaMainHandle:g_NppData._scintillaSecondHandle;
}

void CXbmcPluginEditor::ScintillaGetText(char *text, int start, int end) 
{
  TextRange tr;
  tr.chrg.cpMin = start;
  tr.chrg.cpMax = end;
  tr.lpstrText  = text;
  ScintillaMsg(SCI_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(&tr));
}

/***
 *  ScintillaMsg()
 *
 *  API-Wrapper
 */
LRESULT CXbmcPluginEditor::ScintillaMsg(UINT message, WPARAM wParam, LPARAM lParam)
{
  return ::SendMessage(g_HSource, message, wParam, lParam);
}