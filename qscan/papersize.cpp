#include "papersize.h"

PaperSize::PaperSize() {}

/*!
   \brief Portrait width in mm, or Landscape width if the \c Layout parameter is set to \c
   PageSize::Landscape.

  \param type the PageSize::Type to get the width of.
  \param layout the   optional Layout value, default Portrait.
  \return the width in mm.
*/
int
PaperSize::width(const Type type, Layout layout) const
{
  if (layout == Landscape) {
    return height(type);
  }

  switch (type) {
  case US_Envelope_6_14:
    return 152;

  case US_Envelope_6_34:
    return 165;

  case US_Envelope_7:
    return 172;

  case US_Envelope_7_34_Monarch:
    return 191;

  case US_Envelope_8_58:
    return 219;

  case US_Envelope_9:
    return 225;

  case US_Envelope_10:
    return 241;

  case US_Envelope_11:
    return 264;

  case US_Envelope_12:
    return 279;

  case US_Envelope_14:
    return 292;

  case US_Envelope_16:
    return 305;

  case US_Envelope_A1:
    return 92;

  case US_Envelope_A2_Lady_Grey:
    return 146;

  case US_Envelope_A4:
    return 159;

  case US_Envelope_A6_Thompsons_Standard:
    return 165;

  case US_Envelope_A7_Besselheim:
    return 184;

  case US_Envelope_A8_Carrs:
    return 206;

  case US_Envelope_A9_Diplomat:
    return 222;

  case US_Envelope_A10_Willow:
    return 241;

  case US_Envelope_A_Long:
    return 225;

  case US_Envelope_1:
    return 229;

  case US_Envelope_1_34:
    return 241;

  case US_Envelope_3:
    return 254;

  case US_Envelope_6:
    return 267;

  case US_Envelope_8:
    return 286;

  case US_Envelope_9_34:
    return 286;

  case US_Envelope_10_12:
    return 305;

  case US_Envelope_12_12:
    return 318;

  case US_Envelope_13_12:
    return 330;

  case US_Envelope_14_12:
    return 368;

  case US_Envelope_15:
    return 381;

  case US_Envelope_15_12:
    return 394;

  case ISO_Envelope_DL:
    return 220;

  case ISO_Envelope_B4:
    return 353;

  case ISO_Envelope_B5:
    return 250;

  case ISO_Envelope_B6:
    return 176;

  case ISO_Envelope_C0:
    return 917;

  case ISO_Envelope_C1:
    return 648;

  case ISO_Envelope_C2:
    return 458;

  case ISO_Envelope_C3:
    return 458;

  case ISO_Envelope_C4:
    return 324;

  case ISO_Envelope_C5:
    return 229;

  case ISO_Envelope_C6_C5:
    return 229;

  case ISO_Envelope_C6:
    return 162;

  case ISO_Envelope_C7_C6:
    return 162;

  case ISO_Envelope_C7:
    return 114;

  case ISO_Envelope_E4:
    return 400;

  case Transitional_PA0:
    return 840;

  case Transitional_PA1:
    return 560;

  case Transitional_PA2:
    return 420;

  case Transitional_PA3:
    return 280;

  case Transitional_PA4:
    return 210;

  case Transitional_PA5:
    return 140;

  case Transitional_PA6:
    return 105;

  case Transitional_PA7:
    return 70;

  case Transitional_PA8:
    return 52;

  case Transitional_PA9:
    return 35;

  case Transitional_PA10:
    return 26;

  case Transitional_F0:
    return 841;

  case Transitional_F1:
    return 660;

  case Transitional_F2:
    return 420;

  case Transitional_F3:
    return 330;

  case Transitional_F4:
    return 210;

  case Transitional_F5:
    return 165;

  case Transitional_F6:
    return 105;

  case Transitional_F7:
    return 82;

  case Transitional_F8:
    return 52;

  case Transitional_F9:
    return 41;

  case Transitional_F10:
    return 26;

  case Imperial_Antiquarian:
    return 787;

  case Imperial_Atlas:
    return 660;

  case Imperial_Brief:
    return 343;

  case Imperial_Broadsheet:
    return 457;

  case Imperial_Cartridge:
    return 533;

  case Imperial_Columbier:
    return 597;

  case Imperial_Copy_Draught:
    return 406;

  case Imperial_Crown:
    return 381;

  case Imperial_Demy:
    return 445;

  case Imperial_Double_Demy:
    return 572;

  case Imperial_Quad_Demy:
    return 889;

  case Imperial_Elephant:
    return 584;

  case Imperial_Double_Elephant:
    return 678;

  case Imperial_Emperor:
    return 1219;

  case Imperial_Foolscap:
    return 343;

  case Imperial_Small_Foolscap:
    return 337;

  case Imperial_Grand_Eagle:
    return 730;

  case Imperial_Imperial:
    return 559;

  case Imperial_Medium:
    return 470;

  case Imperial_Monarch:
    return 184;

  case Imperial_Post:
    return 394;

  case Imperial_Sheet_Half_Post:
    return 495;

  case Imperial_Pinched_Post:
    return 375;

  case Imperial_Large_Post:
    return 394;

  case Imperial_Double_Large_Post:
    return 533;

  case Imperial_Double_Post:
    return 483;

  case Imperial_Pott:
    return 318;

  case Imperial_Princess:
    return 546;

  case Imperial_Quarto:
    return 229;

  case Imperial_Royal:
    return 508;

  case Imperial_Super_Royal:
    return 483;

  case British_Dukes:
    return 140;

  case British_Foolscap:
    return 203;

  case British_Imperial:
    return 178;

  case British_Kings:
    return 165;

  case British_Quarto:
    return 203;

  case RAW_RA0:
    return 860;

  case RAW_RA1:
    return 610;

  case RAW_RA2:
    return 430;

  case RAW_RA3:
    return 305;

  case RAW_RA4:
    return 215;

  case RAW_SRA0:
    return 900;

  case RAW_SRA1:
    return 640;

  case RAW_SRA2:
    return 450;

  case RAW_SRA3:
    return 320;

  case RAW_SRA4:
    return 225;

  case RAW_SRA1_Plus:
    return 660;

  case RAW_SRA2_Plus:
    return 480;

  case RAW_SRA3_Plus:
    return 320;

  case RAW_SRA3_Plus_Plus:
    return 320;

  case RAW_A0U:
    return 880;

  case RAW_A1U:
    return 625;

  case RAW_A2U:
    return 450;

  case RAW_A3U:
    return 330;

  case RAW_A4U:
    return 240;

  case French_Cloche:
    return 300;

  case French_Pot_ecolier:
    return 310;

  case French_Telliere:
    return 340;

  case French_Couronne_ecriture:
    return 360;

  case French_Couronne_edition:
    return 370;

  case French_Roberto:
    return 390;

  case French_Ecu:
    return 400;

  case French_Coquille:
    return 440;

  case French_Carre:
    return 450;

  case French_Cavalier:
    return 460;

  case French_Demi_raisin:
    return 325;

  case French_Raisin:
    return 500;

  case French_Double_Raisin:
    return 650;

  case French_Jesus:
    return 560;

  case French_Soliel:
    return 600;

  case French_Colombier_Affiche:
    return 600;

  case French_Colombier_Commercial:
    return 630;

  case French_Petit_Aigle:
    return 700;

  case French_Grand_Aigle:
    return 750;

  case French_Grand_Monde:
    return 900;

  case French_Univers:
    return 1000;

  case Columbian_Carta:
    return 216;

  case Columbian_Extra_Tabloide:
    return 304;

  case Columbian_Oficio:
    return 216;

  case Columbian_1_8_Pliego:
    return 250;

  case Columbian_1_4_Pliego:
    return 350;

  case Columbian_1_2_Pliego:
    return 500;

  case Columbian_Pliego:
    return 700;

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

  case Japanese_Potsutokaado:
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

  case Japanese_Ippitsu_sen:
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

  case JIS_B11:
    return 22;

  case JIS_B12:
    return 16;

  case Japanese_Envelope_Chou1:
    return 142;

  case JIS_Envelope_Chou2:
    return 119;

  case JIS_Envelope_Chou3:
    return 120;

  case Japanese_Envelope_Chou31:
    return 105;

  case Japanese_Envelope_Chou30:
    return 92;

  case Japanese_Envelope_Chou40:
    return 90;

  case JIS_Envelope_Chou4:
    return 90;

  case Japanese_Envelope_KakuA3:
    return 320;

  case Japanese_Envelope_Kaku0:
    return 287;

  case Japanese_Envelope_Kaku1:
    return 270;

  case JIS_Envelope_Kaku2:
    return 240;

  case Japanese_Envelope_Kaku_KokusaiA4:
    return 229;

  case JIS_Kaku3:
    return 216;

  case JIS_Kaku4:
    return 197;

  case JIS_Kaku5:
    return 190;

  case JIS_Kaku6:
    return 162;

  case JIS_Kaku7:
    return 142;

  case JIS_Kaku8:
    return 119;

  case Japanese_Envelope_You0:
    return 173;

  case JIS_You0:
    return 235;

  case JIS_You1:
    return 173;

  case JIS_You2:
    return 162;

  case JIS_You3:
    return 148;

  case JIS_You4:
    return 235;

  case JIS_You5:
    return 217;

  case JIS_You6:
    return 190;

  case JIS_You7:
    return 165;

  case ISO_4A0:
    return 1682;

  case ISO_2A0:
    return 1189;

  case ISO_A0:
    return 841;

  case ISO_A0_Plus:
    return 914;

  case ISO_A1_Plus:
    return 609;

  case ISO_A3_Plus:
    return 329;

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

  case US_Ledger:
    return 432;

  case US_Tabloid:
    return 279;

  case US_Gov_Letter:
    return 203;

  case US_Gov_Legal:
    return 216;

  case US_ANSI_A:
    return 216;

  case US_ANSI_B:
    return 279;

  case US_ANSI_C:
    return 432;

  case US_ANSI_D:
    return 559;

  case US_ANSI_E:
    return 864;

  case US_Arch_A:
    return 229;

  case US_Arch_B:
    return 305;

  case US_Arch_C:
    return 457;

  case US_Arch_D:
    return 610;

  case US_Arch_E:
    return 914;

  case US_Arch_E1:
    return 762;

  case US_Arch_E2:
    return 660;

  case US_Arch_E3:
    return 686;
  }

  return -1;
}

/*!
   \brief Portrait height in mm, or Landscape height if the \c Layout parameter is set to \c
   PageSize::Landscape.

  \param type the PageSize::Type to get the height of.
  \param layout the   optional Layout value, default Portrait.
  \return the height in mm.
*/
int
PaperSize::height(const Type type, Layout layout) const
{
  if (layout == Landscape) {
    return width(type);
  }

  switch (type) {
  case US_Envelope_6_14:
    return 89;

  case US_Envelope_6_34:
    return 92;

  case US_Envelope_7:
    return 95;

  case US_Envelope_7_34_Monarch:
    return 98;

  case US_Envelope_8_58:
    return 92;

  case US_Envelope_9:
    return 98;

  case US_Envelope_10:
    return 104;

  case US_Envelope_11:
    return 114;

  case US_Envelope_12:
    return 121;

  case US_Envelope_14:
    return 127;

  case US_Envelope_16:
    return 152;

  case US_Envelope_A1:
    return 130;

  case US_Envelope_A2_Lady_Grey:
    return 111;

  case US_Envelope_A4:
    return 108;

  case US_Envelope_A6_Thompsons_Standard:
    return 121;

  case US_Envelope_A7_Besselheim:
    return 133;

  case US_Envelope_A8_Carrs:
    return 140;

  case US_Envelope_A9_Diplomat:
    return 146;

  case US_Envelope_A10_Willow:
    return 152;

  case US_Envelope_A_Long:
    return 98;

  case US_Envelope_1:
    return 152;

  case US_Envelope_1_34:
    return 152;

  case US_Envelope_3:
    return 178;

  case US_Envelope_6:
    return 191;

  case US_Envelope_8:
    return 210;

  case US_Envelope_9_34:
    return 222;

  case US_Envelope_10_12:
    return 229;

  case US_Envelope_12_12:
    return 241;

  case US_Envelope_13_12:
    return 254;

  case US_Envelope_14_12:
    return 292;

  case US_Envelope_15:
    return 254;

  case US_Envelope_15_12:
    return 305;

  case ISO_Envelope_DL:
    return 110;

  case ISO_Envelope_B4:
    return 250;

  case ISO_Envelope_B5:
    return 176;

  case ISO_Envelope_B6:
    return 125;

  case ISO_Envelope_C0:
    return 1297;

  case ISO_Envelope_C1:
    return 914;

  case ISO_Envelope_C2:
    return 648;

  case ISO_Envelope_C3:
    return 324;

  case ISO_Envelope_C4:
    return 229;

  case ISO_Envelope_C5:
    return 162;

  case ISO_Envelope_C6_C5:
    return 114;

  case ISO_Envelope_C6:
    return 114;

  case ISO_Envelope_C7_C6:
    return 81;

  case ISO_Envelope_C7:
    return 81;

  case ISO_Envelope_E4:
    return 280;

  case Transitional_PA0:
    return 1120;

  case Transitional_PA1:
    return 840;

  case Transitional_PA2:
    return 560;

  case Transitional_PA3:
    return 420;

  case Transitional_PA4:
    return 280;

  case Transitional_PA5:
    return 210;

  case Transitional_PA6:
    return 140;

  case Transitional_PA7:
    return 105;

  case Transitional_PA8:
    return 70;

  case Transitional_PA9:
    return 52;

  case Transitional_PA10:
    return 35;

  case Transitional_F0:
    return 1321;

  case Transitional_F1:
    return 841;

  case Transitional_F2:
    return 660;

  case Transitional_F3:
    return 420;

  case Transitional_F4:
    return 330;

  case Transitional_F5:
    return 210;

  case Transitional_F6:
    return 165;

  case Transitional_F7:
    return 105;

  case Transitional_F8:
    return 82;

  case Transitional_F9:
    return 52;

  case Transitional_F10:
    return 41;

  case Imperial_Antiquarian:
    return 1346;

  case Imperial_Atlas:
    return 864;

  case Imperial_Brief:
    return 406;

  case Imperial_Broadsheet:
    return 610;

  case Imperial_Cartridge:
    return 660;

  case Imperial_Columbier:
    return 876;

  case Imperial_Copy_Draught:
    return 508;

  case Imperial_Crown:
    return 508;

  case Imperial_Demy:
    return 572;

  case Imperial_Double_Demy:
    return 902;

  case Imperial_Quad_Demy:
    return 1143;

  case Imperial_Elephant:
    return 711;

  case Imperial_Double_Elephant:
    return 1016;

  case Imperial_Emperor:
    return 1829;

  case Imperial_Foolscap:
    return 432;

  case Imperial_Small_Foolscap:
    return 419;

  case Imperial_Grand_Eagle:
    return 1067;

  case Imperial_Imperial:
    return 762;

  case Imperial_Medium:
    return 584;

  case Imperial_Monarch:
    return 267;

  case Imperial_Post:
    return 489;

  case Imperial_Sheet_Half_Post:
    return 597;

  case Imperial_Pinched_Post:
    return 470;

  case Imperial_Large_Post:
    return 508;

  case Imperial_Double_Large_Post:
    return 838;

  case Imperial_Double_Post:
    return 762;

  case Imperial_Pott:
    return 381;

  case Imperial_Princess:
    return 711;

  case Imperial_Quarto:
    return 279;

  case Imperial_Royal:
    return 635;

  case Imperial_Super_Royal:
    return 686;

  case British_Dukes:
    return 178;

  case British_Foolscap:
    return 330;

  case British_Imperial:
    return 229;

  case British_Kings:
    return 203;

  case British_Quarto:
    return 254;

  case RAW_RA0:
    return 1220;

  case RAW_RA1:
    return 860;

  case RAW_RA2:
    return 610;

  case RAW_RA3:
    return 430;

  case RAW_RA4:
    return 305;

  case RAW_SRA0:
    return 1280;

  case RAW_SRA1:
    return 900;

  case RAW_SRA2:
    return 640;

  case RAW_SRA3:
    return 450;

  case RAW_SRA4:
    return 320;

  case RAW_SRA1_Plus:
    return 920;

  case RAW_SRA2_Plus:
    return 650;

  case RAW_SRA3_Plus:
    return 460;

  case RAW_SRA3_Plus_Plus:
    return 464;

  case RAW_A0U:
    return 1230;

  case RAW_A1U:
    return 880;

  case RAW_A2U:
    return 625;

  case RAW_A3U:
    return 450;

  case RAW_A4U:
    return 330;

  case French_Cloche:
    return 400;

  case French_Pot_ecolier:
    return 400;

  case French_Telliere:
    return 440;

  case French_Couronne_ecriture:
    return 360;

  case French_Couronne_edition:
    return 470;

  case French_Roberto:
    return 500;

  case French_Ecu:
    return 520;

  case French_Coquille:
    return 560;

  case French_Carre:
    return 560;

  case French_Cavalier:
    return 620;

  case French_Demi_raisin:
    return 500;

  case French_Raisin:
    return 650;

  case French_Double_Raisin:
    return 1000;

  case French_Jesus:
    return 760;

  case French_Soliel:
    return 800;

  case French_Colombier_Affiche:
    return 800;

  case French_Colombier_Commercial:
    return 900;

  case French_Petit_Aigle:
    return 940;

  case French_Grand_Aigle:
    return 4050;

  case French_Grand_Monde:
    return 1260;

  case French_Univers:
    return 1130;

  case Columbian_Carta:
    return 279;

  case Columbian_Extra_Tabloide:
    return 457; // Actual size is 457.2mm

  case Columbian_Oficio:
    return 330;

  case Columbian_1_8_Pliego:
    return 350;

  case Columbian_1_4_Pliego:
    return 500;

  case Columbian_1_2_Pliego:
    return 700;

  case Columbian_Pliego:
    return 1000;

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

  case Japanese_Ippitsu_sen:
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

  case Japanese_Potsutokaado:
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

  case JIS_B11:
    return 32;

  case JIS_B12:
    return 22;

  case Japanese_Envelope_Chou1:
    return 332;

  case JIS_Envelope_Chou2:
    return 277;

  case JIS_Envelope_Chou3:
    return 235;

  case Japanese_Envelope_Chou31:
    return 235;

  case Japanese_Envelope_Chou30:
    return 235;

  case Japanese_Envelope_Chou40:
    return 225;

  case JIS_Envelope_Chou4:
    return 205;

  case Japanese_Envelope_KakuA3:
    return 440;

  case Japanese_Envelope_Kaku0:
    return 382;

  case Japanese_Envelope_Kaku1:
    return 382;

  case JIS_Envelope_Kaku2:
    return 332;

  case Japanese_Envelope_Kaku_KokusaiA4:
    return 324;

  case JIS_Kaku3:
    return 277;

  case JIS_Kaku4:
    return 267;

  case JIS_Kaku5:
    return 240;

  case JIS_Kaku6:
    return 229;

  case JIS_Kaku7:
    return 205;

  case JIS_Kaku8:
    return 197;

  case Japanese_Envelope_You0:
    return 118;

  case JIS_You0:
    return 120;

  case JIS_You1:
    return 118;

  case JIS_You2:
    return 114;

  case JIS_You3:
    return 98;

  case JIS_You4:
    return 105;

  case JIS_You5:
    return 95;

  case JIS_You6:
    return 98;

  case JIS_You7:
    return 92;

  case ISO_A0_Plus:
    return 1292;

  case ISO_A1_Plus:
    return 914;

  case ISO_A3_Plus:
    return 483;

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
    return 279;

  case US_Tabloid:
    return 432;

  case US_Gov_Letter:
    return 267;

  case US_Gov_Legal:
    return 330;

  case US_ANSI_A:
    return 279;

  case US_ANSI_B:
    return 432;

  case US_ANSI_C:
    return 559;

  case US_ANSI_D:
    return 864;

  case US_ANSI_E:
    return 1118;

  case US_Arch_A:
    return 305;

  case US_Arch_B:
    return 457;

  case US_Arch_C:
    return 610;

  case US_Arch_D:
    return 914;

  case US_Arch_E:
    return 1219;

  case US_Arch_E1:
    return 1067;

  case US_Arch_E2:
    return 965;

  case US_Arch_E3:
    return 991;
  }

  return -1;
}

/*!
  \brief A \c A short string description.

  Supplies a simple description string, suitable for use in a drop-down menu or similar.

  \param type the PageSize::Type to get the description for.
  \return the description string.
*/
QString
PaperSize::description(const Type type) const
{
  switch (type) {
  case US_Envelope_6_14:
    return QStringLiteral("US Envelope 6¼");

  case US_Envelope_6_34:
    return QStringLiteral("US Envelope 6¾");

  case US_Envelope_7:
    return QStringLiteral("US Envelope 7");

  case US_Envelope_7_34_Monarch:
    return QStringLiteral("US Envelope 7¾");

  case US_Envelope_8_58:
    return QStringLiteral("US Envelope 8⅝");

  case US_Envelope_9:
    return QStringLiteral("US Envelope 9");

  case US_Envelope_10:
    return QStringLiteral("US Envelope 10");

  case US_Envelope_11:
    return QStringLiteral("US Envelope 11");

  case US_Envelope_12:
    return QStringLiteral("US Envelope 12");

  case US_Envelope_14:
    return QStringLiteral("US Envelope 14");

  case US_Envelope_16:
    return QStringLiteral("US Envelope 16");

  case US_Envelope_A1:
    return QStringLiteral("US Envelope A1");

  case US_Envelope_A2_Lady_Grey:
    return QStringLiteral("US Envelope A2 (Lady Grey)");

  case US_Envelope_A4:
    return QStringLiteral("US Envelope A4");

  case US_Envelope_A6_Thompsons_Standard:
    return QStringLiteral("US Envelope A6 (Thompsons Standard)");

  case US_Envelope_A7_Besselheim:
    return QStringLiteral("US Envelope A7 (Besselheim)");

  case US_Envelope_A8_Carrs:
    return QStringLiteral("US Envelope A8 (Carrs)");

  case US_Envelope_A9_Diplomat:
    return QStringLiteral("US Envelope A9 (Diplomat)");

  case US_Envelope_A10_Willow:
    return QStringLiteral("US Envelope A10 (Willow)");

  case US_Envelope_A_Long:
    return QStringLiteral("US Envelope A Long");

  case US_Envelope_1:
    return QStringLiteral("US Envelope 1");

  case US_Envelope_1_34:
    return QStringLiteral("US Envelope 1¾");

  case US_Envelope_3:
    return QStringLiteral("US Envelope 3");

  case US_Envelope_6:
    return QStringLiteral("US Envelope 6");

  case US_Envelope_8:
    return QStringLiteral("US Envelope 8");

  case US_Envelope_9_34:
    return QStringLiteral("US Envelope 9¾");

  case US_Envelope_10_12:
    return QStringLiteral("US Envelope 10½");

  case US_Envelope_12_12:
    return QStringLiteral("US Envelope 12½");

  case US_Envelope_13_12:
    return QStringLiteral("US Envelope 13½");

  case US_Envelope_14_12:
    return QStringLiteral("US Envelope 14½");

  case US_Envelope_15:
    return QStringLiteral("US Envelope 15");

  case US_Envelope_15_12:
    return QStringLiteral("US Envelope 15½");

  case ISO_Envelope_DL:
    return QStringLiteral("ISO Envelope DL");

  case ISO_Envelope_B4:
    return QStringLiteral("ISO Envelope B4");

  case ISO_Envelope_B5:
    return QStringLiteral("ISO Envelope B5");

  case ISO_Envelope_B6:
    return QStringLiteral("ISO Envelope B6");

  case ISO_Envelope_C0:
    return QStringLiteral("ISO Envelope C0");

  case ISO_Envelope_C1:
    return QStringLiteral("ISO Envelope C1");

  case ISO_Envelope_C2:
    return QStringLiteral("ISO Envelope C2");

  case ISO_Envelope_C3:
    return QStringLiteral("ISO Envelope C3");

  case ISO_Envelope_C4:
    return QStringLiteral("ISO Envelope C4");

  case ISO_Envelope_C5:
    return QStringLiteral("ISO Envelope C5");

  case ISO_Envelope_C6_C5:
    return QStringLiteral("ISO Envelope C6/C5");

  case ISO_Envelope_C6:
    return QStringLiteral("ISO Envelope C6");

  case ISO_Envelope_C7_C6:
    return QStringLiteral("ISO Envelope C7/C6");

  case ISO_Envelope_C7:
    return QStringLiteral("ISO Envelope DL");

  case ISO_Envelope_E4:
    return QStringLiteral("ISO Envelope DL");

  case Transitional_PA0:
    return QStringLiteral("Transitional ISO PA0");

  case Transitional_PA1:
    return QStringLiteral("Transitional ISO PA1");

  case Transitional_PA2:
    return QStringLiteral("Transitional ISO PA2");

  case Transitional_PA3:
    return QStringLiteral("Transitional ISO PA3");

  case Transitional_PA4:
    return QStringLiteral("Transitional ISO PA4");

  case Transitional_PA5:
    return QStringLiteral("Transitional ISO PA5");

  case Transitional_PA6:
    return QStringLiteral("Transitional ISO PA6");

  case Transitional_PA7:
    return QStringLiteral("Transitional ISO PA7");

  case Transitional_PA8:
    return QStringLiteral("Transitional ISO PA8");

  case Transitional_PA9:
    return QStringLiteral("Transitional ISO PA9");

  case Transitional_PA10:
    return QStringLiteral("Transitional ISO PA10");

  case Transitional_F0:
    return QStringLiteral("Transitional ISO F0");

  case Transitional_F1:
    return QStringLiteral("Transitional ISO F1");

  case Transitional_F2:
    return QStringLiteral("Transitional ISO F2");

  case Transitional_F3:
    return QStringLiteral("Transitional ISO F3");

  case Transitional_F4:
    return QStringLiteral("Transitional ISO F4");

  case Transitional_F5:
    return QStringLiteral("Transitional ISO F5");

  case Transitional_F6:
    return QStringLiteral("Transitional ISO F6");

  case Transitional_F7:
    return QStringLiteral("Transitional ISO F7");

  case Transitional_F8:
    return QStringLiteral("Transitional ISO F8");

  case Transitional_F9:
    return QStringLiteral("Transitional ISO F9");

  case Transitional_F10:
    return QStringLiteral("Transitional ISO F10");

  case Imperial_Antiquarian:
    return QStringLiteral("Imperial British Antiquarian");

  case Imperial_Atlas:
    return QStringLiteral("Imperial British Atlas");

  case Imperial_Brief:
    return QStringLiteral("Imperial British Brief");

  case Imperial_Broadsheet:
    return QStringLiteral("Imperial British Broadsheet");

  case Imperial_Cartridge:
    return QStringLiteral("Imperial British Cartridge");

  case Imperial_Columbier:
    return QStringLiteral("Imperial British Columbier");

  case Imperial_Copy_Draught:
    return QStringLiteral("Imperial British Copy Draught");

  case Imperial_Crown:
    return QStringLiteral("Imperial British Crown");

  case Imperial_Demy:
    return QStringLiteral("Imperial British Demy");

  case Imperial_Double_Demy:
    return QStringLiteral("Imperial British Double Demy");

  case Imperial_Quad_Demy:
    return QStringLiteral("Imperial British Quad Demy");

  case Imperial_Elephant:
    return QStringLiteral("Imperial British Elephent");

  case Imperial_Double_Elephant:
    return QStringLiteral("Imperial British Double Elephant");

  case Imperial_Emperor:
    return QStringLiteral("Imperial British Emperor");

  case Imperial_Foolscap:
    return QStringLiteral("Imperial British Foolscap");

  case Imperial_Small_Foolscap:
    return QStringLiteral("Imperial British Small Foolscap");

  case Imperial_Grand_Eagle:
    return QStringLiteral("Imperial British Grand Eagle");

  case Imperial_Imperial:
    return QStringLiteral("Imperial British Imperial");

  case Imperial_Medium:
    return QStringLiteral("Imperial British Medium");

  case Imperial_Monarch:
    return QStringLiteral("Imperial British Monarch");

  case Imperial_Post:
    return QStringLiteral("Imperial British Post");

  case Imperial_Sheet_Half_Post:
    return QStringLiteral("Imperial British Sheet, Half Post");

  case Imperial_Pinched_Post:
    return QStringLiteral("Imperial British Pinched Post");

  case Imperial_Large_Post:
    return QStringLiteral("Imperial British Large Post");

  case Imperial_Double_Large_Post:
    return QStringLiteral("Imperial British Double Large Post");

  case Imperial_Double_Post:
    return QStringLiteral("Imperial British Double Post");

  case Imperial_Pott:
    return QStringLiteral("Imperial British Pott");

  case Imperial_Princess:
    return QStringLiteral("Imperial British Princess");

  case Imperial_Quarto:
    return QStringLiteral("Imperial British Quarto");

  case Imperial_Royal:
    return QStringLiteral("Imperial British Royal");

  case Imperial_Super_Royal:
    return QStringLiteral("Imperial British Super Royal");

  case British_Dukes:
    return QStringLiteral("Transitional British Dukes");

  case British_Foolscap:
    return QStringLiteral("Transitional British Foolscap");

  case British_Imperial:
    return QStringLiteral("Transitional British Imperial");

  case British_Kings:
    return QStringLiteral("Transitional British Kings");

  case British_Quarto:
    return QStringLiteral("Transitional British Quarto");

  case RAW_RA0:
    return QStringLiteral("RAW RA0");

  case RAW_RA1:
    return QStringLiteral("RAW RA1");

  case RAW_RA2:
    return QStringLiteral("RAW RA2");

  case RAW_RA3:
    return QStringLiteral("RAW RA3");

  case RAW_RA4:
    return QStringLiteral("RAW RA4");

  case RAW_SRA0:
    return QStringLiteral("RAW SRA0");

  case RAW_SRA1:
    return QStringLiteral("RAW SRA1");

  case RAW_SRA2:
    return QStringLiteral("RAW SRA2");

  case RAW_SRA3:
    return QStringLiteral("RAW SRA3");

  case RAW_SRA4:
    return QStringLiteral("RAW SRA4");

  case RAW_SRA1_Plus:
    return QStringLiteral("RAW SRA1+");

  case RAW_SRA2_Plus:
    return QStringLiteral("RAW SRA2+");

  case RAW_SRA3_Plus:
    return QStringLiteral("RAW SRA3+");

  case RAW_SRA3_Plus_Plus:
    return QStringLiteral("RAW SRA3++");

  case RAW_A0U:
    return QStringLiteral("RAW A0U");

  case RAW_A1U:
    return QStringLiteral("RAW A1U");

  case RAW_A2U:
    return QStringLiteral("RAW A2U");

  case RAW_A3U:
    return QStringLiteral("RAW A3U");

  case RAW_A4U:
    return QStringLiteral("RAW A4U");

  case French_Cloche:
    return QStringLiteral("French Cloche");

  case French_Pot_ecolier:
    return QStringLiteral("French Pot, écolier");

  case French_Telliere:
    return QStringLiteral("French Tellière");

  case French_Couronne_ecriture:
    return QStringLiteral("French Couronne écriture");

  case French_Couronne_edition:
    return QStringLiteral("French Couronne édition");

  case French_Roberto:
    return QStringLiteral("French Roberto");

  case French_Ecu:
    return QStringLiteral("French Écu");

  case French_Coquille:
    return QStringLiteral("French Coquille");

  case French_Carre:
    return QStringLiteral("French Carré");

  case French_Cavalier:
    return QStringLiteral("French Cavalier");

  case French_Demi_raisin:
    return QStringLiteral("French Demi-raisin");

  case French_Raisin:
    return QStringLiteral("French Raisin");

  case French_Double_Raisin:
    return QStringLiteral("French Double Raisin");

  case French_Jesus:
    return QStringLiteral("French Jésus");

  case French_Soliel:
    return QStringLiteral("French Soleil");

  case French_Colombier_Affiche:
    return QStringLiteral("French Colombier affiche");

  case French_Colombier_Commercial:
    return QStringLiteral("French Colombier commercial");

  case French_Petit_Aigle:
    return QStringLiteral("French Petit Aigle");

  case French_Grand_Aigle:
    return QStringLiteral("French Grand Aigle");

  case French_Grand_Monde:
    return QStringLiteral("French Grand Monde");

  case French_Univers:
    return QStringLiteral("French Univers");

  case Columbian_Carta:
    return QStringLiteral("Columbian Carta");

  case Columbian_Extra_Tabloide:
    return QStringLiteral("Columbian Extra Tabloide");

  case Columbian_Oficio:
    return QStringLiteral("Columbian Oficio");

  case Columbian_1_8_Pliego:
    return QStringLiteral("Columbian 1/8 Pliego");

  case Columbian_1_4_Pliego:
    return QStringLiteral("Columbian 1/4 Pliego");

  case Columbian_1_2_Pliego:
    return QStringLiteral("Columbian 1/2 Pliego");

  case Columbian_Pliego:
    return QStringLiteral("Columbian Pliego");

  case ISO_A0_Plus:
    return QStringLiteral("A0+");

  case ISO_A1_Plus:
    return QStringLiteral("A1+");

  case ISO_A3_Plus:
    return QStringLiteral("A3+");

  case ISO_4A0:
    return QStringLiteral("4A0");

  case ISO_2A0:
    return QStringLiteral("2A0");

  case ISO_A0:
    return QStringLiteral("A0");

  case ISO_A1:
    return QStringLiteral("A1");

  case ISO_A2:
    return QStringLiteral("A2");

  case ISO_A3:
    return QStringLiteral("A3");

  case ISO_A4:
    return QStringLiteral("A4");

  case ISO_A5:
    return QStringLiteral("A5");

  case ISO_A6:
    return QStringLiteral("A6");

  case ISO_A7:
    return QStringLiteral("A7");

  case ISO_A8:
    return QStringLiteral("A8");

  case ISO_A9:
    return QStringLiteral("A9");

  case ISO_A10:
    return QStringLiteral("A10");

  case ISO_B0:
    return QStringLiteral("B0");

  case ISO_B1:
    return QStringLiteral("B1");

  case ISO_B2:
    return QStringLiteral("B2");

  case ISO_B3:
    return QStringLiteral("B3");

  case ISO_B4:
    return QStringLiteral("B4");

  case ISO_B5:
    return QStringLiteral("B5");

  case ISO_B6:
    return QStringLiteral("B6");

  case ISO_B7:
    return QStringLiteral("B7");

  case ISO_B8:
    return QStringLiteral("B8");

  case ISO_B9:
    return QStringLiteral("B9");

  case ISO_B10:
    return QStringLiteral("B10");

  case ISO_C0:
    return QStringLiteral("C0");

  case ISO_C1:
    return QStringLiteral("C1");

  case ISO_C2:
    return QStringLiteral("C2");

  case ISO_C3:
    return QStringLiteral("C3");

  case ISO_C4:
    return QStringLiteral("C4");

  case ISO_C5:
    return QStringLiteral("C5");

  case ISO_C6:
    return QStringLiteral("C6");

  case ISO_C7:
    return QStringLiteral("C7");

  case ISO_C8:
    return QStringLiteral("C8");

  case ISO_C9:
    return QStringLiteral("C9");

  case ISO_C10:
    return QStringLiteral("C10");

  case US_Half_Letter:
    return QStringLiteral("Half letter");

  case US_Letter:
    return QStringLiteral("Letter");

  case US_Legal:
    return QStringLiteral("Legal");

  case US_Junior_Legal:
    return QStringLiteral("Junior Legal");

  case US_Ledger:
    return QStringLiteral("Ledger");

  case US_Tabloid:
    return QStringLiteral("Tabloid");

  case US_Gov_Letter:
    return QStringLiteral("Government Letter");

  case US_Gov_Legal:
    return QStringLiteral("Government Legal");

  case US_ANSI_A:
    return QStringLiteral("ANSI A");

  case US_ANSI_B:
    return QStringLiteral("ANSI B");

  case US_ANSI_C:
    return QStringLiteral("ANSI C");

  case US_ANSI_D:
    return QStringLiteral("ANSI D");

  case US_ANSI_E:
    return QStringLiteral("ANSI E");

  case US_Arch_A:
    return QStringLiteral("Architectural A");

  case US_Arch_B:
    return QStringLiteral("Architectural B");

  case US_Arch_C:
    return QStringLiteral("Architectural C");

  case US_Arch_D:
    return QStringLiteral("Architectural D");

  case US_Arch_E:
    return QStringLiteral("Architectural E");

  case US_Arch_E1:
    return QStringLiteral("Architectural E1");

  case US_Arch_E2:
    return QStringLiteral("Architectural E2");

  case US_Arch_E3:
    return QStringLiteral("Architectural E3");

  case JIS_B0:
    return QStringLiteral("JIS B0");

  case JIS_B1:
    return QStringLiteral("JIS B1");

  case JIS_B2:
    return QStringLiteral("JIS B2");

  case JIS_B3:
    return QStringLiteral("JIS B3");

  case JIS_B4:
    return QStringLiteral("JIS B4");

  case JIS_B5:
    return QStringLiteral("JIS B5");

  case JIS_B6:
    return QStringLiteral("JIS B6");

  case JIS_B7:
    return QStringLiteral("JIS B7");

  case JIS_B8:
    return QStringLiteral("JIS B8");

  case JIS_B9:
    return QStringLiteral("JIS B9");

  case JIS_B10:
    return QStringLiteral("JIS B10");

  case JIS_B11:
    return QStringLiteral("JIS B11");

  case JIS_B12:
    return QStringLiteral("JIS B12");

  case Japanese_Envelope_Chou1:
    return QStringLiteral("Japanese Envelope Chou1");

  case JIS_Envelope_Chou2:
    return QStringLiteral("JIS Envelope Chou2");

  case JIS_Envelope_Chou3:
    return QStringLiteral("JIS Envelope Chou3");

  case Japanese_Envelope_Chou31:
    return QStringLiteral("Japanese Envelope Chou31");

  case Japanese_Envelope_Chou30:
    return QStringLiteral("Japanese Envelope Chou30");

  case Japanese_Envelope_Chou40:
    return QStringLiteral("Japanese Envelope Chou40");

  case JIS_Envelope_Chou4:
    return QStringLiteral("JIS Envelope Chou4");

  case Japanese_Envelope_KakuA3:
    return QStringLiteral("Japanese Envelope KakuA3");

  case Japanese_Envelope_Kaku0:
    return QStringLiteral("Japanese Envelope Kaku0");

  case Japanese_Envelope_Kaku1:
    return QStringLiteral("Japanese Envelope Kaku1");

  case JIS_Envelope_Kaku2:
    return QStringLiteral("JIS Envelope Kaku2");

  case Japanese_Envelope_Kaku_KokusaiA4:
    return QStringLiteral("Japanese Envelope Kaku Koku-sai A4");

  case JIS_Kaku3:
    return QStringLiteral("JIS Envelope Kaku3");

  case JIS_Kaku4:
    return QStringLiteral("JIS Envelope Kaku4");

  case JIS_Kaku5:
    return QStringLiteral("JIS Envelope Kaku5");

  case JIS_Kaku6:
    return QStringLiteral("JIS Envelope Kaku6");

  case JIS_Kaku7:
    return QStringLiteral("JIS Envelope Kaku7");

  case JIS_Kaku8:
    return QStringLiteral("JIS Envelope Kaku8");

  case Japanese_Envelope_You0:
    return QStringLiteral("Japanese Envelope You0");

  case JIS_You0:
    return QStringLiteral("JIS Envelope You0");

  case JIS_You1:
    return QStringLiteral("JIS Envelope You1");

  case JIS_You2:
    return QStringLiteral("JIS Envelope You2");

  case JIS_You3:
    return QStringLiteral("JIS Envelope You3");

  case JIS_You4:
    return QStringLiteral("JIS Envelope You4");

  case JIS_You5:
    return QStringLiteral("JIS Envelope You5");

  case JIS_You6:
    return QStringLiteral("JIS Envelope You6");

  case JIS_You7:
    return QStringLiteral("JIS Envelope You7");

  case JIS_A_ban:
    return QStringLiteral("JIS A ban");

  case JIS_B_ban:
    return QStringLiteral("JIS B ban");

  case Chinese_D0:
    return QStringLiteral("Chinese D0");

  case Chinese_D1:
    return QStringLiteral("Chinese D1");

  case Chinese_D2:
    return QStringLiteral("Chinese D2");

  case Chinese_D3:
    return QStringLiteral("Chinese D3");

  case Chinese_D4:
    return QStringLiteral("Chinese D4");

  case Chinese_D5:
    return QStringLiteral("Chinese D5");

  case Chinese_D6:
    return QStringLiteral("Chinese D6");

  case Chinese_RD0:
    return QStringLiteral("Chinese RD0");

  case Chinese_RD1:
    return QStringLiteral("Chinese RD1");

  case Chinese_RD2:
    return QStringLiteral("Chinese RD2");

  case Chinese_RD3:
    return QStringLiteral("Chinese RD3");

  case Chinese_RD4:
    return QStringLiteral("Chinese RD4");

  case Chinese_RD5:
    return QStringLiteral("Chinese RD5");

  case Chinese_RD6:
    return QStringLiteral("Chinese RD6");

  case Canadian_P1:
    return QStringLiteral("Canadian P1");

  case Canadian_P2:
    return QStringLiteral("Canadian P2");

  case Canadian_P3:
    return QStringLiteral("Canadian P3");

  case Canadian_P4:
    return QStringLiteral("Canadian P4");

  case Canadian_P5:
    return QStringLiteral("Canadian P5");

  case Canadian_P6:
    return QStringLiteral("Canadian P6");

  case Japanese_AB:
    return QStringLiteral("Japanese AB");

  case Japanese_B40:
    return QStringLiteral("Japanese B40");

  case Japanese_Shikisen:
    return QStringLiteral("Japanese Shikisen");

  case Japanese_A_Koban:
    return QStringLiteral("Japanese A Koban");

  case Japanese_B_Koban:
    return QStringLiteral("Japanese B Koban");

  case Japanese_Shirokuban_4:
    return QStringLiteral("Japanese Shirokuban 4");

  case Japanese_Shirokuban_5:
    return QStringLiteral("Japanese Shirokuban 5");

  case Japanese_Shirokuban_6:
    return QStringLiteral("Japanese Shirokuban 6");

  case Japanese_Kiku_4:
    return QStringLiteral("Japanese Kiku 4");

  case Japanese_Kiku_5:
    return QStringLiteral("Japanese Kiku 5");

  case Japanese_Zenshi:
    return QStringLiteral("Japanese Zenshi");

  case Japanese_Han_Kiri:
    return QStringLiteral("Japanese Han-kiri");

  case Japanese_Yatsu_giri:
    return QStringLiteral("Japanese Yatsu-giri");

  case Japanese_Matsu_giri:
    return QStringLiteral("Japanese Matsu-giri");

  case Japanese_Yotsu_giri:
    return QStringLiteral("Japanese Yotsu-giri");

  case Japanese_Dai_Kyabine:
    return QStringLiteral("Japanese Dai Kyabine");

  case Japanese_Kyabine:
    return QStringLiteral("Japanese Kyabine");

  case Japanese_Nimai_gake:
    return QStringLiteral("Japanese Nimai-gake");

  case Japanese_Potsutokaado:
    return QStringLiteral("Japanese Potsutokaado");

  case Japanese_Dai_Tefuda:
    return QStringLiteral("Japanese  Dai Tefuda");

  case Japanese_Saabisu:
    return QStringLiteral("Japanese Saabisu");

  case Japanese_Tefuda:
    return QStringLiteral("Japanese Tefuda");

  case Japanese_Dai_meishi:
    return QStringLiteral("Japanese Dai Meishi");

  case Japanese_Kokusai_ban:
    return QStringLiteral("Japanese Kosusai-ban");

  case Japanese_Hyoujun_gata:
    return QStringLiteral("Japanese Hyoujun-gata");

  case Japanese_Oo_gata:
    return QStringLiteral("Oo-gata");

  case Japanese_Chuu_gata:
    return QStringLiteral("Chuu-gata");

  case Japanese_Ko_gata:
    return QStringLiteral("Japanese Ko-gata");

  case Japanese_Ippitsu_sen:
    return QStringLiteral("Japanese Ippitsu-sen");

  case Japanese_Hanshi:
    return QStringLiteral("Japanese Washi Hanshi");

  case Japanese_Mino:
    return QStringLiteral("Japanese Washi Mino");

  case Japanese_Oohousho:
    return QStringLiteral("Japanese Washi Oohousho");

  case Japanese_Chuuhousho:
    return QStringLiteral("Japanese Washi Chuuhousho");

  case Japanese_Kohousho:
    return QStringLiteral("Japanese Washi Kohousho");

  case Japanese_Nisho_no_uchi:
    return QStringLiteral("Japanese Washi Nishi no uchi");

  case Japanese_Kusuma:
    return QStringLiteral("Japanese Washi Kusuma");
  }

  return QString();
}
