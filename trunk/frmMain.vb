Imports System.Text.RegularExpressions
Public Class frmMain
    Dim xbmccommunicator As New XBMC.XBMC_Communicator
    Const debugex As Integer = 0
    Dim currentskin As skin
    Dim currentskin_file As skin_file
    Dim control_startline, control_endline As Integer
    Dim control_tabulation As String
    Dim changedavalue As Boolean
#Region "menus"
    'open skin
    Private Sub OpenSkinToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OpenSkinToolStripMenuItem.Click
        frmOpenSkin.ShowDialog()
    End Sub
#End Region

#Region "Skin loading"
    Public Sub LoadSkin(ByVal skinpath As String)
        cmbWindow.Items.Clear()
        currentskin = New skin(skinpath)
        For Each fle As String In currentskin.GetWindows("720p")

            cmbWindow.Items.Add(fle)

        Next
        Dim dir As New IO.DirectoryInfo(skinpath)
        Me.Text = "Xbmc Skin editor Current skin is : " + dir.Name
        cmbWindow.SelectedIndex = 0
        control_startline = 0
        control_endline = 0
        control_tabulation = ""
        changedavalue = False
    End Sub

    Private Sub LoadWindow(ByVal name As String)

    End Sub

    Private Sub LoadWindowNamed(ByVal window As String)
        currentskin_file = currentskin.GetWindow(window)

    End Sub

#End Region
#Region "Forms handles"
    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        If My.Settings.currentskin.Length = 0 Then
            brwFolder.Description = "Select the directory of the skin you want to edit"
            Dim appData As String = System.Environment.GetFolderPath(System.Environment.SpecialFolder.ApplicationData)
            brwFolder.SelectedPath = appData
            If IO.Directory.Exists(appData + "\xbmc") Then
                brwFolder.SelectedPath = appData + "\xbmc"
            End If
            If IO.Directory.Exists(brwFolder.SelectedPath + "\addons") Then
                brwFolder.SelectedPath = brwFolder.SelectedPath + "\addons"
                Dim dir As New IO.DirectoryInfo(brwFolder.SelectedPath)
                For Each dr As IO.DirectoryInfo In dir.GetDirectories()
                    If dr.Name.Contains("skin.") Then
                        brwFolder.SelectedPath = dr.FullName
                        Exit For
                    End If
                Next
            End If


            If brwFolder.ShowDialog() = Windows.Forms.DialogResult.OK Then
                My.Settings.currentskin = brwFolder.SelectedPath
                My.Settings.Save()
            Else
                Me.Close()
            End If

        End If
        LoadSkin(My.Settings.currentskin)
        
        SetXbmcCommunicator()
    End Sub
#End Region
    Public Sub SetXbmcCommunicator()
        Dim ip As String = My.Settings.ip
        If My.Settings.port.Length > 0 Then
            ip = ip + ":" + My.Settings.port
        End If

        xbmccommunicator.SetIp(ip)

        xbmccommunicator.SetConnectionTimeout(3000)
        xbmccommunicator.SetCredentials(My.Settings.username, My.Settings.password)


    End Sub
    Private Sub ExHandling(ByVal msg As String)
        If debugex = 0 Then
            Debug.Print(msg)
        Else
            MsgBox(msg)
        End If

    End Sub
    Private Sub cmbWindow_SelectedIndexChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cmbWindow.SelectedIndexChanged
        LoadWindowNamed(cmbWindow.Text)
        txtWindow.Text = IO.File.ReadAllText(currentskin_file.Xml_File.FullName)
        For Each mtch As Match In StringHelper.RegEx.GetGroupsWithoutMultiLine("control .+?id=""([^""]+)""", txtWindow.Text)
            currentskin_file.ListControlIds.Add(mtch.Groups(1).Value)
        Next

    End Sub

    Function GetControlAttribute(ByVal attrib As List(Of control_attributes), ByVal value As String) As String
        Dim type As String = ""
        For Each ctr As control_attributes In attrib
            If ctr.name.Equals(value) Then
                Return ctr.value
            End If
        Next
        Return type
    End Function

    Private Sub SetControl(ByVal ctrl As xml_control)
        Dim index As Integer = 0
        Dim type As String = GetControlAttribute(ctrl.attributes, "type")

        Dim choicearray As New ArrayList()
        Dim includearray As New ArrayList()
        propGridControl.ItemSet.Clear()
        propGridControl.Refresh()
        For Each ctr As control_attributes In ctrl.attributes
            If ctr.show_in_property = True Then
                index = propGridControl.Item.Add(ctr.name, ctr.value, False, type, "value blablabla", True)

                If ctr.choices > -1 Then
                    Select Case ctr.choices
                        Case ChoiceType.ControlId
                            If choicearray.Count = 0 Then
                                choicearray = currentskin_file.ListControlIds
                            End If
                            propGridControl.Item(index).Choices = New PropertyGridEx.CustomChoices(choicearray, True)
                        Case ChoiceType.Include
                            If includearray.Count = 0 Then
                                includearray.AddRange(currentskin.GetIncludes().ToArray())
                            End If
                            propGridControl.Item(index).Choices = New PropertyGridEx.CustomChoices(includearray, True)
                        Case Else

                    End Select

                End If
            End If
            'propGridControl.ItemSet(0).Add(
        Next
        propGridControl.Refresh()
    End Sub

    Function ConvertListToString(ByVal list As List(Of String)) As String
        Dim retstring As String = ""
        For Each line As String In list

            retstring += line + vbNewLine
        Next
        If retstring.Length > 0 Then
            retstring = retstring.Substring(0, retstring.Length() - 1)
        End If
        Return retstring
    End Function

    Private Sub txtWindow_CursorPositionChanged(ByVal sender As Object, ByVal e As System.EventArgs) Handles txtWindow.CursorPositionChanged
        If changedavalue = True Then
            changedavalue = False
            Exit Sub
        End If
        Dim ResultString As String = ""
        Dim SubjectLines As New List(Of String)
        Dim lsttab As New List(Of Char)
        propGridControl.Item.Clear()
        control_startline = 0
        control_endline = 0
        control_tabulation = ""
        For x As Integer = txtWindow.CurrentLine To 0 Step -1
            If (txtWindow.Lines(x).Contains("control") And txtWindow.Lines(x).Contains("type")) Then
                control_startline = x
                For xx As Integer = x To txtWindow.Lines.Length() - 1
                    SubjectLines.Add(txtWindow.Lines(xx))
                    If txtWindow.Lines(xx).Contains("control") = False And control_tabulation.Length = 0 Then
                        control_tabulation = txtWindow.Lines(xx).Substring(0, txtWindow.Lines(xx).IndexOf("<"))

                    End If
                    If (txtWindow.Lines(xx).Contains("</control>")) Then
                        control_endline = xx
                        Exit For
                    End If

                Next

                ResultString = ConvertListToString(SubjectLines)

                'Try
                '    ResultString = Regex.Match(SubjectString, "(<control.+?</control>)", RegexOptions.Singleline Or RegexOptions.IgnoreCase).Groups(1).Value
                'Catch ex As ArgumentException
                '    'Syntax error in the regular expression
                '    ExHandling(ex.Message)
                '    Exit Sub
                'End Try

            End If
            If control_endline > 0 Then Exit For

        Next
        'got a result
        If ResultString.Length > 0 Then

            Dim ctrl As New xml_control(ResultString)
            SetControl(ctrl)

            'index = propGridControl.Item.Add(ctr.name, ctr.value, False, ctrl.type, "value blablabla", True)
        End If
    End Sub

    Private Sub propGridControl_PropertyValueChanged(ByVal s As Object, ByVal e As System.Windows.Forms.PropertyValueChangedEventArgs) Handles propGridControl.PropertyValueChanged
        Debug.Print("changing value of" + e.ChangedItem.Label)
        Dim newText As New List(Of String)
        Dim lineindex As Integer = 0
        Dim addedline As String = ""
        Dim insertLine As Boolean = True

        For Each lne As String In txtWindow.Lines
            addedline = lne

            If control_startline < lineindex And control_endline > lineindex Then
                If lne.Contains(e.ChangedItem.Label) Then
                    addedline = lne.Replace(e.OldValue, e.ChangedItem.Value)
                    insertLine = False
                End If
            End If
            newText.Add(addedline)
            lineindex += 1
        Next
        If insertLine Then
            newText.Insert(control_endline, control_tabulation + "<" + e.ChangedItem.Label + ">" + e.ChangedItem.Value.ToString + "</" + e.ChangedItem.Label + ">")
            control_endline += 1
        End If
        changedavalue = True
        txtWindow.Text = ConvertListToString(newText)


    End Sub

    Private Sub cmdTestWindow_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles cmdTestWindow.Click

        If currentskin_file.windowid.ToString.Length > 0 Then
            Dim strmwriter As IO.StreamWriter = currentskin_file.Xml_File.CreateText()
            For Each lne As String In txtWindow.Lines
                strmwriter.WriteLine(lne)
            Next
            strmwriter.Close()
            xbmccommunicator.Status.Refresh()
            If xbmccommunicator.Status.IsConnected() = True Then
                xbmccommunicator.Controls.ActivateWindow(currentskin_file.windowid.ToString)
            Else
                MsgBox("xbmc is not open or the settings to control xbmc are not correct")
            End If

        End If
    End Sub

    Private Sub SettingsToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SettingsToolStripMenuItem.Click
        frmSettings.Show()
    End Sub
End Class
