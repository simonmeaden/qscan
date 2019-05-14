#ifndef PARAGRAPHBLOCK_H
#define PARAGRAPHBLOCK_H

#include <QString>
#include <QSharedPointer>

#include "textblock.h"

class ParagraphBlock : public TextBlock
{
public:
  ParagraphBlock();
  explicit ParagraphBlock(const QString& text);
};
using Paragraph = QSharedPointer<ParagraphBlock>;

#endif // PARAGRAPHBLOCK_H
