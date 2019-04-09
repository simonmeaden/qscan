#include "papersize.h"

PaperSize::PaperSize() {}

int
PaperSize::width(const Type type, Layout layout) const
{
  if (layout == Landscape) {
    return height(type);
  }

  switch (type) {
    case Canadian_P1:
      return 560;

    case Canadian_P2:
      return 430;

    case Canadian_P3:
      return 280;

    case Canadian_P4:
      return 215;

    case Canadian_P5:
      return 140;

    case Canadian_P6:
      return 107;

    case Chinese_D0:
      return 764;

    case Chinese_D1:
      return 532;

    case Chinese_D2:
      return 380;

    case Chinese_D3:
      return 264;

    case Chinese_D4:
      return 188;

    case Chinese_D5:
      return 130;

    case Chinese_D6:
      return 92;

    case Chinese_RD0:
      return 787;

    case Chinese_RD1:
      return 546;

    case Chinese_RD2:
      return 393;

    case Chinese_RD3:
      return 273;

    case Chinese_RD4:
      return 196;

    case Chinese_RD5:
      return 136;

    case Chinese_RD6:
      return 98;

    case Japanese_Shirokuban_4:
      return 264;

    case Japanese_Shirokuban_5:
      return 189;

    case Japanese_Shirokuban_6:
      return 127;

    case Japanese_Kiku_4:
      return 227;

    case Japanese_Kiku_5:
      return 151;

    case Japanese_AB:
      return 210;

    case Japanese_B40:
      return 103;

    case Japanese_Shikisen:
      return 84;

    case Japanese_Zenshi:
      return 457;

    case Japanese_Han_Kiri:
      return 356;

    case Japanese_Yatsu_giri:
      return 165;

    case Japanese_Matsu_giri:
      return 203;

    case Japanese_Yotsu_giri:
      return 254;

    case Japanese_Dai_Kyabine:
      return 130;

    case Japanese_Kyabine:
      return 120;

    case Japanese_Nimai_gake:
      return 102;

    case Japanese_Potsuto_Kaado:
      return 89;

    case Japanese_Dai_Tefuda:
      return 90;

    case Japanese_Saabisu:
      return 82;

    case Japanese_Tefuda:
      return 76;

    case Japanese_Dai_meishi:
      return 65;

    case JIS_A_ban:
      return 625;

    case JIS_B_ban:
      return 765;

    case Japanese_A_Koban:
      return 608;

    case Japanese_B_Koban:
      return 754;

    case Japanese_Kokusai_ban:
      return 216;

    case Japanese_Hyoujun_gata:
      return 177;

    case Japanese_Oo_gata:
      return 177;

    case Japanese_Chuu_gata:
      return 162;

    case Japanese_Ko_gata:
      return 148;

    case Japanese_Ippitsu_san:
      return 82;

    case Japanese_Hanshi:
      return 242;

    case Japanese_Mino:
      return 273;

    case Japanese_Oohousho:
      return 394;

    case Japanese_Chuuhousho:
      return 364;

    case Japanese_Kohousho:
      return 333;

    case Japanese_Nisho_no_uchi:
      return 333;

    case Japanese_Kusuma:
      return 939;

    case JIS_B0:
      return 1030;

    case JIS_B1:
      return 728;

    case JIS_B2:
      return 515;

    case JIS_B3:
      return 364;

    case JIS_B4:
      return 257;

    case JIS_B5:
      return 182;

    case JIS_B6:
      return 128;

    case JIS_B7:
      return 91;

    case JIS_B8:
      return 64;

    case JIS_B9:
      return 45;

    case JIS_B10:
      return 32;

    case ISO_4A0:
      return 1682;

    case ISO_2A0:
      return 1189;

    case ISO_A0:
      return 841;

    case ISO_A1:
      return 594;

    case ISO_A2:
      return 420;

    case ISO_A3:
      return 297;

    case ISO_A4:
      return 210;

    case ISO_A5:
      return 148;

    case ISO_A6:
      return 105;

    case ISO_A7:
      return 74;

    case ISO_A8:
      return 52;

    case ISO_A9:
      return 37;

    case ISO_A10:
      return 26;

    case ISO_B0:
      return 1000;

    case ISO_B1:
      return 707;

    case ISO_B2:
      return 500;

    case ISO_B3:
      return 353;

    case ISO_B4:
      return 250;

    case ISO_B5:
      return 176;

    case ISO_B6:
      return 125;

    case ISO_B7:
      return 88;

    case ISO_B8:
      return 62;

    case ISO_B9:
      return 44;

    case ISO_B10:
      return 31;

    case ISO_C0:
      return 917;

    case ISO_C1:
      return 648;

    case ISO_C2:
      return 458;

    case ISO_C3:
      return 324;

    case ISO_C4:
      return 229;

    case ISO_C5:
      return 162;

    case ISO_C6:
      return 114;

    case ISO_C7:
      return 81;

    case ISO_C8:
      return 57;

    case ISO_C9:
      return 40;

    case ISO_C10:
      return 28;

    case US_Half_Letter:
      return 140;

    case US_Letter:
      return 216;

    case US_Legal:
      return 216;

    case US_Junior_Legal:
      return 127;

    case US_Ledger: // these are both the same thing
    case US_Tabloid:
      return 279;
  }
}

int
PaperSize::height(const Type type, Layout layout) const
{
  if (layout == Landscape) {
    return width(type);
  }

  switch (type) {
    case Canadian_P1:
      return 860;

    case Canadian_P2:
      return 560;

    case Canadian_P3:
      return 430;

    case Canadian_P4:
      return 280;

    case Canadian_P5:
      return 215;

    case Canadian_P6:
      return 140;

    case Chinese_D0:
      return 1064;

    case Chinese_D1:
      return 760;

    case Chinese_D2:
      return 528;

    case Chinese_D3:
      return 376;

    case Chinese_D4:
      return 260;

    case Chinese_D5:
      return 184;

    case Chinese_D6:
      return 126;

    case Chinese_RD0:
      return 1092;

    case Chinese_RD1:
      return 787;

    case Chinese_RD2:
      return 546;

    case Chinese_RD3:
      return 393;

    case Chinese_RD4:
      return 273;

    case Chinese_RD5:
      return 196;

    case Chinese_RD6:
      return 136;

    case Japanese_AB:
      return 00;

    case Japanese_B40:
      return 00;

    case Japanese_Shikisen:
      return 00;

    case JIS_A_ban:
      return 00;

    case JIS_B_ban:
      return 00;

    case Japanese_Kokusai_ban:
      return 280;

    case Japanese_Hyoujun_gata:
      return 250;

    case Japanese_Oo_gata:
      return 230;

    case Japanese_Chuu_gata:
      return 210;

    case Japanese_Ko_gata:
      return 210;

    case Japanese_Ippitsu_san:
      return 185;

    case Japanese_Hanshi:
      return 343;

    case Japanese_Mino:
      return 394;

    case Japanese_Oohousho:
      return 530;

    case Japanese_Chuuhousho:
      return 500;

    case Japanese_Kohousho:
      return 470;

    case Japanese_Nisho_no_uchi:
      return 485;

    case Japanese_Kusuma:
      return 1757;

    case Japanese_Zenshi:
      return 560;

    case Japanese_Han_Kiri:
      return 432;

    case Japanese_Yatsu_giri:
      return 216;

    case Japanese_Matsu_giri:
      return 254;

    case Japanese_Yotsu_giri:
      return 305;

    case Japanese_Dai_Kyabine:
      return 180;

    case Japanese_Kyabine:
      return 165;

    case Japanese_Nimai_gake:
      return 127;

    case Japanese_Potsuto_Kaado:
      return 140;

    case Japanese_Dai_Tefuda:
      return 130;

    case Japanese_Saabisu:
      return 114;

    case Japanese_Tefuda:
      return 112;

    case Japanese_Dai_meishi:
      return 90;

    case Japanese_Shirokuban_4:
      return 379;

    case Japanese_Shirokuban_5:
      return 262;

    case Japanese_Shirokuban_6:
      return 188;

    case Japanese_Kiku_4:
      return 306;

    case Japanese_Kiku_5:
      return 227;

    case Japanese_A_Koban:
      return 00;

    case Japanese_B_Koban:
      return 00;

    case JIS_B0:
      return 1456;

    case JIS_B1:
      return 1030;

    case JIS_B2:
      return 728;

    case JIS_B3:
      return 515;

    case JIS_B4:
      return 364;

    case JIS_B5:
      return 257;

    case JIS_B6:
      return 182;

    case JIS_B7:
      return 128;

    case JIS_B8:
      return 91;

    case JIS_B9:
      return 64;

    case JIS_B10:
      return 45;

    case ISO_4A0:
      return 2378;

    case ISO_2A0:
      return 1682;

    case ISO_A0:
      return 1189;

    case ISO_A1:
      return 841;

    case ISO_A2:
      return 594;

    case ISO_A3:
      return 420;

    case ISO_A4:
      return 297;

    case ISO_A5:
      return 210;

    case ISO_A6:
      return 148;

    case ISO_A7:
      return 105;

    case ISO_A8:
      return 74;

    case ISO_A9:
      return 52;

    case ISO_A10:
      return 37;

    case ISO_B0:
      return 1414;

    case ISO_B1:
      return 1000;

    case ISO_B2:
      return 707;

    case ISO_B3:
      return 500;

    case ISO_B4:
      return 353;

    case ISO_B5:
      return 250;

    case ISO_B6:
      return 176;

    case ISO_B7:
      return 125;

    case ISO_B8:
      return 88;

    case ISO_B9:
      return 62;

    case ISO_B10:
      return 44;

    case ISO_C0:
      return 1297;

    case ISO_C1:
      return 917;

    case ISO_C2:
      return 648;

    case ISO_C3:
      return 458;

    case ISO_C4:
      return 324;

    case ISO_C5:
      return 229;

    case ISO_C6:
      return 162;

    case ISO_C7:
      return 114;

    case ISO_C8:
      return 81;

    case ISO_C9:
      return 57;

    case ISO_C10:
      return 40;

    case US_Half_Letter:
      return 216;

    case US_Letter:
      return 279;

    case US_Legal:
      return 356;

    case US_Junior_Legal:
      return 203;

    case US_Ledger:
    case US_Tabloid:
      return 432;
  }
}

QString
PaperSize::description(const Type type) const
{
  switch (type) {
    case ISO_4A0:
      return QStringLiteral("ISO 216 international paper size standard : 4A0");
      ;
    case ISO_2A0:
      return QStringLiteral("ISO 216 international paper size standard : 2A0");
      ;
    case ISO_A0:
      return QStringLiteral("ISO 216 international paper size standard : A0");
      ;
    case ISO_A1:
      return QStringLiteral("ISO 216 international paper size standard : A1");
      ;
    case ISO_A2:
      return QStringLiteral("ISO 216 international paper size standard : A2");
      ;
    case ISO_A3:
      return QStringLiteral("ISO 216 international paper size standard : A3");
      ;
    case ISO_A4:
      return QStringLiteral("ISO 216 international paper size standard : A4");
      ;
    case ISO_A5:
      return QStringLiteral("ISO 216 international paper size standard : A5");
      ;
    case ISO_A6:
      return QStringLiteral("ISO 216 international paper size standard : A6");
      ;
    case ISO_A7:
      return QStringLiteral("ISO 216 international paper size standard : A7");
      ;
    case ISO_A8:
      return QStringLiteral("ISO 216 international paper size standard : A8");
      ;
    case ISO_A9:
      return QStringLiteral("ISO 216 international paper size standard : A9");
      ;
    case ISO_A10:
      return QStringLiteral("ISO 216 international paper size standard : A10");
      ;
    case ISO_B0:
      return QStringLiteral("ISO 216 international paper size standard : B0");
      ;
    case ISO_B1:
      return QStringLiteral("ISO 216 international paper size standard : B1");
      ;
    case ISO_B2:
      return QStringLiteral("ISO 216 international paper size standard : B2");
      ;
    case ISO_B3:
      return QStringLiteral("ISO 216 international paper size standard : B3");
      ;
    case ISO_B4:
      return QStringLiteral("ISO 216 international paper size standard : B4");
      ;
    case ISO_B5:
      return QStringLiteral("ISO 216 international paper size standard : B5");
      ;
    case ISO_B6:
      return QStringLiteral("ISO 216 international paper size standard : B6");
      ;
    case ISO_B7:
      return QStringLiteral("ISO 216 international paper size standard : B7");
      ;
    case ISO_B8:
      return QStringLiteral("ISO 216 international paper size standard : B8");
      ;
    case ISO_B9:
      return QStringLiteral("ISO 216 international paper size standard : B9");
      ;
    case ISO_B10:
      return QStringLiteral("ISO 216 international paper size standard : B10");
      ;
    case ISO_C0:
      return QStringLiteral("ISO 269 international paper size standard : C0, "
                            "these are most commonly used in envelopes");
      ;
    case ISO_C1:
      return QStringLiteral("ISO 269 international paper size standard : C1, "
                            "these are most commonly used in envelopes");
      ;
    case ISO_C2:
      return QStringLiteral("ISO 269 international paper size standard : C2, "
                            "these are most commonly used in envelopes");
      ;
    case ISO_C3:
      return QStringLiteral("ISO 269 international paper size standard : C3, "
                            "these are most commonly used in envelopes");
      ;
    case ISO_C4:
      return QStringLiteral("ISO 269 international paper size standard : C4, "
                            "these are most commonly used in envelopes");
      ;
    case ISO_C5:
      return QStringLiteral("ISO 269 international paper size standard : C5, "
                            "these are most commonly used in envelopes");
      ;
    case ISO_C6:
      return QStringLiteral("ISO 269 international paper size standard : C6, "
                            "these are most commonly used in envelopes");
      ;
    case ISO_C7:
      return QStringLiteral("ISO 269 international paper size standard : C7, "
                            "these are most commonly used in envelopes");
      ;
    case ISO_C8:
      return QStringLiteral("ISO 269 international paper size standard : C8, "
                            "these are most commonly used in envelopes");
      ;
    case ISO_C9:
      return QStringLiteral("ISO 269 international paper size standard : C9, "
                            "these are most commonly used in envelopes");
      ;
    case ISO_C10:
      return QStringLiteral("ISO 269 international paper size standard : C10, "
                            "these are most commonly used in envelopes");
      ;
    case US_Half_Letter:
      return QStringLiteral("");
      ;
    case US_Letter:
      return QStringLiteral("");
      ;
    case US_Legal:
      return QStringLiteral("");
      ;
    case US_Junior_Legal:
      return QStringLiteral("");
      ;
    case US_Ledger:
      return QStringLiteral("");
      ;
    case US_Tabloid:
      return QStringLiteral("");
      ;
    case JIS_B0:
      return QStringLiteral("");
      ;
    case JIS_B1:
      return QStringLiteral("");
      ;
    case JIS_B2:
      return QStringLiteral("");
      ;
    case JIS_B3:
      return QStringLiteral("");
      ;
    case JIS_B4:
      return QStringLiteral("");
      ;
    case JIS_B5:
      return QStringLiteral("");
      ;
    case JIS_B6:
      return QStringLiteral("");
      ;
    case JIS_B7:
      return QStringLiteral("");
      ;
    case JIS_B8:
      return QStringLiteral("");
      ;
    case JIS_B9:
      return QStringLiteral("");
      ;
    case JIS_B10:
      return QStringLiteral("");
      ;
    case JIS_A_ban:
      return QStringLiteral("");
      ;
    case JIS_B_ban:
      return QStringLiteral("");
      ;
    case Chinese_D0:
      return QStringLiteral("");
      ;
    case Chinese_D1:
      return QStringLiteral("");
      ;
    case Chinese_D2:
      return QStringLiteral("");
      ;
    case Chinese_D3:
      return QStringLiteral("");
      ;
    case Chinese_D4:
      return QStringLiteral("");
      ;
    case Chinese_D5:
      return QStringLiteral("");
      ;
    case Chinese_D6:
      return QStringLiteral("");
      ;
    case Chinese_RD0:
      return QStringLiteral("");
      ;
    case Chinese_RD1:
      return QStringLiteral("");
      ;
    case Chinese_RD2:
      return QStringLiteral("");
      ;
    case Chinese_RD3:
      return QStringLiteral("");
      ;
    case Chinese_RD4:
      return QStringLiteral("");
      ;
    case Chinese_RD5:
      return QStringLiteral("");
      ;
    case Chinese_RD6:
      return QStringLiteral("");
      ;
    case Canadian_P1:
      return QStringLiteral("");
      ;
    case Canadian_P2:
      return QStringLiteral("");
      ;
    case Canadian_P3:
      return QStringLiteral("");
      ;
    case Canadian_P4:
      return QStringLiteral("");
      ;
    case Canadian_P5:
      return QStringLiteral("");
      ;
    case Canadian_P6:
      return QStringLiteral("");
      ;
    case Japanese_AB:
      return QStringLiteral("");
      ;
    case Japanese_B40:
      return QStringLiteral("");
      ;
    case Japanese_Shikisen:
      return QStringLiteral("");
      ;
    case Japanese_A_Koban:
      return QStringLiteral("");
      ;
    case Japanese_B_Koban:
      return QStringLiteral("");
      ;
    case Japanese_Shirokuban_4:
      return QStringLiteral("");
      ;
    case Japanese_Shirokuban_5:
      return QStringLiteral("");
      ;
    case Japanese_Shirokuban_6:
      return QStringLiteral("");
      ;
    case Japanese_Kiku_4:
      return QStringLiteral("");
      ;
    case Japanese_Kiku_5:
      return QStringLiteral("");
      ;
    case Japanese_Zenshi:
      return QStringLiteral("");
      ;
    case Japanese_Han_Kiri:
      return QStringLiteral("");
      ;
    case Japanese_Yatsu_giri:
      return QStringLiteral("");
      ;
    case Japanese_Matsu_giri:
      return QStringLiteral("");
      ;
    case Japanese_Yotsu_giri:
      return QStringLiteral("");
      ;
    case Japanese_Dai_Kyabine:
      return QStringLiteral("");
      ;
    case Japanese_Kyabine:
      return QStringLiteral("");
      ;
    case Japanese_Nimai_gake:
      return QStringLiteral("");
      ;
    case Japanese_Potsuto_Kaado:
      return QStringLiteral("");
      ;
    case Japanese_Dai_Tefuda:
      return QStringLiteral("");
      ;
    case Japanese_Saabisu:
      return QStringLiteral("");
      ;
    case Japanese_Tefuda:
      return QStringLiteral("");
      ;
    case Japanese_Dai_meishi:
      return QStringLiteral("");
      ;
    case Japanese_Kokusai_ban:
      return QStringLiteral("");
      ;
    case Japanese_Hyoujun_gata:
      return QStringLiteral("");
      ;
    case Japanese_Oo_gata:
      return QStringLiteral("");
      ;
    case Japanese_Chuu_gata:
      return QStringLiteral("");
      ;
    case Japanese_Ko_gata:
      return QStringLiteral("");
      ;
    case Japanese_Ippitsu_san:
      return QStringLiteral("");
      ;
    case Japanese_Hanshi:
      return QStringLiteral("");
      ;
    case Japanese_Mino:
      return QStringLiteral("");
      ;
    case Japanese_Oohousho:
      return QStringLiteral("");
      ;
    case Japanese_Chuuhousho:
      return QStringLiteral("");
      ;
    case Japanese_Kohousho:
      return QStringLiteral("");
      ;
    case Japanese_Nisho_no_uchi:
      return QStringLiteral("");
      ;
    case Japanese_Kusuma:
      return QStringLiteral("");
      ;
  }
}
