#ifndef SCANTWAIN_H
#define SCANTWAIN_H

#include <QObject>

#include "scanlib.h"

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)

class ScanTwain
  : public QObject
  , public ScanLib
{
public:
  ScanTwain(QObject* parent = nullptr);

  bool init() override;
  QStringList getDevices() override;
  Scanner getDevice(QString name) override;
};

#endif

#endif // SCANTWAIN_H
