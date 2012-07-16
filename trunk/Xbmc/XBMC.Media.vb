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
    Public Class XBMC_Media
        Private parent As XBMC_Communicator = Nothing

        Public Sub New(ByVal p As XBMC_Communicator)
            parent = p
        End Sub

        Public Function GetShares(ByVal type As String, ByVal path As Boolean) As String()
            Dim aMediaShares As String() = parent.Request("GetShares(" & type & ")")

            If aMediaShares IsNot Nothing Then
                Dim aShareNames As String() = New String(aMediaShares.Length - 1) {}
                Dim aSharePaths As String() = New String(aMediaShares.Length - 1) {}

                For x As Integer = 0 To aMediaShares.Length - 1
                    Dim aTmpShare As String() = aMediaShares(x).Split(";"c)

                    If aTmpShare IsNot Nothing Then
                        aShareNames(x) = aTmpShare(0)
                        aSharePaths(x) = aTmpShare(1)
                    End If
                Next

                Return If((path), aSharePaths, aShareNames)
            Else
                Return Nothing
            End If
        End Function

        Public Function GetShares(ByVal type As String) As String()
            Return GetShares(type, False)
        End Function

        Public Function GetDirectoryContentPaths(ByVal directory As String, ByVal mask As String) As String()
            mask = If((mask Is Nothing), "", ";" & mask)

            Dim aDirectoryContent As String() = parent.Request("GetDirectory(" & directory & mask & ")")

            If aDirectoryContent IsNot Nothing Then
                Dim aContentPaths As String() = New String(aDirectoryContent.Length - 1) {}

                For x As Integer = 0 To aDirectoryContent.Length - 1
                    aDirectoryContent(x) = aDirectoryContent(x).Replace("/", "\")
                    aContentPaths(x) = If((aDirectoryContent(x) = "Error:Not folder" OrElse aDirectoryContent(x) = "Error"), Nothing, aDirectoryContent(x))
                Next
                Return aContentPaths
            Else
                Return Nothing
            End If
        End Function

        Public Function GetDirectoryContentPaths(ByVal directory As String) As String()
            Return GetDirectoryContentPaths(directory, Nothing)
        End Function

        Public Function GetDirectoryContentNames(ByVal directory As String, ByVal mask As String) As String()
            Dim aContentPaths As String() = Me.GetDirectoryContentPaths(directory, mask)

            If aContentPaths IsNot Nothing Then
                Dim aContentNames As String() = New String(aContentPaths.Length - 1) {}

                For x As Integer = 0 To aContentPaths.Length - 1
                    If aContentPaths(x) Is Nothing OrElse aContentPaths(x) = "" Then
                        aContentNames(x) = Nothing
                    Else
                        aContentPaths(x) = aContentPaths(x).Replace("/", "\")
                        Dim aTmpContent As String() = aContentPaths(x).Split("\"c)
                        aContentNames(x) = If((aTmpContent(aTmpContent.Length - 1) = ""), aTmpContent(aTmpContent.Length - 2), aTmpContent(aTmpContent.Length - 1))
                    End If
                Next

                Return aContentNames
            Else
                Return Nothing
            End If
        End Function

        Public Function GetDirectoryContentNames(ByVal directory As String) As String()
            Return GetDirectoryContentNames(directory, Nothing)
        End Function
    End Class
End Namespace
