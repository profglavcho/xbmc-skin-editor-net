
#include "DuoTextBox.h"
#include <windowsx.h>
#include <commctrl.h>
#include "resource.h"

#define BOX_HEIGHT 20

CDuoTextBox::CDuoTextBox(int index)
: hSplitterBrush( 0 )
, hSplitterPen( 0 )
, m_pLabel(NULL)
, m_pTextBox(NULL)
{
  m_pIndex = index;
  m_bComboBox = false;
}

void CDuoTextBox::init(HINSTANCE hInst, HWND parent)
{
  Window::init( hInst, parent );
  WNDCLASS wndclass;
  ZeroMemory( &wndclass, sizeof(WNDCLASS) );
  wndclass.style = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc = StaticDuoTextBoxProc;
  wndclass.hInstance = _hInst;
  wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );
  wndclass.hbrBackground = (HBRUSH) CreateSolidBrush( RGB(212,208,200));
  wndclass.lpszClassName = TEXT("DuoTextBox");

  /*test window class*/
  WNDCLASS wndclass2;
  ZeroMemory( &wndclass2, sizeof(WNDCLASS) );
  if (!GetClassInfo(_hInst, TEXT("DuoTextBox"),&wndclass2))
  {
    if ( !::RegisterClass( &wndclass ) )
    {
      DWORD dwErr = GetLastError();
      // Check if class is already registered, if not then we have some other errors
      if ( ERROR_CLASS_ALREADY_EXISTS != dwErr )
      {
        TCHAR errText[512] = TEXT("");
        wsprintf( errText, TEXT("Cannot register window class %s, error code (%d)\r\nPlease remove this plugin and contact the plugin developer with this message"), TEXT("DuoTextBox"), dwErr );
        ::MessageBox( parent, errText, TEXT("Xbmc Skin Plugin error"), MB_OK );
        return;
      }
      return;
    }
  }
  RECT rc;
  GetClientRect(parent, &rc);
  int top = m_pIndex * 25;
  _hSelf = CreateWindowW( TEXT("DuoTextBox"), 0, WS_CHILD | WS_VISIBLE, 0, 45 + top, rc.right, BOX_HEIGHT , _hParent, 0, _hInst, 0 );
  m_pBottom = 45+top;
  if ( !_hSelf )
  {
    DWORD dwErr = GetLastError();
    TCHAR errText[512] = TEXT("");
    wsprintf( errText, TEXT("Cannot create window class %s, error code (%d)\r\nPlease remove this plugin and contact the plugin developer with this message"), TEXT("DuoTextBox"), dwErr );
    ::MessageBox( parent, errText, TEXT("Xbmc Skin Plugin error"), MB_OK );
  }

  // Store the instance pointer within the window user data
  ::SetWindowLongPtr( _hSelf, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this) );

  // Make splitter same colour as the dialog pane
  hSplitterBrush = ::CreateSolidBrush( RGB(212,208,200));
  hSplitterPen = ::CreatePen( PS_SOLID, 0, RGB(212,208,200));
  // create textbox and label
  //m_pLabel->init(hInst, parent);
  //m_pTextBox->init(hInst, parent);
  
  m_pLabel = CreateWindow( TEXT("edit"), NULL,
    WS_CHILD | WS_VISIBLE |  ES_LEFT ,
    rc.left, rc.top , rc.right/2, BOX_HEIGHT, getHSelf(), 0, hInst, NULL );
  if ( !m_pLabel )
  {
    DWORD dwErr = GetLastError();
    TCHAR errText[512] = TEXT("");
    wsprintf( errText, TEXT("Cannot create window class %s, error code (%d)\r\nPlease remove this plugin and contact the plugin developer with this message"), TEXT("DuoTextBox"), dwErr );
    ::MessageBox( parent, errText, TEXT("Xbmc Skin Plugin error"), MB_OK );
  }
  EnableWindow( m_pLabel, FALSE );
  
  

}


void CDuoTextBox::destroy()
{
  //::DeleteObject( hNewFont );
  BOOL res = DestroyWindow (m_pLabel);
  res = DestroyWindow (m_pTextBox);
}


void CDuoTextBox::SetValue(CStdString name, CStdString value)
{
  m_bComboBox = false;
  SetWindowText(m_pLabel, name.c_str());
  RECT rc;
  GetClientRect(getHParent(), &rc);
  //textbox
  m_pTextBox = CreateWindow( TEXT("edit"), NULL,
                             WS_CHILD | WS_VISIBLE |  ES_LEFT,
                             rc.right/2, rc.top , rc.right, BOX_HEIGHT,
                             getHSelf(), 0, getHinst(), NULL );
  
  if (!m_pTextBox)
  {
    DWORD dwErr = GetLastError();
    TCHAR errText[512] = TEXT("");
    wsprintf( errText, TEXT("Cannot create window class %s, error code (%d)\r\nPlease remove this plugin and contact the plugin developer with this message"), TEXT("DuoTextBox"), dwErr );
    ::MessageBox( getHParent(), errText, TEXT("Xbmc Skin Plugin error"), MB_OK );
  }
  SetWindowText(m_pTextBox, value.c_str());
  SendMessage( m_pTextBox, EM_SETREADONLY, (WPARAM) 0, 0 );
 
}

void CDuoTextBox::SetValue(CStdString name, std::vector<CStdString> &value, CStdString currentValue)
{
  m_bComboBox = true;
  SetWindowText(m_pLabel, name.c_str());
  RECT rc;
  GetClientRect(getHParent(), &rc);

  
  //textbox
  m_pTextBox = CreateWindow( TEXT("combobox"), NULL,
                             CBS_DROPDOWN | WS_VSCROLL | WS_CHILD | WS_VISIBLE | CBS_HASSTRINGS,
                             rc.right/2, rc.top , rc.right, 30,
                             getHSelf(), 0, getHinst(), NULL );
  
  if (!m_pTextBox)
  {
    DWORD dwErr = GetLastError();
    TCHAR errText[512] = TEXT("");
    wsprintf( errText, TEXT("Cannot create window class %s, error code (%d)\r\nPlease remove this plugin and contact the plugin developer with this message"), TEXT("combobox"), dwErr );
    ::MessageBox( getHParent(), errText, TEXT("Xbmc Skin Plugin error"), MB_OK );
  }
  TCHAR control_name[MAX_PATH];
  UINT nResult;

  for (std::vector<CStdString>::iterator it = value.begin(); it != value.end(); it++)
  {
    
    //lstrcpy(control_name, it->c_str());
    nResult = ::SendMessage( m_pTextBox, CB_ADDSTRING, 0, (LPARAM)it->c_str());
    //Why calling both?????
    /*if (nResult != CB_ERR)
      ::SendMessage(m_pTextBox, CB_SETITEMDATA, nResult, (LPARAM) nResult+1);*/

  }
  
  BOOL res = SetWindowText(m_pTextBox, currentValue.c_str());
  if (res)
    printf("yeah");
}

void CDuoTextBox::Resize(RECT rc)
{
  rc.top = GetTop();
  //rc.bottom = rc.top + m_bComboBox ? 15 : 30;
  rc.bottom = rc.top + BOX_HEIGHT;

  reSizeToWH(rc);
  //stay at 120 px for the label
  MoveWindow(m_pLabel, 0, 0, 120, BOX_HEIGHT, TRUE);
  InvalidateRect(m_pLabel, NULL, TRUE);
  UpdateWindow(m_pLabel);

  MoveWindow(m_pTextBox, 120, 0, (rc.right -rc.left) - 120, BOX_HEIGHT, TRUE);
  InvalidateRect(m_pTextBox, NULL, TRUE);
  UpdateWindow(m_pTextBox);

}


LRESULT CDuoTextBox::runProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{

  switch ( message )
  {
  case WM_MOVE:
  case WM_SIZE:
    return 0;

  case WM_PAINT:
    {
      HDC hdc;
      PAINTSTRUCT ps;
      RECT rc;
      HBRUSH hOldBrush;
      HPEN hOldPen;

      
      GetWindowRect(_hSelf, &rc);
      hdc = ::BeginPaint( _hSelf, &ps );

      hOldBrush = (HBRUSH)::SelectObject( hdc, hSplitterBrush );
      hOldPen = (HPEN)::SelectObject( hdc, hSplitterPen );
      ::Rectangle( hdc, rc.left, rc.top, rc.right, rc.bottom );

      // clean up
      ::SelectObject( hdc, hOldBrush );
      ::SelectObject( hdc, hOldPen );

      ::EndPaint( _hSelf, &ps );
      return 0;
    }

  case WM_COMMAND:
    if ( (HWND)lParam == m_pTextBox && (HIWORD(wParam) == EN_CHANGE || HIWORD(wParam) == CBN_EDITCHANGE || HIWORD(wParam) == CBN_SELCHANGE))
    {
      return ::SendMessage( _hParent, message, wParam, lParam );
    }
    break;
  case WM_NOTIFY:
    // Being a container window which doesn't care about child window control notifications,
    // forward the child window controls' notifications to the parent window for processing
    return ::SendMessage( _hParent, message, wParam, lParam );

  case WM_DESTROY:
    ::DeleteBrush( hSplitterBrush );
    ::DeletePen( hSplitterPen );
    return 0;
  }
  return ::DefWindowProc( hwnd, message, wParam, lParam );
}

LRESULT CALLBACK CDuoTextBox::StaticDuoTextBoxProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
  CDuoTextBox * pDuoTextBox = reinterpret_cast<CDuoTextBox *>( GetWindowLongPtr( hwnd, GWLP_USERDATA ) );
  if ( !pDuoTextBox )
  {
    return ::DefWindowProc( hwnd, message, wParam, lParam );
  }
  return pDuoTextBox->runProc( hwnd, message, wParam, lParam );
}