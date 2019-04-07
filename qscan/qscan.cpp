#include "qscan.h"

#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
#include "unix/scansane.h"
#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
#include "unix/scantwain.h"
#endif

QScan::QScan(QObject* parent)
  : QObject(parent)
{
  m_logger = Log4Qt::Logger::logger(QStringLiteral("Scan"));

#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  m_lib = new ScanSane(this);
#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
  m_lib = new ScanTwain(this);
#endif
}

bool
QScan::init()
{
  return m_lib->init();
}

QStringList
QScan::getDevices()
{
  return m_lib->getDevices();
}

Device
QScan::getDevice(QString name)
{
  return m_lib->getDevice(name);
}

bool
QScan::openDevice(QString name)
{
  Device device = m_lib->getDevice(name);
  m_logger->info(QString("Name: %1, vendor: %2, model: %3, type: %4")
                   .arg(device->name)
                   .arg(device->model)
                   .arg(device->vendor)
                   .arg(device->type));

  return m_lib->openDevice(name);
}
