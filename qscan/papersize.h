#ifndef PAPERSIZE_H
#define PAPERSIZE_H

#include <QString>

class PaperSize
{
public:
  enum Layout
  {
    Landscape,
    Portrait,
  };
  enum Type
  {
    ISO_4A0,                /*!< ISO 4A0 page size */
    ISO_2A0,                /*!< ISO 2A0 page size */
    ISO_A0,                 /*!< ISO A0 page size */
    ISO_A1,                 /*!< ISO A1 page size */
    ISO_A2,                 /*!< ISO A2 page size */
    ISO_A3,                 /*!< ISO A3 page size */
    ISO_A4,                 /*!< ISO A4 page size */
    ISO_A5,                 /*!< ISO A5 page size */
    ISO_A6,                 /*!< ISO A6 page size */
    ISO_A7,                 /*!< ISO A7 page size */
    ISO_A8,                 /*!< ISO A8 page size */
    ISO_A9,                 /*!< ISO A9 page size */
    ISO_A10,                /*!< ISO A10 page size */
    ISO_B0,                 /*!< ISO B0 page size */
    ISO_B1,                 /*!< ISO B1 page size */
    ISO_B2,                 /*!< ISO B2 page size */
    ISO_B3,                 /*!< ISO B3 page size */
    ISO_B4,                 /*!< ISO B4 page size */
    ISO_B5,                 /*!< ISO B5 page size */
    ISO_B6,                 /*!< ISO B6 page size */
    ISO_B7,                 /*!< ISO B7 page size */
    ISO_B8,                 /*!< ISO B8 page size */
    ISO_B9,                 /*!< ISO B9 page size */
    ISO_B10,                /*!< ISO B10 page size */
    ISO_C0,                 /*!< ISO C0 page size */
    ISO_C1,                 /*!< ISO C1 page size */
    ISO_C2,                 /*!< ISO C2 page size */
    ISO_C3,                 /*!< ISO C3 page size */
    ISO_C4,                 /*!< ISO C4 page size */
    ISO_C5,                 /*!< ISO C5 page size */
    ISO_C6,                 /*!< ISO C6 page size */
    ISO_C7,                 /*!< ISO C7 page size */
    ISO_C8,                 /*!< ISO C8 page size */
    ISO_C9,                 /*!< ISO C9 page size */
    ISO_C10,                /*!< ISO C10 page size */
    US_Half_Letter,         /*!< US Half letter page size */
    US_Letter,              /*!< US Letter page size */
    US_Legal,               /*!< US Legal page size */
    US_Junior_Legal,        /*!< US Junior legal page size */
    US_Ledger,              /*!< US Ledger page size */
    US_Tabloid,             /*!< US Tabloid page size */
    JIS_B0,                 /*!< Japanese B0 page size */
    JIS_B1,                 /*!< Japanese B1 page size */
    JIS_B2,                 /*!< Japanese B2 page size */
    JIS_B3,                 /*!< Japanese B3 page size */
    JIS_B4,                 /*!< Japanese B4 page size */
    JIS_B5,                 /*!< Japanese B5 page size */
    JIS_B6,                 /*!< Japanese B6 page size */
    JIS_B7,                 /*!< Japanese B7 page size */
    JIS_B8,                 /*!< Japanese B8 page size */
    JIS_B9,                 /*!< Japanese B9 page size */
    JIS_B10,                /*!< Japanese B10 page size */
    JIS_A_ban,              /*!< Japanese A-ban page size */
    JIS_B_ban,              /*!< Japanese B-ban page size */
    Chinese_D0,             /*!< Chinese D0 page size */
    Chinese_D1,             /*!< Chinese D1 page size */
    Chinese_D2,             /*!< Chinese D2 page size */
    Chinese_D3,             /*!< Chinese D3 page size */
    Chinese_D4,             /*!< Chinese D4 page size */
    Chinese_D5,             /*!< Chinese D5 page size */
    Chinese_D6,             /*!< Chinese D6 page size */
    Chinese_RD0,            /*!< Chinese RD0 page size */
    Chinese_RD1,            /*!< Chinese RD1 page size */
    Chinese_RD2,            /*!< Chinese RD2 page size */
    Chinese_RD3,            /*!< Chinese RD3 page size */
    Chinese_RD4,            /*!< Chinese RD4 page size */
    Chinese_RD5,            /*!< Chinese RD5 page size */
    Chinese_RD6,            /*!< Chinese RD6 page size */
    Canadian_P1,            /*!< Canadian P1 page size */
    Canadian_P2,            /*!< Canadian P2 page size */
    Canadian_P3,            /*!< Canadian P3 page size */
    Canadian_P4,            /*!< Canadian P4 page size */
    Canadian_P5,            /*!< Canadian P5 page size */
    Canadian_P6,            /*!< Canadian P6 page size */
    Japanese_AB,            /*!< Japanese AB page size */
    Japanese_B40,           /*!< Japanese B40 page size */
    Japanese_Shikisen,      /*!< Japanese Shikisen page size */
    Japanese_A_Koban,       /*!< Japanese A Koban page size */
    Japanese_B_Koban,       /*!< Japanese B Koban page size */
    Japanese_Shirokuban_4,  /*!< Japanese Shiroku ban 4 page size */
    Japanese_Shirokuban_5,  /*!< Japanese Shiroku ban 5 page size */
    Japanese_Shirokuban_6,  /*!< Japanese Shiroku ban 6 page size */
    Japanese_Kiku_4,        /*!< Japanese Kiku 4 page size */
    Japanese_Kiku_5,        /*!< Japanese Kiku 5 page size */
    Japanese_Zenshi,        /*!< Japanese Traditional Zenshi page size */
    Japanese_Han_Kiri,      /*!< Japanese Traditional Han Kiri 5 page size */
    Japanese_Yatsu_giri,    /*!< Japanese Traditional Yatsu-giri page size */
    Japanese_Matsu_giri,    /*!< Japanese Traditional Matsu-giri page size */
    Japanese_Yotsu_giri,    /*!< Japanese Traditional Yotsu-giri page size */
    Japanese_Dai_Kyabine,   /*!< Japanese Traditional Dai-kyabine page size */
    Japanese_Kyabine,       /*!< Japanese Traditional Kyabine page size */
    Japanese_Nimai_gake,    /*!< Japanese Traditional Nimai-gake page size */
    Japanese_Potsuto_Kaado, /*!< Japanese Traditional Potsuto-kaado page size */
    Japanese_Dai_Tefuda,    /*!< Japanese Traditional Dai-teduda page size */
    Japanese_Saabisu,       /*!< Japanese Traditional Saabisu page size */
    Japanese_Tefuda,        /*!< Japanese Traditional Tefuda page size */
    Japanese_Dai_meishi,    /*!< Japanese Traditional Dai-meishi page size */
    Japanese_Kokusai_ban,   /*!< Japanese Traditional Dai-meishi page size */
    Japanese_Hyoujun_gata,  /*!< Japanese Traditional Hyoujun-gata page size */
    Japanese_Oo_gata,       /*!< Japanese Traditional Dai-meishi page size */
    Japanese_Chuu_gata,     /*!< Japanese Traditional Dai-meishi page size */
    Japanese_Ko_gata,       /*!< Japanese Traditional Dai-meishi page size */
    Japanese_Ippitsu_san,   /*!< Japanese Traditional Dai-meishi page size */
    Japanese_Hanshi,        /*!< Japanese Traditional Washi Hanshi page size */
    Japanese_Mino,          /*!< Japanese Traditional Washi Mino page size */
    Japanese_Oohousho,   /*!< Japanese Traditional Washi Oohousho page size */
    Japanese_Chuuhousho, /*!< Japanese Traditional Washi Chuuhousho page size */
    Japanese_Kohousho,   /*!< Japanese Traditional Washi Kohousho page size */
    Japanese_Nisho_no_uchi, /*!< Japanese Traditional Washi Nisho-no-uchi page
                               size */
    Japanese_Kusuma,        /*!< Japanese Traditional Washi Kusuma page size */
  };
  PaperSize();

  int width(const Type type, Layout layout = Portrait) const;
  int height(const Type type, Layout layout = Portrait) const;
  QString description(const Type type) const;

  static double toInches(int value) { return value / 25.4; }

protected:
  int m_width;
  int m_height;
};

#endif // PAPERSIZE_H
