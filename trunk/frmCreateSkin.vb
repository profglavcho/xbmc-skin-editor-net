Public Class frmCreateSkin
    Dim addonfile As IO.FileInfo
    Dim writer As IO.StreamWriter
    Dim resDirectory As IO.DirectoryInfo

    Private Sub AddWindow(ByVal id As String, ByVal file As String)
        'don't overwrite existing file
        If IO.File.Exists(resDirectory.FullName + "\" + file) = True Then
            Exit Sub
        End If
        Dim windowfile As New IO.FileInfo(resDirectory.FullName + "\" + file)

        writer = IO.File.CreateText(windowfile.FullName)
        writer.WriteLine("<window id=""" + id + """>")
        writer.WriteLine("</window>")
        writer.Close()
    End Sub
    Private Sub CreateDefaultWindowXml()
        AddWindow("10000", "Home.xml")
        AddWindow("10001", "MyPrograms.xml")
        AddWindow("10002", "MyPics.xml")
        AddWindow("10003", "FileManager.xml")
        AddWindow("10004", "Settings.xml")
        AddWindow("10005", "SettingsSystemInfo.xml")
        AddWindow("10011", "SettingsScreenCalibration.xml")
        AddWindow("10012", "SettingsCategory.xml")
        AddWindow("10013", "SettingsCategory.xml")
        AddWindow("10017", "SettingsCategory.xml")
        AddWindow("10020", "MyScripts.xml")
        AddWindow("10024", "MyVideo.xml")
        AddWindow("10025", "MyVideoNav.xml")
        AddWindow("10028", "MyVideoPlaylist.xml")
        AddWindow("10029", "LoginScreen.xml")
        AddWindow("10034", "SettingsProfile.xml")
        AddWindow("10040", "AddonBrowser.xml")
        AddWindow("10100", "DialogYesNo.xml")
        AddWindow("10101", "DialogProgress.xml")
        AddWindow("10103", "DialogKeyboard.xml")
        AddWindow("10104", "DialogVolumeBar.xml")
        AddWindow("10106", "DialogContextMenu.xml")
        AddWindow("10107", "DialogKaiToast.xml")
        AddWindow("10109", "DialogNumeric.xml")
        AddWindow("10111", "DialogButtonMenu.xml")
        AddWindow("10112", "DialogMusicScan.xml")
        AddWindow("10113", "DialogMuteBug.xml")
        AddWindow("10114", "PlayerControls.xml")
        AddWindow("10115", "DialogSeekBar.xml")
        AddWindow("10120", "MusicOSD.xml")
        AddWindow("10122", "VisualizationPresetList.xml")
        AddWindow("10123", "VideoOSDSettings.xml")
        AddWindow("10124", "VideoOSDSettings.xml")
        AddWindow("10125", "VideoOSDBookmarks.xml")
        AddWindow("10126", "FileBrowser.xml")
        AddWindow("10128", "DialogNetworkSetup.xml")
        AddWindow("10129", "DialogMediaSource.xml")
        AddWindow("10130", "ProfileSettings.xml")
        AddWindow("10131", "LockSettings.xml")
        AddWindow("10132", "DialogContentSettings.xml")
        AddWindow("10133", "DialogVideoScan.xml")
        AddWindow("10134", "DialogFavourites.xml")
        AddWindow("10135", "DialogSongInfo.xml")
        AddWindow("10136", "SmartPlaylistEditor.xml")
        AddWindow("10137", "SmartPlaylistRule.xml")
        AddWindow("10138", "DialogBusy.xml")
        AddWindow("10139", "DialogPictureInfo.xml")
        AddWindow("10140", "DialogAddonSettings.xml")
        AddWindow("10141", "DialogAccessPoints.xml")
        AddWindow("10142", "DialogFullScreenInfo.xml")
        AddWindow("10143", "DialogKaraokeSongSelector.xml")
        AddWindow("10144", "DialogKaraokeSongSelectorLarge.xml")
        AddWindow("10145", "DialogSlider.xml")
        AddWindow("10146", "DialogAddonInfo.xml")
        AddWindow("10147", "DialogTextViewer.xml")
        AddWindow("10149", "DialogPeripheralManager.xml")
        AddWindow("10150", "DialogPeripheralSettings.xml")
        AddWindow("10500", "MyMusicPlaylist.xml")
        AddWindow("10501", "MyMusicSongs.xml")
        AddWindow("10502", "MyMusicNav.xml")
        AddWindow("10503", "MyMusicPlaylistEditor.xml")
        AddWindow("11000", "DialogSelect.xml")
        AddWindow("12001", "DialogAlbumInfo.xml")
        AddWindow("12002", "DialogOK.xml")
        AddWindow("12003", "DialogVideoInfo.xml")
        AddWindow("12005", "VideoFullScreen.xml")
        AddWindow("12006", "MusicVisualisation.xml")
        AddWindow("12007", "SlideShow.xml")
        AddWindow("12008", "DialogFileStacking.xml")
        AddWindow("12009", "MusicKaraokeLyrics.xml")
        AddWindow("12600", "MyWeather.xml")
        AddWindow("12900", "VideoOSD.xml")
        AddWindow("12902", "startup.xml")


    End Sub

    Private Sub CreateSkin()
        Dim appData As String = System.Environment.GetFolderPath(System.Environment.SpecialFolder.ApplicationData)
        Dim SkinDir As New IO.DirectoryInfo(appData + "\xbmc")

        Dim langsum As New List(Of RichTextBoxEx.doubletextbox.Lang_Sum)
        Dim langdesc As New List(Of RichTextBoxEx.doubletextbox.Lang_Sum)
        langsum = Doubletextbox1.LangSum
        langdesc = Doubletextbox2.LangSum

        SkinDir = IO.Directory.CreateDirectory(SkinDir.FullName + "\addons\" + txtId.TextboxText)
        'create sub dir
        resDirectory = IO.Directory.CreateDirectory(SkinDir.FullName + "\" + txtDefaultRes.TextboxText)
        IO.Directory.CreateDirectory(SkinDir.FullName + "\colors")
        IO.Directory.CreateDirectory(SkinDir.FullName + "\fonts")
        IO.Directory.CreateDirectory(SkinDir.FullName + "\language")
        IO.Directory.CreateDirectory(SkinDir.FullName + "\media")
        IO.Directory.CreateDirectory(SkinDir.FullName + "\sounds")

        addonfile = New IO.FileInfo(SkinDir.FullName + "\" + "addon.xml")
        writer = IO.File.CreateText(addonfile.FullName)
        writer.WriteLine("<?xml version=""1.0"" encoding=""UTF-8""?>")
        writer.WriteLine("<addon")
        writer.WriteLine("  id=""" + txtId.TextboxText + """")
        writer.WriteLine("  version=""" + txtVersion.TextboxText + """")
        writer.WriteLine("  name=""" + txtName.TextboxText + """")
        writer.WriteLine("  provider-name=""" + txtProvider.TextboxText + """>")
        writer.WriteLine("  <requires>")
        writer.WriteLine("    <import addon=""xbmc.gui"" version=""" + txtGuiVersion.TextboxText + """/>")
        writer.WriteLine("  </requires>")
        writer.WriteLine("  <extension")
        writer.WriteLine("    point=""" + txtPoint.TextboxText + """")
        writer.WriteLine("    defaultresolution=""" + txtDefaultRes.TextboxText + """")
        writer.WriteLine("    defaultresolutionwide=""" + txtDefaultResWide.TextboxText + """")
        writer.WriteLine("    defaultthemename=""" + txtDefaultTheme.TextboxText + """")
        writer.WriteLine("    effectslowdown=""" + txtEffectSlowDown.TextboxText + """")
        writer.WriteLine("    debugging=""" + txtDebugging.TextboxText + """/>")
        writer.WriteLine("  <extension point=""xbmc.addon.metadata"">""")
        For Each lg As RichTextBoxEx.doubletextbox.Lang_Sum In langsum
            writer.WriteLine("    <summary ""lang""" + lg.lang + """>" + lg.sum + "</summary>")
        Next
        For Each lg As RichTextBoxEx.doubletextbox.Lang_Sum In langdesc
            writer.WriteLine("    <description ""lang""" + lg.lang + """>" + lg.sum + "</description>")
        Next
        writer.WriteLine("    <platform>" + txtPlatform.TextboxText + "</platform>")
        writer.WriteLine("  </extension>")
        writer.WriteLine("</addon>")

        writer.Close()

        CreateDefaultWindowXml()
        MsgBox("skin created in directory " + SkinDir.FullName)
        Me.Close()
    End Sub

    Private Sub cmdAddLanguage_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cmdAddLanguage.Click
        Doubletextbox1.AddItem()
        Doubletextbox2.AddItem()
    End Sub

    Private Sub cmdCreate_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cmdCreate.Click
        CreateSkin()
    End Sub

    Private Sub cmdCancel_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cmdCancel.Click
        Me.Close()
    End Sub

    Private Sub frmCreateSkin_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

    End Sub
End Class