/*
 *      Copyright (C) 2005-2009 Team XBMC
 *      http://www.xbmc.org
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, write to
 *  the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *  http://www.gnu.org/copyleft/gpl.html
 *
 */

#include "lib/libsquish/squish.h"

#include "TextureBundleXBT.h"
#include "Texture.h"
/*
#include "GraphicContext.h"
#include "utils/log.h"
#include "addons/Skin.h"
#include "settings/GUISettings.h"
#include "filesystem/SpecialProtocol.h"
#include "utils/EndianSwap.h"
#include "utils/URIUtils.h"
#include "XBTF.h"*/
#include "lib/lzo/lzo1x.h"

#ifdef _WIN32
#pragma comment(lib,"liblzo2.lib")
#endif

CTextureBundleXBT::CTextureBundleXBT(void)
{
  m_themeBundle = false;
  strCurrentPath = "";
}

CTextureBundleXBT::~CTextureBundleXBT(void)
{
  Cleanup();
}

bool CTextureBundleXBT::OpenBundle(CStdString Filename)
{
  Cleanup();
  //theme not used yet
#if 0
  // Find the correct texture file (skin or theme)
  CStdString strPath;

  if (m_themeBundle)
  {
    // if we are the theme bundle, we only load if the user has chosen
    // a valid theme (or the skin has a default one)
    CStdString theme = g_guiSettings.GetString("lookandfeel.skintheme");
    if (!theme.IsEmpty() && theme.CompareNoCase("SKINDEFAULT"))
    {
      CStdString themeXBT(URIUtils::ReplaceExtension(theme, ".xbt"));
      strPath = URIUtils::AddFileToFolder(g_graphicsContext.GetMediaDir(), "media");
      strPath = URIUtils::AddFileToFolder(strPath, themeXBT);
    }
    else
    {
      return false;
    }
  }
  else
  {
    strPath = URIUtils::AddFileToFolder(g_graphicsContext.GetMediaDir(), "media/Textures.xbt");
  }

  strPath = CSpecialProtocol::TranslatePathConvertCase(strPath);
#endif
  
  
  // Load the texture file
  if (!m_XBTFReader.Open(Filename))
  {
    return false;
  }

  //CLog::Log(LOGDEBUG, "%s - Opened bundle %s", __FUNCTION__, strPath.c_str());

  m_TimeStamp = m_XBTFReader.GetLastModificationTimestamp();

  if (lzo_init() != LZO_E_OK)
  {
    return false;
  }

  return true;
}

bool CTextureBundleXBT::HasFile(const CStdString& Filename)
{
  if (!m_XBTFReader.IsOpen() && !OpenBundle(strCurrentPath))
    return false;

  if (m_XBTFReader.GetLastModificationTimestamp() > m_TimeStamp)
  {
    //CLog::Log(LOGINFO, "Texture bundle has changed, reloading");
    if (!OpenBundle(strCurrentPath))
      return false;
  }

  CStdString name = Normalize(Filename);
  return m_XBTFReader.Exists(name);
}

void CTextureBundleXBT::GetTexturesFromPath(const CStdString &path, std::vector<CStdString> &textures)
{
  /*if (path.GetLength() > 1 && path[1] == ':')
    return;*/
  strCurrentPath = path;
  if (!m_XBTFReader.IsOpen() && !OpenBundle(path))
    return;

  CStdString testPath = path;//Normalize(path);
  //URIUtils::AddSlashAtEnd(testPath);

  int testLength = testPath.GetLength();
  
  std::vector<CXBTFFile>& files = m_XBTFReader.GetFiles();
  for (size_t i = 0; i < files.size(); i++)
  {
    CStdString path = files[i].GetPath();
    //if (path.Left(testLength).Equals(testPath))
    textures.push_back(path);
  }
}

bool CTextureBundleXBT::ConvertFrameToTexture(const CStdString& name, CXBTFFrame& frame, CBaseTexture** ppTexture)
{
  

  // found texture - allocate the necessary buffers
  squish::u8 *buffer = new squish::u8[(size_t)frame.GetPackedSize()];
  if (buffer == NULL)
  {
    //CLog::Log(LOGERROR, "Out of memory loading texture: %s (need %"PRIu64" bytes)", name.c_str(), frame.GetPackedSize());
    return false;
  }

  // load the compressed texture
  if (!m_XBTFReader.Load(frame, buffer))
  {
    //CLog::Log(LOGERROR, "Error loading texture: %s", name.c_str());
    delete[] buffer;
    return false;
  }

  // check if it's packed with lzo
  if (frame.IsPacked())
  { // unpack
    squish::u8 *unpacked = new squish::u8[(size_t)frame.GetUnpackedSize()];
    if (unpacked == NULL)
    {
      //CLog::Log(LOGERROR, "Out of memory unpacking texture: %s (need %"PRIu64" bytes)", name.c_str(), frame.GetUnpackedSize());
      delete[] buffer;
      return false;
    }
    lzo_uint s = (lzo_uint)frame.GetUnpackedSize();
    if (lzo1x_decompress(buffer, (lzo_uint)frame.GetPackedSize(), unpacked, &s, NULL) != LZO_E_OK ||
        s != frame.GetUnpackedSize())
    {
      //CLog::Log(LOGERROR, "Error loading texture: %s: Decompression error", name.c_str());
      delete[] buffer;
      delete[] unpacked;
      return false;
    }
    delete[] buffer;
    buffer = unpacked;
  }

  *ppTexture = new CTexture();
  (*ppTexture)->LoadFromMemory(frame.GetWidth(), frame.GetHeight(), 0, frame.GetFormat(), frame.HasAlpha(), buffer);
  delete[] buffer;

  return true;
}


bool CTextureBundleXBT::LoadTexture(const CStdString& Filename, CBaseTexture** ppTexture,
                                     int &width, int &height)
{
  CStdStringA path = Filename;
  if (path.Find("\\media\\",0)>0)
  {
    for (int i = 0 ; i < path.Find("\\media\\",0)+6; i++)
      path.pop_back();
    path.Insert(path.size(),"\\textures.xbt");
  }
  CStdString pathW = path;
  if (!strCurrentPath.Equals(pathW))
  {
    m_XBTFReader.Close();
  }
  if (!m_XBTFReader.IsOpen())
  {
    if (!OpenBundle(path))
      return false;
    else
    {
      strCurrentPath = path;
    }
  }
    
  //clean it before
  CStdStringA trimmedname = Filename;
  trimmedname.Replace("\\","/");
  int placementofmedia = trimmedname.Find("/media/",0);
  trimmedname.Delete(0,placementofmedia+7);

  //start from other function
  CXBTFFile* file = m_XBTFReader.Find(trimmedname);
  if (!file)
    return false;

  if (file->GetFrames().size() == 0)
    return false;

  CXBTFFrame& frame = file->GetFrames().at(0);
  //end from other function

  if (!ConvertFrameToTexture(Filename, frame, ppTexture))
  {
    return false;
  }

  width = frame.GetWidth();
  height = frame.GetHeight();

  return true;
}

#if 0
int CTextureBundleXBT::LoadAnim(const CStdString& Filename, CBaseTexture*** ppTextures,
                              int &width, int &height, int& nLoops, int** ppDelays)
{
  CStdString name = Normalize(Filename);

  CXBTFFile* file = m_XBTFReader.Find(name);
  if (!file)
    return false;

  if (file->GetFrames().size() == 0)
    return false;

  size_t nTextures = file->GetFrames().size();
  *ppTextures = new CBaseTexture*[nTextures];
  *ppDelays = new int[nTextures];

  for (size_t i = 0; i < nTextures; i++)
  {
    CXBTFFrame& frame = file->GetFrames().at(i);

    if (!ConvertFrameToTexture(Filename, frame, &((*ppTextures)[i])))
    {
      return false;
    }

    (*ppDelays)[i] = frame.GetDuration();
  }

  width = file->GetFrames().at(0).GetWidth();
  height = file->GetFrames().at(0).GetHeight();
  nLoops = file->GetLoop();

  return nTextures;
}
#endif

void CTextureBundleXBT::Cleanup()
{
  if (m_XBTFReader.IsOpen())
  {
    m_XBTFReader.Close();
    //CLog::Log(LOGDEBUG, "%s - Closed %sbundle", __FUNCTION__, m_themeBundle ? "theme " : "");
  }
}

void CTextureBundleXBT::SetThemeBundle(bool themeBundle)
{
  m_themeBundle = themeBundle;
}

// normalize to how it's stored within the bundle
// lower case + using forward slash rather than back slash
CStdString CTextureBundleXBT::Normalize(const CStdString &name)
{
  CStdString newName(name);
  newName.Normalize();
  newName.Replace('\\','/');

  return newName;
}
