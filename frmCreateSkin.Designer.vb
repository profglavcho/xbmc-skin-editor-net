<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class frmCreateSkin
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
        Me.cmdCancel = New System.Windows.Forms.Button()
        Me.cmdCreate = New System.Windows.Forms.Button()
        Me.Panel1 = New System.Windows.Forms.Panel()
        Me.txtDebugging = New RichTextBoxEx.TextButton()
        Me.txtEffectSlowDown = New RichTextBoxEx.TextButton()
        Me.txtDefaultTheme = New RichTextBoxEx.TextButton()
        Me.txtDefaultResWide = New RichTextBoxEx.TextButton()
        Me.txtDefaultRes = New RichTextBoxEx.TextButton()
        Me.txtPoint = New RichTextBoxEx.TextButton()
        Me.lblExtension = New System.Windows.Forms.Label()
        Me.Panel2 = New System.Windows.Forms.Panel()
        Me.Label3 = New System.Windows.Forms.Label()
        Me.Label2 = New System.Windows.Forms.Label()
        Me.Doubletextbox2 = New RichTextBoxEx.doubletextbox()
        Me.Doubletextbox1 = New RichTextBoxEx.doubletextbox()
        Me.cmdAddLanguage = New System.Windows.Forms.Button()
        Me.txtPlatform = New RichTextBoxEx.TextButton()
        Me.Label1 = New System.Windows.Forms.Label()
        Me.txtGuiVersion = New RichTextBoxEx.TextButton()
        Me.txtProvider = New RichTextBoxEx.TextButton()
        Me.txtName = New RichTextBoxEx.TextButton()
        Me.txtVersion = New RichTextBoxEx.TextButton()
        Me.txtId = New RichTextBoxEx.TextButton()
        Me.Panel1.SuspendLayout()
        Me.Panel2.SuspendLayout()
        Me.SuspendLayout()
        '
        'cmdCancel
        '
        Me.cmdCancel.Location = New System.Drawing.Point(642, 486)
        Me.cmdCancel.Name = "cmdCancel"
        Me.cmdCancel.Size = New System.Drawing.Size(80, 23)
        Me.cmdCancel.TabIndex = 3
        Me.cmdCancel.Text = "Cancel"
        Me.cmdCancel.UseVisualStyleBackColor = True
        '
        'cmdCreate
        '
        Me.cmdCreate.Location = New System.Drawing.Point(556, 486)
        Me.cmdCreate.Name = "cmdCreate"
        Me.cmdCreate.Size = New System.Drawing.Size(80, 23)
        Me.cmdCreate.TabIndex = 2
        Me.cmdCreate.Text = "Create"
        Me.cmdCreate.UseVisualStyleBackColor = True
        '
        'Panel1
        '
        Me.Panel1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.Panel1.Controls.Add(Me.txtDebugging)
        Me.Panel1.Controls.Add(Me.txtEffectSlowDown)
        Me.Panel1.Controls.Add(Me.txtDefaultTheme)
        Me.Panel1.Controls.Add(Me.txtDefaultResWide)
        Me.Panel1.Controls.Add(Me.txtDefaultRes)
        Me.Panel1.Controls.Add(Me.txtPoint)
        Me.Panel1.Controls.Add(Me.lblExtension)
        Me.Panel1.Location = New System.Drawing.Point(2, 182)
        Me.Panel1.Name = "Panel1"
        Me.Panel1.Size = New System.Drawing.Size(332, 221)
        Me.Panel1.TabIndex = 11
        '
        'txtDebugging
        '
        Me.txtDebugging.Align = System.Windows.Forms.HorizontalAlignment.Right
        Me.txtDebugging.ButtonBorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.txtDebugging.LabelText = "debugging"
        Me.txtDebugging.Location = New System.Drawing.Point(3, 187)
        Me.txtDebugging.Name = "txtDebugging"
        Me.txtDebugging.Size = New System.Drawing.Size(322, 28)
        Me.txtDebugging.TabIndex = 20
        Me.txtDebugging.TextboxText = "false"
        '
        'txtEffectSlowDown
        '
        Me.txtEffectSlowDown.Align = System.Windows.Forms.HorizontalAlignment.Right
        Me.txtEffectSlowDown.ButtonBorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.txtEffectSlowDown.LabelText = "effectslowdown"
        Me.txtEffectSlowDown.Location = New System.Drawing.Point(3, 153)
        Me.txtEffectSlowDown.Name = "txtEffectSlowDown"
        Me.txtEffectSlowDown.Size = New System.Drawing.Size(322, 28)
        Me.txtEffectSlowDown.TabIndex = 19
        Me.txtEffectSlowDown.TextboxText = "1.0"
        '
        'txtDefaultTheme
        '
        Me.txtDefaultTheme.Align = System.Windows.Forms.HorizontalAlignment.Right
        Me.txtDefaultTheme.ButtonBorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.txtDefaultTheme.LabelText = "default theme name"
        Me.txtDefaultTheme.Location = New System.Drawing.Point(3, 119)
        Me.txtDefaultTheme.Name = "txtDefaultTheme"
        Me.txtDefaultTheme.Size = New System.Drawing.Size(322, 28)
        Me.txtDefaultTheme.TabIndex = 17
        Me.txtDefaultTheme.TextboxText = "textures.xbt"
        '
        'txtDefaultResWide
        '
        Me.txtDefaultResWide.Align = System.Windows.Forms.HorizontalAlignment.Right
        Me.txtDefaultResWide.ButtonBorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.txtDefaultResWide.LabelText = "defaultresolutionwide"
        Me.txtDefaultResWide.Location = New System.Drawing.Point(3, 85)
        Me.txtDefaultResWide.Name = "txtDefaultResWide"
        Me.txtDefaultResWide.Size = New System.Drawing.Size(322, 28)
        Me.txtDefaultResWide.TabIndex = 15
        Me.txtDefaultResWide.TextboxText = "720p"
        '
        'txtDefaultRes
        '
        Me.txtDefaultRes.Align = System.Windows.Forms.HorizontalAlignment.Right
        Me.txtDefaultRes.ButtonBorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.txtDefaultRes.LabelText = "defaultresolution"
        Me.txtDefaultRes.Location = New System.Drawing.Point(3, 55)
        Me.txtDefaultRes.Name = "txtDefaultRes"
        Me.txtDefaultRes.Size = New System.Drawing.Size(322, 28)
        Me.txtDefaultRes.TabIndex = 14
        Me.txtDefaultRes.TextboxText = "720p"
        '
        'txtPoint
        '
        Me.txtPoint.Align = System.Windows.Forms.HorizontalAlignment.Right
        Me.txtPoint.ButtonBorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.txtPoint.LabelText = "point"
        Me.txtPoint.Location = New System.Drawing.Point(3, 21)
        Me.txtPoint.Name = "txtPoint"
        Me.txtPoint.Size = New System.Drawing.Size(322, 28)
        Me.txtPoint.TabIndex = 13
        Me.txtPoint.TextboxText = "xbmc.gui.skin"
        '
        'lblExtension
        '
        Me.lblExtension.AutoSize = True
        Me.lblExtension.Location = New System.Drawing.Point(8, 5)
        Me.lblExtension.Name = "lblExtension"
        Me.lblExtension.Size = New System.Drawing.Size(56, 13)
        Me.lblExtension.TabIndex = 0
        Me.lblExtension.Text = "Extension:"
        '
        'Panel2
        '
        Me.Panel2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.Panel2.Controls.Add(Me.Label3)
        Me.Panel2.Controls.Add(Me.Label2)
        Me.Panel2.Controls.Add(Me.Doubletextbox2)
        Me.Panel2.Controls.Add(Me.Doubletextbox1)
        Me.Panel2.Controls.Add(Me.cmdAddLanguage)
        Me.Panel2.Controls.Add(Me.txtPlatform)
        Me.Panel2.Controls.Add(Me.Label1)
        Me.Panel2.Location = New System.Drawing.Point(344, 13)
        Me.Panel2.Name = "Panel2"
        Me.Panel2.Size = New System.Drawing.Size(378, 467)
        Me.Panel2.TabIndex = 12
        '
        'Label3
        '
        Me.Label3.AutoSize = True
        Me.Label3.Location = New System.Drawing.Point(3, 224)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(60, 13)
        Me.Label3.TabIndex = 26
        Me.Label3.Text = "Description"
        '
        'Label2
        '
        Me.Label2.AutoSize = True
        Me.Label2.Location = New System.Drawing.Point(3, 31)
        Me.Label2.Name = "Label2"
        Me.Label2.Size = New System.Drawing.Size(50, 13)
        Me.Label2.TabIndex = 25
        Me.Label2.Text = "Summary"
        '
        'Doubletextbox2
        '
        Me.Doubletextbox2.Location = New System.Drawing.Point(6, 236)
        Me.Doubletextbox2.Name = "Doubletextbox2"
        Me.Doubletextbox2.Size = New System.Drawing.Size(320, 28)
        Me.Doubletextbox2.TabIndex = 24
        '
        'Doubletextbox1
        '
        Me.Doubletextbox1.Location = New System.Drawing.Point(6, 43)
        Me.Doubletextbox1.Name = "Doubletextbox1"
        Me.Doubletextbox1.Size = New System.Drawing.Size(320, 28)
        Me.Doubletextbox1.TabIndex = 23
        '
        'cmdAddLanguage
        '
        Me.cmdAddLanguage.Location = New System.Drawing.Point(65, 2)
        Me.cmdAddLanguage.Name = "cmdAddLanguage"
        Me.cmdAddLanguage.Size = New System.Drawing.Size(75, 23)
        Me.cmdAddLanguage.TabIndex = 22
        Me.cmdAddLanguage.Text = "Add"
        Me.cmdAddLanguage.UseVisualStyleBackColor = True
        '
        'txtPlatform
        '
        Me.txtPlatform.Align = System.Windows.Forms.HorizontalAlignment.Right
        Me.txtPlatform.ButtonBorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.txtPlatform.LabelText = "platform"
        Me.txtPlatform.Location = New System.Drawing.Point(-2, 417)
        Me.txtPlatform.Name = "txtPlatform"
        Me.txtPlatform.Size = New System.Drawing.Size(378, 28)
        Me.txtPlatform.TabIndex = 21
        Me.txtPlatform.TextboxText = "all"
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.Location = New System.Drawing.Point(3, 12)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(55, 13)
        Me.Label1.TabIndex = 1
        Me.Label1.Text = "Language"
        '
        'txtGuiVersion
        '
        Me.txtGuiVersion.Align = System.Windows.Forms.HorizontalAlignment.Right
        Me.txtGuiVersion.ButtonBorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.txtGuiVersion.LabelText = "xbmc.gui version"
        Me.txtGuiVersion.Location = New System.Drawing.Point(2, 148)
        Me.txtGuiVersion.Name = "txtGuiVersion"
        Me.txtGuiVersion.Size = New System.Drawing.Size(322, 28)
        Me.txtGuiVersion.TabIndex = 10
        Me.txtGuiVersion.TextboxText = "3.00"
        '
        'txtProvider
        '
        Me.txtProvider.Align = System.Windows.Forms.HorizontalAlignment.Right
        Me.txtProvider.ButtonBorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.txtProvider.LabelText = "provider name"
        Me.txtProvider.Location = New System.Drawing.Point(2, 114)
        Me.txtProvider.Name = "txtProvider"
        Me.txtProvider.Size = New System.Drawing.Size(322, 28)
        Me.txtProvider.TabIndex = 9
        Me.txtProvider.TextboxText = ""
        '
        'txtName
        '
        Me.txtName.Align = System.Windows.Forms.HorizontalAlignment.Right
        Me.txtName.ButtonBorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.txtName.LabelText = "name"
        Me.txtName.Location = New System.Drawing.Point(2, 80)
        Me.txtName.Name = "txtName"
        Me.txtName.Size = New System.Drawing.Size(322, 28)
        Me.txtName.TabIndex = 7
        Me.txtName.TextboxText = ""
        '
        'txtVersion
        '
        Me.txtVersion.Align = System.Windows.Forms.HorizontalAlignment.Right
        Me.txtVersion.ButtonBorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.txtVersion.LabelText = "version"
        Me.txtVersion.Location = New System.Drawing.Point(2, 46)
        Me.txtVersion.Name = "txtVersion"
        Me.txtVersion.Size = New System.Drawing.Size(322, 28)
        Me.txtVersion.TabIndex = 6
        Me.txtVersion.TextboxText = ""
        '
        'txtId
        '
        Me.txtId.Align = System.Windows.Forms.HorizontalAlignment.Right
        Me.txtId.ButtonBorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.txtId.LabelText = "id"
        Me.txtId.Location = New System.Drawing.Point(2, 12)
        Me.txtId.Name = "txtId"
        Me.txtId.Size = New System.Drawing.Size(322, 28)
        Me.txtId.TabIndex = 5
        Me.txtId.TextboxText = ""
        '
        'frmCreateSkin
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(723, 511)
        Me.Controls.Add(Me.Panel2)
        Me.Controls.Add(Me.Panel1)
        Me.Controls.Add(Me.txtGuiVersion)
        Me.Controls.Add(Me.txtProvider)
        Me.Controls.Add(Me.txtName)
        Me.Controls.Add(Me.txtVersion)
        Me.Controls.Add(Me.txtId)
        Me.Controls.Add(Me.cmdCancel)
        Me.Controls.Add(Me.cmdCreate)
        Me.Name = "frmCreateSkin"
        Me.Text = "frmCreateSkin"
        Me.Panel1.ResumeLayout(False)
        Me.Panel1.PerformLayout()
        Me.Panel2.ResumeLayout(False)
        Me.Panel2.PerformLayout()
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents cmdCancel As System.Windows.Forms.Button
    Friend WithEvents cmdCreate As System.Windows.Forms.Button
    Friend WithEvents txtId As RichTextBoxEx.TextButton
    Friend WithEvents txtVersion As RichTextBoxEx.TextButton
    Friend WithEvents txtName As RichTextBoxEx.TextButton
    Friend WithEvents txtProvider As RichTextBoxEx.TextButton
    Friend WithEvents txtGuiVersion As RichTextBoxEx.TextButton
    Friend WithEvents Panel1 As System.Windows.Forms.Panel
    Friend WithEvents txtDefaultResWide As RichTextBoxEx.TextButton
    Friend WithEvents txtDefaultRes As RichTextBoxEx.TextButton
    Friend WithEvents txtPoint As RichTextBoxEx.TextButton
    Friend WithEvents lblExtension As System.Windows.Forms.Label
    Friend WithEvents txtDebugging As RichTextBoxEx.TextButton
    Friend WithEvents txtEffectSlowDown As RichTextBoxEx.TextButton
    Friend WithEvents txtDefaultTheme As RichTextBoxEx.TextButton
    Friend WithEvents Panel2 As System.Windows.Forms.Panel
    Friend WithEvents txtPlatform As RichTextBoxEx.TextButton
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents cmdAddLanguage As System.Windows.Forms.Button
    Friend WithEvents Doubletextbox1 As RichTextBoxEx.doubletextbox
    Friend WithEvents Doubletextbox2 As RichTextBoxEx.doubletextbox
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents Label2 As System.Windows.Forms.Label
End Class
