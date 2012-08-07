

#include "ExtractionDialog.h"
#include "PluginInterface.h"
#include "NativeLang_def.h"


void CExtractionDialog::doDialog()
{
  if (!isCreated())
    create(IDD_EXTRACTING_DLG);
  //m_pListBoxLog = GetDlgItem(_hSelf, IDC_LIST_LOG_EXTRACT);
  goToCenter();
  RECT rc;
  
  ::GetWindowRect(_hSelf,&rc);
  int width = rc.right - rc.left;
  int height = rc.bottom - rc.top;
  //PUSHBUTTON      "Cancel",IDCANCEL,259,164,50,14
  m_pOkButton = CreateWindow( TEXT("button"), NULL,
    WS_CHILD | WS_VISIBLE |  ES_LEFT ,
    height/2, width/2, width/2, 20, getHSelf(), 0, getHinst(), NULL );
  if (!m_pOkButton)
    assert(0);
  ::SetWindowText(m_pOkButton, L"Cancel");
}

void CExtractionDialog::addLog(CStdString text)
{
  BOOL res = ::SetWindowText( GetDlgItem(_hSelf, IDC_LIST_LOG_EXTRACT), text.c_str() );
}
BOOL CALLBACK CExtractionDialog::run_dlgProc( HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam )
{
  switch (Message)
  {
    case WM_INITDIALOG:
    {
      NLChangeDialog( _hInst, _nppData._nppHandle, _hSelf, TEXT("Extracting") );
      return TRUE;
    }
    case WM_COMMAND:
    {
      switch (HIWORD(wParam))
      {
        case BN_CLICKED:
          if ((HWND)lParam == m_pOkButton)
          {
            display(false);
          }
          
          return TRUE;

        default :
          break;
      }
    }
  }
  return FALSE;
}

