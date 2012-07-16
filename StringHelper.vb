Imports System.Text.RegularExpressions
Namespace StringHelper

    Public Class StringConverter

    End Class

    Public Class RegEx
        Public Shared Function GetMatch(ByVal regex As String, ByVal subject As String) As String
            Dim result As String = ""
            Try
                result = System.Text.RegularExpressions.Regex.Match(subject, regex, RegexOptions.Singleline Or RegexOptions.IgnoreCase).Groups(1).Value
            Catch ex As ArgumentException
                'Syntax error in the regular expression
                Debug.Print(ex.Message)

            End Try
            Return result
        End Function

        Public Shared Function GetGroups(ByVal regex As String, ByVal subject As String) As MatchCollection
            Dim result As MatchCollection

            Try
                result = System.Text.RegularExpressions.Regex.Matches(subject, regex, RegexOptions.Singleline Or RegexOptions.IgnoreCase)
            Catch ex As ArgumentException
                'Syntax error in the regular expression
                Debug.Print(ex.Message)

            End Try
            Return result
        End Function

        Public Shared Function GetGroupsWithoutMultiLine(ByVal regex As String, ByVal subject As String) As MatchCollection
            Dim result As MatchCollection

            Try
                result = System.Text.RegularExpressions.Regex.Matches(subject, regex, RegexOptions.IgnoreCase)
            Catch ex As ArgumentException
                'Syntax error in the regular expression
                Debug.Print(ex.Message)

            End Try
            Return result
        End Function

    End Class

End Namespace