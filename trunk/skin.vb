Public Class skin_resolution
    Dim SkinFile As List(Of skin_file)
    Public Resolution_Name As String

    Public Sub New(ByVal resname As String)
        Resolution_Name = resname
        SkinFile = New List(Of skin_file)
    End Sub

    Public Sub AddXml(ByVal thexml As String)
        Dim thefile As New skin_file(thexml)
        Try
            SkinFile.Add(thefile)
        Catch ex As Exception
            Debug.Print(ex.Message)
        End Try

    End Sub

    Function GetWindows() As List(Of String)
        Dim thelist As New List(Of String)
        For Each fle As skin_file In SkinFile
            thelist.Add(fle.Xml_File.Name)
        Next
        Return thelist
    End Function

    Function GetWindowsFile() As List(Of IO.FileInfo)
        Dim thelist As New List(Of IO.FileInfo)
        For Each fle As skin_file In SkinFile
            thelist.Add(fle.Xml_File)
        Next
        Return thelist
    End Function
    

    Function GetWindow(ByVal name As String) As skin_file
        GetWindow = SkinFile.Find(Function(value As skin_file) value.Xml_File.Name = name)

        Return GetWindow
    End Function

End Class



Public Class skin
    Dim Resolutions As List(Of skin_resolution)
    Dim cur_res As skin_resolution
    Dim includes As New List(Of String)
    Private Sub LoadResolution(ByVal respath As String)
        Dim res As New skin_resolution(respath)
        For Each file As String In IO.Directory.GetFiles(respath, "*.xml")
            res.AddXml(file)
        Next
        Resolutions.Add(res)
        cur_res = res

    End Sub

    Function GetWindow(ByVal name As String) As skin_file

        GetWindow = cur_res.GetWindow(name)

        Return GetWindow
    End Function

    Function GetWindows(ByVal str_res As String) As List(Of String)

        Dim thelist As New List(Of String)
        For Each res As skin_resolution In Resolutions
            If res.Resolution_Name.Contains(str_res) Then
                cur_res = res
                thelist = res.GetWindows()
            End If
        Next
        Return thelist
    End Function
    Private Sub LoadIncludes()
        includes.Clear()
        Dim fileText As String = ""
        For Each wdw As IO.FileInfo In cur_res.GetWindowsFile()
            fileText = IO.File.ReadAllText(wdw.FullName)
            For Each mtch As System.Text.RegularExpressions.Match In StringHelper.RegEx.GetGroupsWithoutMultiLine("include.+?name=""([^""]+)""", fileText)
                includes.Add(mtch.Groups(1).Value)
            Next
        Next
    End Sub
    Function GetIncludes() As List(Of String)

        
        Return includes
    End Function

    Public Sub New(ByVal skinpath As String)
        Resolutions = New List(Of skin_resolution)
        For Each path As String In IO.Directory.GetDirectories(skinpath)
            If path.Contains("720p") Then
                LoadResolution(path)
            End If
        Next

        LoadIncludes()

    End Sub
End Class
