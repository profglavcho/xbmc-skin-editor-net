
Enum ChoiceType
    ControlId = 0
    Include = 1
End Enum

Public Structure control_attributes
    Dim name As String
    Dim value As String
    Dim show_in_property As Boolean
    Dim choices As Integer
    Dim description As String
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

    Private Sub InsertAttributes(ByVal attrib As String, ByVal value As String, Optional ByVal description As String = "", Optional ByVal showinprop As Boolean = True, Optional ByVal choices As Integer = -1)
        find_attrib = attrib
        Dim indx As Integer = attributes.FindIndex(AddressOf FindAttributes)
        If indx > -1 Then
            description = attributes(indx).description
            showinprop = attributes(indx).show_in_property
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
        ctrl.description = description
        If indx > -1 Then
            attributes.Insert(indx, ctrl)
        Else
            attributes.Add(ctrl)
        End If


    End Sub

    
    Private Sub SetType(ByVal type As String)
        'Specific to every control
        InsertAttributes("type", type, "The type of control.", False)
        InsertAttributes("description", "", "")
        InsertAttributes("id", "", "Specifies the control's id. The value this takes depends on the control type, and the window that you are using the control on. There are special control id's that must be present in each window. Any other controls that the skinner adds can be any id they like. Any controls that the skinner specifies content needs not have an id unless it's needed for animation purposes. For instance, most image and label controls don't need an id if the skinner specifies they're content.")
        InsertAttributes("posx", "", "Specifies where the left edge of the control should be drawn, relative to it's parent's left edge. If an ""r"" is included (eg 180r) then the measurement is taken from the parent's right edge (in the left direction). ")
        InsertAttributes("posy", "", "Specifies where the top edge of the control should be drawn, relative to it's parent's top edge. If an ""r"" is included (eg 180r) then the measurement is taken from the parent's bottom edge (in the up direction). ")
        InsertAttributes("width", "", "Specifies the width that should be used to draw the control. ")
        InsertAttributes("height", "", "Specifies the height that should be used to draw the control. ")
        InsertAttributes("visible", "", "Specifies a condition as to when this control will be visible. Can be true, false, or a condition. See Conditional Visibility for more information. Defaults to true. ")
        InsertAttributes("animation", "", "	Specifies the animation to be run when the control enters a particular state. See Animating your skin for more information.")
        InsertAttributes("camera", "", "Specifies the location (relative to the parent's coordinates) of the camera. Useful for the 3D animations such as rotatey. Format is <camera x=""20"" y=""30"" />")
        InsertAttributes("colordiffuse", "", "This specifies the color to be used for the texture basis. It's in hex AARRGGBB format. If you define <colordiffuse>FFFF00FF</colordiffuse> (magenta), the image will be given a magenta tint when rendered. Defaults to FFFFFFFF (no tint). You can also specify this as a name from the colour theme. ")
        InsertAttributes("onup", "", "Specifies the <id> of the control that should be moved to when the user moves up off this control. Can point to a control group (which remembers previous focused items).", True, ChoiceType.ControlId)
        InsertAttributes("ondown", "", "Specifies the <id> of the control that should be moved to when the user moves down off this control. Can point to a control group (which remembers previous focused items).", True, ChoiceType.ControlId)
        InsertAttributes("onleft", "", "Specifies the <id> of the control that should be moved to when the user moves left off this control. Can point to a control group (which remembers previous focused items).", True, ChoiceType.ControlId)
        InsertAttributes("onright", "", "Specifies the <id> of the control that should be moved to when the user moves right off this control. Can point to a control group (which remembers previous focused items).", True, ChoiceType.ControlId)
        InsertAttributes("hitrect", "", "Specifies the location and size of the ""focus area"" of this control (relative to the parent's coordinates) used by the mouse cursor. Format is <hitrect x=""20"" y=""30"" w=""50"" h=""10"" />")
        InsertAttributes("enable", "", "Specifies a condition as to when this control will be enabled. Can be true, false, or a condition. See Conditional Visibility for more information. Defaults to true.")
        InsertAttributes("pulseonselect", "", "This specifies whether or not a button type will ""pulse"" when it has focus. This is done by varying the alpha channel of the button. Defaults to true.")
        'Specific to type
        Select Case type
            Case "label"
                InsertAttributes("align", "", "Can be left, right, or center. Aligns the text within the given label <width>. Defaults to left")
                InsertAttributes("aligny", "", "Can be top or center. Aligns the text within its given label <height>. Defaults to top")
                InsertAttributes("scroll", "", "When true, the text will scroll if longer than the label's <width>. If false, the text will be truncated. Defaults to false.")
                InsertAttributes("label", "", "Specifies the text which should be drawn. You should specify an entry from the strings.xml here (either the XBMC strings.xml or your skin's strings.xml file), however you may also hardcode a piece of text also if you wish, though ofcourse it will not be localisable. You can use the full label formatting syntax and you may also specify more than one piece of information here by using the $INFO and $LOCALIZE formats.")
                InsertAttributes("info", "", "Specifies the information that should be presented. XBMC will auto-fill in this info in place of the <label>.")
                InsertAttributes("number", "", "Specifies a number that should be presented. This is just here to allow a skinner to use a number rather than a text label (as any number given to <label> will be used to lookup in strings.xml) ")
                InsertAttributes("angle", "", "The angle the text should be rendered at, in degrees. A value of 0 is horizontal. ")
                InsertAttributes("haspath", "", "Specifies whether or not this label is filled with a path. Long paths are shortened by compressing the file path while keeping the actual filename full length. ")
                InsertAttributes("font", "", "Specifies the font to use from the font.xml file. ")
                InsertAttributes("textcolor", "", "Specifies the color the text should be, in hex AARRGGBB format, or a name from the colour theme.")
                InsertAttributes("shadowcolor", "", "Specifies the color of the drop shadow on the text, in AARRGGBB format, or a name from the colour theme.")
                InsertAttributes("wrapmultiline", "", "If true, any text that doesn't fit on one line will be wrapped onto multiple lines.")
                InsertAttributes("scrollspeed", "", "Scroll speed of text in pixels per second. Defaults to 60. ")
                InsertAttributes("scrollsuffix", "", "Specifies the suffix used in scrolling labels. Defaults to "" | "". ")
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
                InsertAttributes("align", "", "Can be left, right, or center. Aligns the text within the given label <width>. Defaults to left")
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
                InsertAttributes("align", "", "Can be left, right, or center. Aligns the text within the given label <width>. Defaults to left")
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
                InsertAttributes("align", "", "Can be left, right, or center. Aligns the text within the given label <width>. Defaults to left")
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
                InsertAttributes("align", "", "Can be left, right, or center. Aligns the text within the given label <width>. Defaults to left")
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
                InsertAttributes("align", "", "Can be left, right, or center. Aligns the text within the given label <width>. Defaults to left")
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


End Class
