/*
This file is part of Xbmc skin editor for notepad++
Copyright (C) 2012 Ti-BEN

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/

#include "stdafx.h"
#include "XbmcControlsFactory.h"
#include <algorithm>
#include <vector>
#include "lib/ScintillaHelper.h"
#include <iostream>//for temp file writing
#include <fstream>
#include "XbmcPluginEditor.h"

typedef struct
{
  const wchar_t* name;
} XBMC_CONTROL;

static const XBMC_CONTROL control_default[] =
{
  {L"type"},
  {L"description"},
  {L"id"},
  {L"posx"},
  {L"posy"},
  {L"width"},
  {L"height"},
  {L"visible"},
  {L"animation"},
  {L"camera"},
  {L"colordiffuse"},
  {L"onup"},
  {L"ondown"},
  {L"onleft"},
  {L"onright"},
  {L"hitrect"},
  {L"enable"},
  {L"pulseonselect"},
  {L"include"}//it can be in any control
};

static const XBMC_CONTROL control_label[] =
{
  {L"align"},
  {L"aligny"},
  {L"scroll"},
  {L"label"},
  {L"info"},
  {L"number"},
  {L"angle"},
  {L"haspath"},
  {L"font"},
  {L"textcolor"},
  {L"shadowcolor"},
  {L"wrapmultiline"},
  {L"scrollspeed"},
  {L"scrollsuffix"}
};
static const XBMC_CONTROL control_fadelabel[] =
{
  {L"scroll"},
  {L"scrollout"},
  {L"pauseatend"},
  {L"label"},
  //"info" ---> List(Of String)
  {L"font"},
  {L"textcolor"},
  {L"textoffsetx"}
};
static const XBMC_CONTROL control_button[] =
{
  {L"texturefocus"},
  {L"texturenofocus"},
  {L"label"},
  {L"font"},
  {L"textcolor"},
  {L"disabledcolor"},
  {L"align"},
  {L"aligny"},
  {L"textoffsetx"},
  {L"textoffsety"},
  {L"onclick"},
  {L"onfocus"}
};

static const XBMC_CONTROL control_multiselect[] =
{
  {L"texturefocus"},
  {L"texturenofocus"},
  {L"label"},
  {L"font"},
  {L"textcolor"},
  {L"disabledcolor"},
  {L"aligny"},
  {L"textoffsetx"},
  {L"textoffsety"}
};

static const XBMC_CONTROL control_image[] =
{
  {L"fadetime"},
  {L"bordersize"},
  {L"info"},
  {L"aspectratio"},
  {L"texture"},
  {L"bordertexture"}
  // texture border="5" flipy="true" flipx="false"","")
  // bordertexture border="5"","")
};
static const XBMC_CONTROL control_multiimage[] =
{
  {L"imagepath"},
  {L"info"},
  {L"timeperimage"},
  {L"fadetime"},
  {L"pauseatend"},
  {L"randomize"},
  {L"aspectratio"}
};

static const XBMC_CONTROL control_radiobutton[] =
{
  {L"texturefocus"},
  {L"texturenofocus"},
  {L"textureradioon"},
  {L"textureradiooff"},
  {L"selected"},
  {L"onclick"},
  {L"label"},
  {L"font"},
  {L"textcolor"},
  {L"disabledcolor"},
  {L"align"},
  {L"aligny"},
  {L"textoffsetx"},
  {L"textoffsety"}
};

static const XBMC_CONTROL control_selectbutton[] =
{
  {L"texturefocus"},
  {L"texturenofocus"},
  {L"texturebg"},
  {L"textureleft"},
  {L"textureleftfocus"},
  {L"textureright"},
  {L"texturerightfocus"},
  {L"label"},
  {L"font"},
  {L"textcolor"},
  {L"disabledcolor"},
  {L"align"},
  {L"alignY"},
  {L"textoffsetx"},
  {L"textoffsety"}
};

static const XBMC_CONTROL control_togglebutton[] =
{
  {L"texturefocus"},
  {L"texturenofocus"},
  {L"alttexturefocus"},
  {L"alttexturenofocus"},
  {L"usealttexture"},
  {L"label"},
  {L"altlabel"},
  {L"font"},
  {L"textcolor"},
  {L"disabledcolor"},
  {L"align"},
  {L"aligny"},
  {L"textoffsetx"},
  {L"textoffsety"},
  {L"onclick"},
  {L"onfocus"}
};

static const XBMC_CONTROL control_buttonscrollers[] =
{
  {L"texturefocus"},
  {L"texturenofocus"},
  {L"font"},
  {L"textcolor"},
  {L"align"},
  {L"aligny"},
  {L"textoffsetx"},
  {L"textoffsety"},
  {L"numbuttons"},
  {L"buttongap"},
  {L"orientation"},
  {L"defaultbutton"},
  {L"movement"},
  {L"alpha"},
  {L"wraparound"},
  {L"smoothscrolling"}
};

static const XBMC_CONTROL control_panel[] =
{
  {L"viewtype"},
  {L"orientation"},
  {L"pagecontrol"},
  {L"scrolltime"},
  {L"itemlayout"},
  {L"focusedlayout"},
  {L"content"},
  {L"preloaditems"}
};
CXbmcControlsFactory* g_XbmcControlsFactory;

CXbmcControlsFactory::CXbmcControlsFactory()
{
  LoadControls();
  m_pTempXmlControlPath = "";
  
}

CXbmcControlsFactory::~CXbmcControlsFactory()
{
}

void CXbmcControlsFactory::ReloadTempFile()
{
  CStdString path = g_configDir;
  path.append(L"\\tempcontrol.xml");
  m_pTempControlsPath = g_configDir;
  m_pTempControlsPath.append("tempcontrol2.xml");
  m_pTempXmlControlPath = g_Scintilla.W_to_A(path);
}

void CXbmcControlsFactory::SetAttribute(Attributes & attrib, CStdString name, CStdString value)
{
  attrib.find(name)->second = value;
}

void CXbmcControlsFactory::SetAttribute(CStdString name, CStdString value)
{
  for (int i = 0; i < m_pAllControls.size(); i++)
  {

    if (m_pAllControls[i].find("type")->second.Equals(m_pCurrent))
    {
      if (m_pAllControls[i].find(name) == m_pAllControls[i].end())
      {
        assert(0);
        return;
      }
      m_pAllControls[i].find(name)->second = value;
    }
  }
}

void CXbmcControlsFactory::ResetAttributeForType(CStdString type)
{
  for (int i = 0; i < m_pAllControls.size(); i++)
  {

    if (m_pAllControls[i].find("type")->second.Equals(type))
    {
      for (Attributes::iterator it = m_pAllControls[i].begin(); it != m_pAllControls[i].end(); it++)
      {
        if (it->first.Equals(L"type") == false)
          it->second = L"";
      }
    }
  }
}


CStdString CXbmcControlsFactory::GetType(const TiXmlElement *pControlNode,CStdStringA attrib)
{
  CStdString type;
  const char *szType = pControlNode->Attribute(attrib.c_str());
  if (szType)
    type = szType;
  return type;
}

bool CXbmcControlsFactory::GetString(const TiXmlNode* pRootNode, const char* strTag, CStdString& strStringValue)
{
  const TiXmlElement* pElement = pRootNode->FirstChildElement(strTag );
  if (!pElement) return false;
  const char* encoded = pElement->Attribute("urlencoded");
  const TiXmlNode* pNode = pElement->FirstChild();
  if (pNode != NULL)
  {
    strStringValue = pNode->Value();
    if (encoded && strcmpi(encoded,"yes") == 0)
      assert(0);//CURL::Decode(strStringValue);
    return true;
  }
  strStringValue.Empty();
  return false;
}

newLineString CXbmcControlsFactory::GetLineModification(CStdString strControl, CStdString name, CStdString value)
{
  ReloadTempFile();
  CStdStringA strControlA  = g_Scintilla.W_to_A(strControl);
  CStdStringA nameA  = g_Scintilla.W_to_A(name);
  CStdStringA valueA  = g_Scintilla.W_to_A(value);
  std::ofstream myfile;
  myfile.open (m_pTempXmlControlPath.c_str());
  myfile.write(strControlA.c_str(),strControlA.size());
  myfile.close();
  TiXmlDocument doc(m_pTempXmlControlPath.c_str());
  bool loadOkay = doc.LoadFile();
  CStdString type;
  CStdString printed = "";
  TiXmlPrinter printer;
  printer.SetLineBreak("");
  newLineString returnline;

  returnline.newline = false;
  returnline.strNewLine = "";
  returnline.index = 0;
  if (loadOkay)
  {
    TiXmlElement* ele = doc.FirstChildElement("control");
    type = GetType(ele, "type");
    if (!ControlTypeExist(type))
      return returnline;
    TiXmlNode* pNodeAttribute;
    TiXmlNode* pNodeValue;
    pNodeAttribute = ele->FirstChildElement(nameA.c_str());
    //if attribute exist set the value to this one
    if (pNodeAttribute)
    {
      TiXmlText * newText = new TiXmlText(valueA.c_str());
      pNodeAttribute->Clear();
      pNodeAttribute->LinkEndChild(newText);
      //set value set the name not the value
      pNodeAttribute->Accept(&printer);
      returnline.newline = false;
      
    }
    else
    {
      //insert new one
      TiXmlElement* newEle = new TiXmlElement(nameA.c_str());
      TiXmlText * newText = new TiXmlText(valueA.c_str());
      newEle->LinkEndChild( newText );
      ele->LinkEndChild(newEle);
      
      newEle->Accept(&printer);
      returnline.newline = true;
      
    }
    //saved file correctly update it
    if (doc.SaveFile( m_pTempXmlControlPath.c_str()))
    {
      if (doc.LoadFile())
      {
        TiXmlElement* ele = doc.FirstChildElement("control");
        TiXmlNode* pNodeAttribute;
        pNodeAttribute = ele->FirstChildElement(nameA.c_str());
        returnline.index = pNodeAttribute->Row()-1;
      }
    }
  }
  
  
  returnline.strNewLine = g_Scintilla.A_to_W(printer.CStr());
  return returnline;
}

void CXbmcControlsFactory::LoadControlGroup(TiXmlElement* pControlGroup)
{
  TiXmlNode* pNodeAttribute;
  TiXmlNode* pNodeValue;
  TiXmlElement *pControl = pControlGroup->FirstChildElement();
  
        
  while (pControl)
  {
    if (strcmpi(pControl->Value(), "control") == 0)
    {
      CStdString strValue = GetType(pControl,"type");
     
      LoadControl(pControl);
      //if it is a group add it and parse the group within it
      if (strValue.size()>0 && strValue.Equals(L"group"))
        LoadControlGroup(pControl);

      
    }
    pControl = pControl->NextSiblingElement();
  }
}

void CXbmcControlsFactory::LoadControl(TiXmlElement* pControl)
{
  Attributes pAttribute;
  TiXmlNode* pNodeAttribute;
  TiXmlNode* pNodeValue;
  pNodeAttribute = pControl->FirstChild();
  CStdString strAttribute, strValue;
  strValue = GetType(pControl,"type");
  if (strValue.size()>0)
    pAttribute["type"] = strValue;

  strValue = GetType(pControl,"id");
  if (strValue.size()>0)
    pAttribute["id"] = strValue;
  if (pControl->Row()> 0)
  {
    CStdString strLine;
    
    strLine.Format(L"%i",pControl->Row());
    pAttribute["line"] = strLine;
  }
  while (pNodeAttribute)
  {
    strAttribute = pNodeAttribute->Value();
    
    pNodeValue = pNodeAttribute->FirstChild();
    if (pNodeValue && pNodeValue->Value() == "")
    {
      strValue = pNodeValue->Value();

      pAttribute[strAttribute] = strValue;
    }
    pNodeAttribute = pNodeAttribute->NextSibling();
  }
  m_pGuiControls.push_back(pAttribute);


}

void CXbmcControlsFactory::LoadCurrentControls()
{
  ReloadTempFile();
  m_pGuiControls.clear();
  CStdStringA pText;
  g_Scintilla.getEntireText(pText);
  //create temporary file for loading the xml with the col and row
  std::ofstream myfile;
  myfile.open (m_pTempControlsPath.c_str());
  pText.Replace("\n","");//stupid line insertion fix
  myfile.write(pText.c_str(),pText.size());
  myfile.close();

  m_pCurrentDoc = TiXmlDocument();
  
  bool loadOkay = m_pCurrentDoc.LoadFile(m_pTempControlsPath.c_str());
  if (!loadOkay)
    return;
  
  TiXmlElement* pRootElement = m_pCurrentDoc.RootElement();
  if (strcmpi(pRootElement->Value(), "window"))
    return;

  TiXmlElement *pChild = pRootElement->FirstChildElement();
  while (pChild)
  {
    CStdStringA strValue = pChild->Value();
    if (strValue == "type" && pChild->FirstChild())
    {
      // if we have are a window type (ie not a dialog), and we have <type>dialog</type>
      // then make this window act like a dialog
      if (strcmpi(pChild->FirstChild()->Value(), "dialog") == 0)
      {}//m_isDialog = true;
    }
    else if (strValue == "previouswindow" && pChild->FirstChild())
    {
      //m_previousWindow = CButtonTranslator::TranslateWindow(pChild->FirstChild()->Value());
    }
    else if (strValue == "defaultcontrol" && pChild->FirstChild())
    {
      /*const char *always = pChild->Attribute("always");
      if (always && strcmpi(always, "true") == 0)
        m_defaultAlways = true;
      m_defaultControl = atoi(pChild->FirstChild()->Value());*/
    }
    else if (strValue == "visible" && pChild->FirstChild())
    {
      /*CStdString condition;
      CGUIControlFactory::GetConditionalVisibility(pRootElement, condition);
      m_visibleCondition = g_infoManager.Register(condition, GetID());*/
    }
    else if (strValue == "animation" && pChild->FirstChild())
    {
      /*CRect rect(0, 0, (float)m_coordsRes.iWidth, (float)m_coordsRes.iHeight);
      CAnimation anim;
      anim.Create(pChild, rect, GetID());
      m_animations.push_back(anim);*/
    }
    else if (strValue == "zorder" && pChild->FirstChild())
    {
      //m_renderOrder = atoi(pChild->FirstChild()->Value());
    }
    else if (strValue == "coordinates")
    {
      /*XMLUtils::GetFloat(pChild, "posx", m_posX);
      XMLUtils::GetFloat(pChild, "posy", m_posY);

      TiXmlElement *originElement = pChild->FirstChildElement("origin");
      while (originElement)
      {
        COrigin origin;
        originElement->QueryFloatAttribute("x", &origin.x);
        originElement->QueryFloatAttribute("y", &origin.y);
        if (originElement->FirstChild())
          origin.condition = g_infoManager.Register(originElement->FirstChild()->Value(), GetID());
        m_origins.push_back(origin);
        originElement = originElement->NextSiblingElement("origin");
      }*/
    }
    else if (strValue == "camera")
    { // z is fixed
      /*pChild->QueryFloatAttribute("x", &m_camera.x);
      pChild->QueryFloatAttribute("y", &m_camera.y);
      m_hasCamera = true;*/
    }
    else if (strValue == "controls")
    {
      TiXmlElement *pControl = pChild->FirstChildElement();
      while (pControl)
      {
        if (strcmpi(pControl->Value(), "control") == 0)
        {
          CStdString strValue = GetType(pControl,"type");
          if (strValue.size()>0 && strValue.Equals(L"group"))
            LoadControlGroup(pControl);
          else
            LoadControl(pControl);
        }
        pControl = pControl->NextSiblingElement();
      }
    }
    else if (strValue == "allowoverlay")
    {
      /*bool overlay = false;
      if (XMLUtils::GetBoolean(pRootElement, "allowoverlay", overlay))
        m_overlayState = overlay ? OVERLAY_STATE_SHOWN : OVERLAY_STATE_HIDDEN;*/
    }

    pChild = pChild->NextSiblingElement();
  }

  printf("yeah");



}

void CXbmcControlsFactory::LoadCurrentControl(CStdString strControl)
{
  ReloadTempFile();
  CStdString strAttribute, strValue;
  CStdStringA strControlA  = g_Scintilla.W_to_A(strControl);
  std::ofstream myfile;
  myfile.open (m_pTempXmlControlPath.c_str());

  myfile.write(strControlA.c_str(),strControlA.size());
  myfile.close();

  TiXmlDocument doc(m_pTempXmlControlPath.c_str());
  bool loadOkay = doc.LoadFile();
  CStdString type;
  if (loadOkay)
  {
    TiXmlElement* ele = doc.FirstChildElement("control");
    type = GetType(ele,"type");
    if (!ControlTypeExist(type))
      return;
    else
    {
      //set the current type to be able to set the value to the right control
      m_pCurrent = type;
      ResetAttributeForType(type);
    }

    TiXmlNode* pNodeAttribute;
    TiXmlNode* pNodeValue;
    pNodeAttribute = ele->FirstChild();

    while (pNodeAttribute)
    {
      strAttribute = pNodeAttribute->Value();
      pNodeValue = pNodeAttribute->FirstChild();
      if (pNodeValue)
      {
        strValue = pNodeValue->Value();
        SetAttribute(strAttribute,strValue);
      }
      pNodeAttribute = pNodeAttribute->NextSibling();
      
    }
    
    printf("yeah");
  }
  else
    return;
  theApp.clipXbmcControls.ResetCurrentControl();
  theApp.clipXbmcControls.SetCurrentControl(type);
}

#if 1
bool CXbmcControlsFactory::ControlTypeExist(CStdString type)
{
  for (int i = 0; i < m_pAllControls.size(); i++)
  {
    if (m_pAllControls[i].find("type") != m_pAllControls[i].end())
    {
      if (m_pAllControls[i]["type"].Equals(type))
        return true;
    }
  }
  return false;
}

Attributes CXbmcControlsFactory::GetAttributeVector(CStdString name)
{
  for (int i = 0; i < m_pAllControls.size(); i++)
  {
    if (m_pAllControls[i].find("type") != m_pAllControls[i].end())
    {
      if (m_pAllControls[i]["type"].Equals(name))
        return m_pAllControls[i];
    }
  }
}


Attributes CXbmcControlsFactory::GetAttributes(CStdString name)
{
  m_pCurrent = name;
  return GetAttributeVector(name);
}

std::vector<CStdString> CXbmcControlsFactory::GetControlsList()
{
  std::vector<CStdString> list;
  for (int i = 0 ; i < m_pAllControls.size(); i++)
    list.push_back(m_pAllControls[i].find("type")->second);
  return list;

}


#define CLEAR_CONTROL pControl.clear(); for ( i = 0; i < sizeof(control_default)/sizeof(XBMC_CONTROL); i++) pControl[control_default[i].name] = L"";
void CXbmcControlsFactory::LoadControls()
{
  unsigned int i = 0;
  Attributes pControl;
  
 /*pControl.clear() for ( i = 0; i < sizeof(control_default)/sizeof(XBMC_CONTROL); i++)
    pControl[control_default[i].name] = "");*/
  
 CLEAR_CONTROL
  for ( i = 0; i < sizeof(control_label)/sizeof(XBMC_CONTROL); i++)
    pControl[control_label[i].name] = L"";
  SetAttribute(pControl, "type","label");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  for ( i = 0; i < sizeof(control_fadelabel)/sizeof(XBMC_CONTROL); i++)
    pControl[control_fadelabel[i].name] = L"";
  SetAttribute(pControl, "type","fadelabel");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  for ( i = 0; i < sizeof(control_button)/sizeof(XBMC_CONTROL); i++)
    pControl[control_button[i].name] = L"";
  SetAttribute(pControl, "type","button");
  m_pAllControls.push_back(pControl);


  CLEAR_CONTROL
  for ( i = 0; i < sizeof(control_multiselect)/sizeof(XBMC_CONTROL); i++)
    pControl[control_multiselect[i].name] = L"";
  SetAttribute(pControl, "type","multiselect");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  for ( i = 0; i < sizeof(control_image)/sizeof(XBMC_CONTROL); i++)
    pControl[control_image[i].name] = L"";
  SetAttribute(pControl, "type","image");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  for ( i = 0; i < sizeof(control_multiimage)/sizeof(XBMC_CONTROL); i++)
    pControl[control_multiimage[i].name] = L"";
  SetAttribute(pControl, "type","multiimage");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  for ( i = 0; i < sizeof(control_radiobutton)/sizeof(XBMC_CONTROL); i++)
    pControl[control_radiobutton[i].name] = L"";
  SetAttribute(pControl, "type","radiobutton");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  for ( i = 0; i < sizeof(control_selectbutton)/sizeof(XBMC_CONTROL); i++)
    pControl[control_selectbutton[i].name] = L"";
  SetAttribute(pControl, "type","selectbutton");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  for ( i = 0; i < sizeof(control_togglebutton)/sizeof(XBMC_CONTROL); i++)
    pControl[control_togglebutton[i].name] = L"";
  SetAttribute(pControl, "type","togglebutton");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  for ( i = 0; i < sizeof(control_buttonscrollers)/sizeof(XBMC_CONTROL); i++)
    pControl[control_buttonscrollers[i].name] = L"";
  SetAttribute(pControl, "type","buttonscrollers");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  for ( i = 0; i < sizeof(control_panel)/sizeof(XBMC_CONTROL); i++)
    pControl[control_panel[i].name] = L"";
  SetAttribute(pControl, "type","panel");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  SetAttribute(pControl, "type","buttonsblock");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  SetAttribute(pControl, "type","spin");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  SetAttribute(pControl, "type","settingsspin");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  SetAttribute(pControl, "type","slider");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  SetAttribute(pControl, "type","listcontainer");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  SetAttribute(pControl, "type","wraplistcontainer");
  
  CLEAR_CONTROL
  SetAttribute(pControl, "type","fixedlistcontainer");
  m_pAllControls.push_back(pControl);
  
  CLEAR_CONTROL
  SetAttribute(pControl, "type","slider");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  SetAttribute(pControl, "type","panelcontainer");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  SetAttribute(pControl, "type","progress");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  SetAttribute(pControl, "type","textbox");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  SetAttribute(pControl, "type","rssfeed");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  SetAttribute(pControl, "type","visualisation");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  SetAttribute(pControl, "type","video");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  SetAttribute(pControl, "type","mover");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  SetAttribute(pControl, "type","resize");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  SetAttribute(pControl, "type","edit");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  SetAttribute(pControl, "type","console");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  SetAttribute(pControl, "type","checkmark");
  m_pAllControls.push_back(pControl);

  CLEAR_CONTROL
  SetAttribute(pControl, "type","extendedlist");
  m_pAllControls.push_back(pControl);

}
#endif