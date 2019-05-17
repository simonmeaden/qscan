#ifndef TEXTBLOCK_H
#define TEXTBLOCK_H

#include <QString>
#include <QSharedPointer>

#include "block.h"

class TextBlock : public Block
{
public:
  TextBlock();
  explicit TextBlock(const QString& title);

  void toHtml();

protected:
  QString m_text;
};
using DocumentText = QSharedPointer<TextBlock>;

#endif // TEXTBLOCK_H
