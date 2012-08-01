#pragma once

#ifndef __AFXWIN_H__
  #error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"    // main symbols

#include "PluginInterface.h"

#include "SciSubClassWrp.h"
#include "MCSubClassWndProc.h"
#include "MultiClipboardProxy.h"
#include "LoonySettingsManager.h"
#include "MultiClipboardSettingsDialog.h"
#include "AboutDialog.h"
#include "NativeLang_def.h"
//xbmc
#include "xbmc/XbmcControllerDialog.h"
#include "ControlsList.h"
#include "xbmc/XbmcImagePreviewer.h"
#include "xbmc/XbmcControlsDialog.h"
//mvc
#include "ClipboardList.h"
#include "OSClipboardController.h"
#include "MultiClipViewerDialog.h"
#include "MultiClipPasteMenu.h"
#include "MultiClipCyclicPaste.h"
#include "SelectedTextAutoCopier.h"

#define NUMBER_OF_FUNCTIONS 6
#define DIALOG_SKIN_CONTROLS   0
#define DIALOG_TESTER          1
#define DIALOG_IMAGE_PREVIEWER 2
#define DIALOG_CONTROLS        3
#define DIALOG_OPTIONS         4
#define DIALOG_ABOUT           5



class CXbmcPluginEditor : public CWinApp
{
public:
  CXbmcPluginEditor();

// Overrides
public:
  virtual BOOL InitInstance();
  virtual int ExitInstance(); // return app exit code
  DECLARE_MESSAGE_MAP()
public:
  // Function prototypes for this plugin
void ShutDownPlugin();

// load and save properties from/into settings file
void LoadSettings();
void SaveSettings();



LRESULT ScintillaMsg(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
void UpdateHSCI(void);
void ScintillaGetText(char *text, int start, int end);

  CONST INT  nbFunc;
toolbarIcons    g_TBWndMgr;
SciSubClassWrp    g_ScintillaMain, g_ScintillaSecond;

public:
// dialog classes
AboutDialog      AboutDlg;
// settings

// ini file name
TCHAR SettingsFilePath[MAX_PATH];

// MVC components for plugin
ClipboardList clipboardList;
OSClipboardController OSClipboard;
MultiClipViewerDialog clipViewerDialog;
MultiClipPasteMenu clipPasteMenu;
MultiClipCyclicPaste cyclicPaste;
SelectedTextAutoCopier autoCopier;
// Xbmc components for the plugin
ControlsList controlsList;
XbmcControllerDialog clipXbmcDialog;
XbmcImagePreviewer clipXbmcImage;
CXbmcControlsDialog clipXbmcControls;
public:
AFX_MODULE_STATE* modulestate;
UINT        currentSCI;

protected:
};

extern CXbmcPluginEditor theApp;
extern HWND  g_HSource;
extern NppData g_NppData;
extern CStdString g_configDir;