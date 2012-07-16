<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class frmSettings
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.cmdSave = New System.Windows.Forms.Button()
        Me.cmdCancel = New System.Windows.Forms.Button()
        Me.brwFolder = New System.Windows.Forms.FolderBrowserDialog()
        Me.lblIp = New System.Windows.Forms.Label()
        Me.lblPort = New System.Windows.Forms.Label()
        Me.lblPassword = New System.Windows.Forms.Label()
        Me.lblUsername = New System.Windows.Forms.Label()
        Me.txtIp = New System.Windows.Forms.TextBox()
        Me.txtPort = New System.Windows.Forms.TextBox()
        Me.txtPassword = New System.Windows.Forms.TextBox()
        Me.txtUsername = New System.Windows.Forms.TextBox()
        Me.cmdChangeSkin = New System.Windows.Forms.Button()
        Me.txtSkinPath = New System.Windows.Forms.TextBox()
        Me.SuspendLayout()
        '
        'cmdSave
        '
        Me.cmdSave.Location = New System.Drawing.Point(460, 432)
        Me.cmdSave.Name = "cmdSave"
        Me.cmdSave.Size = New System.Drawing.Size(80, 23)
        Me.cmdSave.TabIndex = 0
        Me.cmdSave.Text = "Save"
        Me.cmdSave.UseVisualStyleBackColor = True
        '
        'cmdCancel
        '
        Me.cmdCancel.Location = New System.Drawing.Point(546, 432)
        Me.cmdCancel.Name = "cmdCancel"
        Me.cmdCancel.Size = New System.Drawing.Size(80, 23)
        Me.cmdCancel.TabIndex = 1
        Me.cmdCancel.Text = "Cancel"
        Me.cmdCancel.UseVisualStyleBackColor = True
        '
        'brwFolder
        '
        Me.brwFolder.Description = "Choose the skin folder"
        '
        'lblIp
        '
        Me.lblIp.AutoSize = True
        Me.lblIp.Location = New System.Drawing.Point(17, 26)
        Me.lblIp.Name = "lblIp"
        Me.lblIp.Size = New System.Drawing.Size(17, 13)
        Me.lblIp.TabIndex = 2
        Me.lblIp.Text = "IP"
        '
        'lblPort
        '
        Me.lblPort.AutoSize = True
        Me.lblPort.Location = New System.Drawing.Point(17, 48)
        Me.lblPort.Name = "lblPort"
        Me.lblPort.Size = New System.Drawing.Size(26, 13)
        Me.lblPort.TabIndex = 3
        Me.lblPort.Text = "Port"
        '
        'lblPassword
        '
        Me.lblPassword.AutoSize = True
        Me.lblPassword.Location = New System.Drawing.Point(17, 95)
        Me.lblPassword.Name = "lblPassword"
        Me.lblPassword.Size = New System.Drawing.Size(53, 13)
        Me.lblPassword.TabIndex = 5
        Me.lblPassword.Text = "Password"
        '
        'lblUsername
        '
        Me.lblUsername.AutoSize = True
        Me.lblUsername.Location = New System.Drawing.Point(17, 73)
        Me.lblUsername.Name = "lblUsername"
        Me.lblUsername.Size = New System.Drawing.Size(55, 13)
        Me.lblUsername.TabIndex = 4
        Me.lblUsername.Text = "Username"
        '
        'txtIp
        '
        Me.txtIp.Location = New System.Drawing.Point(94, 27)
        Me.txtIp.Name = "txtIp"
        Me.txtIp.Size = New System.Drawing.Size(108, 20)
        Me.txtIp.TabIndex = 6
        '
        'txtPort
        '
        Me.txtPort.Location = New System.Drawing.Point(94, 48)
        Me.txtPort.Name = "txtPort"
        Me.txtPort.Size = New System.Drawing.Size(108, 20)
        Me.txtPort.TabIndex = 7
        '
        'txtPassword
        '
        Me.txtPassword.Location = New System.Drawing.Point(94, 92)
        Me.txtPassword.Name = "txtPassword"
        Me.txtPassword.Size = New System.Drawing.Size(108, 20)
        Me.txtPassword.TabIndex = 8
        '
        'txtUsername
        '
        Me.txtUsername.Location = New System.Drawing.Point(94, 70)
        Me.txtUsername.Name = "txtUsername"
        Me.txtUsername.Size = New System.Drawing.Size(108, 20)
        Me.txtUsername.TabIndex = 9
        '
        'cmdChangeSkin
        '
        Me.cmdChangeSkin.Location = New System.Drawing.Point(20, 125)
        Me.cmdChangeSkin.Name = "cmdChangeSkin"
        Me.cmdChangeSkin.Size = New System.Drawing.Size(75, 23)
        Me.cmdChangeSkin.TabIndex = 10
        Me.cmdChangeSkin.Text = "Choose Skin"
        Me.cmdChangeSkin.UseVisualStyleBackColor = True
        '
        'txtSkinPath
        '
        Me.txtSkinPath.Location = New System.Drawing.Point(121, 127)
        Me.txtSkinPath.Name = "txtSkinPath"
        Me.txtSkinPath.Size = New System.Drawing.Size(488, 20)
        Me.txtSkinPath.TabIndex = 11
        '
        'frmSettings
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(636, 467)
        Me.Controls.Add(Me.txtSkinPath)
        Me.Controls.Add(Me.cmdChangeSkin)
        Me.Controls.Add(Me.txtUsername)
        Me.Controls.Add(Me.txtPassword)
        Me.Controls.Add(Me.txtPort)
        Me.Controls.Add(Me.txtIp)
        Me.Controls.Add(Me.lblPassword)
        Me.Controls.Add(Me.lblUsername)
        Me.Controls.Add(Me.lblPort)
        Me.Controls.Add(Me.lblIp)
        Me.Controls.Add(Me.cmdCancel)
        Me.Controls.Add(Me.cmdSave)
        Me.Name = "frmSettings"
        Me.Text = "Settings"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents cmdSave As System.Windows.Forms.Button
    Friend WithEvents cmdCancel As System.Windows.Forms.Button
    Friend WithEvents brwFolder As System.Windows.Forms.FolderBrowserDialog
    Friend WithEvents lblIp As System.Windows.Forms.Label
    Friend WithEvents lblPort As System.Windows.Forms.Label
    Friend WithEvents lblPassword As System.Windows.Forms.Label
    Friend WithEvents lblUsername As System.Windows.Forms.Label
    Friend WithEvents txtIp As System.Windows.Forms.TextBox
    Friend WithEvents txtPort As System.Windows.Forms.TextBox
    Friend WithEvents txtPassword As System.Windows.Forms.TextBox
    Friend WithEvents txtUsername As System.Windows.Forms.TextBox
    Friend WithEvents cmdChangeSkin As System.Windows.Forms.Button
    Friend WithEvents txtSkinPath As System.Windows.Forms.TextBox
End Class
