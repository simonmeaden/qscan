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
#include "version.h"

namespace QScanner {

Version::Version(quint8 major_version,
                 quint8 minor_version,
                 quint16 build_version)
  : m_major_version(major_version)
  , m_minor_version(minor_version)
  , m_build_version(build_version)
{}

#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)

Version::Version(SANE_Int version_code)
  : m_major_version(SANE_VERSION_MAJOR(version_code))
  , m_minor_version(SANE_VERSION_MINOR(version_code))
  , m_build_version(SANE_VERSION_BUILD(version_code))
{}

quint16 Version::buildVersion() const
{
  return m_build_version;
}

quint8 Version::minorVersion() const
{
  return m_minor_version;
}

#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)

Version::Version(int version_code = 0) {}

#endif

quint8 Version::majorVersion() const
{
  return m_major_version;
}

bool operator<=(const Version& lhs, const Version& rhs)
{
  return !(lhs > rhs);
}

bool operator<(const Version& lhs, const Version& rhs)
{
  return !(lhs >= rhs);
}

bool operator>=(const Version& lhs, const Version& rhs)
{
  return lhs > rhs || lhs == rhs;
}

bool operator>(const Version& lhs, const Version& rhs)
{
  if (lhs.m_major_version > rhs.m_major_version) {
    return true;
  }

  if (lhs.m_major_version < rhs.m_major_version) {
    return false;
  }

  if (lhs.m_minor_version > rhs.m_minor_version) {
    return true;
  }

  if (lhs.m_minor_version < rhs.m_minor_version) {
    return false;
  }

  if (lhs.m_build_version > rhs.m_build_version) {
    return true;
  }

  return false;
}

bool operator==(const Version& lhs, const Version& rhs)
{
  return (lhs.m_major_version == rhs.m_major_version &&
          lhs.m_minor_version == rhs.m_minor_version &&
          lhs.m_build_version == rhs.m_build_version);
}

bool operator!=(const Version& lhs, const Version& rhs)
{
  return !(lhs == rhs);
}

} // end of namespace QScanner
