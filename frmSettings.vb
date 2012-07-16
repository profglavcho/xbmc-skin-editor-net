Public Class frmSettings

    Private Sub frmSettings_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        txtIp.Text = My.Settings.ip
        txtPort.Text = My.Settings.port
        txtUsername.Text = My.Settings.username
        txtPassword.Text = My.Settings.password
    End Sub

    Private Sub cmdSave_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles cmdSave.Click
        My.Settings.ip = txtIp.Text
        My.Settings.port = txtPort.Text
        My.Settings.username = txtUsername.Text
        My.Settings.password = txtPassword.Text
        My.Settings.Save()
        frmMain.SetXbmcCommunicator()
        Me.Close()
    End Sub

    Private Sub cmdCancel_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles cmdCancel.Click
        Me.Close()
    End Sub
End Class