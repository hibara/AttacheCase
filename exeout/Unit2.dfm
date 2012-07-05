object Form2: TForm2
  Left = 396
  Top = 257
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Form2'
  ClientHeight = 177
  ClientWidth = 319
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  DesignSize = (
    319
    177)
  PixelsPerInch = 96
  TextHeight = 13
  object imgMainIcon: TImage
    Left = 13
    Top = 16
    Width = 32
    Height = 32
    AutoSize = True
  end
  object lblWebSite: TLabel
    Left = 27
    Top = 126
    Width = 60
    Height = 13
    Cursor = crHandPoint
    Caption = 'lblWebSite'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = 16751001
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold, fsUnderline]
    ParentFont = False
    OnClick = lblWebSiteClick
    OnMouseMove = lblWebSiteMouseMove
    OnMouseLeave = lblWebSiteMouseLeave
  end
  object lblCopyright: TLabel
    Left = 8
    Top = 53
    Width = 303
    Height = 13
    Alignment = taCenter
    Anchors = [akLeft, akRight]
    AutoSize = False
    Caption = 'lblCopyright'
    ExplicitTop = 54
  end
  object lblAppName: TLabel
    Left = 54
    Top = 16
    Width = 257
    Height = 13
    Anchors = [akLeft, akTop, akRight]
    AutoSize = False
    Caption = 'lblAppName'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object lblVersion: TLabel
    Left = 54
    Top = 35
    Width = 257
    Height = 13
    Anchors = [akLeft, akTop, akRight]
    AutoSize = False
    Caption = 'lblVersion'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object lblDescription01: TLabel
    Left = 27
    Top = 78
    Width = 75
    Height = 13
    Caption = 'lblDescription01'
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object lblDescription02: TLabel
    Left = 27
    Top = 93
    Width = 75
    Height = 13
    Caption = 'lblDescription02'
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object lblDescription03: TLabel
    Left = 27
    Top = 109
    Width = 75
    Height = 13
    Caption = 'lblDescription03'
    Color = clBlack
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clGreen
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentColor = False
    ParentFont = False
  end
  object cmdExit: TButton
    Left = 228
    Top = 137
    Width = 75
    Height = 25
    Cancel = True
    Caption = '&OK'
    TabOrder = 0
    OnClick = cmdExitClick
  end
end
