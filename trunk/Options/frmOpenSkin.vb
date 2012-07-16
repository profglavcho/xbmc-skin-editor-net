Public Class frmOpenSkin
    Dim skinfolder As New List(Of String)

    Private Sub frmOpenSkin_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Dim addonfolder As String = "C:\Users\ben\AppData\Roaming\XBMC\addons"
        For Each Dir As String In IO.Directory.GetDirectories(addonfolder)

            If Dir.Contains("skin.") Then
                'its a skin so add it
                skinfolder.Add(Dir)
            End If
        Next
        For Each skin As String In skinfolder
            Try
                lstSkinList.Items.Add(skin)
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
        My.Forms.frmMain.LoadSkin(lstSkinList.SelectedItem().ToString())
        Me.Close()
    End Sub
End Class