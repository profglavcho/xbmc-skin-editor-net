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
Imports System.Windows.Forms

Namespace XBMC
	Public Class XBMC_Database
		Private parent As XBMC_Communicator = Nothing

		Public Sub New(p As XBMC_Communicator)
			parent = p
		End Sub



		Public Function GetArtists(searchString As String) As String()
			Dim condition As String = If((searchString Is Nothing), "", " WHERE strArtist LIKE '%%" & searchString & "%%'")
			Return parent.Request("QueryMusicDatabase", "SELECT strArtist FROM artist" & condition & " ORDER BY strArtist")
		End Function

		Public Function GetArtists() As String()
			Return Me.GetArtists(Nothing)
		End Function



		Public Function GetArtistIds(searchString As String) As String()
			Dim condition As String = If((searchString Is Nothing), "", " WHERE strArtist LIKE '%%" & searchString & "%%'")
			Return parent.Request("QueryMusicDatabase", "SELECT idArtist FROM artist" & condition & " ORDER BY strArtist")
		End Function

		Public Function GetArtistIds() As String()
			Return Me.GetArtistIds(Nothing)
		End Function



		Public Function GetAlbums(searchString As String) As String()
			Dim condition As String = If((searchString Is Nothing), "", " WHERE strAlbum LIKE '%%" & searchString & "%%'")
			Return parent.Request("QueryMusicDatabase", "SELECT strAlbum FROM album" & condition & " ORDER BY strAlbum")
		End Function

		Public Function GetAlbums() As String()
			Return Me.GetAlbums(Nothing)
		End Function



		Public Function GetAlbumIds(searchString As String) As String()
			Dim condition As String = If((searchString Is Nothing), "", " WHERE strAlbum LIKE '%%" & searchString & "%%'")
			Return parent.Request("QueryMusicDatabase", "SELECT idAlbum FROM album" & condition & " ORDER BY strAlbum")
		End Function

		Public Function GetAlbumIds() As String()
			Return Me.GetAlbumIds(Nothing)
		End Function



		Public Function GetArtistId(artist As String) As String
			Dim aArtistId As String() = parent.Request("QueryMusicDatabase", "SELECT idArtist FROM artist WHERE strArtist='" & artist & "'")
			Return If((aArtistId IsNot Nothing), aArtistId(0), Nothing)
		End Function



		Public Function GetAlbumId(artist As String, album As String) As String
			Dim conditions As String = " WHERE strAlbum='" & album & "'"
			If artist IsNot Nothing Then
				conditions += " AND idArtist='" & Me.GetArtistId(artist) & "'"
			End If
			Dim aArtistId As String() = parent.Request("QueryMusicDatabase", "SELECT idAlbum FROM album" & conditions)
			Return If((aArtistId IsNot Nothing), aArtistId(0), Nothing)
		End Function

		Public Function GetAlbumId(album As String) As String
			Return Me.GetAlbumId(Nothing, album)
		End Function



		Public Function GetPathById(pathId As String) As String
			Dim aPath As String() = parent.Request("QueryMusicDatabase", "SELECT strPath FROM path WHERE idPath='" & pathId & "'")
			Return If((aPath IsNot Nothing), aPath(0), Nothing)
		End Function



		Public Function GetAlbumPath(albumId As String) As String
			Dim aPathId As String() = parent.Request("QueryMusicDatabase", "SELECT idPath FROM song WHERE idAlbum='" & albumId & "'")
			Return If((aPathId IsNot Nothing), GetPathById(aPathId(0)), Nothing)
		End Function



		Public Function GetSongPath(songId As String) As String
			Dim aPathId As String() = parent.Request("QueryMusicDatabase", "SELECT idPath FROM song WHERE idSong='" & songId & "'")
			Return If((aPathId IsNot Nothing), GetPathById(aPathId(0)), Nothing)
		End Function



		Public Function GetAlbumsByArtist(artist As String) As String()
			Return GetAlbumsByArtistId(GetArtistId(artist))
		End Function

		Public Function GetAlbumsByArtistId(artistId As String) As String()
			Return If((artistId Is Nothing), Nothing, parent.Request("QueryMusicDatabase", "SELECT strAlbum FROM album WHERE idArtist='" & artistId & "' ORDER BY strAlbum"))
		End Function



		Public Function GetAlbumIdsByArtist(artist As String) As String()
			Return GetAlbumIdsByArtistId(GetArtistId(artist))
		End Function

		Public Function GetAlbumIdsByArtistId(artistId As String) As String()
			Return If((artistId Is Nothing), Nothing, parent.Request("QueryMusicDatabase", "SELECT idAlbum FROM album WHERE idArtist='" & artistId & "' ORDER BY strAlbum"))
		End Function



		Public Function GetTitlesByAlbum(artist As String, album As String) As String()
			Return GetTitlesByAlbumId(GetAlbumId(artist, album))
		End Function

		Public Function GetTitlesByAlbum(album As String) As String()
			Return GetTitlesByAlbum(Nothing, album)
		End Function

		Public Function GetTitlesByAlbumId(albumId As String) As String()
			Return If((albumId Is Nothing), Nothing, parent.Request("QueryMusicDatabase", "SELECT strTitle FROM song WHERE idAlbum='" & albumId & "' ORDER BY iTrack"))
		End Function



		Public Function GetPathsByAlbum(artist As String, album As String) As String()
			Return GetPathsByAlbumId(GetAlbumId(artist, album))
		End Function

		Public Function GetPathsByAlbum(album As String) As String()
			Return GetPathsByAlbum(Nothing, album)
		End Function

		Public Function GetPathsByAlbumId(albumId As String) As String()
			Dim aPath As String() = Nothing

			If albumId IsNot Nothing Then
				Dim aFileName As String() = parent.Request("QueryMusicDatabase", "SELECT strFileName FROM song WHERE idAlbum='" & albumId & "' ORDER BY iTrack")

				If aFileName IsNot Nothing Then
					aPath = New String(aFileName.Length - 1) {}
					aPath(0) = ""

					For x As Integer = 0 To aFileName.Length - 1
						aPath(x) = GetAlbumPath(albumId) & aFileName(x)
					Next
				End If
			End If

			Return aPath
		End Function



		Public Function GetSearchSongTitles(searchString As String) As String()
			Return parent.Request("QueryMusicDatabase", "SELECT strTitle FROM song WHERE strTitle LIKE '%%" & searchString & "%%' ORDER BY strTitle")
		End Function

		Public Function GetSearchSongPaths(searchString As String) As String()
			Dim aSongPaths As String() = Nothing
			Dim aSongPathIds As String() = parent.Request("QueryMusicDatabase", "SELECT idPath FROM song WHERE strTitle LIKE '%%" & searchString & "%%' ORDER BY strTitle")
			Dim aFileNames As String() = parent.Request("QueryMusicDatabase", "SELECT strFileName FROM song WHERE strTitle LIKE '%%" & searchString & "%%' ORDER BY strTitle")

			If aSongPathIds IsNot Nothing Then
				aSongPaths = New String(aSongPathIds.Length - 1) {}

				For x As Integer = 0 To aSongPathIds.Length - 1
					Dim aSongPath As String() = parent.Request("QueryMusicDatabase", "SELECT strPath FROM path WHERE idPath='" & aSongPathIds(x) & "'")
					aSongPaths(x) = aSongPath(0) & aFileNames(x)
				Next
			End If

			Return aSongPaths
		End Function



		Public Function GetTitlesByArtist(artist As String) As String()
			Return GetTitlesByArtistId(GetArtistId(artist))
		End Function

		Public Function GetTitlesByArtistId(artistId As String) As String()
			Return If((artistId Is Nothing), Nothing, parent.Request("QueryMusicDatabase", "SELECT strTitle FROM song WHERE idArtist='" & artistId & "' ORDER BY iTrack"))
		End Function




		Public Function GetPathsByArtist(artist As String) As String()
			Return GetPathsByArtistId(GetArtistId(artist))
		End Function

		Public Function GetPathsByArtistId(artistId As String) As String()
			Dim aPaths As String() = Nothing
			Dim aSongPathIds As String() = Nothing
			Dim aFileNames As String() = Nothing

			If artistId IsNot Nothing Then
				aSongPathIds = parent.Request("QueryMusicDatabase", "SELECT idPath FROM song WHERE idArtist='" & artistId & "'")
				aFileNames = parent.Request("QueryMusicDatabase", "SELECT strFileName FROM song WHERE idArtist='" & artistId & "'")

				If aFileNames IsNot Nothing Then
					aPaths = New String(aFileNames.Length - 1) {}
					For x As Integer = 0 To aFileNames.Length - 1
						aPaths(x) = GetPathById(aSongPathIds(x)) & aFileNames(x)
					Next
				End If
			End If

			Return aPaths
		End Function




		Public Function GetPathBySongTitle(artistAlbumId As String, songTitle As String, artist As Boolean) As String
			Dim songPath As String = Nothing
			Dim idField As String = If((artist), "idArtist", "idAlbum")
			Dim aSongPathIds As String() = parent.Request("QueryMusicDatabase", "SELECT idPath FROM song WHERE " & idField & "='" & artistAlbumId & "' AND strTitle='" & songTitle & "'")
			Dim aSongFileNames As String() = parent.Request("QueryMusicDatabase", "SELECT strFileName FROM song WHERE " & idField & "='" & artistAlbumId & "' AND strTitle='" & songTitle & "'")

			If aSongPathIds IsNot Nothing Then
				Dim aSongPaths As String() = parent.Request("QueryMusicDatabase", "SELECT strPath FROM path WHERE idPath='" & aSongPathIds(0) & "'")

				If aSongPaths IsNot Nothing Then
					songPath = aSongPaths(0) & aSongFileNames(0)
				End If
			End If

			Return songPath
		End Function

		Public Function GetPathBySongTitle(artistAlbumId As String, songTitle As String) As String
			Return GetPathBySongTitle(artistAlbumId, songTitle, False)
		End Function
	End Class
End Namespace
