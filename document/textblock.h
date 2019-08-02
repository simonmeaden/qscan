#ifndef TEXTBLOCK_H
#define TEXTBLOCK_H

#include <QString>
#include <QSharedPointer>

#include "block.h"
#include "styledstring.h"

class TextBlock : public Block
{
public:
  TextBlock();
  explicit TextBlock(const StyledString& title);

  void toHtml();

protected:
  StyledString m_text;
};
using DocumentText = QSharedPointer<TextBlock>;

#endif // TEXTBLOCK_H
