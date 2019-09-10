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

#include "scandevice.h"
#include "scanoptions.h"

namespace QScanner {

ScanDevice::ScanDevice(QObject* parent)
  : QObject(parent)
  , m_available_options_set(false)
{
  m_options = new ScanOptions(parent);
}

ScanDevice::ScanDevice(const ScanDevice& other)
  : QObject(other.parent())
  , m_available_options_set(other.areAvailableOptionsSet())
{
  m_options = other.m_options;
}

QString ScanDevice::descriptor()
{
  return m_descriptor;
}

void ScanDevice::setDescriptor(QString name)
{
  m_descriptor = name;
}

QString ScanDevice::vendor() const
{
  return m_vendor;
}

void ScanDevice::setVendor(const QString& vendor)
{
  m_vendor = vendor;
}

QString ScanDevice::model() const
{
  return m_model;
}

void ScanDevice::setModel(const QString& model)
{
  m_model = model;
}

QString ScanDevice::type() const
{
  return m_type;
}

void ScanDevice::setType(const QString& type)
{
  m_type = type;
}

ScanOptions* ScanDevice::options() const
{
  return m_options;
}

void ScanDevice::setOptions(ScanOptions* options)
{
  m_options = options;
}

bool ScanDevice::areAvailableOptionsSet() const
{
  return m_available_options_set;
}

void ScanDevice::setAvailableOptionsSet(bool available_options_set)
{
  m_available_options_set = available_options_set;
}

#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)

//SANE_Handle ScanDevice::handle()
//{
//  return m_handle;
//}

//void ScanDevice::setHandle(const SANE_Handle& handle)
//{
//  m_handle = handle;
//}

void ScanDevice::setDisplayName(const QString& display_name)
{
  m_display_name = display_name;
}

QString ScanDevice::displayName() const
{
  return m_display_name;
}

#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
// TODO
#endif

} // end of namespace QScanner
