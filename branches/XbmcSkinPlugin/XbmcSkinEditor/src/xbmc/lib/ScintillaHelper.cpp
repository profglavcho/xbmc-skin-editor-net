#include "stdafx.h"
#include "ScintillaHelper.h"
#include <Windows.h>
#include "../../MultiClipboard.h"


CScintillaHelper g_Scintilla;

CScintillaHelper::CScintillaHelper()
{
}

CScintillaHelper::~CScintillaHelper()
{

}

void CScintillaHelper::ScintillaGetText(char *text, int start, int end) 
{
	TextRange tr;
	tr.chrg.cpMin = start;
	tr.chrg.cpMax = end;
	tr.lpstrText  = text;
	ScintillaMsg(SCI_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(&tr));
  
}
void CScintillaHelper::ScintillaSearch(char *text, int start, int end) 
{
	TextRange tr;
	tr.chrg.cpMin = start;
	tr.chrg.cpMax = end;
	tr.lpstrText  = text;
	ScintillaMsg(SCI_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(&tr));
  
}

int CScintillaHelper::getLineIndentation(int line) const
{
  //return the width of space characters in the indentation of the line
  return ScintillaMsg(SCI_GETLINEINDENTATION, line);

}
/***
 *	ScintillaMsg()
 *
 *	API-Wrapper
 */
LRESULT CScintillaHelper::ScintillaMsg(UINT message, WPARAM wParam, LPARAM lParam)
{
	return ::SendMessage(g_HSource, message, wParam, lParam);
}

LRESULT CScintillaHelper::ScintillaMsg(UINT uMsg, WPARAM wParam , LPARAM lParam ) const
{
    return ::SendMessage(g_HSource, uMsg, wParam, lParam);
}

void CScintillaHelper::beginUndoAction()
{
    ScintillaMsg(SCI_BEGINUNDOACTION);
}

void CScintillaHelper::endUndoAction()
{
    ScintillaMsg(SCI_ENDUNDOACTION);
}

int CScintillaHelper::getCurrentLine() const
{
  return SendMessage(g_NppData._nppHandle, NPPM_GETCURRENTLINE , 0,0);
}

int CScintillaHelper::getLineCount() const
{
  return ScintillaMsg(SCI_GETLINECOUNT);
}

int CScintillaHelper::getLineFromPosition(int pos) const
{
  return ScintillaMsg(SCI_LINEFROMPOSITION, pos);
}

int CScintillaHelper::getLineStartPosition(int current_line) const
{
  return ScintillaMsg(SCI_POSITIONFROMLINE, current_line);
}

int CScintillaHelper::getLineEndPosition(int current_line) const
{
  return ScintillaMsg(SCI_GETLINEENDPOSITION, current_line);
}

unsigned char CScintillaHelper::getCharAt(int pos) const
{
    return (unsigned char) ScintillaMsg(SCI_GETCHARAT, (WPARAM) pos);
}

unsigned int CScintillaHelper::getCodePage() const
{
    return (unsigned int) ScintillaMsg(SCI_GETCODEPAGE);
}

int CScintillaHelper::getCurrentPos() const
{
    return (int) ScintillaMsg(SCI_GETCURRENTPOS);
}

LRESULT CScintillaHelper::getDocPointer() const
{
    return ScintillaMsg(SCI_GETDOCPOINTER);
}

int CScintillaHelper::getSelectionMode() const
{
    return (int) ScintillaMsg(SCI_GETSELECTIONMODE);
}

int CScintillaHelper::getSelectionEnd() const
{
    return (int) ScintillaMsg(SCI_GETSELECTIONEND);
}

int CScintillaHelper::getSelectionStart() const
{
    return (int) ScintillaMsg(SCI_GETSELECTIONSTART);
}

int CScintillaHelper::getSelText(char* pText) const
{
    return (int) ScintillaMsg( SCI_GETSELTEXT, 0, (LPARAM) pText );
}

int CScintillaHelper::getText(int len, char* pText) const
{
    return (int) ScintillaMsg( SCI_GETTEXT, (WPARAM) len, (LPARAM) pText );
}

CStdStringW CScintillaHelper::A_to_W (const char* p)
{
  CStdString ws;
  if (p)
  {
    setlocale (LC_CTYPE, "") ;
    std::vector<wchar_t>  buf (strlen(p)+1, 0) ;
    mbstowcs (&buf[0], p, buf.size()-1) ;
    ws = &buf[0] ;
  }
  return ws ;
}

CStdStringA CScintillaHelper::W_to_A (const wchar_t* p)
{
  CStdStringA ws;
  if (p)
  {
    setlocale (LC_CTYPE, "") ;
    std::vector<char>  buf (wcslen(p)+1, 0) ;
    wcstombs (&buf[0], p, buf.size()-1) ;
    
    ws = &buf[0] ;
  }
  return ws ;
}

CStdString CScintillaHelper::getTextLine(int line) const
{
  int line_start = getLineStartPosition(line);
	int line_end = getLineEndPosition(line);
  int buffer_size = line_end-line_start+1;
  LPSTR strLine = (LPSTR) new CHAR[buffer_size];

  g_Scintilla.ScintillaGetText(strLine, line_start,line_end);
  std::string pStrLineA = strLine;
  CStdStringW pStrLine = g_Scintilla.A_to_W(pStrLineA.c_str());
  return pStrLine;
}

int CScintillaHelper::getTextLength() const
{
    return (int) ScintillaMsg(SCI_GETTEXTLENGTH);
}

int CScintillaHelper::getTextRange(int pos1, int pos2, char* pText) const
{
    TextRange tr;
    tr.chrg.cpMin = pos1;
    tr.chrg.cpMax = pos2;
    tr.lpstrText = pText;
    return (int) ScintillaMsg( SCI_GETTEXTRANGE, 0, (LPARAM) &tr );
}

void CScintillaHelper::goToPos(int pos)
{
    ScintillaMsg( SCI_GOTOPOS, (WPARAM) pos );
}

int CScintillaHelper::insertLine(int line) const
{
  int line_start = getLineEndPosition(line)+1;
  CStdStringA newline = "\n";
  
  
  ScintillaMsg( SCI_INSERTTEXT, line_start, (LPARAM) newline.c_str() );
  return line_start;
}

void CScintillaHelper::insertText(int position, CStdStringW text)
{
  CStdStringA str = g_Scintilla.W_to_A(text);
  ScintillaMsg( SCI_INSERTTEXT, position, (LPARAM) str.c_str() );
  
}
//SCI_INSERTTEXT(int pos, const char *text)

bool CScintillaHelper::isModified() const
{
    return ScintillaMsg(SCI_GETMODIFY) ? true : false;
}

bool CScintillaHelper::isSelectionRectangle() const
{
    return ScintillaMsg(SCI_SELECTIONISRECTANGLE) ? true : false;
}

void CScintillaHelper::setCodePage(unsigned int codePage)
{
    ScintillaMsg( SCI_SETCODEPAGE, (WPARAM) codePage );
}

void CScintillaHelper::setSel(int anchorPos, int currentPos)
{
    ScintillaMsg( SCI_SETSEL, (WPARAM) anchorPos, (LPARAM) currentPos );
}

void CScintillaHelper::setSelectionMode(int mode)
{
    ScintillaMsg( SCI_SETSELECTIONMODE, (WPARAM) mode );
}

void CScintillaHelper::setSelectionEnd(int pos)
{
    ScintillaMsg( SCI_SETSELECTIONEND, (WPARAM) pos );
}

void CScintillaHelper::setSelectionStart(int pos)
{
    ScintillaMsg( SCI_SETSELECTIONSTART, (WPARAM) pos );
}

void CScintillaHelper::setSelText(const char* pText)
{
    ScintillaMsg( SCI_REPLACESEL, 0, (LPARAM) pText );
}

void CScintillaHelper::setText(const char* pText)
{
    ScintillaMsg( SCI_SETTEXT, 0, (LPARAM) pText );
}