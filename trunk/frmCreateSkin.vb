Public Class frmCreateSkin
    Dim addonfile As IO.FileInfo
    Dim writer As IO.StreamWriter
    Dim resDirectory As IO.DirectoryInfo

    Private Sub WriteLine(ByVal line As String)
        writer.WriteLine(line)
    End Sub

    Private Sub CreateDefaultWindowXml()
        'addonfile = New IO.FileInfo(resDirectory.FullName + "\" + "addon.xml")
        'writer = IO.File.CreateText(addonfile.FullName)

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
        WriteLine("<?xml version=""1.0"" encoding=""UTF-8""?>")
        WriteLine("<addon")
        WriteLine("  id=""" + txtId.TextboxText + """")
        WriteLine("  version=""" + txtVersion.TextboxText + """")
        WriteLine("  name=""" + txtName.TextboxText + """")
        WriteLine("  provider-name=""" + txtProvider.TextboxText + """>")
        WriteLine("  <requires>")
        WriteLine("    <import addon=""xbmc.gui"" version=""" + txtGuiVersion.TextboxText + """/>")
        WriteLine("  </requires>")
        WriteLine("  <extension")
        WriteLine("    point=""" + txtPoint.TextboxText + """")
        WriteLine("    defaultresolution=""" + txtDefaultRes.TextboxText + """")
        WriteLine("    defaultresolutionwide=""" + txtDefaultResWide.TextboxText + """")
        WriteLine("    defaultthemename=""" + txtDefaultTheme.TextboxText + """")
        WriteLine("    effectslowdown=""" + txtEffectSlowDown.TextboxText + """")
        WriteLine("    debugging=""" + txtDebugging.TextboxText + """/>")
        WriteLine("  <extension point=""xbmc.addon.metadata"">""")
        For Each lg As RichTextBoxEx.doubletextbox.Lang_Sum In langsum
            WriteLine("    <summary ""lang""" + lg.lang + """>" + lg.sum + "</summary>")
        Next
        For Each lg As RichTextBoxEx.doubletextbox.Lang_Sum In langdesc
            WriteLine("    <description ""lang""" + lg.lang + """>" + lg.sum + "</description>")
        Next
        WriteLine("    <platform>" + txtPlatform.TextboxText + "</platform>")
        WriteLine("  </extension>")
        WriteLine("</addon>")

        writer.Close()

        CreateDefaultWindowXml()

    End Sub

    Private Sub cmdAddLanguage_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cmdAddLanguage.Click
        Doubletextbox1.AddItem()
        Doubletextbox2.AddItem()
    End Sub

    Private Sub cmdCreate_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cmdCreate.Click

        CreateSkin()

    End Sub
End Class