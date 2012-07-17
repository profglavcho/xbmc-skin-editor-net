Public Class frmPreview

    Public Sub LoadImage(ByVal image As String)
        If IO.File.Exists(image) Then
            PictureBox1.Load(image)


        End If

    End Sub

    Private Sub frmPreview_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load

    End Sub
End Class