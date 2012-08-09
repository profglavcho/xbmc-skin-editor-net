#ifndef XBMC_CONTROLS_FACTORY
#define XBMC_CONTROLS_FACTORY

#include "system.h"
#include "tinyxml.h"
/*#define BOOST_NO_USER_CONFIG
#define BOOST_NO_COMPILER_CONFIG
#define BOOST_NO_STDLIB_CONFIG
#define BOOST_NO_PLATFORM_CONFIG*/
#define USEBOOST 0
#if USEBOOST
#include "boost/unordered_map.hpp"
#else
#include <map>
#endif

#if USEBOOST
typedef boost::unordered_map<CStdString, CStdString> Attributes;
typedef boost::unordered_map<CStdString, CStdString>::iterator AttributesIt;
#else
typedef std::map<CStdString, CStdString> Attributes;
typedef std::map<CStdString, CStdString>::iterator AttributesIt;

#endif

struct newLineString
{
  bool newline;
  int index;
  CStdString strNewLine;
};

class CXbmcControlsFactory
{
public:
  CXbmcControlsFactory();
  ~CXbmcControlsFactory();
  void LoadControls();
  std::vector<CStdString> GetControlsList();
  std::vector<Attributes> GetCurrentControlsList() { return m_pGuiControls;}
  bool ControlTypeExist(CStdString type);
  Attributes GetAttributeVector(CStdString name);
  //Also set the current control which is used by the dialog
  Attributes GetAttributes(CStdString name);
  void SetAttribute(Attributes & attrib, CStdString name, CStdString value);
  void SetAttribute(CStdString name, CStdString value);
  void ResetAttributeForType(CStdString type);

  //for listing controls
  void LoadCurrentControls();
  void LoadControl(TiXmlElement* pControl);
  void LoadControlGroup(TiXmlElement* pControl);

  //for control modification
  void LoadCurrentControl(CStdString strControl);
  newLineString GetLineModification(CStdString strControl, CStdString name, CStdString value);

  //xml loading stuff
  CStdString GetType(const TiXmlElement *pControlNode, CStdStringA attrib);
  bool GetString(const TiXmlNode* pRootNode, const char* strTag, CStdString& strStringValue);
protected:
  CStdString m_pCurrent;
  CStdStringA m_pTempXmlControlPath;
  CStdStringA m_pTempControlsPath;
  std::vector<Attributes> m_pAllControls;
  std::vector<Attributes> m_pGuiControls;
  void ReloadTempFile();
  TiXmlDocument m_pCurrentDoc;
};

extern CXbmcControlsFactory* g_XbmcControlsFactory;

#endif