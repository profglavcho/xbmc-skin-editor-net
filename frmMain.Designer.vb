<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class frmMain
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
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(frmMain))
        Me.SplitContainer1 = New System.Windows.Forms.SplitContainer()
        Me.cmbWindow = New System.Windows.Forms.ComboBox()
        Me.SplitContainer2 = New System.Windows.Forms.SplitContainer()
        Me.txtWindow = New RichTextBoxEx.RichTextBoxEx()
        Me.propGridControl = New PropertyGridEx.PropertyGridEx()
        Me.MenuStrip1 = New System.Windows.Forms.MenuStrip()
        Me.FileToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.OpenSkinToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem()
        Me.cmdTestWindow = New System.Windows.Forms.Button()
        Me.SplitContainer1.Panel1.SuspendLayout()
        Me.SplitContainer1.Panel2.SuspendLayout()
        Me.SplitContainer1.SuspendLayout()
        Me.SplitContainer2.Panel1.SuspendLayout()
        Me.SplitContainer2.Panel2.SuspendLayout()
        Me.SplitContainer2.SuspendLayout()
        Me.MenuStrip1.SuspendLayout()
        Me.SuspendLayout()
        '
        'SplitContainer1
        '
        Me.SplitContainer1.Dock = System.Windows.Forms.DockStyle.Fill
        Me.SplitContainer1.IsSplitterFixed = True
        Me.SplitContainer1.Location = New System.Drawing.Point(0, 24)
        Me.SplitContainer1.Name = "SplitContainer1"
        Me.SplitContainer1.Orientation = System.Windows.Forms.Orientation.Horizontal
        '
        'SplitContainer1.Panel1
        '
        Me.SplitContainer1.Panel1.Controls.Add(Me.cmdTestWindow)
        Me.SplitContainer1.Panel1.Controls.Add(Me.cmbWindow)
        '
        'SplitContainer1.Panel2
        '
        Me.SplitContainer1.Panel2.Controls.Add(Me.SplitContainer2)
        Me.SplitContainer1.Size = New System.Drawing.Size(1300, 623)
        Me.SplitContainer1.SplitterDistance = 40
        Me.SplitContainer1.TabIndex = 2
        '
        'cmbWindow
        '
        Me.cmbWindow.FormattingEnabled = True
        Me.cmbWindow.Location = New System.Drawing.Point(993, 3)
        Me.cmbWindow.Name = "cmbWindow"
        Me.cmbWindow.RightToLeft = System.Windows.Forms.RightToLeft.Yes
        Me.cmbWindow.Size = New System.Drawing.Size(184, 21)
        Me.cmbWindow.TabIndex = 0
        Me.cmbWindow.Text = "home.xml"
        '
        'SplitContainer2
        '
        Me.SplitContainer2.Dock = System.Windows.Forms.DockStyle.Fill
        Me.SplitContainer2.Location = New System.Drawing.Point(0, 0)
        Me.SplitContainer2.Name = "SplitContainer2"
        '
        'SplitContainer2.Panel1
        '
        Me.SplitContainer2.Panel1.Controls.Add(Me.txtWindow)
        '
        'SplitContainer2.Panel2
        '
        Me.SplitContainer2.Panel2.Controls.Add(Me.propGridControl)
        Me.SplitContainer2.Size = New System.Drawing.Size(1300, 579)
        Me.SplitContainer2.SplitterDistance = 987
        Me.SplitContainer2.SplitterWidth = 6
        Me.SplitContainer2.TabIndex = 5
        '
        'txtWindow
        '
        Me.txtWindow.Dock = System.Windows.Forms.DockStyle.Fill
        Me.txtWindow.Location = New System.Drawing.Point(0, 0)
        Me.txtWindow.Name = "txtWindow"
        Me.txtWindow.Size = New System.Drawing.Size(987, 579)
        Me.txtWindow.TabIndex = 0
        Me.txtWindow.Text = ""
        '
        'propGridControl
        '
        '
        '
        '
        Me.propGridControl.DocCommentDescription.AccessibleName = ""
        Me.propGridControl.DocCommentDescription.AutoEllipsis = True
        Me.propGridControl.DocCommentDescription.Cursor = System.Windows.Forms.Cursors.Default
        Me.propGridControl.DocCommentDescription.Location = New System.Drawing.Point(3, 18)
        Me.propGridControl.DocCommentDescription.Name = ""
        Me.propGridControl.DocCommentDescription.Size = New System.Drawing.Size(301, 37)
        Me.propGridControl.DocCommentDescription.TabIndex = 1
        Me.propGridControl.DocCommentImage = Nothing
        '
        '
        '
        Me.propGridControl.DocCommentTitle.Cursor = System.Windows.Forms.Cursors.Default
        Me.propGridControl.DocCommentTitle.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold)
        Me.propGridControl.DocCommentTitle.Location = New System.Drawing.Point(3, 3)
        Me.propGridControl.DocCommentTitle.Name = ""
        Me.propGridControl.DocCommentTitle.Size = New System.Drawing.Size(301, 15)
        Me.propGridControl.DocCommentTitle.TabIndex = 0
        Me.propGridControl.DocCommentTitle.UseMnemonic = False
        Me.propGridControl.Dock = System.Windows.Forms.DockStyle.Fill
        Me.propGridControl.Location = New System.Drawing.Point(0, 0)
        Me.propGridControl.Name = "propGridControl"
        Me.propGridControl.SelectedObject = CType(resources.GetObject("propGridControl.SelectedObject"), Object)
        Me.propGridControl.ShowCustomProperties = True
        Me.propGridControl.Size = New System.Drawing.Size(307, 579)
        Me.propGridControl.TabIndex = 5
        '
        '
        '
        Me.propGridControl.ToolStrip.AccessibleName = "ToolBar"
        Me.propGridControl.ToolStrip.AccessibleRole = System.Windows.Forms.AccessibleRole.ToolBar
        Me.propGridControl.ToolStrip.AllowMerge = False
        Me.propGridControl.ToolStrip.AutoSize = False
        Me.propGridControl.ToolStrip.CanOverflow = False
        Me.propGridControl.ToolStrip.Dock = System.Windows.Forms.DockStyle.None
        Me.propGridControl.ToolStrip.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden
        Me.propGridControl.ToolStrip.Location = New System.Drawing.Point(0, 1)
        Me.propGridControl.ToolStrip.Name = ""
        Me.propGridControl.ToolStrip.Padding = New System.Windows.Forms.Padding(2, 0, 1, 0)
        Me.propGridControl.ToolStrip.Size = New System.Drawing.Size(307, 25)
        Me.propGridControl.ToolStrip.TabIndex = 1
        Me.propGridControl.ToolStrip.TabStop = True
        Me.propGridControl.ToolStrip.Text = "PropertyGridToolBar"
        '
        'MenuStrip1
        '
        Me.MenuStrip1.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.FileToolStripMenuItem})
        Me.MenuStrip1.Location = New System.Drawing.Point(0, 0)
        Me.MenuStrip1.Name = "MenuStrip1"
        Me.MenuStrip1.Size = New System.Drawing.Size(1300, 24)
        Me.MenuStrip1.TabIndex = 3
        Me.MenuStrip1.Text = "MenuStrip1"
        '
        'FileToolStripMenuItem
        '
        Me.FileToolStripMenuItem.DropDownItems.AddRange(New System.Windows.Forms.ToolStripItem() {Me.OpenSkinToolStripMenuItem})
        Me.FileToolStripMenuItem.Name = "FileToolStripMenuItem"
        Me.FileToolStripMenuItem.Size = New System.Drawing.Size(35, 20)
        Me.FileToolStripMenuItem.Text = "File"
        '
        'OpenSkinToolStripMenuItem
        '
        Me.OpenSkinToolStripMenuItem.Name = "OpenSkinToolStripMenuItem"
        Me.OpenSkinToolStripMenuItem.Size = New System.Drawing.Size(122, 22)
        Me.OpenSkinToolStripMenuItem.Text = "Open Skin"
        '
        'cmdTestWindow
        '
        Me.cmdTestWindow.Location = New System.Drawing.Point(12, 14)
        Me.cmdTestWindow.Name = "cmdTestWindow"
        Me.cmdTestWindow.Size = New System.Drawing.Size(96, 23)
        Me.cmdTestWindow.TabIndex = 1
        Me.cmdTestWindow.Text = "Test Window"
        Me.cmdTestWindow.UseVisualStyleBackColor = True
        '
        'frmMain
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(1300, 647)
        Me.Controls.Add(Me.SplitContainer1)
        Me.Controls.Add(Me.MenuStrip1)
        Me.MainMenuStrip = Me.MenuStrip1
        Me.Name = "frmMain"
        Me.Text = "Xbmc Skin editor"
        Me.SplitContainer1.Panel1.ResumeLayout(False)
        Me.SplitContainer1.Panel2.ResumeLayout(False)
        Me.SplitContainer1.ResumeLayout(False)
        Me.SplitContainer2.Panel1.ResumeLayout(False)
        Me.SplitContainer2.Panel2.ResumeLayout(False)
        Me.SplitContainer2.ResumeLayout(False)
        Me.MenuStrip1.ResumeLayout(False)
        Me.MenuStrip1.PerformLayout()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents SplitContainer1 As System.Windows.Forms.SplitContainer
    Friend WithEvents cmbWindow As System.Windows.Forms.ComboBox
    Friend WithEvents MenuStrip1 As System.Windows.Forms.MenuStrip
    Friend WithEvents FileToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents OpenSkinToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents SplitContainer2 As System.Windows.Forms.SplitContainer
    Friend WithEvents propGridControl As PropertyGridEx.PropertyGridEx
    Friend WithEvents txtWindow As RichTextBoxEx.RichTextBoxEx
    Friend WithEvents cmdTestWindow As System.Windows.Forms.Button

End Class
