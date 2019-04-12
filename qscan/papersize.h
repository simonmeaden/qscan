#ifndef PAPERSIZE_H
#define PAPERSIZE_H

#include <QList>
#include <QString>

/*!
  \class PaperSize
  \brief The PaperSize classgives values for width, height plus a simple QString definition.

  Defines paper sizes for a large number of standard and non-standard paper sizes
  from a number of different countries. They include the standard European (ISO) and
  American (Letter etc.), Canadian (P1 etc.), Japanese (JIS) and others, plus a number of
  non-standard traditional sizes.

  The class comprises three main methods, \c width(Type,Layout), \c height(Type,Layout)
  and \c description(Type). \c width() and \c height() both return a \c qreal as a
  result, while \c description() returns a simple \c QString description: \c
  description(PageSize::ISO_A4)for instace return a simple "A4", \c
  description(PageSize::Japanese_Yotsu_giri), however, yields a more complicated "Japanese
  Yotsu-giri". These, however, are all simple enough that they could be used in a descriptive
  drop-down combo box or similar.

  If no value of \c Layout is supplied then Portrait is assumed and ISO_A4 will return 210mm.
  However if \c PaperSize::Landscape is supplied then the value of 297mm is returned, the rotated
  width. The same applies to the \c height() method.

  If you want the sizes in Inches you can use the static \c toInches(int) method which returns a
  \c double value.

  This set is by no means complete and if there are any others that you need, or you notice a
  mistake in these existing values, let me know at simonmeaden@virginmedia.com. OK I was bored and
  got a bit carried away as a lot of these are not available except for specialists, Washi papers,
  for instance are made from the fibers of special plants and are unlikely to be used by computers,
  although they could possibly be used in a scanner, the reason I initially started this.
*/
class PaperSize
{
public:
  enum Layout
  {
    Landscape,
    Portrait,
  };

  /*!
    \brief The \c PaperSize::Type defines the selected paper size type.

    For example \c width(PaperSize::ISO_A4) will return a value of 210mm.
  */
  enum Type
  {
    ISO_4A0,                           /*!< ISO 4A0 page size */
    ISO_2A0,                           /*!< ISO 2A0 page size */
    ISO_A0,                            /*!< ISO A0 page size */
    ISO_A0_Plus,                       /*!< ISO A0+ page size */
    ISO_A1,                            /*!< ISO A1 page size */
    ISO_A1_Plus,                       /*!< ISO A1+ page size */
    ISO_A2,                            /*!< ISO A2 page size */
    ISO_A3,                            /*!< ISO A3+ page size */
    ISO_A3_Plus,                       /*!< ISO A3 page size */
    ISO_A4,                            /*!< ISO A4 page size */
    ISO_A5,                            /*!< ISO A5 page size */
    ISO_A6,                            /*!< ISO A6 page size */
    ISO_A7,                            /*!< ISO A7 page size */
    ISO_A8,                            /*!< ISO A8 page size */
    ISO_A9,                            /*!< ISO A9 page size */
    ISO_A10,                           /*!< ISO A10 page size */
    ISO_B0,                            /*!< ISO B0 page size */
    ISO_B1,                            /*!< ISO B1 page size */
    ISO_B2,                            /*!< ISO B2 page size */
    ISO_B3,                            /*!< ISO B3 page size */
    ISO_B4,                            /*!< ISO B4 page size */
    ISO_B5,                            /*!< ISO B5 page size */
    ISO_B6,                            /*!< ISO B6 page size */
    ISO_B7,                            /*!< ISO B7 page size */
    ISO_B8,                            /*!< ISO B8 page size */
    ISO_B9,                            /*!< ISO B9 page size */
    ISO_B10,                           /*!< ISO B10 page size */
    ISO_C0,                            /*!< ISO C0 page size */
    ISO_C1,                            /*!< ISO C1 page size */
    ISO_C2,                            /*!< ISO C2 page size */
    ISO_C3,                            /*!< ISO C3 page size */
    ISO_C4,                            /*!< ISO C4 page size */
    ISO_C5,                            /*!< ISO C5 page size */
    ISO_C6,                            /*!< ISO C6 page size */
    ISO_C7,                            /*!< ISO C7 page size */
    ISO_C8,                            /*!< ISO C8 page size */
    ISO_C9,                            /*!< ISO C9 page size */
    ISO_C10,                           /*!< ISO C10 page size */
    US_Half_Letter,                    /*!< US Half letter page size */
    US_Letter,                         /*!< US Letter page size */
    US_Legal,                          /*!< US Legal page size */
    US_Junior_Legal,                   /*!< US Junior legal page size */
    US_Ledger,                         /*!< US Ledger page size */
    US_Tabloid,                        /*!< US Tabloid page size */
    US_Gov_Letter,                     /*!< US Government Letter page size */
    US_Gov_Legal,                      /*!< US Government Legal page size */
    US_ANSI_A,                         /*!< US ANSI A page size */
    US_ANSI_B,                         /*!< US ANSI B page size */
    US_ANSI_C,                         /*!< US ANSI C page size */
    US_ANSI_D,                         /*!< US ANSI D page size */
    US_ANSI_E,                         /*!< US ANSI E page size */
    US_Arch_A,                         /*!< US Archtectural A page size */
    US_Arch_B,                         /*!< US Archtectural B page size */
    US_Arch_C,                         /*!< US Archtectural C page size */
    US_Arch_D,                         /*!< US Archtectural D page size */
    US_Arch_E,                         /*!< US Archtectural E page size */
    US_Arch_E1,                        /*!< US Archtectural E1 page size */
    US_Arch_E2,                        /*!< US Archtectural E2 page size */
    US_Arch_E3,                        /*!< US Archtectural E3 page size */
    JIS_B0,                            /*!< Japanese B0 page size */
    JIS_B1,                            /*!< Japanese B1 page size */
    JIS_B2,                            /*!< Japanese B2 page size */
    JIS_B3,                            /*!< Japanese B3 page size */
    JIS_B4,                            /*!< Japanese B4 page size */
    JIS_B5,                            /*!< Japanese B5 page size */
    JIS_B6,                            /*!< Japanese B6 page size */
    JIS_B7,                            /*!< Japanese B7 page size */
    JIS_B8,                            /*!< Japanese B8 page size */
    JIS_B9,                            /*!< Japanese B9 page size */
    JIS_B10,                           /*!< Japanese B10 page size */
    JIS_B11,                           /*!< Japanese B11 page size */
    JIS_B12,                           /*!< Japanese B12 page size */
    JIS_A_ban,                         /*!< Japanese A-ban page size */
    JIS_B_ban,                         /*!< Japanese B-ban page size */
    Chinese_D0,                        /*!< Chinese D0 page size */
    Chinese_D1,                        /*!< Chinese D1 page size */
    Chinese_D2,                        /*!< Chinese D2 page size */
    Chinese_D3,                        /*!< Chinese D3 page size */
    Chinese_D4,                        /*!< Chinese D4 page size */
    Chinese_D5,                        /*!< Chinese D5 page size */
    Chinese_D6,                        /*!< Chinese D6 page size */
    Chinese_RD0,                       /*!< Chinese RD0 page size */
    Chinese_RD1,                       /*!< Chinese RD1 page size */
    Chinese_RD2,                       /*!< Chinese RD2 page size */
    Chinese_RD3,                       /*!< Chinese RD3 page size */
    Chinese_RD4,                       /*!< Chinese RD4 page size */
    Chinese_RD5,                       /*!< Chinese RD5 page size */
    Chinese_RD6,                       /*!< Chinese RD6 page size */
    Canadian_P1,                       /*!< Canadian P1 page size */
    Canadian_P2,                       /*!< Canadian P2 page size */
    Canadian_P3,                       /*!< Canadian P3 page size */
    Canadian_P4,                       /*!< Canadian P4 page size */
    Canadian_P5,                       /*!< Canadian P5 page size */
    Canadian_P6,                       /*!< Canadian P6 page size */
    Japanese_AB,                       /*!< Japanese AB page size */
    Japanese_B40,                      /*!< Japanese B40 page size */
    Japanese_Shikisen,                 /*!< Japanese Shikisen page size */
    Japanese_A_Koban,                  /*!< Japanese A Koban page size */
    Japanese_B_Koban,                  /*!< Japanese B Koban page size */
    Japanese_Shirokuban_4,             /*!< Japanese Shiroku ban 4 page size */
    Japanese_Shirokuban_5,             /*!< Japanese Shiroku ban 5 page size */
    Japanese_Shirokuban_6,             /*!< Japanese Shiroku ban 6 page size */
    Japanese_Kiku_4,                   /*!< Japanese Kiku 4 page size */
    Japanese_Kiku_5,                   /*!< Japanese Kiku 5 page size */
    Japanese_Zenshi,                   /*!< Japanese Traditional Zenshi page size */
    Japanese_Han_Kiri,                 /*!< Japanese Traditional Han Kiri 5 page size */
    Japanese_Yatsu_giri,               /*!< Japanese Traditional Yatsu-giri page size */
    Japanese_Matsu_giri,               /*!< Japanese Traditional Matsu-giri page size */
    Japanese_Yotsu_giri,               /*!< Japanese Traditional Yotsu-giri page size */
    Japanese_Dai_Kyabine,              /*!< Japanese Traditional Dai-kyabine page size */
    Japanese_Kyabine,                  /*!< Japanese Traditional Kyabine page size */
    Japanese_Nimai_gake,               /*!< Japanese Traditional Nimai-gake page size */
    Japanese_Potsutokaado,             /*!< Japanese Traditional Potsuto-kaado page size */
    Japanese_Dai_Tefuda,               /*!< Japanese Traditional Dai-teduda page size */
    Japanese_Saabisu,                  /*!< Japanese Traditional Saabisu page size */
    Japanese_Tefuda,                   /*!< Japanese Traditional Tefuda page size */
    Japanese_Dai_meishi,               /*!< Japanese Traditional Dai-meishi page size */
    Japanese_Kokusai_ban,              /*!< Japanese Traditional Dai-meishi page size */
    Japanese_Hyoujun_gata,             /*!< Japanese Traditional Hyoujun-gata page size */
    Japanese_Oo_gata,                  /*!< Japanese Traditional Dai-meishi page size */
    Japanese_Chuu_gata,                /*!< Japanese Traditional Dai-meishi page size */
    Japanese_Ko_gata,                  /*!< Japanese Traditional Dai-meishi page size */
    Japanese_Ippitsu_sen,              /*!< Japanese Traditional Dai-meishi page size */
    Japanese_Hanshi,                   /*!< Japanese Traditional Washi Hanshi page size */
    Japanese_Mino,                     /*!< Japanese Traditional Washi Mino page size */
    Japanese_Oohousho,                 /*!< Japanese Traditional Washi Oohousho page size */
    Japanese_Chuuhousho,               /*!< Japanese Traditional Washi Chuuhousho page size */
    Japanese_Kohousho,                 /*!< Japanese Traditional Washi Kohousho page size */
    Japanese_Nisho_no_uchi,            /*!< Japanese Traditional Washi Nisho-no-uchi page
                                          size */
    Japanese_Kusuma,                   /*!< Japanese Traditional Washi Kusuma page size */
    Japanese_Envelope_Chou1,           /*!< Japanese Non-JIS Envelope Chou1 */
    JIS_Envelope_Chou2,                /*!< Japanese JIS Envelope Chou2*/
    JIS_Envelope_Chou3,                /*!< Japanese JIS Envelope Chou3 */
    Japanese_Envelope_Chou31,          /*!< Japanese Non-JIS Envelope Chou31 */
    Japanese_Envelope_Chou30,          /*!< Japanese Non-JIS Envelope Chou30 */
    Japanese_Envelope_Chou40,          /*!< Japanese Non-JIS Envelope Chou40 */
    JIS_Envelope_Chou4,                /*!< Japanese JIS Envelope Chou4 */
    Japanese_Envelope_KakuA3,          /*!< Japanese Non-JIS Envelope KakuA3 */
    Japanese_Envelope_Kaku0,           /*!< Japanese Non-JIS Envelope Kaku0 */
    Japanese_Envelope_Kaku1,           /*!< Japanese Non-JIS Envelope Kaku1 */
    JIS_Envelope_Kaku2,                /*!< Japanese JIS Envelope Kaku2 */
    Japanese_Envelope_Kaku_KokusaiA4,  /*!< Japanese Non-JIS Envelope KakuA3 */
    JIS_Kaku3,                         /*!< Japanese JIS Envelope Kaku3 */
    JIS_Kaku4,                         /*!< Japanese JIS Envelope Kaku4 */
    JIS_Kaku5,                         /*!< Japanese JIS Envelope Kaku5 */
    JIS_Kaku6,                         /*!< Japanese JIS Envelope Kaku6 */
    JIS_Kaku7,                         /*!< Japanese JIS Envelope Kaku7 */
    JIS_Kaku8,                         /*!< Japanese JIS Envelope Kaku8 */
    Japanese_Envelope_You0,            /*!< Japanese Non-JIS Envelope You1 */
    JIS_You0,                          /*!< Japanese JIS Envelope You0 */
    JIS_You1,                          /*!< Japanese JIS Envelope You1 */
    JIS_You2,                          /*!< Japanese JIS Envelope You2 */
    JIS_You3,                          /*!< Japanese JIS Envelope You3 */
    JIS_You4,                          /*!< Japanese JIS Envelope You4 */
    JIS_You5,                          /*!< Japanese JIS Envelope You5 */
    JIS_You6,                          /*!< Japanese JIS Envelope You6 */
    JIS_You7,                          /*!< Japanese JIS Envelope You7 */
    Columbian_Carta,                   /*!< Columbian B40 page size */
    Columbian_Extra_Tabloide,          /*!< Columbian Wxtra Tabloide page size */
    Columbian_Oficio,                  /*!< Columbian Oficio page size */
    Columbian_1_8_Pliego,              /*!< Columbian 1/8 Pliago page size */
    Columbian_1_4_Pliego,              /*!< Columbian 1/4 Pliego page size */
    Columbian_1_2_Pliego,              /*!< Columbian 1/2 Pliego page size */
    Columbian_Pliego,                  /*!< Columbian Pliego page size */
    French_Cloche,                     /*!< French Cloche page size */
    French_Pot_ecolier,                /*!< French Pot, écolier page size */
    French_Telliere,                   /*!< French Tellière page size */
    French_Couronne_ecriture,          /*!< French Couronne écriture page size */
    French_Couronne_edition,           /*!< French Couronne édition page size */
    French_Roberto,                    /*!< French Roberto page size */
    French_Ecu,                        /*!< French Écu page size */
    French_Coquille,                   /*!< French Coquille page size */
    French_Carre,                      /*!< French Carré page size */
    French_Cavalier,                   /*!< French Cavalier page size */
    French_Demi_raisin,                /*!< French Demi-raisin page size */
    French_Raisin,                     /*!< French Raisin page size */
    French_Double_Raisin,              /*!< French Double Raisin page size */
    French_Jesus,                      /*!< French Jésus page size */
    French_Soliel,                     /*!< French Soleil page size */
    French_Colombier_Affiche,          /*!< French Colombier affiche page size */
    French_Colombier_Commercial,       /*!< French Colombier commercial page size */
    French_Petit_Aigle,                /*!< French Petit Aigle page size */
    French_Grand_Aigle,                /*!< French Grand Aigle page size */
    French_Grand_Monde,                /*!< French Grand Monde page size */
    French_Univers,                    /*!< French Univers page size */
    RAW_RA0,                           /*!< ISO 217:1995 RAW format RA0 page size */
    RAW_RA1,                           /*!< ISO 217:1995 RAW format RA1 page size */
    RAW_RA2,                           /*!< ISO 217:1995 RAW format RA2 page size */
    RAW_RA3,                           /*!< ISO 217:1995 RAW format RA3 page size */
    RAW_RA4,                           /*!< ISO 217:1995 RAW format RA4 page size */
    RAW_SRA0,                          /*!< ISO 217:1995 RAW format SRA0 page size */
    RAW_SRA1,                          /*!< ISO 217:1995 RAW format SRA1 page size */
    RAW_SRA2,                          /*!< ISO 217:1995 RAW format SRA2 page size */
    RAW_SRA3,                          /*!< ISO 217:1995 RAW format SRA3 page size */
    RAW_SRA4,                          /*!< ISO 217:1995 RAW format SRA4 page size */
    RAW_SRA1_Plus,                     /*!< ISO 217:1995 RAW format SRA1+ page size */
    RAW_SRA2_Plus,                     /*!< ISO 217:1995 RAW format SRA2+ page size */
    RAW_SRA3_Plus,                     /*!< ISO 217:1995 RAW format SRA3+ page size */
    RAW_SRA3_Plus_Plus,                /*!< ISO 217:1995 RAW format SRA3++ page size */
    RAW_A0U,                           /*!< ISO 217:1995 RAW format A0U page size */
    RAW_A1U,                           /*!< ISO 217:1995 RAW format A1U page size */
    RAW_A2U,                           /*!< ISO 217:1995 RAW format A2U page size */
    RAW_A3U,                           /*!< ISO 217:1995 RAW format A3U page size */
    RAW_A4U,                           /*!< ISO 217:1995 RAW format A4U page size */
    British_Dukes,                     /*!< Transitional British Dukes page size */
    British_Foolscap,                  /*!< Transitional British Foolscap page size */
    British_Imperial,                  /*!< Transitional British Imperial page size */
    British_Kings,                     /*!< Transitional British Kings page size */
    British_Quarto,                    /*!< Transitional British Quarto page size */
    Imperial_Antiquarian,              /*!< British Imperial Antiquarian page size */
    Imperial_Atlas,                    /*!< British Imperial Atlas page size */
    Imperial_Brief,                    /*!< British Imperial Brief page size */
    Imperial_Broadsheet,               /*!< British Imperial Broadsheet page size */
    Imperial_Cartridge,                /*!< British Imperial Cartridge page size */
    Imperial_Columbier,                /*!< British Imperial Columbier page size */
    Imperial_Copy_Draught,             /*!< British Imperial Copy Draught page size */
    Imperial_Crown,                    /*!< British Imperial Crown page size */
    Imperial_Demy,                     /*!< British Imperial Demy page size */
    Imperial_Double_Demy,              /*!< British Imperial Quarto page size */
    Imperial_Quad_Demy,                /*!< British Imperial Double Demy page size */
    Imperial_Elephant,                 /*!< British Imperial Elephant page size */
    Imperial_Double_Elephant,          /*!< British Imperial Double Elephant page size */
    Imperial_Emperor,                  /*!< British Imperial Emperor page size */
    Imperial_Foolscap,                 /*!< British Imperial Foolscap page size */
    Imperial_Small_Foolscap,           /*!< British Imperial Small Foolscap page size */
    Imperial_Grand_Eagle,              /*!< British Imperial Grand Eagle page size */
    Imperial_Imperial,                 /*!< British Imperial Imperial page size */
    Imperial_Medium,                   /*!< British Imperial Medium page size */
    Imperial_Monarch,                  /*!< British Imperial Monarch page size */
    Imperial_Post,                     /*!< British Imperial Post page size */
    Imperial_Sheet_Half_Post,          /*!< British Imperial Sheet, Half Post page size */
    Imperial_Pinched_Post,             /*!< British Imperial Pinched Post page size */
    Imperial_Large_Post,               /*!< British Imperial Large Post page size */
    Imperial_Double_Large_Post,        /*!< British Imperial Double Large Post page size */
    Imperial_Double_Post,              /*!< British Imperial Double Post page size */
    Imperial_Pott,                     /*!< British Imperial Pott page size */
    Imperial_Princess,                 /*!< British Imperial Princess page size */
    Imperial_Quarto,                   /*!< British Imperial Quarto page size */
    Imperial_Royal,                    /*!< British Imperial Royal page size */
    Imperial_Super_Royal,              /*!< British Imperial Super Royal page size */
    Transitional_PA0,                  /*!< Transtional PA0 page size */
    Transitional_PA1,                  /*!< Transtional PA1 page size */
    Transitional_PA2,                  /*!< Transtional PA2 page size */
    Transitional_PA3,                  /*!< Transtional PA3 page size */
    Transitional_PA4,                  /*!< Transtional PA4 page size */
    Transitional_PA5,                  /*!< Transtional PA5 page size */
    Transitional_PA6,                  /*!< Transtional PA6 page size */
    Transitional_PA7,                  /*!< Transtional PA7 page size */
    Transitional_PA8,                  /*!< Transtional PA8 page size */
    Transitional_PA9,                  /*!< Transtional PA9 page size */
    Transitional_PA10,                 /*!< Transtional PA10 page size */
    Transitional_F0,                   /*!< Transtional F0 page size */
    Transitional_F1,                   /*!< Transtional F1 page size */
    Transitional_F2,                   /*!< Transtional F2 page size */
    Transitional_F3,                   /*!< Transtional F3 page size */
    Transitional_F4,                   /*!< Transtional F4 page size */
    Transitional_F5,                   /*!< Transtional F5 page size */
    Transitional_F6,                   /*!< Transtional F6 page size */
    Transitional_F7,                   /*!< Transtional F7 page size */
    Transitional_F8,                   /*!< Transtional F8 page size */
    Transitional_F9,                   /*!< Transtional F9 page size */
    Transitional_F10,                  /*!< Transtional F10 page size */
    ISO_Envelope_DL,                   /*!< ISO Envelope DL size */
    ISO_Envelope_B4,                   /*!< ISO Envelope B4 size */
    ISO_Envelope_B5,                   /*!< ISO Envelope B5 size */
    ISO_Envelope_B6,                   /*!< ISO Envelope B6 size */
    ISO_Envelope_C0,                   /*!< ISO Envelope C0 size */
    ISO_Envelope_C1,                   /*!< ISO Envelope C1 size */
    ISO_Envelope_C2,                   /*!< ISO Envelope C2 size */
    ISO_Envelope_C3,                   /*!< ISO Envelope C3 size */
    ISO_Envelope_C4,                   /*!< ISO Envelope C4 size */
    ISO_Envelope_C5,                   /*!< ISO Envelope C5 size */
    ISO_Envelope_C6_C5,                /*!< ISO Envelope C6/C5 size */
    ISO_Envelope_C6,                   /*!< ISO Envelope C6 size */
    ISO_Envelope_C7_C6,                /*!< ISO Envelope C7/C6 size */
    ISO_Envelope_C7,                   /*!< ISO Envelope C7 size */
    ISO_Envelope_E4,                   /*!< ISO Envelope E4 size */
    US_Envelope_6_14,                  /*!< US Envelope 6¼ size */
    US_Envelope_6_34,                  /*!< US Envelope 6¼ size */
    US_Envelope_7,                     /*!< US Envelope 7 size */
    US_Envelope_7_34_Monarch,          /*!< US Envelope 7¾ size */
    US_Envelope_8_58,                  /*!< US Envelope 8⅝ size */
    US_Envelope_9,                     /*!< US Envelope 9 size */
    US_Envelope_10,                    /*!< US Envelope 10 size */
    US_Envelope_11,                    /*!< US Envelope 11 size */
    US_Envelope_12,                    /*!< US Envelope 12 size */
    US_Envelope_14,                    /*!< US Envelope 14 size */
    US_Envelope_16,                    /*!< US Envelope 16 size */
    US_Envelope_A1,                    /*!< US Envelope A1 size */
    US_Envelope_A2_Lady_Grey,          /*!< US Envelope A2 (Lady Grey) size */
    US_Envelope_A4,                    /*!< US Envelope A4 size */
    US_Envelope_A6_Thompsons_Standard, /*!< US Envelope A6 (Thompsons Standard) size */
    US_Envelope_A7_Besselheim,         /*!< US Envelope A7 (Besselheim) size */
    US_Envelope_A8_Carrs,              /*!< US Envelope A8 (Carrs) size */
    US_Envelope_A9_Diplomat,           /*!< US Envelope A9 (Diplomat) size */
    US_Envelope_A10_Willow,            /*!< US Envelope A10 (Willow) size */
    US_Envelope_A_Long,                /*!< US Envelope A Long size */
    US_Envelope_1,                     /*!< US Envelope 1 size */
    US_Envelope_1_34,                  /*!< US Envelope 1¾ size */
    US_Envelope_3,                     /*!< US Envelope 3 size */
    US_Envelope_6,                     /*!< US Envelope 6 size */
    US_Envelope_8,                     /*!< US Envelope 8 size */
    US_Envelope_9_34,                  /*!< US Envelope 9¾ size */
    US_Envelope_10_12,                 /*!< US Envelope 10½ size */
    US_Envelope_12_12,                 /*!< US Envelope 12½ size */
    US_Envelope_13_12,                 /*!< US Envelope 13½ size */
    US_Envelope_14_12,                 /*!< US Envelope 14½ size */
    US_Envelope_15,                    /*!< US Envelope 15 size */
    US_Envelope_15_12,                 /*!< US Envelope 15½ size */

  };
  PaperSize();

  /*!
    \brief Portrait width in mm, or Landscape width if the \c Layout parameter is set to \c
     PageSize::Landscape.

    \param type the PageSize::Type to get the width of.
    \param layout the optional Layout value, default Portrait.
    \return the width in mm.
  */
  int
  width(const Type type, Layout layout = Portrait) const;

  /*!
    \brief Portrait height in mm, or Landscape height if the \c Layout parameter is set to \c
     PageSize::Landscape.

    \param type the PageSize::Type to get the height of.
    \param layout the optional Layout value, default Portrait.
    \return the height in mm.
  */
  int
  height(const Type type, Layout layout = Portrait) const;

  /*!
    \brief A \c QString description.

    Supplies a simple description string, suitable for use in a drop-down menu or similar.

    \param type the PageSize::Type to get the description for.
    \return the description string.
  */
  QString
  description(const Type type) const;

  /*!
     \brief static \c toInches method. Converts mm to inches.
     \param value the mm value.
     \return the inch value as a double.
  */
  static double
  toInches(int value)
  {
    return value / 25.4;
  }

protected:
  int m_width;
  int m_height;
};

#endif // PAPERSIZE_H
