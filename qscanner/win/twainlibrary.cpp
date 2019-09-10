/*
  Copyright © Simon Meaden 2019.
  This file was developed as part of the Biblios application but could
  easily be used elsewhere.

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
#include "twainlibrary.h"

#include <cstring>

namespace QScanner {

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)

using namespace Twpp;

TwainLibrary::TwainLibrary(QObject* parent)
  : ScanLibrary(parent)
{
  initLib();
}

TwainLibrary::TwainLibrary(QString manufacturer,
                           QString product,
                           QString product_name,
                           QObject* parent)
  : ScanLibrary(manufacturer, product, product_name, parent)
{
  initLib();
}

void TwainLibrary::initLib()
{
  QString version_str = QString("%1.%2").arg(QSCAN_VERSION_MAJOR).arg(QSCAN_VERSION_MINOR);

  strncpy(m_version_char, version_str.toStdString().c_str(), 32);
  strncpy(m_manufacturer_char, m_manufacturer.toStdString().c_str(), 32);
  strncpy(m_product_char, m_product.toStdString().c_str(), 32);
  strncpy(m_product_name_char, m_product_name.toStdString().c_str(), 32);
}

void TwainLibrary::setManufacturer(const QString& manufacturer)
{
  m_manufacturer = manufacturer;
  strncpy(m_manufacturer_char, m_manufacturer.toStdString().c_str(), 32);
}

void TwainLibrary::setProduct(const QString& product)
{
  m_product = product;
  strncpy(m_product_char, m_product.toStdString().c_str(), 32);
}

void TwainLibrary::setProductName(const QString& product_name)
{
  m_product_name = product_name;
  strncpy(m_product_name_char, m_product_name.toStdString().c_str(), 32);
}

bool TwainLibrary::init()
{
  const Str32 version32(m_version_char);
  const Str32 manufacturer32(m_manufacturer_char);
  const Str32 product32(m_product_char);
  const Str32 product_name32(m_product_name_char);

  m_twain_manager = Manager(
                      Identity(
                        Version(UInt16(QSCAN_VERSION_MAJOR),
                                UInt16(QSCAN_VERSION_MINOR),
                                Language::English,
                                Country::UnitedKingdom,
                                version32),
                        DataGroup::Image,
                        manufacturer32,
                        product32,
                        product_name32
                      )
                    );

  m_twain_manager.load(); // loads DSM DLL/SO/Framework
  m_twain_manager.open(); // opens DSM

  std::vector<Source> sources;
  m_twain_manager.sources(sources);
  m_sources = QList<Source>::fromVector(QVector<Source>::fromStdVector(sources));
}

void TwainLibrary::exit()
{
  m_twain_manager.close(); // Closes DSM
  m_twain_manager.unload(); // Unloads the DSM DLL/SO/Framework
}

void TwainLibrary::detectDevices()
{
  CapType type;
}

ScanDevice* TwainLibrary::device(QString name)
{
  return nullptr;
}

QStringList TwainLibrary::devices()
{

}

bool TwainLibrary::detectAvailableOptions(QString device_name)
{

}

ScanDevice* TwainLibrary::currentDevice()
{

}

void TwainLibrary::setCurrentDevice(ScanDevice* current_device)
{

}

bool TwainLibrary::startScan(QString device_name)
{

}

void TwainLibrary::cancelScan()
{

}

QRect TwainLibrary::geometry(QString device_name)
{

}

void TwainLibrary::setTopLeftX(ScanDevice* device, int x)
{

}

void TwainLibrary::setTopLeftY(ScanDevice* device, int x)
{

}

void TwainLibrary::setBottomRightX(ScanDevice* device, int value)
{

}

void TwainLibrary::setBottomRightY(ScanDevice* device, int x)
{

}

void TwainLibrary::setContrast(ScanDevice* device, int value)
{

}

void TwainLibrary::setBrightness(ScanDevice* device, int value)
{

}

void TwainLibrary::setResolution(ScanDevice* device, int value)
{

}

void TwainLibrary::setResolutionX(ScanDevice* device, int value)
{

}

void TwainLibrary::setResolutionY(ScanDevice* device, int value)
{

}

void TwainLibrary::setPreview(ScanDevice* device)
{

}

void TwainLibrary::clearPreview(ScanDevice* device)
{

}

void TwainLibrary::setMode(ScanDevice* device, const QString& mode)
{

}

void TwainLibrary::setSource(ScanDevice* device, const QString& source)
{

}

bool TwainLibrary::isScanning() const
{

}

#endif // MS Check

} // end of namespace QScanner
