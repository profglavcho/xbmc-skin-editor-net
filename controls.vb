Namespace Controls
    Public Class control_value
        Public name As String
        Public attributes As String
        Public value As String
    End Class

    Public Class default_control
        Public binding As Windows.Forms.Binding
        Private dataset As DataSet
        Public type As String
        Public controls As New List(Of control_value)

        Public Sub InsertControl(ByVal name As String, ByVal attributes As String, ByVal value As String)
            Dim ctrl As New control_value
            ctrl.name = name
            ctrl.attributes = attributes
            ctrl.value = value
            controls.Add(ctrl)
        End Sub
        Function GetBinding() As Windows.Forms.Binding
            Return binding
        End Function
        Public Sub New()

            type = Me.GetType().ToString()
            type = type.Substring(type.LastIndexOf(".") + 1)
            dataset = New DataSet(type)
            dataset.Tables.Add("label")
            binding = New Windows.Forms.Binding(type, dataset, type)
            InsertControl("description", "", "")
            InsertControl("id", "", "")
            InsertControl("posx", "", "")
            InsertControl("posy", "", "")
            InsertControl("width", "", "")
            InsertControl("height", "", "")
            InsertControl("visible", "", "")
            InsertControl("animation", "", "")
            InsertControl("camera", "", "")
            InsertControl("colordiffuse", "", "")
            InsertControl("onup", "", "")
            InsertControl("ondown", "", "")
            InsertControl("onleft", "", "")
            InsertControl("onright", "", "")
            InsertControl("hitrect", "", "")
            InsertControl("enable", "", "")
            InsertControl("pulseonselect", "", "")
        End Sub
        Function GetControlValue(ByVal name As String) As control_value
            For Each ctrl As control_value In controls
                If ctrl.name = name Then
                    Return ctrl
                End If
            Next
            Return Nothing
        End Function
    End Class

    Public Class label
        Inherits default_control

        Public Sub New()
            InsertControl("align", "", "")
        End Sub



        Public aligny As String
        Public scroll As String
        Public label As String
        Public info As String
        Public number As String
        Public angle As String
        Public haspath As String
        Public font As String
        Public textcolor As String
        Public shadowcolor As String
        Public wrapmultiline As String
        Public scrollspeed As String
        Public scrollsuffix As String

    End Class

    Public Class fade_label
        Inherits default_control
        Public scroll As String
        Public scrollout As String
        Public pauseatend As String
        Public label As String
        Public info As List(Of String)
        Public font As String
        Public textcolor As String
        Public textoffsetx As String
    End Class
    Public Class button
        Inherits default_control

        Public texturefocus As String
        Public texturenofocus As String
        Public label As String
        Public font As String
        Public textcolor As String
        Public disabledcolor As String
        Public align As String
        Public aligny As String
        Public textoffsetx As String
        Public textoffsety As String

        Public onclick As String
        Public onfocus As String

    End Class
    Public Class multiselect
        Inherits default_control

        Public texturefocus As String
        Public texturenofocus As String
        Public label As String
        Public font As String
        Public textcolor As String
        Public disabledcolor As String
        Public aligny As String
        Public textoffsetx As String
        Public textoffsety As String

    End Class
    Public Class image
        Inherits default_control
        Public fadetime As String
        'public texture border="5" flipy="true" flipx="false" as string
        'public bordertexture border="5" as string
        Public bordersize As String
        Public info As String
        Public aspectratio As String
    End Class
    Public Class multiimage
        Inherits default_control
        Public imagepath As String
        Public info As String
        Public timeperimage As String
        Public fadetime As String
        Public pauseatend As String
        Public randomize As String
        'public loop as string
        Public aspectratio As String
    End Class
    Public Class radio_button
        Inherits default_control

        Public texturefocus As String
        Public texturenofocus As String
        Public textureradioon As String
        Public textureradiooff As String
        Public selected As String
        Public onclick As String
        Public label As String
        Public font As String
        Public textcolor As String
        Public disabledcolor As String
        Public align As String
        Public aligny As String
        Public textoffsetx As String
        Public textoffsety As String


    End Class
    Public Class select_button
        Inherits default_control

        Public texturefocus As String
        Public texturenofocus As String
        Public texturebg As String
        Public textureleft As String
        Public textureleftfocus As String
        Public textureright As String
        Public texturerightfocus As String
        Public label As String
        Public font As String
        Public textcolor As String
        Public disabledcolor As String
        Public align As String
        Public alignY As String
        Public textoffsetx As String
        Public textoffsety As String

    End Class
    Public Class toggle_button
        Inherits default_control

        Public texturefocus As String
        Public texturenofocus As String
        Public alttexturefocus As String
        Public alttexturenofocus As String
        Public usealttexture As String
        Public label As String
        Public altlabel As String
        Public font As String
        Public textcolor As String
        Public disabledcolor As String
        Public align As String
        Public aligny As String
        Public textoffsetx As String
        Public textoffsety As String

        Public onclick As String
        Public onfocus As String

    End Class
    Public Class button_scrollers
        Inherits default_control

        Public texturefocus As String
        Public texturenofocus As String
        Public font As String
        Public textcolor As String
        Public align As String
        Public aligny As String
        Public textoffsetx As String
        Public textoffsety As String

        Public numbuttons As String
        Public buttongap As String
        Public orientation As String
        Public defaultbutton As String
        Public movement As String
        Public alpha As String
        Public wraparound As String
        Public smoothscrolling As String
        Public buttons As List(Of button)

    End Class
    Public Class buttons_block
        Inherits default_control
    End Class
    Public Class spin
        Inherits default_control
    End Class
    Public Class settings_spin
        Inherits default_control
    End Class

    Public Class slider
        Inherits default_control
    End Class

    Public Class list_container
        Inherits default_control
    End Class

    Public Class wraplist_container
        Inherits default_control
    End Class

    Public Class fixedlist_container
        Inherits default_control
    End Class

    Public Class panel_container
        Inherits default_control
    End Class

    Public Class progress
        Inherits default_control
    End Class

    Public Class text_box
        Inherits default_control
    End Class

    Public Class rss_feed
        Inherits default_control
    End Class

    Public Class visualisation
        Inherits default_control
    End Class

    Public Class video
        Inherits default_control
    End Class

    Public Class mover
        Inherits default_control
    End Class

    Public Class resize
        Inherits default_control
    End Class

    Public Class edit
        Inherits default_control
    End Class

    Public Class console
        Inherits default_control
    End Class
    Public Class checkmark
        Inherits default_control
    End Class
    Public Class extended_list
        Inherits default_control
    End Class



End Namespace
