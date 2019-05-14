#ifndef TEXTBLOCK_H
#define TEXTBLOCK_H

#include <QString>

#include "block.h"

class TextBlock : public Block
{
public:
  TextBlock();
  TextBlock(QString title);

protected:
  QString m_text;
};
using DocumentText = QSharedPointer<TextBlock>;

#endif // TEXTBLOCK_H
