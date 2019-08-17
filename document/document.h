#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "document_global.h"

#include "logging.h"
#include "block.h"

class DOCUMENTSHARED_EXPORT Document
{

public:
  Document();

protected:
  QMap<int, DocumentBlock> m_blocks;
};

#endif // DOCUMENT_H
