#ifndef PAPERSIZE_H
#define PAPERSIZE_H

class PaperSize
{
public:
  enum Type
  {
    Paper_4A0,
    Paper_2A0,
    Paper_A0,
    Paper_A1,
    Paper_A2,
    Paper_A3,
    Paper_A4,
    Paper_A5,
    Paper_A6,
    Paper_A7,
    Paper_A8,
    Paper_A9,
    Paper_A10,
    Paper_B0,
    Paper_B1,
    Paper_B2,
    Paper_B3,
    Paper_B4,
    Paper_B5,
    Paper_B6,
    Paper_B7,
    Paper_B8,
    Paper_B9,
    Paper_B10,
    Paper_C0,
    Paper_C1,
    Paper_C2,
    Paper_C3,
    Paper_C4,
    Paper_C5,
    Paper_C6,
    Paper_C7,
    Paper_C8,
    Paper_C9,
    Paper_C10,
    Paper_Half_Letter,
    Paper_Letter,
    Paper_Legal,
    Paper_Junior_Legal,
    Paper_Ledger,
    Paper_Tabloid,
  };
  PaperSize();

  int width(const Type type) const;
  int height(const Type type) const;

  static double toInches(int value) { return value / 25.4; }

protected:
  int m_width;
  int m_height;
};

#endif // PAPERSIZE_H
