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

#ifndef XBMC_DUOTEXTBOX
#define XBMC_DUOTEXTBOX


#ifndef UNITY_BUILD_SINGLE_INCLUDE
#include "Window.h"
#include "../lib/StdString.h"
#include <vector>
#endif


class CDuoTextBox : public Window
{
public:
  CDuoTextBox(int index);

  virtual void init(HINSTANCE hInst, HWND parent);
  virtual void destroy();

  virtual void SetValue(CStdString name, CStdString value);
  
  virtual void Resize(RECT rc);
  virtual int GetLastPosition() { return GetTop() + 20;}
  virtual int GetBottomPosition() { return m_pBottom; }
  HWND m_pLabel;
  HWND m_pTextBox;
private:
  int GetTop() { return m_pIndex * 20; }
  std::vector<CStdString> lstText;
  HFONT hNewFont;

  HBRUSH hSplitterBrush;
  HPEN hSplitterPen;

  WNDPROC oldWndProc;

  int m_pIndex;
  int m_pBottom;


  // Subclass the list box's wnd proc for customised behavior
  static LRESULT CALLBACK StaticDuoTextBoxProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
  
  LRESULT runProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );
};


#endif