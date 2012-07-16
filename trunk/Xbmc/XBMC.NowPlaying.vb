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
Imports System.IO
Imports System.Drawing
Imports System.Net
Imports System.Windows.Forms

Namespace XBMC
	Public Class XBMC_NowPlaying
		Private parent As XBMC_Communicator = Nothing
		Private maNowPlayingInfo As String(,) = Nothing
		Private [error] As String = Nothing

		Public Sub New(p As XBMC_Communicator)
			parent = p
		End Sub

		Public Function [Get](field As String, refresh As Boolean) As String
			Dim returnValue As String = Nothing

			If refresh Then
				Dim aNowPlayingTemp As String() = parent.Request("GetCurrentlyPlaying")

				If aNowPlayingTemp IsNot Nothing Then
					maNowPlayingInfo = New String(aNowPlayingTemp.Length - 1, 1) {}
					For x As Integer = 0 To aNowPlayingTemp.Length - 1
						Dim splitIndex As Integer = aNowPlayingTemp(x).IndexOf(":"C) + 1

						If splitIndex > 2 Then
							maNowPlayingInfo(x, 0) = aNowPlayingTemp(x).Substring(0, splitIndex - 1).Replace(" ", "").ToLower()
							maNowPlayingInfo(x, 1) = aNowPlayingTemp(x).Substring(splitIndex, aNowPlayingTemp(x).Length - splitIndex)

							If maNowPlayingInfo(x, 0) = field Then
								returnValue = Me.maNowPlayingInfo(x, 1)
							End If
						End If
					Next
				End If
			Else
				For x As Integer = 0 To Me.maNowPlayingInfo.GetLength(0) - 1
					If Me.maNowPlayingInfo(x, 0) = field Then
						returnValue = Me.maNowPlayingInfo(x, 1)
					End If
				Next
			End If

			Return returnValue
		End Function

		Public Function [Get](field As String) As String
			Return Me.[Get](field, False)
		End Function

		Public Function GetCoverArt() As Image
			Dim stringList As [String]()
			Dim stream As MemoryStream = Nothing
			Dim thumbnail As Image = Nothing
			Dim client As New WebClient()
			Dim xbmcThumbUri As New Uri("http://" & parent.GetIp() & "/thumb.jpg")
			stringList = parent.Request("GetCurrentlyPlaying", "q:/web/thumb.jpg")

			Try
				stream = New MemoryStream(client.DownloadData(xbmcThumbUri))
				thumbnail = New Bitmap(Image.FromStream(stream))
			Catch e As Exception
				[error] = e.Message
			Finally
				client.Dispose()
			End Try

			Return thumbnail
		End Function

		Public Function GetMediaType() As String
			Return Me.[Get]("type", True)
		End Function
	End Class
End Namespace
