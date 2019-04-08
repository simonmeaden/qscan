#ifndef SANESCANETHREAD_H
#define SANESCANETHREAD_H

#include <QImage>
#include <QObject>

#include <log4qt/logger.h>

#include "scaninterface.h"

class SaneWorker : public QObject
{
  Q_OBJECT
public:
  SaneWorker(Device device, QObject* parent = nullptr);
  ~SaneWorker() {}

  void scan();

signals:
  void scanCompleted(Image);
  void scanFailed();
  void scanProgress(double);
  void finished();

protected:
  Image m_image;
  Device m_device;
  Log4Qt::Logger* m_logger;
};

#endif // SANESCANETHREAD_H
