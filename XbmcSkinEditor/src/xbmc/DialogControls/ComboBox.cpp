#ifndef UNITY_BUILD_SINGLE_INCLUDE
#include "ComboBox.h"
#include <commctrl.h>
#endif


void CXBMCComboBox::init(HINSTANCE hInst, HWND parent)
{
  hNewFont = 0;

  Window::init( hInst, parent );
  DWORD dwComboStyle = CBS_DROPDOWNLIST | WS_VSCROLL | WS_CHILD | WS_VISIBLE;
    dwComboStyle |=  CBS_HASSTRINGS;
  _hSelf = CreateWindow( TEXT("combobox"), NULL, dwComboStyle,
                           0, 5, 188, 30, parent, 0, hInst, NULL );

  if ( !_hSelf )
  {
    return;
  }

  // subclass the listbox control
  oldWndProc = (WNDPROC)::SetWindowLong( _hSelf, GWL_WNDPROC, (LONG)StaticListboxProc );

  // associate this class instance with the listbox instance
  ::SetWindowLongPtr( _hSelf, GWL_USERDATA, (LONG)this );

  // Make items draggable
  

  hNewFont = (HFONT)::SendMessage( _hSelf, WM_GETFONT, 0, 0 );
  if ( hNewFont == NULL )
  {
    hNewFont = (HFONT)::GetStockObject( SYSTEM_FONT );
  }
  LOGFONT lf;
  ::GetObject( hNewFont, sizeof( lf ), &lf );
  lf.lfHeight = 15;
  lf.lfWidth = 0;
  lf.lfWeight = FW_NORMAL;
  lstrcpy( lf.lfFaceName, TEXT("Courier New") );
  hNewFont = ::CreateFontIndirect( &lf );
  ::SendMessage( _hSelf, WM_SETFONT, (WPARAM)hNewFont, 1 );
}


void CXBMCComboBox::destroy()
{
  ::DeleteObject( hNewFont );
}


void CXBMCComboBox::AddItem( CStdString item )
{
  lstText.push_back(item);
  TCHAR control_name[MAX_PATH];
  lstrcpy(control_name, item.c_str());
  UINT nResult = ::SendMessage( _hSelf, CB_ADDSTRING, 0, (LPARAM)control_name );
  if (nResult != CB_ERR)
    ::SendMessage(_hSelf,CB_SETITEMDATA,nResult,(LPARAM) nResult+1);
}


void CXBMCComboBox::ClearAll()
{
  lstText.clear();
  ::SendMessage( _hSelf, CB_RESETCONTENT, 0, 0 );
}


INT CXBMCComboBox::GetItemCount()
{
  return (INT)::SendMessage( _hSelf, CB_GETCOUNT, 0, 0 );
}


INT CXBMCComboBox::GetCurrentSelectionIndex()
{
  return (INT)::SendMessage( _hSelf, CB_GETCURSEL, 0, 0 );
}


CStdString CXBMCComboBox::GetCurrentSelectionText()
{
  INT index = (INT)::SendMessage( _hSelf, CB_GETCURSEL, 0, 0 );
  if (lstText.size() > index)
  {
    return lstText.at(index);
  }
  return L"";

}

void CXBMCComboBox::SetCurrentSelectedItem( INT NewSelectionIndex, BOOL bStrictSelect )
{
  if ( bStrictSelect )
  {
    ::SendMessage( _hSelf, CB_SETCURSEL, NewSelectionIndex, 0 );
  }
  else
  {
    INT ItemCount = GetItemCount();
    if ( ItemCount == CB_ERR || ItemCount <= 0)
    {
      // Error, can't get item count or no items, don't select anything
      return;
    }
    if ( ItemCount > NewSelectionIndex )
    {
      // Requested index is valid, select it
      ::SendMessage( _hSelf, CB_SETCURSEL, NewSelectionIndex, 0 );
    }
    else
    {
      // Requested index out of bounds, select the last item
      ::SendMessage( _hSelf, CB_SETCURSEL, ItemCount-1, 0 );
    }
  }
}

void CXBMCComboBox::Resize(RECT rc)
{//0, 5, 188, 30
  rc.top = 5;
  rc.bottom = 35;
    
  reSizeToWH(rc);

}


LRESULT CXBMCComboBox::runProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
  return ::CallWindowProc( oldWndProc, hwnd, message, wParam, lParam );
}