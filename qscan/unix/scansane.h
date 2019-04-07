#ifndef SCANSANE_H
#define SCANSANE_H

#include <QImage>
#include <QMutexLocker>
#include <QObject>

#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)

#include <sane/sane.h>
#include <sane/saneopts.h>

#include <log4qt/consoleappender.h>
#include <log4qt/logger.h>
#include <log4qt/logmanager.h>
#include <log4qt/ttcclayout.h>

#include "scanlib.h"
#include "version.h"

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

// typedef struct
//{
//  uint8_t* data;
//  int width;
//  int height;
//  int x;
//  int y;
//} Image;

//#define STRIP_HEIGHT 256
// static SANE_Handle device = nullptr;
// static int verbose;
// static int progress = 0;
// static SANE_Byte* buffer;
// static size_t buffer_size;

// using sane_authorization_callback = SANE_Auth_Callback;

class ScanSane final
  : public QObject
  , public ScanInterface
{
  Q_OBJECT
public:
  //  using callback_type = void(const std::string& resource,
  //                             std::string& username,
  //                             std::string& password);

  explicit ScanSane(QObject* parent = nullptr);
  ~ScanSane() override;

  bool init() override;

  QStringList getDevices() override;
  Device getDevice(QString name) override;
  bool openDevice(QString device) override;
  bool startScan(QString name);

  //  SANE_Status startScan(SANE_Handle sane_handle, SANE_String_Const
  //  fileName);

  // void cancelScan(SANE_Handle sane_handle);

  // void closeDevice(SANE_Handle sane_handle);
  void exit();

  const Version& version() const;

protected:
  SANE_Status doScan(const char* fileName);
  Log4Qt::Logger* m_logger;
  Devices m_scanners;
  QImage* m_image;

  static QMutex _mutex;
  static Version _version;

  bool scan(Device device);

  static void callbackWrapper(SANE_String_Const resource,
                              SANE_Char* name,
                              SANE_Char* password);
};

#endif

#endif // SCANSANE_H
