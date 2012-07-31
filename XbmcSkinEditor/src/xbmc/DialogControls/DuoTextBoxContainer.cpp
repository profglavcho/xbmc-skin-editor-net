/*
This file is part of MultiClipboard Plugin for Notepad++
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

#ifndef UNITY_BUILD_SINGLE_INCLUDE
#include "DuoTextBoxContainer.h"
#include <windowsx.h>
#include <commctrl.h>
#include "resource.h"
#include <Richedit.h>
#endif

#define DUO_TEXTBOX_CONTAINER_CLASS_NAME TEXT( "DuoTextBoxContainer" )


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

	// Store the instance pointer within the window user data
	::SetWindowLongPtr( _hSelf, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this) );

	// Load the cursors for moving the splitter
	hSplitterCursorUpDown    = ::LoadCursor( _hInst, MAKEINTRESOURCE(IDC_UPDOWN) );
	// Make splitter same colour as the dialog pane
	hSplitterBrush = ::CreateSolidBrush( RGB(212,208,200));//::GetSysColor(CTLCOLOR_DLG) );
	hSplitterPen = ::CreatePen( PS_SOLID, 0, RGB(212,208,200));//::GetSysColor(CTLCOLOR_DLG) );

	DragListMessage = ::RegisterWindowMessage( DRAGLISTMSGSTRING );
}

void CDuoTextBoxContainer::AddAttribute(CStdString name, CStdString value)
{
  CDuoTextBox* pDuoTextBox = new CDuoTextBox(m_pDuoTextBox.size());
  pDuoTextBox->init(getHinst(), getHSelf());
  pDuoTextBox->SetValue(name, value);
  
  m_pDuoTextBox.push_back(pDuoTextBox);
  m_pBottomPosition = pDuoTextBox->GetBottomPosition();
  UpdateScroll();
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
  //wParam is the type of scroll
  RECT rc;
  getClientRect(rc);
  BOOL res = 0;
  int pixel_missing = m_pBottomPosition - rc.bottom - rc.top;
  int pixel_per_step = pixel_missing / 100;

  /*SCROLLINFO current;
  ZeroMemory(&current, sizeof(current));
  current.cbSize = sizeof(current);
  current.fMask = SIF_ALL;*/
  int currentpos = GetScrollPos(getHSelf(), SB_VERT);
  if (LOWORD(wParam) == SB_LINEDOWN)
   currentpos = currentpos + 1; 
  
  if (currentpos == 0)
    return 0;
  int newpos = currentpos * pixel_per_step;
  ScrollWindowEx(getHSelf(), 0, (newpos * -1), NULL, NULL, NULL, NULL, SW_INVALIDATE|SW_ERASE);
  SCROLLINFO si;
  si.cbSize = sizeof(si); 
  si.fMask  = SIF_POS; 
	si.nPos   = newpos; 
  SetScrollInfo(getHSelf(), SB_VERT, &si, TRUE); 
  UpdateWindow (getHSelf());
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
    if (HIWORD(wParam) == EN_CHANGE)//768 1024
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
  UpdateScroll();
}

void CDuoTextBoxContainer::UpdateScroll()
{
  RECT rc;
  getClientRect(rc);
  BOOL res = 0;
  RECT rcTextBox;
  if ((rc.bottom-rc.top)< m_pBottomPosition)
  {
    int ammountpossible = (rc.bottom-rc.top)/20;
    int ammountmissing = m_pDuoTextBox.size()-ammountpossible;

    res = ShowScrollBar(getHSelf(), SB_VERT, TRUE);
    res = SetScrollRange(getHSelf(), SB_VERT, 0, /*ammountmissing*/100, TRUE);
    res = EnableScrollBar(getHSelf(), SB_VERT, ESB_ENABLE_BOTH);
  }
  else
  {
    res = ShowScrollBar(getHSelf(), SB_VERT, 0);
  }
}

void CDuoTextBoxContainer::GetLabel( CStdString & text )
{
  // Use stl's vector to store the text from editbox, instead of new and delete array
	// because it is safer against memory leaks

	// Get length of text in edit box
	INT textLength = ::GetWindowTextLength(m_pCurrentLabel);
	// Use stl's vector to store the text, make sure to reserve space for null terminator
	std::vector< std::wstring::value_type > data( textLength + 1, 0 );
	// Get the window text into the vector
	::GetWindowText( m_pCurrentLabel, &data[0], (int)data.capacity() );

	// return the text
	text.assign( &data[0] );
}

void CDuoTextBoxContainer::GetTextBox( CStdString & text )
{
  // Use stl's vector to store the text from editbox, instead of new and delete array
	// because it is safer against memory leaks

	// Get length of text in edit box
	INT textLength = ::GetWindowTextLength(m_pCurrentTextBox);
	// Use stl's vector to store the text, make sure to reserve space for null terminator
	std::vector< std::wstring::value_type > data( textLength + 1, 0 );
	// Get the window text into the vector
	::GetWindowText( m_pCurrentTextBox, &data[0], (int)data.capacity() );

	// return the text
	text.assign( &data[0] );
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