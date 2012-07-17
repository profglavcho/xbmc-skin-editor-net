
Enum ChoiceType
    ControlId = 0
    Include = 1
End Enum

Public Structure control_attributes
    Dim name As String
    Dim value As String
    Dim show_in_property As Boolean
    Dim choices As Integer

End Structure

Public Class default_control
    Public attributes As List(Of control_attributes)
    Public control_id As Integer
    Public Sub New(ByVal control As Integer, ByVal attribute As List(Of control_attributes))
        control_id = control
        attributes = attribute
    End Sub
End Class

'********************************************************************************
'****************************************XMLCONTROL******************************
'********************************************************************************
Public Class xml_control
    Public attributes As List(Of control_attributes)
    Dim find_attrib As String

    Private Function FindAttributes(ByVal attrib As control_attributes) As Boolean
        Return attrib.name.Equals(find_attrib)
    End Function

    Private Sub InsertAttributes(ByVal attrib As String, ByVal value As String, Optional ByVal showinprop As Boolean = True, Optional ByVal choices As Integer = -1)
        find_attrib = attrib
        Dim indx As Integer = attributes.FindIndex(AddressOf FindAttributes)
        If indx > -1 Then

            attributes.RemoveAt(indx)
        End If

        If attrib.Contains("onup") Or attrib.Contains("ondown") Or attrib.Contains("onleft") Or attrib.Contains("onright") Then
            choices = ChoiceType.ControlId
        End If
        If attrib.Contains("include") Then
            choices = ChoiceType.Include
        End If
        Dim ctrl As New control_attributes
        ctrl.name = attrib
        ctrl.value = value
        ctrl.show_in_property = showinprop
        ctrl.choices = choices

        attributes.Add(ctrl)
    End Sub

    
    Private Sub SetType(ByVal type As String)
        'Specific to every control
        InsertAttributes("type", type, False)
        InsertAttributes("description", "")
        InsertAttributes("id", "")
        InsertAttributes("posx", "")
        InsertAttributes("posy", "")
        InsertAttributes("width", "")
        InsertAttributes("height", "")
        InsertAttributes("visible", "")
        InsertAttributes("animation", "")
        InsertAttributes("camera", "")
        InsertAttributes("colordiffuse", "")
        InsertAttributes("onup", "", True, ChoiceType.ControlId)
        InsertAttributes("ondown", "", True, ChoiceType.ControlId)
        InsertAttributes("onleft", "", True, ChoiceType.ControlId)
        InsertAttributes("onright", "", True, ChoiceType.ControlId)
        InsertAttributes("hitrect", "")
        InsertAttributes("enable", "")
        InsertAttributes("pulseonselect", "")
        InsertAttributes("pulseonselect", "")
        'Specific to type
        Select Case type
            Case "label"
                InsertAttributes("align", "")
                InsertAttributes("aligny", "")
                InsertAttributes("scroll", "")
                InsertAttributes("label", "")
                InsertAttributes("info", "")
                InsertAttributes("number", "")
                InsertAttributes("angle", "")
                InsertAttributes("haspath", "")
                InsertAttributes("font", "")
                InsertAttributes("textcolor", "")
                InsertAttributes("shadowcolor", "")
                InsertAttributes("wrapmultiline", "")
                InsertAttributes("scrollspeed", "")
                InsertAttributes("scrollsuffix", "")
            Case "fadelabel"
                InsertAttributes("scroll", "")
                InsertAttributes("scrollout", "")
                InsertAttributes("pauseatend", "")
                InsertAttributes("label", "")
                'InsertAttributes("info As List(Of String)
                InsertAttributes("font", "")
                InsertAttributes("textcolor", "")
                InsertAttributes("textoffsetx", "")
            Case "button"
                InsertAttributes("texturefocus", "")
                InsertAttributes("texturenofocus", "")
                InsertAttributes("label", "")
                InsertAttributes("font", "")
                InsertAttributes("textcolor", "")
                InsertAttributes("disabledcolor", "")
                InsertAttributes("align", "")
                InsertAttributes("aligny", "")
                InsertAttributes("textoffsetx", "")
                InsertAttributes("textoffsety", "")
                InsertAttributes("onclick", "")
                InsertAttributes("onfocus", "")
            Case "multiselect"
                InsertAttributes("texturefocus", "")
                InsertAttributes("texturenofocus", "")
                InsertAttributes("label", "")
                InsertAttributes("font", "")
                InsertAttributes("textcolor", "")
                InsertAttributes("disabledcolor", "")
                InsertAttributes("aligny", "")
                InsertAttributes("textoffsetx", "")
                InsertAttributes("textoffsety", "")
            Case "image"
                InsertAttributes("fadetime", "")
                'public texture border="5" flipy="true" flipx="false"","")
                'public bordertexture border="5"","")
                InsertAttributes("bordersize", "")
                InsertAttributes("info", "")
                InsertAttributes("aspectratio", "")
            Case "multiimage"
                InsertAttributes("imagepath", "")
                InsertAttributes("info", "")
                InsertAttributes("timeperimage", "")
                InsertAttributes("fadetime", "")
                InsertAttributes("pauseatend", "")
                InsertAttributes("randomize", "")
                'public loop","")
                InsertAttributes("aspectratio", "")
            Case "radiobutton"
                InsertAttributes("texturefocus", "")
                InsertAttributes("texturenofocus", "")
                InsertAttributes("textureradioon", "")
                InsertAttributes("textureradiooff", "")
                InsertAttributes("selected", "")
                InsertAttributes("onclick", "")
                InsertAttributes("label", "")
                InsertAttributes("font", "")
                InsertAttributes("textcolor", "")
                InsertAttributes("disabledcolor", "")
                InsertAttributes("align", "")
                InsertAttributes("aligny", "")
                InsertAttributes("textoffsetx", "")
                InsertAttributes("textoffsety", "")
            Case "selectbutton"
                InsertAttributes("texturefocus", "")
                InsertAttributes("texturenofocus", "")
                InsertAttributes("texturebg", "")
                InsertAttributes("textureleft", "")
                InsertAttributes("textureleftfocus", "")
                InsertAttributes("textureright", "")
                InsertAttributes("texturerightfocus", "")
                InsertAttributes("label", "")
                InsertAttributes("font", "")
                InsertAttributes("textcolor", "")
                InsertAttributes("disabledcolor", "")
                InsertAttributes("align", "")
                InsertAttributes("alignY", "")
                InsertAttributes("textoffsetx", "")
                InsertAttributes("textoffsety", "")
            Case "togglebutton"
                InsertAttributes("texturefocus", "")
                InsertAttributes("texturenofocus", "")
                InsertAttributes("alttexturefocus", "")
                InsertAttributes("alttexturenofocus", "")
                InsertAttributes("usealttexture", "")
                InsertAttributes("label", "")
                InsertAttributes("altlabel", "")
                InsertAttributes("font", "")
                InsertAttributes("textcolor", "")
                InsertAttributes("disabledcolor", "")
                InsertAttributes("align", "")
                InsertAttributes("aligny", "")
                InsertAttributes("textoffsetx", "")
                InsertAttributes("textoffsety", "")
                InsertAttributes("onclick", "")
                InsertAttributes("onfocus", "")
            Case "buttonscrollers"
                InsertAttributes("texturefocus", "")
                InsertAttributes("texturenofocus", "")
                InsertAttributes("font", "")
                InsertAttributes("textcolor", "")
                InsertAttributes("align", "")
                InsertAttributes("aligny", "")
                InsertAttributes("textoffsetx", "")
                InsertAttributes("textoffsety", "")
                InsertAttributes("numbuttons", "")
                InsertAttributes("buttongap", "")
                InsertAttributes("orientation", "")
                InsertAttributes("defaultbutton", "")
                InsertAttributes("movement", "")
                InsertAttributes("alpha", "")
                InsertAttributes("wraparound", "")
                InsertAttributes("smoothscrolling", "")
                'InsertAttributes("buttons As List(Of button)
            Case "buttonsblock"
            Case "spin"
            Case "settingsspin"
            Case "slider"
            Case "listcontainer"
            Case "wraplistcontainer"
            Case "fixedlistcontainer"
            Case "panelcontainer"
            Case "progress"
            Case "textbox"
            Case "rssfeed"
            Case "visualisation"
            Case "video"
            Case "mover"
            Case "resize"
            Case "edit"
            Case "console"
            Case "checkmark"
            Case "extendedlist"
        End Select
    End Sub

    Public Sub New(ByVal str_xml As String)
        Dim attrib As New control_attributes
        attributes = New List(Of control_attributes)
        attrib.name = "type"
        attrib.value = StringHelper.RegEx.GetMatch("type=""([^""]+)", str_xml)
        SetType(attrib.value)
        attrib.name = "id"
        attrib.value = StringHelper.RegEx.GetMatch("id=""([^""]+)", str_xml)
        InsertAttributes("id", attrib.value)

        attributes.Add(attrib)
        Dim matchs As System.Text.RegularExpressions.MatchCollection
        matchs = StringHelper.RegEx.GetGroups("<([^>]+)>([^<]+)</", str_xml)

        For Each mtch As System.Text.RegularExpressions.Match In matchs
            InsertAttributes(mtch.Groups(1).Value, mtch.Groups(2).Value)
            'Debug.Print(mtch.Groups(1).Value)
        Next
    End Sub

End Class

'********************************************************************************
'****************************************skin_file*******************************
'********************************************************************************
Public Class skin_file
    Public Xml_File As IO.FileInfo
    Private ctrls As New List(Of Controls.default_control)
    Public windowid As Integer
    Public defaultcontrol As default_control
    Public allowoverlay As String
    Public views As List(Of Integer)
    Public ListControlIds As New ArrayList

    Public Sub New(ByVal path As String)

        Xml_File = New IO.FileInfo(path)
        LoadWindow()

    End Sub

    Function GetControls() As List(Of Controls.default_control)
        Return ctrls
    End Function

    Function GetControlFromString(ByVal str As String) As Controls.default_control

        Dim ctrl As New Controls.default_control()

    End Function


    Private Sub LoadDefaultControl(ByVal node As Xml.XmlNode)
        Dim attribs As New List(Of control_attributes)
        Dim attrib As control_attributes
        For Each att As Xml.XmlAttribute In node.Attributes
            attrib.name = att.Name
            attrib.value = att.Value
            attribs.Add(attrib)
        Next
        defaultcontrol = New default_control(node.Value, attribs)
    End Sub

    Private Sub LoadAllowOverlay(ByVal node As Xml.XmlNode)
        allowoverlay = node.Value
    End Sub

    Private Sub LoadViews(ByVal node As Xml.XmlNode)
        views = New List(Of Integer)

        Dim splitstr As String()

        splitstr = node.FirstChild.Value.Split(",")
        For Each Str As String In splitstr
            views.Add(Integer.Parse(Str))
        Next

    End Sub
    Private Sub LoadControls(ByVal xmlnode As Xml.XmlNode)
        For Each node As Xml.XmlNode In xmlnode.ChildNodes

        Next
    End Sub

    Private Sub LoadWindow()
        Try


            Dim xml As New Xml.XmlDocument
            Dim xmlnode As Xml.XmlNode
            xml.Load(Xml_File.FullName)

            Dim button As New Controls.label
            xmlnode = xml.FirstChild
            windowid = xmlnode.Attributes("id").Value
            Dim done As Boolean = False
            For Each node As Xml.XmlNode In xmlnode.ChildNodes
                If node.Name.Equals("defaultcontrol") Then
                    LoadDefaultControl(node)
                ElseIf node.Name.Equals("allowoverlay") Then
                    LoadAllowOverlay(node)
                ElseIf node.Name.Equals("views") Then
                    LoadViews(node)
                ElseIf node.Name.Equals("controls") Then
                    LoadControls(node)
                End If
            Next
            'done = True

            ctrls.Add(button)
            'Debug.Print(button.type)
        Catch ex As Exception

        End Try
    End Sub


End Class
