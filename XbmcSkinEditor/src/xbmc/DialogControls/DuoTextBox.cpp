#ifndef UNITY_BUILD_SINGLE_INCLUDE
#include "DuoTextBox.h"
#include <windowsx.h>
#include <commctrl.h>
#include "resource.h"
#include <Richedit.h>
#endif

#define DUO_TEXTBOX_CLASS_NAME TEXT( "DuoTextBox" )

CDuoTextBox::CDuoTextBox(int index)
: hSplitterBrush( 0 )
, hSplitterPen( 0 )
, m_pLabel(NULL)
, m_pTextBox(NULL)
{
  m_pIndex = index;
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
  wndclass.lpszClassName = DUO_TEXTBOX_CLASS_NAME;

  /*test window class*/
  WNDCLASS wndclass2;
  ZeroMemory( &wndclass2, sizeof(WNDCLASS) );
  if (!GetClassInfo(_hInst, DUO_TEXTBOX_CLASS_NAME,&wndclass2))
  {
    if ( !::RegisterClass( &wndclass ) )
    {
      DWORD dwErr = GetLastError();
      // Check if class is already registered, if not then we have some other errors
      if ( ERROR_CLASS_ALREADY_EXISTS != dwErr )
      {
        TCHAR errText[512] = TEXT("");
        wsprintf( errText, TEXT("Cannot register window class %s, error code (%d)\r\nPlease remove this plugin and contact the plugin developer with this message"), DUO_TEXTBOX_CLASS_NAME, dwErr );
        ::MessageBox( parent, errText, TEXT("Xbmc Skin Plugin error"), MB_OK );
        return;
      }
      return;
    }
  }
  RECT rc;
  GetClientRect(parent, &rc);
  int top = m_pIndex * 20;
  _hSelf = CreateWindowW( DUO_TEXTBOX_CLASS_NAME, 0, WS_CHILD | WS_VISIBLE, 0, 45 + top, rc.right, 20 , _hParent, 0, _hInst, 0 );
  m_pBottom = 45+top;
  if ( !_hSelf )
  {
    DWORD dwErr = GetLastError();
    TCHAR errText[512] = TEXT("");
    wsprintf( errText, TEXT("Cannot create window class %s, error code (%d)\r\nPlease remove this plugin and contact the plugin developer with this message"), DUO_TEXTBOX_CLASS_NAME, dwErr );
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
    rc.left, rc.top , rc.right/2, 15, getHSelf(), 0, hInst, NULL );
  if ( !m_pLabel )
  {
    DWORD dwErr = GetLastError();
    TCHAR errText[512] = TEXT("");
    wsprintf( errText, TEXT("Cannot create window class %s, error code (%d)\r\nPlease remove this plugin and contact the plugin developer with this message"), DUO_TEXTBOX_CLASS_NAME, dwErr );
    ::MessageBox( parent, errText, TEXT("Xbmc Skin Plugin error"), MB_OK );
  }
  EnableWindow( m_pLabel, FALSE );
  //LoadLibrary(L"riched32.dll");
  m_pTextBox = CreateWindow( TEXT("edit"), NULL,
    WS_CHILD | WS_VISIBLE |  ES_LEFT,
    rc.right/2, rc.top , rc.right, 15, getHSelf(), 0, hInst, NULL );
  
  if (!m_pTextBox)
  {
    DWORD dwErr = GetLastError();
    TCHAR errText[512] = TEXT("");
    wsprintf( errText, TEXT("Cannot create window class %s, error code (%d)\r\nPlease remove this plugin and contact the plugin developer with this message"), DUO_TEXTBOX_CLASS_NAME, dwErr );
    ::MessageBox( parent, errText, TEXT("Xbmc Skin Plugin error"), MB_OK );
  }

}


void CDuoTextBox::destroy()
{
  //::DeleteObject( hNewFont );
  BOOL res = DestroyWindow (m_pLabel);
  res = DestroyWindow (m_pTextBox);
}


void CDuoTextBox::SetValue(CStdString name, CStdString value)
{
  SetWindowText(m_pLabel, name.c_str());
  SetWindowText(m_pTextBox, value.c_str());
  SendMessage( m_pTextBox, EM_SETREADONLY, (WPARAM) 0, 0 );
 
}

void CDuoTextBox::Resize(RECT rc)
{
  rc.top = GetTop();
  rc.bottom = rc.top+15;
  
  reSizeToWH(rc);
  //stay at 120 px for the label
  MoveWindow(m_pLabel, 0, 0, 120, 15, TRUE);
  InvalidateRect(m_pLabel, NULL, TRUE);
  UpdateWindow(m_pLabel);

  MoveWindow(m_pTextBox, 120, 0, (rc.right -rc.left) - 120, 15, TRUE);
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
    if ( (HWND)lParam == m_pTextBox && HIWORD(wParam) == EN_CHANGE)
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