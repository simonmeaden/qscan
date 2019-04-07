#include "scantwain.h"

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)

ScanTwain::ScanTwain(QObject* parent)
  : QObject(parent)
{}

bool
ScanTwain::init()
{}

#endif
