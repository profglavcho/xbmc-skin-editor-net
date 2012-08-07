/*
This file is part of Xbmc skin editor for notepad++
Copyright (C) 2009 LoonyChewy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/


#include "DuoTextBoxContainer.h"
#include <windowsx.h>
#include <commctrl.h>
#include "resource.h"
#include <Richedit.h>
#include "../XbmcControlsFactory.h"

#define DUO_TEXTBOX_CONTAINER_CLASS_NAME TEXT( "DuoTextBoxContainer" )
#define VERT_PTS 5

CDuoTextBoxContainer::CDuoTextBoxContainer()
: hSplitterCursorUpDown( 0 )
, hSplitterBrush( 0 )
, hSplitterPen( 0 )
, DragListMessage( 0 )
{
}


void CDuoTextBoxContainer::init( HINSTANCE hInst, HWND parent )
{
  Window::init( hInst, parent );

  WNDCLASS wndclass;
  ZeroMemory( &wndclass, sizeof(WNDCLASS) );
  wndclass.style = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc = StaticDuoTextBoxPanelProc;
  wndclass.hInstance = _hInst;
  wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );
  wndclass.hbrBackground = (HBRUSH) CreateSolidBrush( RGB(212,208,200));
  wndclass.lpszClassName = DUO_TEXTBOX_CONTAINER_CLASS_NAME;

  /*test window class*/
  WNDCLASS wndclass2;
  ZeroMemory( &wndclass2, sizeof(WNDCLASS) );
  if (!GetClassInfo(_hInst, DUO_TEXTBOX_CONTAINER_CLASS_NAME,&wndclass2))
  {
    if ( !::RegisterClass( &wndclass ) )
    {
      DWORD dwErr = GetLastError();
      // Check if class is already registered, if not then we have some other errors
      if ( ERROR_CLASS_ALREADY_EXISTS != dwErr )
      {
        TCHAR errText[512] = TEXT("");
        wsprintf( errText, TEXT("Cannot register window class %s, error code (%d)\r\nPlease remove this plugin and contact the plugin developer with this message"), DUO_TEXTBOX_CONTAINER_CLASS_NAME, dwErr );
        ::MessageBox( parent, errText, TEXT("Xbmc Skin Plugin error"), MB_OK );
        return;
      }
      return;
    }
  }
  

  RECT rc;
  GetClientRect(parent, &rc);
  _hSelf = CreateWindowW( DUO_TEXTBOX_CONTAINER_CLASS_NAME, 0, WS_CHILD | WS_VISIBLE | WS_VSCROLL , 0, 45,rc.right,rc.bottom-45, _hParent, 0, _hInst, 0 );
  if ( !_hSelf )
  {
    DWORD dwErr = GetLastError();
    TCHAR errText[512] = TEXT("");
    wsprintf( errText, TEXT("Cannot create window class %s, error code (%d)\r\nPlease remove this plugin and contact the plugin developer with this message"), DUO_TEXTBOX_CONTAINER_CLASS_NAME, dwErr );
    ::MessageBox( parent, errText, TEXT("Xbmc Skin Plugin error"), MB_OK );
  }
  MakeDragList( _hSelf );

  // Store the instance pointer within the window user data
  ::SetWindowLongPtr( _hSelf, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this) );

  // Load the cursors for moving the splitter
  hSplitterCursorUpDown    = ::LoadCursor( _hInst, MAKEINTRESOURCE(IDC_UPDOWN) );
  // Make splitter same colour as the dialog pane
  hSplitterBrush = ::CreateSolidBrush( RGB(212,208,200));//::GetSysColor(CTLCOLOR_DLG) );
  hSplitterPen = ::CreatePen( PS_SOLID, 0, RGB(212,208,200));//::GetSysColor(CTLCOLOR_DLG) );

  DragListMessage = ::RegisterWindowMessage( DRAGLISTMSGSTRING );
  
  m_nVscrollPos = 0;
  GetClientRect(_hSelf,&m_ClientRect);
  ResetScrollbar();
  SetupScrollbar();
}

void CDuoTextBoxContainer::AddAttribute(CStdString name, CStdString value)
{
  CDuoTextBox* pDuoTextBox = new CDuoTextBox(m_pDuoTextBox.size());
  pDuoTextBox->init(getHinst(), getHSelf());
  pDuoTextBox->SetValue(name, value);
  
  m_pDuoTextBox.push_back(pDuoTextBox);
  int heightpanel = 45;
  for (size_t i = 0; i < m_pDuoTextBox.size(); i++)
  {
    heightpanel = heightpanel + m_pDuoTextBox[i]->getHeight();
  }
  m_pBottomPosition = heightpanel;

  //m_pBottomPosition = pDuoTextBox->GetBottomPosition();
  m_ClientRect.bottom = m_pBottomPosition;
  SetupScrollbar();
}

void CDuoTextBoxContainer::AddAttribute(CStdString name, std::vector<CStdString> &value, CStdString currentValue)
{
  CDuoTextBox* pDuoTextBox = new CDuoTextBox(m_pDuoTextBox.size());
  pDuoTextBox->init(getHinst(), getHSelf());
  pDuoTextBox->SetValue(name, value, currentValue);
  
  m_pDuoTextBox.push_back(pDuoTextBox);
  int heightpanel = 45;
  for (size_t i = 0; i < m_pDuoTextBox.size(); i++)
  {
    heightpanel = heightpanel + m_pDuoTextBox[i]->getHeight();
  }
  m_pBottomPosition = heightpanel;
  m_ClientRect.bottom = m_pBottomPosition;
  SetupScrollbar();
}

void CDuoTextBoxContainer::ClearAttributes()
{
  CDuoTextBox* pDuoTextBox;
  for (int i = 0; i < m_pDuoTextBox.size(); i++)
  {
    pDuoTextBox = m_pDuoTextBox[i];
    pDuoTextBox->destroy();

  }
  m_pDuoTextBox.clear();
}

RECT CDuoTextBoxContainer::GetContainerRect()
{
  RECT rc;
  getClientRect(rc);
  return rc;
}

int CDuoTextBoxContainer::OnScroll(WPARAM wParam)
{
  int nInc;
  int nPos = (short)HIWORD(wParam);
  switch (LOWORD(wParam))
  {
    case SB_TOP:        nInc = -m_nVscrollPos;               break;
    case SB_BOTTOM:     nInc = m_nVscrollMax-m_nVscrollPos;  break;
    case SB_LINEUP:     nInc = -1;                           break;
    case SB_LINEDOWN:   nInc = 1;                            break;
    case SB_PAGEUP:     nInc = min(-1, -m_nVertInc);         break;
    case SB_PAGEDOWN:   nInc = max(1, m_nVertInc);           break;
    case SB_THUMBTRACK: nInc = nPos - m_nVscrollPos;         break;
    default:            nInc = 0;
  }
  nInc = max(-m_nVscrollPos, min(nInc, m_nVscrollMax - m_nVscrollPos));
  if (nInc)
  {
      m_nVscrollPos += nInc;
      int iMove = -VERT_PTS * nInc;
      ScrollWindow(getHSelf(), 0, iMove, NULL, NULL);
      SetScrollPos(getHSelf(), SB_VERT, m_nVscrollPos, TRUE);
  }
  return 1;
}

LRESULT CDuoTextBoxContainer::DuoTextBoxProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
  DWORD command;
  if ( message == DragListMessage )
  {
    // Being a container window which doesn't care about child window control notifications,
    // forward the child window controls' notifications to the parent window for processing
    return ::SendMessage( _hParent, message, wParam, lParam );
  }

  switch ( message )
  {
  case WM_MOVE:
  case WM_SIZE:
    return 0;
  case WM_VSCROLL:
    {
      return OnScroll(wParam);
    }
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
    command = HIWORD(wParam);
    if (HIWORD(wParam) == EN_CHANGE || HIWORD(wParam) == CBN_EDITCHANGE || HIWORD(wParam) == CBN_SELCHANGE)//768 1024
    {
      for (int i  = 0; i < m_pDuoTextBox.size(); i++)
      {
        if ((HWND)lParam == m_pDuoTextBox[i]->m_pTextBox)
        {
          m_pCurrentTextBox = (HWND)lParam;
          m_pCurrentLabel = m_pDuoTextBox[i]->m_pLabel;
        }
      }
      
      return ::SendMessage( _hParent, message, wParam, (LPARAM)getHSelf() );
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

void CDuoTextBoxContainer::Resize(RECT rc)
{
  if (m_pDuoTextBox.size()>0)
  {
    RECT rect;
    GetClientRect(getHSelf(), &rect);
    for (std::vector<CDuoTextBox*>::iterator it = m_pDuoTextBox.begin(); it != m_pDuoTextBox.end(); it++)
      (*it)->Resize(rect);
  }
  rc.top = 45;
  reSizeToWH(rc);
  SetupScrollbar();
}

void CDuoTextBoxContainer::ResetScrollbar()
{
  CRect tempRect;
  GetClientRect(getHSelf(),&tempRect);
  BOOL bMaximized;
    
    //Max Vertical Scrolling is the difference between size
  //of the Whole Property Page with Controls and that with
  //the current one devided by the Indentation you set

  m_nVertInc = (m_ClientRect.Height() - tempRect.Height())/VERT_PTS;

  m_nVscrollMax = max(0, m_nVertInc);
  m_nVscrollPos = min(m_nVscrollPos, m_nVscrollMax);
  SetScrollRange(getHSelf(), SB_VERT, 0, m_nVscrollMax, FALSE);
  SetScrollPos(getHSelf(), SB_VERT, m_nVscrollPos, TRUE);  
}

void CDuoTextBoxContainer::SetupScrollbar()
{
  CRect tempRect;
  GetClientRect(getHSelf(), &tempRect);
  BOOL bMaximized;
    
    //Max Vertical Scrolling is the difference between size
  //of the Whole Property Page with Controls and that with
  //the current one devided by the Indentation you set

  m_nVertInc = (m_ClientRect.Height() - tempRect.Height())/VERT_PTS;

  m_nVscrollMax = max(0, m_nVertInc);
  m_nVscrollPos = min(m_nVscrollPos, m_nVscrollMax);
  SetScrollRange(getHSelf(), SB_VERT, 0, m_nVscrollMax, FALSE);
  SetScrollPos(getHSelf(), SB_VERT, m_nVscrollPos, TRUE);
}

void CDuoTextBoxContainer::GetLabel(CStdString & text, HWND control)
{
  // Use stl's vector to store the text from editbox, instead of new and delete array
  // because it is safer against memory leaks

  // Get length of text in edit box
  INT textLength;
  std::vector< std::wstring::value_type > data;
  if (control != NULL)
  {
    textLength = ::GetWindowTextLength(control);
    data = std::vector< std::wstring::value_type >(textLength + 1, 0);
    ::GetWindowText( control, &data[0], (int)data.capacity() );
  }
  else
  {
    textLength = ::GetWindowTextLength(m_pCurrentLabel);
    data = std::vector< std::wstring::value_type >( textLength + 1, 0 );
    ::GetWindowText( m_pCurrentLabel, &data[0], (int)data.capacity() );
  }

  // return the text
  text.assign(&data[0]);
}

bool CDuoTextBoxContainer::GetTextBox(CStdString & text, HWND control)
{
  // Use stl's vector to store the text from editbox, instead of new and delete array
  // because it is safer against memory leaks

  if (control == NULL)
    control = m_pCurrentTextBox;
  TCHAR szClass[128];
  RealGetWindowClass(control, szClass,1228);
  CStdString classW = szClass;
  if (classW.Equals(L"ComboBox"))
  {

    int cursel = SendMessage( control, CB_GETCURSEL, 0, 0 );
    if (cursel != -1)
    {
      int textLength = SendMessage( control, CB_GETLBTEXTLEN, cursel,0);
      std::vector< std::wstring::value_type > data( textLength + 1, 0 );

      SendMessage( control, CB_GETLBTEXT, cursel, (LPARAM)&data[0]);
      text.assign( &data[0] );
    }
    else
    {
      INT textLength = ::GetWindowTextLength(control);
      std::vector< std::wstring::value_type > data( textLength + 1, 0 );

      GetWindowText( control, &data[0], (int)data.capacity());
      text.assign( &data[0] );
    }
    return true;
  }
  else
  {
    INT textLength = ::GetWindowTextLength(control);
    // Use stl's vector to store the text, make sure to reserve space for null terminator
    std::vector< std::wstring::value_type > data( textLength + 1, 0 );
    // Get the window text into the vector
    ::GetWindowText( control, &data[0], (int)data.capacity() );

    // return the text
    text.assign( &data[0] );
    return false;
  }
}

void CDuoTextBoxContainer::GetXmlControl( CStdString & xmlControl )
{
  CStdString controlname;
  CStdString controlvalue;
  Attributes control;
  std::pair<std::map<CStdString,CStdString>::iterator,bool> ret; // could be used to verify insertion
  for (int i  = 0; i < m_pDuoTextBox.size(); i++)
  {
    controlname.clear();
    controlvalue.clear();
    GetLabel(controlname, m_pDuoTextBox[i]->m_pLabel);
    GetTextBox(controlvalue, m_pDuoTextBox[i]->m_pTextBox);//we can't use the same due to the fact we use a combobox sometime

    if (controlvalue.size()>0)
    {
      control.insert(std::pair<CStdString,CStdString>(controlname,controlvalue));
      //control[controlname] = controlvalue;
    }
  }
  if (m_pDuoTextBox.size() == 0)
    return;
  TiXmlDocument doc;
  TiXmlElement* elements = new TiXmlElement("control");
  doc.LinkEndChild(elements);
  CStdStringA nameA;
  CStdStringA valueA;
  valueA = control.find(L"type")->second;
  elements->SetAttribute("type",valueA.c_str());
  TiXmlElement* element;
  for (AttributesIt it = control.begin(); it != control.end(); it++)
  {
    if (it->first.Equals(L"type") == false)
    {
      nameA = it->first;
      valueA = it->second;
      element = new TiXmlElement(nameA.c_str());
	    element->LinkEndChild( new TiXmlText(valueA.c_str()));
	    elements->LinkEndChild(element);
    }
  }
  TiXmlPrinter printer;
  //printer.SetLineBreak("");
  elements->Accept(&printer);
  xmlControl = printer.CStr();

}

LRESULT CALLBACK CDuoTextBoxContainer::StaticDuoTextBoxPanelProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
  CDuoTextBoxContainer * pDuoTextBox = reinterpret_cast<CDuoTextBoxContainer *>( GetWindowLongPtr( hwnd, GWLP_USERDATA ) );
  if ( !pDuoTextBox )
  {
    return ::DefWindowProc( hwnd, message, wParam, lParam );
  }
  return pDuoTextBox->DuoTextBoxProc( hwnd, message, wParam, lParam );
}