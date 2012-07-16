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
Imports System.IO
Imports System.Drawing
Imports System.Net
Imports XBMControl.Properties


Namespace XBMC
	Public Class XBMC_Video
		Private parent As XBMC_Communicator = Nothing

		Public Sub New(p As XBMC_Communicator)
			parent = p
		End Sub


		Public Function Hash(input As String) As String
			Dim bytes As Byte()
			Dim m_crc As UInteger = &HffffffffUI
			Dim encoding As New System.Text.ASCIIEncoding()
			bytes = encoding.GetBytes(input.ToLower())
			For Each myByte As Byte In bytes
				m_crc = m_crc Xor (CUInt(myByte) << 24)
				For i As Integer = 0 To 7
					If (System.Convert.ToUInt32(m_crc) And &H80000000UI) = &H80000000UI Then
						m_crc = (m_crc << 1) Xor &H4c11db7
					Else
						m_crc <<= 1
					End If
				Next
			Next
			Return [String].Format("{0:x8}", m_crc)
		End Function

		Public Function getVideoThumb(videoID As String) As System.Drawing.Image
			Dim hashName As String
			Dim strPath As String()
			Dim downloadData As String()
			Dim ipString As String
			Dim fileExist As String()
			Dim thumbnail As Image = Nothing
			Dim client As New WebClient()
			Dim condition As String = If((videoID Is Nothing), "", " WHERE C09 LIKE '%%" & videoID & "%%'")
			strPath = parent.Request("QueryVideoDatabase", "SELECT strpath FROM movieview " & condition)
			hashName = Hash(strPath(0) & "VIDEO_TS.IFO")

			ipString = "P:\Thumbnails\Video\" & hashName(0) & "\" & hashName & ".tbn"

			fileExist = parent.Request("FileExists", ipString)

			If fileExist(0) = "True" Then
				Try
					downloadData = parent.Request("FileDownload", ipString)

					' Convert Base64 String to byte[]
					Dim imageBytes As Byte() = Convert.FromBase64String(downloadData(0))
					Dim ms As New MemoryStream(imageBytes, 0, imageBytes.Length)

					' Convert byte[] to Image
					ms.Write(imageBytes, 0, imageBytes.Length)
					thumbnail = Image.FromStream(ms, True)
                Catch e As Exception
                    thumbnail = My.Resources.video_32x32
				End Try
			Else
                thumbnail = My.Resources.video_32x32
			End If

			Return thumbnail
		End Function

		Public Function GetVideoPath(movieName As String) As String
			Dim tempString As String
			Dim tempStringList As String()

			Dim condition As String = If((movieName Is Nothing), "", " WHERE C00 LIKE '%%" & movieName & "%%'")
			tempStringList = parent.Request("QueryVideoDatabase", "SELECT strpath FROM movieview " & condition)
			tempString = tempStringList(0)
			tempString += "VIDEO_TS.IFO"
			Return tempString
		End Function

		Public Function GetVideoNames(searchString As String) As String()
			Dim condition As String = If((searchString Is Nothing), "", " WHERE C00 LIKE '%%" & searchString & "%%'")
			Return parent.Request("QueryVideoDatabase", "SELECT C00 FROM movie " & condition)
		End Function

		Public Function GetVideoNames() As String()
			Return Me.GetVideoNames(Nothing)
		End Function

		Public Function GetVideoYears(searchString As String) As String()
			Dim condition As String = If((searchString Is Nothing), "", " WHERE C00 LIKE '%%" & searchString & "%%'")
			Return parent.Request("QueryVideoDatabase", "SELECT C07 FROM movie " & condition)
		End Function

		Public Function GetVideoYears() As String()
			Return Me.GetVideoYears(Nothing)
		End Function

		Public Function GetVideoIMDB(searchString As String) As String()
			Dim condition As String = If((searchString Is Nothing), "", " WHERE C00 LIKE '%%" & searchString & "%%'")
			Return parent.Request("QueryVideoDatabase", "SELECT C09 FROM movie " & condition)
		End Function

		Public Function GetVideoIMDB() As String()
			Return Me.GetVideoIMDB(Nothing)
		End Function

		Public Function GetVideoIds(searchString As String) As String()
			Dim condition As String = If((searchString Is Nothing), "", " WHERE C00 LIKE '%%" & searchString & "%%'")
			Return parent.Request("QueryVideoDatabase", "SELECT idMovie FROM movie" & condition & " ORDER BY idMovie")
		End Function

		Public Function GetVideoIds() As String()
			Return Me.GetVideoIds(Nothing)
		End Function

		Public Function GetVideoLibraryInfo(videoID As String) As String()
			Dim condition As String = If((videoID Is Nothing), "", " WHERE C09 LIKE '%%" & videoID & "%%'")
			Return parent.Request("QueryVideoDatabase", "SELECT * FROM movie " & condition)
		End Function

		Public Sub sendAction(buttonAction As String)
			parent.Request("action", buttonAction)
		End Sub
	End Class
End Namespace
