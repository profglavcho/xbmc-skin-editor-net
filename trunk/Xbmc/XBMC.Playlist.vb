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

Namespace XBMC
	Public Class XBMC_Playlist
		Private parent As XBMC_Communicator = Nothing
		Private aCurrentPlaylist As String() = Nothing

		Public Sub New(p As XBMC_Communicator)
			parent = p
		End Sub

		Public Function [Get](parse As Boolean, refresh As Boolean) As String()
			If refresh Then
				Dim aPlaylistTemp As String() = parent.Request("GetPlaylistContents(GetCurrentPlaylist)")

				If parse = True Then
					If aPlaylistTemp IsNot Nothing Then
						aCurrentPlaylist = New String(aPlaylistTemp.Length - 1) {}
						For x As Integer = 0 To aPlaylistTemp.Length - 1
							Dim i As Integer = aPlaylistTemp(x).LastIndexOf(".")
							If i > 1 Then
								Dim extension As String = aPlaylistTemp(x).Substring(i, aPlaylistTemp(x).Length - i)
								aPlaylistTemp(x) = aPlaylistTemp(x).Replace("/", "\")
								Dim aPlaylistEntry As String() = aPlaylistTemp(x).Split("\"C)
								Dim playlistEntry As String = aPlaylistEntry(aPlaylistEntry.Length - 1).Replace(extension, "")
								aCurrentPlaylist(x) = playlistEntry
							Else
								aCurrentPlaylist(x) = ""
							End If
						Next
					End If
				Else
					aCurrentPlaylist = aPlaylistTemp
				End If
			End If

			Return aCurrentPlaylist
		End Function

		Public Sub PlaySong(position As Integer)
			parent.Request("SetPlaylistSong(" & position.ToString() & ")")
		End Sub

		Public Sub Remove(position As Integer)
			parent.Request("RemoveFromPlaylist(" & position.ToString() & ")")
		End Sub

		Public Function GetCurrentIdentifier() As String
			Dim curPlaylist As String() = parent.Request("GetCurrentPlaylist()")
			Return If((curPlaylist Is Nothing), Nothing, curPlaylist(0))
		End Function

		Public Sub Clear()
			parent.Request("ClearPlayList()")
		End Sub

		Public Sub AddDirectoryContent(folderPath As String, mask As String, recursive As Boolean)
			Dim p As String = ""
			Dim m As String = ""
			Dim r As String = ""

			If mask IsNot Nothing Then
				m = ";[" & mask & "]"
				p = ";0"
				r = If((recursive), ";1", ";0")
			End If

			parent.Request("AddToPlayList(" & folderPath & p & m & r & ")")
		End Sub

		Public Sub AddDirectoryContent(folderPath As String, mask As String)
			Me.AddDirectoryContent(folderPath, mask, False)
		End Sub

		Public Sub AddFilesToPlaylist(filePath As String)
			Me.AddDirectoryContent(filePath, Nothing)
		End Sub

		Public Sub SetSong(position As Integer)
			parent.Request("SetPlaylistSong(" & position.ToString() & ")")
		End Sub

		Public Sub SetType(type As String)
			Dim playlistType As String = If((type = "video"), "1", "0")
			parent.Request("SetCurrentPlaylist(" & playlistType & ")")
		End Sub
	End Class
End Namespace
