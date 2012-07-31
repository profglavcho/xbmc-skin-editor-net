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

#ifndef DUO_TEXTBOX_CONTAINER_H
#define DUO_TEXTBOX_CONTAINER_H

#ifndef UNITY_BUILD_SINGLE_INCLUDE
#include "Window.h"
#include "DuoTextBox.h"
#endif



class CDuoTextBoxContainer : public Window
{
public:
  CDuoTextBoxContainer();

  virtual void init(HINSTANCE hInst, HWND parent);
  virtual void destroy() {}
  virtual void Resize(RECT rc);
  virtual void AddAttribute(CStdString name, CStdString value);
  virtual RECT GetContainerRect();
  virtual void ClearAttributes();
  HWND m_pCurrentLabel;
  HWND m_pCurrentTextBox;
  virtual void GetLabel( CStdString & text );
  virtual void GetTextBox( CStdString & text );
private :
  void ResetScrollbar();
  void SetupScrollbar();
  std::vector<CDuoTextBox*> m_pDuoTextBox;
  HCURSOR hSplitterCursorUpDown;
  HBRUSH hSplitterBrush;
  HPEN hSplitterPen;
  int m_pBottomPosition;
  int  m_nVertInc,m_nVscrollMax,m_nVscrollPos;
  CRect m_ClientRect;

  // For passing through drag list box messages
  UINT DragListMessage;

  int OnScroll(WPARAM wParam);

  LRESULT CALLBACK DuoTextBoxProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );

  static LRESULT CALLBACK StaticDuoTextBoxPanelProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
};

#endif //SPLITTER_PANEL_H
