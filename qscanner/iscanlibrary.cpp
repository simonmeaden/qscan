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
#include "iscanlibrary.h"

//#include "scanoptions.h"
//#include "scandevice.h"

namespace QScanner {

ScanLibrary::ScanLibrary(QObject* parent)
  : QObject(parent)
{}

ScanLibrary::ScanLibrary(const QString& manufacturer,
                         const QString& product,
                         const QString& product_name,
                         QObject* parent)
  : QObject(parent)
  , m_manufacturer(manufacturer)
  , m_product(product)
  , m_product_name(product_name)
{
}

void ScanLibrary::setCurrentDevice(ScanDevice* current_device)
{
  if (current_device != m_current_device) {
    m_current_device = current_device;

    if (!m_current_device->areAvailableOptionsSet()) {
      emit getAvailableOptions(m_current_device);

    } else {
      // options are already downloaded. No need to repeat.
      emit optionsSet();
    }
  }
}

ScanDevice* ScanLibrary::currentDevice()
{
  return m_current_device;
}

QString ScanLibrary::productName() const
{
  return m_product_name;
}

void ScanLibrary::setProductName(const QString& product_name)
{
  m_product_name = product_name;
}

QString ScanLibrary::product() const
{
  return m_product;
}

void ScanLibrary::setProduct(const QString& product)
{
  m_product = product;
}

QString ScanLibrary::manufacturer() const
{
  return m_manufacturer;
}

void ScanLibrary::setManufacturer(const QString& manufacturer)
{
  m_manufacturer = manufacturer;
}

} // end of namespace QScanner
