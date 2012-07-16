<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class frmOpenSkin
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
        Me.lstSkinList = New System.Windows.Forms.ListBox()
        Me.cmdOpen = New System.Windows.Forms.Button()
        Me.SuspendLayout()
        '
        'lstSkinList
        '
        Me.lstSkinList.FormattingEnabled = True
        Me.lstSkinList.Location = New System.Drawing.Point(29, 50)
        Me.lstSkinList.Name = "lstSkinList"
        Me.lstSkinList.Size = New System.Drawing.Size(224, 316)
        Me.lstSkinList.TabIndex = 0
        '
        'cmdOpen
        '
        Me.cmdOpen.Location = New System.Drawing.Point(358, 54)
        Me.cmdOpen.Name = "cmdOpen"
        Me.cmdOpen.Size = New System.Drawing.Size(199, 71)
        Me.cmdOpen.TabIndex = 1
        Me.cmdOpen.Text = "Open"
        Me.cmdOpen.UseVisualStyleBackColor = True
        '
        'frmOpenSkin
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(787, 489)
        Me.Controls.Add(Me.cmdOpen)
        Me.Controls.Add(Me.lstSkinList)
        Me.Name = "frmOpenSkin"
        Me.Text = "Choose a skin"
        Me.ResumeLayout(False)

    End Sub
    Friend WithEvents lstSkinList As System.Windows.Forms.ListBox
    Friend WithEvents cmdOpen As System.Windows.Forms.Button
End Class
