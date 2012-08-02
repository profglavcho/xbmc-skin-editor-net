#ifndef XBMC_INCLUDES_FACTORY_H
#define XBMC_INCLUDES_FACTORY_H

#include "lib/StdString.h"
#include "tinyxml.h"

#include <map>

class CXbmcIncludesFactory
{
public:
  CXbmcIncludesFactory();
  ~CXbmcIncludesFactory();
  void LoadIncludes(CStdString path);
  void LoadTextures(CStdString path);
  std::vector<CStdString> GetIncludes() { return m_pIncludedNames; }
  std::vector<CStdString> GetTextures() { return m_pTextures; }
  std::vector<CStdString> m_pIncludedNames;
  std::vector<CStdString> m_pTextures;
protected:
  void LoadInclude(CStdStringA path);
  std::vector<CStdString> m_pIncludedFiles;
  
};

#endif