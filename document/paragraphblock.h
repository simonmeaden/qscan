#ifndef PARAGRAPHBLOCK_H
#define PARAGRAPHBLOCK_H

#include <QString>
#include <QSharedPointer>

#include "logging.h"
#include "textblock.h"
#include "styledstring.h"

class ParagraphBlock : public TextBlock
{
public:
  ParagraphBlock();
  explicit ParagraphBlock(const StyledString& text);
};
using Paragraph = QSharedPointer<ParagraphBlock>;

#endif // PARAGRAPHBLOCK_H
