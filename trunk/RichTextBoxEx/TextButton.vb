Public Class TextButton

    Private _LabelText As String
    Public Property LabelText() As String
        Get
            Return _LabelText
        End Get
        Set(ByVal value As String)
            _LabelText = value
            Label1.Text = value
        End Set
    End Property


    Private _TextboxText As String
    Public Property TextboxText() As String
        Get
            _TextboxText = TextBox1.Text
            Return _TextboxText
        End Get
        Set(ByVal value As String)
            _TextboxText = value
            TextBox1.Text = value
        End Set
    End Property


    Private _Align As System.Windows.Forms.HorizontalAlignment
    Public Property Align() As System.Windows.Forms.HorizontalAlignment
        Get
            _Align = TextBox1.TextAlign
            Return _Align
        End Get
        Set(ByVal value As System.Windows.Forms.HorizontalAlignment)
            TextBox1.TextAlign = value
            _Align = value
        End Set
    End Property

    Private _BorderStyle As System.Windows.Forms.BorderStyle
    Public Property ButtonBorderStyle() As System.Windows.Forms.BorderStyle
        Get
            _BorderStyle = Panel1.BorderStyle
            Return _BorderStyle
        End Get
        Set(ByVal value As System.Windows.Forms.BorderStyle)
            Panel1.BorderStyle = value
            _BorderStyle = value
        End Set
    End Property

End Class
