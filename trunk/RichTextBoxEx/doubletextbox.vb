Public Class doubletextbox
    Dim listText1 As List(Of TextBox)
    Dim listText2 As List(Of TextBox)
    Dim textHeight As Integer
    Dim textTop As Integer
    Public Sub AddItem()
        Dim txt1 As New TextBox
        Dim txt2 As New TextBox
        txt1.Left = txtLang.Left
        txt1.Width = txtLang.Width

        txt2.Left = txtDescription.Left
        txt2.Width = txtDescription.Width
        txt1.Top = Me.Height + textTop
        txt2.Top = Me.Height + textTop
        Me.Height = Me.Height + textHeight
        txt1.Visible = True
        txt2.Visible = True
        Me.Controls.Add(txt1)
        Me.Controls.Add(txt2)
        listText1.Add(txt1)
        listText2.Add(txt2)
    End Sub
    Public Structure Lang_Sum
        Dim lang As String
        Dim sum As String
    End Structure
    Private _LangSum As New List(Of Lang_Sum)
    Public ReadOnly Property LangSum() As List(Of Lang_Sum)
        Get
            _LangSum.Clear()
            Dim lgsum As New Lang_Sum
            Dim box2 As New TextBox
            Dim indx As Integer = 0
            For Each box As TextBox In listText1
                lgsum.lang = box.Text
                lgsum.sum = listText2(indx).Text
                _LangSum.Add(lgsum)
                indx += 1
            Next

            Return _LangSum
        End Get
    End Property

    Public Sub New()
        ' This call is required by the designer.
        InitializeComponent()
        textHeight = Me.Height
        textTop = txtDescription.Top
        listText1 = New List(Of TextBox)
        listText2 = New List(Of TextBox)
        listText1.Add(txtLang)
        listText2.Add(txtDescription)
        ' Add any initialization after the InitializeComponent() call.

    End Sub
End Class
