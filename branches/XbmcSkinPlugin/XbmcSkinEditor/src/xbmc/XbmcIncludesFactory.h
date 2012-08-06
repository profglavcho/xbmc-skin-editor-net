#ifndef XBMC_INCLUDES_FACTORY_H
#define XBMC_INCLUDES_FACTORY_H

#include "system.h"
#include "tinyxml.h"

#include <map>
#include "TextureBundleXBT.h"


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
  bool GetFrame(CStdString file, CXBTFFrame& frame, CBaseTexture** texture);
protected:
  void LoadInclude(CStdStringA path);
  std::vector<CStdString> m_pIncludedFiles;
  CTextureBundleXBT m_tbXBT;

};
extern CXbmcIncludesFactory* g_XbmcIncludeFactory;

#endif