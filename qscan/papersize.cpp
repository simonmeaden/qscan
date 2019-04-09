#include "papersize.h"

PaperSize::PaperSize() {}

int
PaperSize::width(const Type type) const
{
  switch (type) {
    case Paper_4A0:
      return 1189;

    case Paper_2A0:
      return 1682;

    case Paper_A0:
      return 841;

    case Paper_A1:
      return 594;

    case Paper_A2:
      return 420;

    case Paper_A3:
      return 297;

    case Paper_A4:
      return 210;

    case Paper_A5:
      return 148;

    case Paper_A6:
      return 105;

    case Paper_A7:
      return 74;

    case Paper_A8:
      return 52;

    case Paper_A9:
      return 37;

    case Paper_A10:
      return 26;

    case Paper_B0:
      return 1000;

    case Paper_B1:
      return 707;

    case Paper_B2:
      return 500;

    case Paper_B3:
      return 353;

    case Paper_B4:
      return 250;

    case Paper_B5:
      return 176;

    case Paper_B6:
      return 125;

    case Paper_B7:
      return 88;

    case Paper_B8:
      return 62;

    case Paper_B9:
      return 44;

    case Paper_B10:
      return 31;

    case Paper_C0:
      return 917;

    case Paper_C1:
      return 648;

    case Paper_C2:
      return 458;

    case Paper_C3:
      return 324;

    case Paper_C4:
      return 229;

    case Paper_C5:
      return 162;

    case Paper_C6:
      return 114;

    case Paper_C7:
      return 81;

    case Paper_C8:
      return 57;

    case Paper_C9:
      return 40;

    case Paper_C10:
      return 28;

    case Paper_Half_Letter:
      return 140;

    case Paper_Letter:
      return 216;

    case Paper_Legal:
      return 216;

    case Paper_Junior_Legal:
      return 127;

    case Paper_Ledger: // these are both the same thing
    case Paper_Tabloid:
      return 279;
  }
}

int
PaperSize::height(const Type type) const
{
  switch (type) {
    case Paper_4A0:
      return 00;

    case Paper_2A0:
      return 00;

    case Paper_A0:
      return 00;

    case Paper_A1:
      return 00;

    case Paper_A2:
      return 00;

    case Paper_A3:
      return 00;

    case Paper_A4:
      return 00;

    case Paper_A5:
      return 00;

    case Paper_A6:
      return 00;

    case Paper_A7:
      return 00;

    case Paper_A8:
      return 00;

    case Paper_A9:
      return 00;

    case Paper_A10:
      return 00;

    case Paper_B0:
      return 00;

    case Paper_B1:
      return 00;

    case Paper_B2:
      return 00;

    case Paper_B3:
      return 00;

    case Paper_B4:
      return 00;

    case Paper_B5:
      return 00;

    case Paper_B6:
      return 00;

    case Paper_B7:
      return 00;

    case Paper_B8:
      return 00;

    case Paper_B9:
      return 00;

    case Paper_B10:
      return 00;

    case Paper_C0:
      return 00;

    case Paper_C1:
      return 00;

    case Paper_C2:
      return 00;

    case Paper_C3:
      return 00;

    case Paper_C4:
      return 00;

    case Paper_C5:
      return 00;

    case Paper_C6:
      return 00;

    case Paper_C7:
      return 00;

    case Paper_C8:
      return 00;

    case Paper_C9:
      return 00;

    case Paper_C10:
      return 00;

    case Paper_Half_Letter:
      return 00;

    case Paper_Letter:
      return 00;

    case Paper_Legal:
      return 00;

    case Paper_Junior_Legal:
      return 00;

    case Paper_Ledger:
      return 00;

    case Paper_Tabloid:
      return 00;
  }
