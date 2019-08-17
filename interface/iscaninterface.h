#ifndef ISCANINTERFACE_H
#define ISCANINTERFACE_H

#include <QObject>

#include "iplugininterface.h"
#include "interface_global.h"

namespace QScanner {

class ScanDevice;

class INTERFACESHARED_EXPORT IScanInterface : public IPluginInterface
{
public:

  virtual int topLeftX() = 0;
  virtual void setTopLeftX(int value) = 0;
  virtual int topLeftY() = 0;
  virtual void setTopLeftY(int value) = 0;
  virtual int bottomRightX() = 0;
  virtual void setBottomRightX(int value) = 0;
  virtual int bottomRightY() = 0;
  virtual void setBottomRightY(int value) = 0;
  virtual int contrast() = 0;
  virtual void setContrast(int value) = 0;
  virtual int brightness() = 0;
  virtual void setBrightness(int value) = 0;
  virtual int resolution() = 0;
  virtual void setResolution(int value) = 0;
  virtual int resolutionX() = 0;
  virtual void setResolutionX(int value) = 0;
  virtual int resolutionY() = 0;
  virtual void setResolutionY(int value) = 0;
  virtual void setPreview() = 0;
  virtual void clearPreview() = 0;
  virtual void setScanMode(const QString& mode) = 0;
  virtual void setSource(const QString& source) = 0;

};

} // end of namespace QScanner

#define IScanInterface_iid "org.smelecomp.IScanInterface/0.1.0"
Q_DECLARE_INTERFACE(QScanner::IScanInterface, IScanInterface_iid)

#endif // ISCANINTERFACE_H
