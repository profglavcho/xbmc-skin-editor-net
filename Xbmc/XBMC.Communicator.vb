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
Imports System.Net
Imports System.IO
Imports System.Windows.Forms

Namespace XBMC
    Public Class XBMC_Communicator
        Public Database As XBMC_Database = Nothing
        Public Playlist As XBMC_Playlist = Nothing
        Public Controls As XBMC_Controls = Nothing
        Public NowPlaying As XBMC_NowPlaying = Nothing
        Public Status As XBMC_Status = Nothing
        Public Media As XBMC_Media = Nothing
        Public Video As XBMC_Video = Nothing

        Private configuredIp As String = Nothing
        Private xbmcUsername As String = Nothing
        Private xbmcPassword As String = Nothing
        Private apiPath As String = "/xbmcCmds/xbmcHttp"
        Private logFile As String = "log/xbmcontrol.log"
        Private connectionTimeout As Integer = 2000

        Public Sub New()
            Database = New XBMC_Database(Me)
            Playlist = New XBMC_Playlist(Me)
            Controls = New XBMC_Controls(Me)
            NowPlaying = New XBMC_NowPlaying(Me)
            Status = New XBMC_Status(Me)
            Media = New XBMC_Media(Me)
            Video = New XBMC_Video(Me)
        End Sub

        Private Function MySplitString(ByVal s As String, ByVal delimeter As String) As String()
            If s Is Nothing Then
                Throw New ArgumentNullException("stringToBeSplitted is null.")
            End If
            If delimeter Is Nothing Then
                Throw New ArgumentNullException("delimeter is null.")
            End If

            Dim dsum As Integer = 0
            Dim ssum As Integer = 0
            Dim dl As Integer = delimeter.Length
            Dim sl As Integer = s.Length

            If dl = 0 OrElse sl = 0 OrElse sl < dl Then
                Return New String() {s}
            End If

            Dim cd As Char() = delimeter.ToCharArray()
            Dim cs As Char() = s.ToCharArray()
            Dim retlist As New List(Of String)()

            For i As Integer = 0 To dl - 1
                dsum += Char.GetNumericValue(cd(i))
                ssum += Char.GetNumericValue(cs(i))
            Next

            Dim start As Integer = 0
            For i As Integer = start To sl - dl - 1
                If i >= start AndAlso dsum = ssum AndAlso s.Substring(i, dl) = delimeter Then
                    retlist.Add(s.Substring(start, i - start))
                    start = i + dl
                End If

                ssum += Char.GetNumericValue(cs(i + dl)) - Char.GetNumericValue(cs(i))
            Next

            If dsum = ssum AndAlso s.Substring(sl - dl, dl) = delimeter Then
                retlist.Add(s.Substring(start, sl - dl - start))
                retlist.Add("")
            Else
                retlist.Add(s.Substring(start, sl - start))
            End If

            Return retlist.ToArray()
        End Function

        Public Function Request(ByVal command As String, ByVal parameter As String, ByVal ip As String) As String()
            Dim pageItems As String() = Nothing
            Dim request__1 As HttpWebRequest = Nothing
            Dim response As HttpWebResponse = Nothing
            Dim reader As StreamReader = Nothing
            Dim pageContent As String() = Nothing
            Dim tempString As String = Nothing

            Dim isQuery As Boolean = If((command.ToLower() = "querymusicdatabase" OrElse command.ToLower() = "queryvideodatabase"), True, False)

            Dim ipAddress As String = If((ip Is Nothing), configuredIp, ip)
            parameter = If((parameter Is Nothing), "", parameter)
            command = "?command=" & Uri.EscapeDataString(command)
            command += If((parameter Is Nothing OrElse parameter = ""), "", "&parameter=" & Uri.EscapeDataString(parameter))
            Dim uri__2 As String = "http://" & ipAddress & apiPath & command

            'WriteToLog(uri);

            Try
                request__1 = DirectCast(HttpWebRequest.Create(uri__2), HttpWebRequest)
                request__1.Method = "GET"
                request__1.Timeout = connectionTimeout
                If xbmcUsername <> "" AndAlso xbmcPassword <> "" Then
                    request__1.Credentials = New NetworkCredential(xbmcUsername, xbmcPassword)
                End If
                response = DirectCast(request__1.GetResponse(), HttpWebResponse)
                reader = New StreamReader(response.GetResponseStream(), Encoding.UTF8)

                If isQuery Then
                    tempString = reader.ReadToEnd().Replace("</field>", "").Replace(vbLf, "").Replace("<html>", "").Replace("</html>", "")
                    pageContent = MySplitString(tempString, "<field>")
                Else
                    tempString = reader.ReadToEnd().Replace(vbLf, "").Replace("<html>", "").Replace("</html>", "")
                    pageContent = MySplitString(tempString, "<li>")
                End If

                If pageContent IsNot Nothing Then
                    If pageContent.Length > 1 Then
                        pageItems = New String(pageContent.Length - 2) {}

                        For x As Integer = 1 To pageContent.Length - 1
                            pageItems(x - 1) = pageContent(x)
                        Next
                    Else
                        pageItems = New String(0) {}
                        pageItems(0) = pageContent(0)
                    End If
                End If
            Catch e As WebException
                WriteToLog("ERROR - " & e.Message)
            Finally
                If response IsNot Nothing Then
                    response.Close()
                End If
                If reader IsNot Nothing Then
                    reader.Close()
                End If
            End Try

            Return pageItems
        End Function

        Public Function Request(ByVal command As String, ByVal parameter As String) As String()
            Return Request(command, parameter, Nothing)
        End Function

        Public Function Request(ByVal command As String) As String()
            Return Request(command, Nothing, Nothing)
        End Function

        Private Sub WriteToLog(ByVal message As String)

            Dim sw As StreamWriter = Nothing
            Dim [error] As String = Nothing

            Try
                sw = New StreamWriter(logFile, True)
                sw.WriteLine(DateTime.Now & " : " & message)
            Catch e As Exception
                [error] = e.Message
            End Try

            If sw IsNot Nothing Then
                sw.Flush()
                sw.Close()
            End If
        End Sub

        Public Sub SetIp(ByVal ip As String)
            configuredIp = ip

            If configuredIp Is Nothing OrElse configuredIp = "" Then
                Status.DisableHeartBeat()
            Else
                Status.EnableHeartBeat()
            End If
        End Sub

        Public Function GetIp() As String
            Return configuredIp
        End Function

        Public Sub SetCredentials(ByVal username As String, ByVal password As String)
            xbmcUsername = username
            xbmcPassword = password
        End Sub

        Public Sub SetConnectionTimeout(ByVal timeOut As Integer)
            connectionTimeout = timeOut
        End Sub
    End Class
End Namespace
