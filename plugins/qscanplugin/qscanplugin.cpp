#include "qscanplugin.h"

#include "qscan.h"

namespace QScanner {

const QString QScanPlugin::m_plugin_name = "QScan Plugin";
const QString QScanPlugin::m_plugin_group = "QScan";
const QString QScanPlugin::m_vendor = "SM Electronic Components";
const int QScanPlugin::m_major_version = QSCAN_VERSION_MAJOR;
const int QScanPlugin::m_minor_version = QSCAN_VERSION_MINOR;
const int QScanPlugin::m_build_version = QSCAN_VERSION_BUILD;
const QString QScanPlugin::m_version = QString("%1.%2.%3")
                                       .arg(QScanPlugin::m_major_version)
                                       .arg(QScanPlugin::m_minor_version)
                                       .arg(QScanPlugin::m_build_version);
bool QScanPlugin::m_loaded = false;

QScanPlugin::QScanPlugin(QObject* parent)
  : QObject(parent)
  , m_current_device(nullptr)
{
  m_qscan = new QScan(this);
}

bool QScanPlugin::isDeviceSet()
{
  if (m_current_device) {
    return true;
  }

  return false;
}

QString QScanPlugin::pluginGroup() const
{
  return m_plugin_group;
}

QString QScanPlugin::pluginName() const
{
  return m_plugin_name;
}

QString QScanPlugin::vendor() const
{
  return "SM Electronic Components";
}

bool QScanPlugin::loaded() const
{
  return m_loaded;
}

void QScanPlugin::setLoaded(bool loaded)
{
  m_loaded = loaded;
}

QString QScanPlugin::version() const
{
  return m_version;
}

int QScanPlugin::majorVersion() const
{
  return m_major_version;
}

int QScanPlugin::minorVersion() const
{
  return m_minor_version;
}

int QScanPlugin::buildVersion() const
{
  return m_build_version;
}

int QScanPlugin::topLeftX()
{
  if (m_current_device) {
    return m_qscan->topLeftX(m_current_device);
  }

  return -1;

}

void QScanPlugin::setTopLeftX(int value)
{
  if (m_current_device) {
    m_qscan->setTopLeftX(m_current_device, value);
  }
}

int QScanPlugin::topLeftY()
{
  if (m_current_device) {
    return m_qscan->topLeftY(m_current_device);
  }

  return -1;

}

void QScanPlugin::setTopLeftY(int value)
{
  if (m_current_device) {
    m_qscan->setTopLeftY(m_current_device, value);
  }
}

int QScanPlugin::bottomRightX()
{
  if (m_current_device) {
    return m_qscan->bottomRightX(m_current_device);
  }

  return -1;

}

void QScanPlugin::setBottomRightX(int value)
{
  if (m_current_device) {
    m_qscan->setBottomRightX(m_current_device, value);
  }
}

int QScanPlugin::bottomRightY()
{
  if (m_current_device) {
    return m_qscan->bottomRightY(m_current_device);
  }

  return -1;

}

void QScanPlugin::setBottomRightY(int value)
{
  if (m_current_device) {
    m_qscan->setBottomRightY(m_current_device, value);
  }
}

int QScanPlugin::contrast()
{
  if (m_current_device) {
    return m_qscan->contrast(m_current_device);
  }

  return -1;
}

void QScanPlugin::setContrast(int value)
{
  if (m_current_device) {
    m_qscan->setContrast(m_current_device, value);
  }
}

int QScanPlugin::brightness()
{
  if (m_current_device) {
    return m_qscan->brightness(m_current_device);
  }

  return -1;
}

void QScanPlugin::setBrightness(int value)
{
  if (m_current_device) {
    m_qscan->setBrightness(m_current_device, value);
  }
}

int QScanPlugin::resolution()
{
  if (m_current_device) {
    return m_qscan->resolution(m_current_device);
  }

  return -1;
}

void QScanPlugin::setResolution(int value)
{
  if (m_current_device) {
    m_qscan->setResolution(m_current_device, value);
  }
}

int QScanPlugin::resolutionX()
{
  if (m_current_device) {
    return m_qscan->resolutionX(m_current_device);
  }

  return -1;
}

void QScanPlugin::setResolutionX(int value)
{
  if (m_current_device) {
    m_qscan->setResolutionX(m_current_device, value);
  }
}

int QScanPlugin::resolutionY()
{
  if (m_current_device) {
    return m_qscan->resolutionY(m_current_device);
  }

  return -1;
}

void QScanPlugin::setResolutionY(int value)
{
  if (m_current_device) {
    m_qscan->setResolutionY(m_current_device, value);
  }
}

void QScanPlugin::setPreview()
{
  if (m_current_device) {
    m_qscan->setPreview(m_current_device);
  }
}

void QScanPlugin::clearPreview()
{
  if (m_current_device) {
    m_qscan->clearPreview(m_current_device);
  }
}

void QScanPlugin::setScanMode(const QString& mode)
{
  if (m_current_device) {
    m_qscan->setScanMode(m_current_device, mode);
  }
}

void QScanPlugin::setSource(const QString& source)
{
  if (m_current_device) {
    m_qscan->setSource(m_current_device, source);
  }
}

} // end of namespace QScanner
