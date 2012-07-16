' ------------------------------------------------------------------------
'    XBMControl - A compact remote controller for XBMC (.NET 3.5)
'    Copyright (C) 2008  Bram van Oploo (bramvano@gmail.com)
'                        Mike Thiels (Mike.Thiels@gmail.com)
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
Imports System.Windows.Forms

Namespace XBMC
	Public Class XBMC_Status
		Private parent As XBMC_Communicator = Nothing

		'XBMC Properties
        Friend _isConnected As Boolean = False
        Private _isPlaying As Boolean = False
        Private _isNotPlaying As Boolean = True
		Private isPlayingLastFm As Boolean = False
        Private _isPaused As Boolean = False
        Private _isMuted As Boolean = False
		Private volume As Integer = 0
		Private progress As Integer = 1
		Private mediaNowPlaying As String = Nothing
		Private newMediaPlaying As Boolean = True
		Private heartBeatTimer As Timer = Nothing
		Private connectedInterval As Integer = 5000
		Private disconnectedInterval As Integer = 10000

		Public Sub New(p As XBMC_Communicator)
			parent = p
			heartBeatTimer = New Timer()
			heartBeatTimer.Interval = connectedInterval
			AddHandler heartBeatTimer.Tick, New EventHandler(AddressOf HeartBeat_Tick)
		End Sub

		Public Sub Refresh()
            If _isConnected Then
                If mediaNowPlaying <> parent.NowPlaying.[Get]("filename", True) OrElse mediaNowPlaying Is Nothing Then
                    mediaNowPlaying = parent.NowPlaying.[Get]("filename")
                    newMediaPlaying = True
                Else
                    newMediaPlaying = False
                End If

                _isPlaying = If((parent.NowPlaying.[Get]("playstatus", True) = "Playing"), True, False)
                _isPaused = If((parent.NowPlaying.[Get]("playstatus", True) = "Paused"), True, False)
                _isNotPlaying = If((mediaNowPlaying = "[Nothing Playing]" OrElse mediaNowPlaying Is Nothing), True, False)

                If mediaNowPlaying Is Nothing OrElse isNotPlaying OrElse mediaNowPlaying.Length < 6 Then
                    isPlayingLastFm = False
                Else
                    isPlayingLastFm = If((mediaNowPlaying.Substring(0, 6) = "lastfm"), True, False)
                End If

                Dim aVolume As String() = parent.Request("GetVolume")
                Dim aProgress As String() = parent.Request("GetPercentage")

                If aVolume Is Nothing OrElse aVolume(0) = "Error" Then
                    volume = 0
                Else
                    volume = Convert.ToInt32(aVolume(0))
                End If

                If aProgress Is Nothing OrElse aProgress(0) = "Error" OrElse aProgress(0) = "0" OrElse Convert.ToInt32(aProgress(0)) > 99 Then
                    progress = 1
                Else
                    progress = Convert.ToInt32(aProgress(0))
                End If

                _isMuted = If((volume = 0), True, False)
            End If
		End Sub

		Private Sub HeartBeat_Tick(sender As Object, e As EventArgs)
            _isConnected = parent.Controls.SetResponseFormat()
			heartBeatTimer.Interval = If((isConnected), connectedInterval, disconnectedInterval)
		End Sub

		Public Function IsConnected() As Boolean
            Return _isConnected
		End Function

		Public Sub EnableHeartBeat()
			HeartBeat_Tick(Nothing, Nothing)
			heartBeatTimer.Enabled = True
		End Sub

		Public Sub DisableHeartBeat()
			heartBeatTimer.Enabled = False
		End Sub

		Public Function WebServerEnabled() As Boolean
			Dim webserverEnabled__1 As String() = parent.Request("WebServerStatus")

			If webserverEnabled__1 Is Nothing Then
				Return False
			Else
				Return If((webserverEnabled__1(0) = "On"), True, False)
			End If
		End Function

		Public Function IsNewMediaPlaying() As Boolean
			Return newMediaPlaying
		End Function

		Public Function IsPlaying(lastfm As String) As Boolean
			Return If((lastfm IsNot Nothing), isPlayingLastFm, isPlaying)
		End Function

		Public Function IsPlaying() As Boolean
			Return Me.IsPlaying(Nothing)
		End Function

		Public Function IsNotPlaying() As Boolean
            Return _isNotPlaying
		End Function

		Public Function IsPaused() As Boolean
            Return _isPaused
		End Function

		Public Function IsMuted() As Boolean
            Return _isMuted
		End Function

		Public Function GetVolume() As Integer
			Return volume
		End Function

		Public Function GetProgress() As Integer
			Return progress
		End Function

		Public Function LastFmEnabled() As Boolean
			Dim aLastFmUsername As String() = parent.Request("GetGuiSetting(3;lastfm.username)")
			Dim aLastFmPassword As String() = parent.Request("GetGuiSetting(3;lastfm.password)")

			If aLastFmUsername Is Nothing OrElse aLastFmPassword Is Nothing Then
				Return False
			Else
				Return If((aLastFmUsername(0) = "" OrElse aLastFmPassword(0) = ""), False, True)
			End If
		End Function

		Public Function RepeatEnabled() As Boolean
			Dim aRepeatEnabled As String() = parent.Request("GetGuiSetting(1;musicfiles.repeat)")
			If aRepeatEnabled Is Nothing Then
				Return False
			Else
				Return If((aRepeatEnabled(0) = "False"), False, True)
			End If
		End Function
	End Class
End Namespace
