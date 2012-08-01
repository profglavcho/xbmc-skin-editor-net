
#include "stdafx.h"
#include "XbmcIncludesFactory.h"
#include <algorithm>
#include <vector>
#include "lib/ScintillaHelper.h"
#include <iostream>//for temp file writing
#include <fstream>
#include "XbmcPluginEditor.h"
#include "lib/SADirRead.h"

CXbmcIncludesFactory::CXbmcIncludesFactory()
{ 
}

CXbmcIncludesFactory::~CXbmcIncludesFactory()
{
}

void CXbmcIncludesFactory::LoadInclude(CStdStringA path)
{
  
  TiXmlDocument doc(path.c_str());
  bool loadOkay = doc.LoadFile();
  TiXmlNode* pNodeAttribute;
  CStdString strAttribute;
  CStdString strAttributeValue;
  CStdString strValue;
  if (loadOkay)
  {
    TiXmlElement* ele = doc.FirstChildElement("includes");
    if (!ele)
      return;

    pNodeAttribute = ele->FirstChild();
    while (pNodeAttribute)
    {
      strAttribute = pNodeAttribute->Value();
      TiXmlElement* eleinc = pNodeAttribute->ToElement();
      if (eleinc)
      {
        strAttributeValue = eleinc->Attribute("name");
        if (strAttributeValue.length()>0)
          m_pIncludedNames.push_back(strAttributeValue);
      }
      pNodeAttribute = pNodeAttribute->NextSibling();
      
    }
  }
}

void CXbmcIncludesFactory::LoadIncludes(CStdString path)
{
  m_pIncludedFiles.clear();
  m_pIncludedNames.clear();
  CStdStringA pathA = path;
  TiXmlDocument doc(pathA.c_str());
  bool loadOkay = doc.LoadFile();
  TiXmlNode* pNodeAttribute;
  CStdString strAttribute;
  CStdString strAttributeValue;
  CStdString strValue;
  
  if (loadOkay)
  {
    TiXmlElement* ele = doc.FirstChildElement("includes");
    if (!ele)
      return;

    pNodeAttribute = ele->FirstChild();
    while (pNodeAttribute)
    {
      strAttribute = pNodeAttribute->Value();
      TiXmlElement* eleinc = pNodeAttribute->ToElement();
      if (eleinc)
      {
        strAttributeValue = eleinc->Attribute("file");
        if (strAttributeValue.length()>0)
          m_pIncludedFiles.push_back(strAttributeValue);
        strAttributeValue = eleinc->Attribute("name");
        if (strAttributeValue.length()>0)
          m_pIncludedNames.push_back(strAttributeValue);

      }
      pNodeAttribute = pNodeAttribute->NextSibling();
      
    }
    for (size_t i = 0 ; i < m_pIncludedFiles.size(); i++)
    {
      CStdString newpath = path;
      newpath.Replace(L"includes.xml",m_pIncludedFiles[i]);
      LoadInclude(newpath);
    }

  }

}

void CXbmcIncludesFactory::LoadTextures(CStdString path)
{
  m_pTextures.clear();
  CSADirRead dr;
  //get media dir
  TCHAR dirpath[ MAX_PATH];
  SendMessage(g_NppData._nppHandle, NPPM_GETCURRENTDIRECTORY , MAX_PATH,(LPARAM)dirpath);
     
  std::wstring wpath = dirpath;
    
  for (;;)
  {
    int last = wpath.find_last_of(L"\\");
    if (wpath.size()-1 == last)
      break;
    else
      wpath.pop_back();
  }
  wpath.insert(wpath.size(),L"media\\");
  CStdStringA convertedpath;
  convertedpath = g_Scintilla.W_to_A(wpath.c_str());

  dr.GetDirs(convertedpath.c_str(), true);
  dr.GetFiles("*.png");
  dr.GetFiles("*.jpg");
  dr.GetFiles("*.bmp");
  CSADirRead::SAFileVector &fileVector = dr.Files();
  for (CSADirRead::SAFileVector::const_iterator it = fileVector.begin(); it != fileVector.end(); it++)
	{
    CStdString currentFile;
    currentFile = it->m_sName;
    if (!it->bIsFolder)
    {
      currentFile.Replace(wpath.c_str(),L"");
      m_pTextures.push_back(currentFile);
    }
	}
}
