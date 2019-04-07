#ifndef SCANLIB_H
#define SCANLIB_H

#include <QSharedPointer>
#include <QStringList>

#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
#include <sane/sane.h>
#endif

struct ScanDevice final
{
  QString name;
  QString vendor;
  QString model;
  QString type;
#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  SANE_Handle sane_handle = nullptr;
#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
  // TODO
#endif
};
typedef QSharedPointer<ScanDevice> Device;
typedef QMap<QString, Device> Devices;

class ScanInterface
{
public:
  typedef enum
  {
    SCAN_STATUS_GOOD = 0,     /* everything A-OK */
    SCAN_STATUS_UNSUPPORTED,  /* operation is not supported */
    SCAN_STATUS_CANCELLED,    /* operation was cancelled */
    SCAN_STATUS_DEVICE_BUSY,  /* device is busy; try again later */
    SCAN_STATUS_INVAL,        /* data is invalid (includes no dev at open) */
    SCAN_STATUS_EOF,          /* no more data available (end-of-file) */
    SCAN_STATUS_JAMMED,       /* document feeder jammed */
    SCAN_STATUS_NO_DOCS,      /* document feeder out of documents */
    SCAN_STATUS_COVER_OPEN,   /* scanner cover is open */
    SCAN_STATUS_IO_ERROR,     /* error during device I/O */
    SCAN_STATUS_NO_MEM,       /* out of memory */
    SCAN_STATUS_ACCESS_DENIED /* access to resource has been denied */
  } SCAN_Status;

  virtual ~ScanInterface() {}

  virtual bool init() = 0;
  virtual QStringList getDevices() = 0;
  virtual Device getDevice(QString name) = 0;
  virtual bool openDevice(QString name) = 0;

protected:
};

#endif // SCANLIB_H
