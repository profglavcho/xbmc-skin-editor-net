' ------------------------------------------------------------------------
'    XBMControl - A compact remote controller for XBMC (.NET 3.5)
'    Copyright (C) 2008  Bram van Oploo (bramvano@gmail.com)
'
'    This program is free software: you can redistribute it and/or modify
'    it under the terms of the GNU General Public License as published by
'    the Free Software Foundation, either version 3 of the License, or
'    (at your option) any later version.
'
'    This program is distributed in the hope that it will be useful,
'    but WITHOUT ANY WARRANTY; without even the implied warranty of
'    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
'    GNU General Public License for more details.
'
'    You should have received a copy of the GNU General Public License
'    along with this program.  If not, see <http://www.gnu.org/licenses/>.
' ------------------------------------------------------------------------

Imports System.Collections.Generic
Imports System.Text
Imports System.Drawing
Imports System.IO
Imports System.Windows.Forms

Namespace XBMC
	Public Class XBMC_Controls
		Private parent As XBMC_Communicator = Nothing

		Public Sub New(p As XBMC_Communicator)
			parent = p
		End Sub

		Public Sub Play()
			parent.Request("ExecBuiltIn", "PlayerControl(Play)")
		End Sub

		Public Sub PlayFile(filename As String)
			parent.Request("PlayFile(" & filename & ")")
		End Sub

		Public Sub PlayMedia(media As String)
			parent.Request("ExecBuiltIn", "PlayMedia(" & media & ")")
		End Sub

		Public Sub [Stop]()
			parent.Request("ExecBuiltIn", "PlayerControl(Stop)")
		End Sub

		Public Sub [Next]()
			parent.Request("ExecBuiltIn", "PlayerControl(Next)")
		End Sub

		Public Sub PlayListNext()
			parent.Request("PlayListNext")
		End Sub

		Public Sub Previous()
			parent.Request("ExecBuiltIn", "PlayerControl(Previous)")
		End Sub

		Public Sub ToggleShuffle()
			parent.Request("ExecBuiltIn", "PlayerControl(Random)")
		End Sub

		Public Sub TogglePartymode()
			parent.Request("ExecBuiltIn", "PlayerControl(Partymode(music))")
		End Sub

		Public Sub Repeat(enable As Boolean)
			Dim mode As String = If((enable), "Repeat", "RepeatOff")
			parent.Request("ExecBuiltIn", "PlayerControl(" & mode & ")")
		End Sub

		Public Sub LastFmLove()
			parent.Request("ExecBuiltIn", "LastFM.Love(false)")
		End Sub

		Public Sub LastFmHate()
			parent.Request("ExecBuiltIn", "LastFM.Ban(false)")
		End Sub

		Public Sub ToggleMute()
			parent.Request("ExecBuiltIn", "Mute")
		End Sub

		Public Sub SetVolume(percentage As Integer)
			parent.Request("ExecBuiltIn", "SetVolume(" & Convert.ToString(percentage) & ")")
		End Sub

		Public Sub SeekPercentage(percentage As Integer)
			parent.Request("SeekPercentage", Convert.ToString(percentage))
		End Sub

		Public Sub Reboot()
			parent.Request("ExecBuiltIn", "Reboot")
		End Sub

		Public Sub Shutdown()
			parent.Request("ExecBuiltIn", "Shutdown")
		End Sub

		Public Sub Restart()
			parent.Request("ExecBuiltIn", "RestartApp")
		End Sub

        Public Sub ActivateWindow(ByVal window As String)
            parent.Request("ExecBuiltIn", "ActivateWindow(" + window + ")")
        End Sub

        Public Function GetGuiDescription(ByVal field As String) As String
            Dim returnValue As String = Nothing
            Dim aGuiDescription As String() = parent.Request("GetGUIDescription")

            For x As Integer = 0 To aGuiDescription.Length - 1
                Dim splitIndex As Integer = aGuiDescription(x).IndexOf(":"c) + 1
                If splitIndex > 1 Then
                    Dim resultField As String = aGuiDescription(x).Substring(0, splitIndex - 1).Replace(" ", "").ToLower()
                    If resultField = field Then
                        returnValue = aGuiDescription(x).Substring(splitIndex, aGuiDescription(x).Length - splitIndex)
                    End If
                End If
            Next

            Return returnValue
        End Function

        Public Function GetScreenshotBase64() As String
            Dim base64screenshot As String() = parent.Request("takescreenshot", "screenshot.png;false;0;" & Me.GetGuiDescription("width") & ";" & Me.GetGuiDescription("height") & ";75;true;")
            Return If((base64screenshot Is Nothing), Nothing, base64screenshot(0))
        End Function

        Public Function Base64StringToImage(ByVal base64String As String) As Image
            Dim file As Bitmap = Nothing
            Dim bytes As Byte() = Convert.FromBase64String(base64String)
            Dim stream As New MemoryStream(bytes)

            If base64String IsNot Nothing AndAlso base64String <> "" Then
                file = New Bitmap(Image.FromStream(stream))
            End If

            Return file
        End Function

        Public Function GetScreenshot() As Image
            Dim screenshot As Image = Nothing
            Dim base64ImageString As String = Me.GetScreenshotBase64()

            If base64ImageString IsNot Nothing AndAlso Not base64ImageString.Contains("Error") Then
                screenshot = Me.Base64StringToImage(base64ImageString)
            End If

            Return screenshot
        End Function

        Public Sub UpdateLibrary(ByVal library As String)
            If library = "music" OrElse library = "video" Then
                parent.Request("ExecBuiltIn", "updatelibrary(" & library & ")")
            End If
        End Sub

        Public Function SetResponseFormat() As Boolean
            Dim aResult As String() = Nothing
            Dim ip As String = parent.GetIp()

            If ip IsNot Nothing AndAlso ip <> "" Then
                aResult = parent.Request("SetResponseFormat", Nothing, ip)
            End If

            If aResult Is Nothing Then
                Return False
            Else
                Return If((aResult(0) = "OK"), True, False)
            End If
        End Function
    End Class
End Namespace
