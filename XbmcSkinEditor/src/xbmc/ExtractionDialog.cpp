

#include "ExtractionDialog.h"
#include "PluginInterface.h"
#include "NativeLang_def.h"


void CExtractionDialog::doDialog()
{
  if (!isCreated())
    create(IDD_EXTRACTING_DLG);
  m_pListBoxLog = GetDlgItem(_hSelf, IDC_LIST_LOG_EXTRACT);
  goToCenter();
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
      switch (wParam)
      {
        case IDCANCEL:
          display(FALSE);
          return TRUE;

        default :
          break;
      }
    }
  }
  return FALSE;
}

