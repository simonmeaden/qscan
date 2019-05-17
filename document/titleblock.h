#ifndef TITLEBLOCK_H
#define TITLEBLOCK_H

#include <QString>
#include <QSharedPointer>

#include "block.h"
#include "textblock.h"

class TitleBlock : public TextBlock
{
public:
  TitleBlock();
  TitleBlock(const QString& title);

protected:

};
using DocumentTitle = QSharedPointer<TitleBlock>;

#endif // TITLEBLOCK_H
