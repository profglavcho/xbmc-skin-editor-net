#ifndef SCINTILLA_HELPER_H
#define SCINTILLA_HELPER_H

#include "system.h"
#include "Scintilla.h"
class CScintillaHelper
{
public:
  CScintillaHelper();
  ~CScintillaHelper();

  void ScintillaSearch(char *text, int start, int end);
  void ScintillaGetText(char *text, int start, int end);
  LRESULT ScintillaMsg(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
  LRESULT ScintillaMsg(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0) const;
  
  void    beginUndoAction();
  void    endUndoAction();
  void    deleteRange(int pos, int length);
  int     getCurrentLine() const;
  int     getLineStartPosition(int current_line) const;
  int     getLineEndPosition(int current_line) const;
  unsigned char getCharAt(int pos) const;
  unsigned int  getCodePage() const; // 0 (non-Unicode), SC_CP_UTF8, DBCS etc.
  int     getCurrentPos() const;
  int     getLineFromPosition(int pos) const;
  int     getLineCount() const;
  int     getLineIndentation(int line) const;
  LRESULT       getDocPointer() const; // identifies the document
  int     getSelectionMode() const; // SC_SEL_STREAM, SC_SEL_RECTANGLE, SC_SEL_LINES
  int     getSelectionEnd() const;
  int     getSelectionStart() const;
  int     getSelText(char* pText) const;
  int     getEntireText(CStdStringA &pText) const;
  int     getText(int len, char* pText) const;
  int     getTextLength() const;
  int     getTextRange(int pos1, int pos2, char* pText) const;
  CStdString getTextLine(int line) const;
  void    goToPos(int pos);
  int     insertLine(int line) const;//return the position in the text of this line
  void    insertText(int position, CStdStringW text);
  void    insertText(int position, CStdStringA text);
  bool    isModified() const;
  bool    isSelectionRectangle() const;
  void    setCodePage(unsigned int codePage);
  void    setSel(int anchorPos, int currentPos);
  void    setSelectionMode(int mode); // SC_SEL_STREAM, SC_SEL_RECTANGLE, SC_SEL_LINES
  void    setSelectionEnd(int pos);
  void    setSelectionStart(int pos);
  void    setSelText(const char* pText);
  void    setText(const char* pText);
  CStdStringW A_to_W (const char* p);
  CStdStringA W_to_A (const wchar_t* p);

};
extern CScintillaHelper g_Scintilla;
#endif