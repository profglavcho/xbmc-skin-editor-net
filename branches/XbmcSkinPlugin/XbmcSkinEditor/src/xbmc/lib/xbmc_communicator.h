#ifndef _XBMC_COMMUNICATOR
#define _XBMC_COMMUNICATOR
#include <string>

class XbmcCommunicator
{
public:
  XbmcCommunicator();
  ~XbmcCommunicator();
  void SendCommand(std::wstring ip,std::wstring login,std::wstring password,std::wstring wget_path, std::wstring window);
  std::wstring GetWindowId(std::wstring thewindow);
protected:
  
  
  
};

#endif