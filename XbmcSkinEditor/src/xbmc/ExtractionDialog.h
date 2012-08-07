
#ifndef EXTRACTION_DIALOG_H
#define EXTRACTION_DIALOG_H

#include "PluginInterface.h"
#include "StaticDialog.h"
#include "resource.h"
#include "lib/StdString.h"

class CExtractionDialog : public StaticDialog
{

public:
  CExtractionDialog() : StaticDialog() {};

  void Init( HINSTANCE hInst, NppData nppData )
  {
    _nppData = nppData;
    Window::init(hInst, nppData._nppHandle);
  };

  void doDialog();
  void addLog(CStdString text);
  virtual void destroy() {};


protected :
  virtual BOOL CALLBACK run_dlgProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );

private:
  // Handles
  NppData      _nppData;
  HWND      _HSource;
  HWND m_pListBoxLog;
  HWND m_pOkButton;
};

#endif