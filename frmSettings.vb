Public Class frmSettings

    Private Sub frmSettings_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        txtIp.Text = My.Settings.ip
        txtPort.Text = My.Settings.port
        txtUsername.Text = My.Settings.username
        txtPassword.Text = My.Settings.password
        txtSkinPath.Text = My.Settings.currentskin
    End Sub

    Private Sub cmdSave_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles cmdSave.Click
        My.Settings.ip = txtIp.Text
        My.Settings.port = txtPort.Text
        My.Settings.username = txtUsername.Text
        My.Settings.password = txtPassword.Text

        If My.Settings.currentskin.Contains(txtSkinPath.Text) = False Then
            frmMain.LoadSkin(txtSkinPath.Text)
        End If
        My.Settings.currentskin = txtSkinPath.Text
        My.Settings.Save()
        frmMain.SetXbmcCommunicator()

        Me.Close()
    End Sub

    Private Sub cmdCancel_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles cmdCancel.Click
        Me.Close()
    End Sub

    Private Sub cmdChangeSkin_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles cmdChangeSkin.Click
        brwFolder.SelectedPath = txtSkinPath.Text
        If brwFolder.ShowDialog = Windows.Forms.DialogResult.OK Then
            txtSkinPath.Text = brwFolder.SelectedPath

        End If
    End Sub
End Class