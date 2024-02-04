object Form1: TForm1
  Left = 192
  Top = 256
  Width = 544
  Height = 375
  Caption = 'Form1'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object btnInput: TButton
    Left = 72
    Top = 48
    Width = 75
    Height = 25
    Caption = 'Input'
    TabOrder = 0
    OnClick = btnInputClick
  end
  object btnDrop: TButton
    Left = 72
    Top = 120
    Width = 75
    Height = 25
    Caption = 'Drop'
    TabOrder = 1
    OnClick = btnDropClick
  end
  object btnShow: TButton
    Left = 72
    Top = 88
    Width = 75
    Height = 25
    Caption = 'Show'
    TabOrder = 2
    OnClick = btnShowClick
  end
end
