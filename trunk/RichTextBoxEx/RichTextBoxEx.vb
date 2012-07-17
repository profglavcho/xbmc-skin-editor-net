Imports System.Drawing
Imports System.Windows.Forms

Public Class RichTextBoxEx
    Inherits System.Windows.Forms.RichTextBox
    Public Event CursorPositionChanged As EventHandler

    Private Property AutoScaleMode As AutoScaleMode

    Protected Overridable Sub OnCursorPositionChanged(ByVal e As EventArgs)
        RaiseEvent CursorPositionChanged(Me, e)
    End Sub

    Protected Overrides Sub OnSelectionChanged(ByVal e As EventArgs)
        If SelectionLength = 0 Then
            OnCursorPositionChanged(e)
        Else
            MyBase.OnSelectionChanged(e)
        End If
    End Sub

    Public ReadOnly Property CurrentColumn() As Integer
        Get
            Return CursorPosition.Column(Me, SelectionStart)
        End Get
    End Property

    Public ReadOnly Property CurrentLine() As Integer
        Get
            Return CursorPosition.Line(Me, SelectionStart)
        End Get
    End Property

    Public ReadOnly Property CurrentPosition() As Integer
        Get
            Return Me.SelectionStart
        End Get
    End Property

    Public ReadOnly Property SelectionEnd() As Integer
        Get
            Return SelectionStart + SelectionLength
        End Get
    End Property
End Class

Friend Class CursorPosition
    <System.Runtime.InteropServices.DllImport("user32")> _
    Public Shared Function GetCaretPos(ByRef lpPoint As Point) As Integer
    End Function

    Private Shared Function GetCorrection(ByVal e As RichTextBox, ByVal index As Integer) As Integer
        Dim pt1 As Point = Point.Empty
        GetCaretPos(pt1)
        Dim pt2 As Point = e.GetPositionFromCharIndex(index)

        If pt1 <> pt2 Then
            Return 1
        Else
            Return 0
        End If
    End Function

    Public Shared Function Line(ByVal e As RichTextBox, ByVal index As Integer) As Integer
        Dim correction As Integer = GetCorrection(e, index)
        Return e.GetLineFromCharIndex(index) - correction + 1
    End Function

    Public Shared Function Column(ByVal e As RichTextBox, ByVal index1 As Integer) As Integer
        Dim correction As Integer = GetCorrection(e, index1)
        Dim p As Point = e.GetPositionFromCharIndex(index1 - correction)

        If p.X = 1 Then
            Return 1
        End If

        p.X = 0
        Dim index2 As Integer = e.GetCharIndexFromPosition(p)

        Dim col As Integer = index1 - index2 + 1

        Return col
    End Function
End Class

