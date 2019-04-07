/*
    Copyright © Simon Meaden 2019.
    This file is part of the QScan cpp library.

    QScan is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    QScan is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with QScan.  If not, see <http://www.gnu.org/licenses/>.
    It is also available on request from Simon Meaden simonmeaden@sky.com.
*/
#include "version.h"

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

#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)

Version::Version(int version_code = 0) {}

#endif

quint8
Version::majorVersion()
{
  return m_major_version;
}

bool
operator<=(const Version& lhs, const Version& rhs)
{
  return !(lhs > rhs);
}

bool
operator<(const Version& lhs, const Version& rhs)
{
  return !(lhs >= rhs);
}

bool
operator>=(const Version& lhs, const Version& rhs)
{
  return lhs > rhs || lhs == rhs;
}

bool
operator>(const Version& lhs, const Version& rhs)
{
  if (lhs.m_major_version > rhs.m_major_version)
    return true;

  if (lhs.m_major_version < rhs.m_major_version)
    return false;

  if (lhs.m_minor_version > rhs.m_minor_version)
    return true;

  if (lhs.m_minor_version < rhs.m_minor_version)
    return false;

  if (lhs.m_build_version > rhs.m_build_version)
    return true;

  return false;
}

bool
operator==(const Version& lhs, const Version& rhs)
{
  return (lhs.m_major_version == rhs.m_major_version &&
          lhs.m_minor_version == rhs.m_minor_version &&
          lhs.m_build_version == rhs.m_build_version);
}

bool
operator!=(const Version& lhs, const Version& rhs)
{
  return !(lhs == rhs);
}
