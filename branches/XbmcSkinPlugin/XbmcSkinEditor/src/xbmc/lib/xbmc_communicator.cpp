#include "xbmc_communicator.h"
#include "stdafx.h"
#include <locale>

#define WINDOW_HOME                       (L"10000")
#define WINDOW_PROGRAMS                   (L"10001")
#define WINDOW_PICTURES                   (L"10002")
#define WINDOW_FILES                      (L"10003")
#define WINDOW_SETTINGS_MENU              (L"10004")
#define WINDOW_MUSIC                      (L"10005") // virtual window to return the music start window.
#define WINDOW_VIDEOS                     (L"10006")
#define WINDOW_SYSTEM_INFORMATION         (L"10007")
#define WINDOW_TEST_PATTERN               (L"10008")
#define WINDOW_SCREEN_CALIBRATION         (L"10011")

#define WINDOW_SETTINGS_MYPICTURES        (L"10012")
#define WINDOW_SETTINGS_MYPROGRAMS        (L"10013")
#define WINDOW_SETTINGS_MYWEATHER         (L"10014")
#define WINDOW_SETTINGS_MYMUSIC           (L"10015")
#define WINDOW_SETTINGS_SYSTEM            (L"10016")
#define WINDOW_SETTINGS_MYVIDEOS          (L"10017")
#define WINDOW_SETTINGS_NETWORK           (L"10018")
#define WINDOW_SETTINGS_APPEARANCE        (L"10019")

#define WINDOW_SCRIPTS                    (L"10020") // virtual window for backward compatibility

#define WINDOW_VIDEO_FILES                (L"10024")
#define WINDOW_VIDEO_NAV                  (L"10025")
#define WINDOW_VIDEO_PLAYLIST             (L"10028")

#define WINDOW_LOGIN_SCREEN               (L"10029")
#define WINDOW_SETTINGS_PROFILES          (L"10034")

#define WINDOW_ADDON_BROWSER              (L"10040")

#define WINDOW_DIALOG_POINTER             (L"10099")
#define WINDOW_DIALOG_YES_NO              (L"10100")
#define WINDOW_DIALOG_PROGRESS            (L"10101")
#define WINDOW_DIALOG_KEYBOARD            (L"10103")
#define WINDOW_DIALOG_VOLUME_BAR          (L"10104")
#define WINDOW_DIALOG_SUB_MENU            (L"10105")
#define WINDOW_DIALOG_CONTEXT_MENU        (L"10106")
#define WINDOW_DIALOG_KAI_TOAST           (L"10107")
#define WINDOW_DIALOG_NUMERIC             (L"10109")
#define WINDOW_DIALOG_GAMEPAD             (L"10110")
#define WINDOW_DIALOG_BUTTON_MENU         (L"10111")
#define WINDOW_DIALOG_MUSIC_SCAN          (L"10112")
#define WINDOW_DIALOG_MUTE_BUG            (L"10113")
#define WINDOW_DIALOG_PLAYER_CONTROLS     (L"10114")
#define WINDOW_DIALOG_SEEK_BAR            (L"10115")
#define WINDOW_DIALOG_MUSIC_OSD           (L"10120")
#define WINDOW_DIALOG_VIS_SETTINGS        (L"10121")
#define WINDOW_DIALOG_VIS_PRESET_LIST     (L"10122")
#define WINDOW_DIALOG_VIDEO_OSD_SETTINGS  (L"10123")
#define WINDOW_DIALOG_AUDIO_OSD_SETTINGS  (L"10124")
#define WINDOW_DIALOG_VIDEO_BOOKMARKS     (L"10125")
#define WINDOW_DIALOG_FILE_BROWSER        (L"10126")
#define WINDOW_DIALOG_NETWORK_SETUP       (L"10128")
#define WINDOW_DIALOG_MEDIA_SOURCE        (L"10129")
#define WINDOW_DIALOG_PROFILE_SETTINGS    (L"10130")
#define WINDOW_DIALOG_LOCK_SETTINGS       (L"10131")
#define WINDOW_DIALOG_CONTENT_SETTINGS    (L"10132")
#define WINDOW_DIALOG_VIDEO_SCAN          (L"10133")
#define WINDOW_DIALOG_FAVOURITES          (L"10134")
#define WINDOW_DIALOG_SONG_INFO           (L"10135")
#define WINDOW_DIALOG_SMART_PLAYLIST_EDITOR (L"10136")
#define WINDOW_DIALOG_SMART_PLAYLIST_RULE   (L"10137")
#define WINDOW_DIALOG_BUSY                (L"10138")
#define WINDOW_DIALOG_PICTURE_INFO        (L"10139")
#define WINDOW_DIALOG_ADDON_SETTINGS      (L"10140")
#define WINDOW_DIALOG_ACCESS_POINTS       (L"10141")
#define WINDOW_DIALOG_FULLSCREEN_INFO     (L"10142")
#define WINDOW_DIALOG_KARAOKE_SONGSELECT  (L"10143")
#define WINDOW_DIALOG_KARAOKE_SELECTOR    (L"10144")
#define WINDOW_DIALOG_SLIDER              (L"10145")
#define WINDOW_DIALOG_ADDON_INFO          (L"10146")
#define WINDOW_DIALOG_TEXT_VIEWER         (L"10147")
#define WINDOW_DIALOG_PLAY_EJECT          (L"10148")
#define WINDOW_DIALOG_PERIPHERAL_MANAGER  (L"10149")
#define WINDOW_DIALOG_PERIPHERAL_SETTINGS (L"10150")

#define WINDOW_MUSIC_PLAYLIST             (L"10500")
#define WINDOW_MUSIC_FILES                (L"10501")
#define WINDOW_MUSIC_NAV                  (L"10502")
#define WINDOW_MUSIC_PLAYLIST_EDITOR      (L"10503")

#define WINDOW_DIALOG_OSD_TELETEXT        (L"10600")

//#define WINDOW_VIRTUAL_KEYBOARD           11000
#define WINDOW_DIALOG_SELECT              (L"12000")
#define WINDOW_DIALOG_MUSIC_INFO          (L"12001")
#define WINDOW_DIALOG_OK                  (L"12002")
#define WINDOW_DIALOG_VIDEO_INFO          (L"12003")
#define WINDOW_FULLSCREEN_VIDEO           (L"12005")
#define WINDOW_VISUALISATION              (L"12006")
#define WINDOW_SLIDESHOW                  (L"12007")
#define WINDOW_DIALOG_FILESTACKING        (L"12008")
#define WINDOW_KARAOKELYRICS              (L"12009")
#define WINDOW_WEATHER                    (L"12600")
#define WINDOW_SCREENSAVER                (L"12900")
#define WINDOW_DIALOG_VIDEO_OSD           (L"12901")

#define WINDOW_VIDEO_MENU                 (L"12902")
#define WINDOW_DIALOG_MUSIC_OVERLAY       (L"12903")
#define WINDOW_DIALOG_VIDEO_OVERLAY       (L"12904")
#define WINDOW_VIDEO_TIME_SEEK            (L"12905") // virtual window for time seeking during fullscreen video

#define WINDOW_START                      (L"12998") // first window to load
#define WINDOW_STARTUP_ANIM               (L"12999") // for startup animations

typedef struct
{
  const wchar_t* window_name;
  std::wstring xml;
  const wchar_t* window_id;
} WINDOW_CONVERT;

static const WINDOW_CONVERT windows[] =
       {{L"home"                     , L"home.xml", WINDOW_HOME},
        {L"programs"                 , L"myprograms.xml", WINDOW_PROGRAMS},
        {L"pictures"                 , L"mypics.xml", WINDOW_PICTURES},
        {L"filemanager"              , L"filemanager.xml", WINDOW_FILES},
        {L"files"                    , L"filemanager.xml", WINDOW_FILES}, // backward compat
        {L"settings"                 , L"settings.xml", WINDOW_SETTINGS_MENU},
        {L"music"                    , L"home.xml", WINDOW_MUSIC},
        {L"video"                    , L"home.xml", WINDOW_VIDEOS},
        {L"videos"                   , L"home.xml", WINDOW_VIDEO_NAV},
        {L"systeminfo"               , L"settingssysteminfo.xml", WINDOW_SYSTEM_INFORMATION},
        {L"testpattern"              , L"", WINDOW_TEST_PATTERN},
        {L"screencalibration"        , L"settingsscreencalibration.xml", WINDOW_SCREEN_CALIBRATION},
        {L"guicalibration"           , L".xml", WINDOW_SCREEN_CALIBRATION}, // backward compat
        {L"picturessettings"         , L"settingscategory.xml", WINDOW_SETTINGS_MYPICTURES},
        {L"programssettings"         , L"settingscategory.xml", WINDOW_SETTINGS_MYPROGRAMS},
        {L"weathersettings"          , L"settingscategory.xml", WINDOW_SETTINGS_MYWEATHER},
        {L"musicsettings"            , L"settingscategory.xml", WINDOW_SETTINGS_MYMUSIC},
        {L"systemsettings"           , L"settingscategory.xml", WINDOW_SETTINGS_SYSTEM},
        {L"videossettings"           , L"settingscategory.xml", WINDOW_SETTINGS_MYVIDEOS},
        {L"networksettings"          , L"settingscategory.xml", WINDOW_SETTINGS_NETWORK},
        {L"appearancesettings"       , L"settingscategory.xml", WINDOW_SETTINGS_APPEARANCE},
        {L"scripts"                  , L"myscripts.xml", WINDOW_PROGRAMS}, // backward compat
        {L"videofiles"               , L"myvideo.xml", WINDOW_VIDEO_FILES},
        {L"videolibrary"             , L"myvideonav.xml", WINDOW_VIDEO_NAV},
        {L"videoplaylist"            , L"myvideoplaylist.xml", WINDOW_VIDEO_PLAYLIST},
        {L"loginscreen"              , L"loginscreen.xml", WINDOW_LOGIN_SCREEN},
        {L"profiles"                 , L"settingsprofile.xml", WINDOW_SETTINGS_PROFILES},
        {L"addonbrowser"             , L"addonbrowser.xml", WINDOW_ADDON_BROWSER},
        {L"yesnodialog"              , L"dialogyesno.xml", WINDOW_DIALOG_YES_NO},
        {L"progressdialog"           , L"home.xml", WINDOW_DIALOG_PROGRESS},
        {L"virtualkeyboard"          , L"", WINDOW_DIALOG_KEYBOARD},
        {L"volumebar"                , L"", WINDOW_DIALOG_VOLUME_BAR},
        {L"submenu"                  , L"", WINDOW_DIALOG_SUB_MENU},
        {L"favourites"               , L"", WINDOW_DIALOG_FAVOURITES},
        {L"contextmenu"              , L"", WINDOW_DIALOG_CONTEXT_MENU},
        {L"infodialog"               , L"", WINDOW_DIALOG_KAI_TOAST},
        {L"numericinput"             , L"", WINDOW_DIALOG_NUMERIC},
        {L"gamepadinput"             , L"", WINDOW_DIALOG_GAMEPAD},
        {L"shutdownmenu"             , L"", WINDOW_DIALOG_BUTTON_MENU},
        {L"musicscan"                , L"", WINDOW_DIALOG_MUSIC_SCAN},
        {L"mutebug"                  , L"", WINDOW_DIALOG_MUTE_BUG},
        {L"playercontrols"           , L"", WINDOW_DIALOG_PLAYER_CONTROLS},
        {L"seekbar"                  , L"", WINDOW_DIALOG_SEEK_BAR},
        {L"musicosd"                 , L"", WINDOW_DIALOG_MUSIC_OSD},
        {L"addonsettings"            , L"", WINDOW_DIALOG_ADDON_SETTINGS},
        {L"visualisationsettings"    , L"", WINDOW_DIALOG_ADDON_SETTINGS}, // backward compat
        {L"visualisationpresetlist"  , L"", WINDOW_DIALOG_VIS_PRESET_LIST},
        {L"osdvideosettings"         , L"", WINDOW_DIALOG_VIDEO_OSD_SETTINGS},
        {L"osdaudiosettings"         , L"", WINDOW_DIALOG_AUDIO_OSD_SETTINGS},
        {L"videobookmarks"           , L"", WINDOW_DIALOG_VIDEO_BOOKMARKS},
        {L"filebrowser"              , L"", WINDOW_DIALOG_FILE_BROWSER},
        {L"networksetup"             , L"", WINDOW_DIALOG_NETWORK_SETUP},
        {L"mediasource"              , L"", WINDOW_DIALOG_MEDIA_SOURCE},
        {L"profilesettings"          , L"", WINDOW_DIALOG_PROFILE_SETTINGS},
        {L"locksettings"             , L"", WINDOW_DIALOG_LOCK_SETTINGS},
        {L"contentsettings"          , L"", WINDOW_DIALOG_CONTENT_SETTINGS},
        {L"videoscan"                , L"", WINDOW_DIALOG_VIDEO_SCAN},
        {L"favourites"               , L"", WINDOW_DIALOG_FAVOURITES},
        {L"songinformation"          , L"", WINDOW_DIALOG_SONG_INFO},
        {L"smartplaylisteditor"      , L"", WINDOW_DIALOG_SMART_PLAYLIST_EDITOR},
        {L"smartplaylistrule"        , L"", WINDOW_DIALOG_SMART_PLAYLIST_RULE},
        {L"busydialog"               , L"", WINDOW_DIALOG_BUSY},
        {L"pictureinfo"              , L"", WINDOW_DIALOG_PICTURE_INFO},
        {L"accesspoints"             , L"", WINDOW_DIALOG_ACCESS_POINTS},
        {L"fullscreeninfo"           , L"", WINDOW_DIALOG_FULLSCREEN_INFO},
        {L"karaokeselector"          , L"", WINDOW_DIALOG_KARAOKE_SONGSELECT},
        {L"karaokelargeselector"     , L"", WINDOW_DIALOG_KARAOKE_SELECTOR},
        {L"sliderdialog"             , L"", WINDOW_DIALOG_SLIDER},
        {L"addoninformation"         , L"", WINDOW_DIALOG_ADDON_INFO},
        {L"musicplaylist"            , L"", WINDOW_MUSIC_PLAYLIST},
        {L"musicfiles"               , L"", WINDOW_MUSIC_FILES},
        {L"musiclibrary"             , L"", WINDOW_MUSIC_NAV},
        {L"musicplaylisteditor"      , L"", WINDOW_MUSIC_PLAYLIST_EDITOR},
        {L"teletext"                 , L"", WINDOW_DIALOG_OSD_TELETEXT},
        {L"selectdialog"             , L"", WINDOW_DIALOG_SELECT},
        {L"musicinformation"         , L"", WINDOW_DIALOG_MUSIC_INFO},
        {L"okdialog"                 , L"", WINDOW_DIALOG_OK},
        {L"movieinformation"         , L"", WINDOW_DIALOG_VIDEO_INFO},
        {L"textviewer"               , L"", WINDOW_DIALOG_TEXT_VIEWER},
        {L"fullscreenvideo"          , L"", WINDOW_FULLSCREEN_VIDEO},
        {L"visualisation"            , L"", WINDOW_VISUALISATION},
        {L"slideshow"                , L"", WINDOW_SLIDESHOW},
        {L"filestackingdialog"       , L"", WINDOW_DIALOG_FILESTACKING},
        {L"karaoke"                  , L"", WINDOW_KARAOKELYRICS},
        {L"weather"                  , L"", WINDOW_WEATHER},
        {L"screensaver"              , L"", WINDOW_SCREENSAVER},
        {L"videoosd"                 , L"", WINDOW_DIALOG_VIDEO_OSD},
        {L"videomenu"                , L"", WINDOW_VIDEO_MENU},
        {L"videotimeseek"            , L"", WINDOW_VIDEO_TIME_SEEK},
        {L"musicoverlay"             , L"", WINDOW_DIALOG_MUSIC_OVERLAY},
        {L"videooverlay"             , L"", WINDOW_DIALOG_VIDEO_OVERLAY},
        {L"startwindow"              , L"", WINDOW_START},
        {L"startup"                  , L"", WINDOW_STARTUP_ANIM},
        {L"peripherals"              , L"", WINDOW_DIALOG_PERIPHERAL_MANAGER},
        {L"peripheralsettings"       , L"", WINDOW_DIALOG_PERIPHERAL_SETTINGS}
};

XbmcCommunicator::XbmcCommunicator()
{
}

XbmcCommunicator::~XbmcCommunicator()
{

}

std::wstring XbmcCommunicator::GetWindowId(std::wstring thewindow)
{
  for(int i=0; i<thewindow.size();++i)
   thewindow[i] = tolower(thewindow[i]);
  for (unsigned int i = 0; i < sizeof(windows)/sizeof(WINDOW_CONVERT); i++)
  {
    if (windows[i].xml.compare(thewindow) == 0)
    {
      return windows[i].window_id;
    }
  }
  return thewindow;
}

void XbmcCommunicator::SendCommand(std::wstring ip, std::wstring login, std::wstring password, std::wstring wget_path, std::wstring window)
{
  //wget --http-user=xbox --http-password=xbox "http://192.168.0.109/xbmcCmds/xbmcHttp?command=ExecBuiltIn&parameter=ActivateWindow(10000)"
  wchar_t a = 34; //the chr 34 is the ""
  std::wstring command;
  std::wstring args;
  command.push_back(a);

  for(std::wstring::iterator it = wget_path.begin(); it != wget_path.end(); it++)
    command.push_back(*it);
  
  std::wstring string1 = L"\\";
  
  //"J:\\Coding\\XBMC_MAIN\\project\\BuildDependencies\\bin\\wget.exe --http-user=xbox --http-password=xbox ";
  if (command.find_last_of(L"\\") != (command.length()-1))
  {
    command.push_back(string1.front());
  }
  
  string1 = L"wget.exe";
  for(std::wstring::iterator it = string1.begin(); it != string1.end(); it++)
    command.push_back(*it);

  command.push_back(a);

  string1 = L"--http-user=";
  for(std::wstring::iterator it = string1.begin(); it != string1.end(); it++)
    args.push_back(*it);

  for(std::wstring::iterator it = login.begin(); it != login.end(); it++)
    args.push_back(*it);

  string1 = L" --http-password=";
  for(std::wstring::iterator it = string1.begin(); it != string1.end(); it++)
    args.push_back(*it);

  for(std::wstring::iterator it = password.begin(); it != password.end(); it++)
    args.push_back(*it);

  args.push_back(wchar_t(32));

  args.push_back(a);

  string1 = L"http://";
  for(std::wstring::iterator it = string1.begin(); it != string1.end(); it++)
    args.push_back(*it);

  for(std::wstring::iterator it = ip.begin(); it != ip.end(); it++)
    args.push_back(*it);

  string1 = L"/xbmcCmds/xbmcHttp?command=ExecBuiltIn&parameter=ActivateWindow(";
  for(std::wstring::iterator it = string1.begin(); it != string1.end(); it++)
    args.push_back(*it);

  std::wstring ttwindow = GetWindowId(window);
  for(std::wstring::iterator it = ttwindow.begin(); it != ttwindow.end(); it++)
    args.push_back(*it);

  string1 = L")";
  for(std::wstring::iterator it = string1.begin(); it != string1.end(); it++)
    args.push_back(*it);

  args.push_back(a);
  //std::string url = "http://192.168.0.109/xbmcCmds/xbmcHttp?command=ExecBuiltIn&parameter=ActivateWindow(10000)";



  STARTUPINFOW si;
  memset(&si, 0, sizeof(si));
  si.cb = sizeof(si);
  si.dwFlags = STARTF_USESHOWWINDOW;
  si.wShowWindow = SW_HIDE;

  PROCESS_INFORMATION m_processInfo;
  command.push_back(wchar_t(32));

  for(std::wstring::iterator it = args.begin(); it != args.end(); it++)
    command.push_back(*it);
  BOOL ret = CreateProcessW(NULL,(LPWSTR)command.c_str(), NULL, NULL, FALSE, NULL, 
                            NULL, wget_path.c_str(), &si, &m_processInfo);

  if (ret == FALSE)
  {
    DWORD lastError = GetLastError();
    HRESULT hr = HRESULT_FROM_WIN32(lastError);
    return;
  }
  int res = WaitForSingleObject(m_processInfo.hProcess, INFINITE);

  CloseHandle(m_processInfo.hThread);
  m_processInfo.hThread = 0;
  CloseHandle(m_processInfo.hProcess);
  m_processInfo.hProcess = 0;

  return;
}