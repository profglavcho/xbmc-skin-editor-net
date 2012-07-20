Public Class frmOpenSkin

    Dim skinfolder As New List(Of IO.DirectoryInfo)

    Private Sub frmOpenSkin_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Dim strfolder As String = System.Environment.GetFolderPath(System.Environment.SpecialFolder.ApplicationData)
        If IO.Directory.Exists(strfolder + "\xbmc\addons") = False Then
            Exit Sub
        End If
        Dim addonFolder As New IO.DirectoryInfo(strfolder + "\xbmc\addons")

        For Each Dir As IO.DirectoryInfo In addonFolder.GetDirectories()

            If Dir.Name.StartsWith("skin.") Then
                'its a skin so add it
                'TODO
                'don't get the name of the folder but load it from the addon.xml
                skinfolder.Add(Dir)
            End If
        Next
        For Each skin As IO.DirectoryInfo In skinfolder
            Try
                lstSkinList.Items.Add(skin.Name)
            Catch ex As Exception
                Debug.Print(ex.Message)
            End Try

        Next
        cmdOpen.Enabled = False
    End Sub

    Private Sub lstSkinList_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles lstSkinList.SelectedIndexChanged
        If lstSkinList.SelectedItems().Count = 0 Then
            cmdOpen.Enabled = False
            Exit Sub
        End If
        cmdOpen.Enabled = True
    End Sub

    Private Sub cmdOpen_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cmdOpen.Click
        If lstSkinList.SelectedIndex() > -1 = False Then Exit Sub

        frmMain.LoadSkin(skinfolder(lstSkinList.SelectedIndex).FullName)
        Me.Close()
    End Sub
End Class