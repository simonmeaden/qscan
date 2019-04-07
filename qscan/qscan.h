#ifndef SCAN_H
#define SCAN_H

#include <QObject>

#include "log4qt/logger.h"

#include "qscan_global.h"
#include "scanlib.h"

class SCANSHARED_EXPORT QScan : public QObject
{
  Q_OBJECT
public:
  explicit QScan(QObject* parent = nullptr);
  ~QScan() {}

  bool init();
  QStringList getDevices();
  Device getDevice(QString name);
  bool openDevice(QString name);

protected:
  Log4Qt::Logger* m_logger;
#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  ScanInterface* m_lib;

#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
#include "win/scantwain.h"

#endif
};

#endif // SCAN_H
