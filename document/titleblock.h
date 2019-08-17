#ifndef TITLEBLOCK_H
#define TITLEBLOCK_H

#include <QSharedPointer>

#include "logging.h"
#include "block.h"
#include "textblock.h"
#include "styledstring.h"

class TitleBlock : public TextBlock
{
public:
  TitleBlock();
  TitleBlock(const StyledString& title);

protected:

};
using DocumentTitle = QSharedPointer<TitleBlock>;

#endif // TITLEBLOCK_H
