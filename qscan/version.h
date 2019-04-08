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
#ifndef VERSION_H
#define VERSION_H

#include <QObject>
#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
#include <sane/sane.h>
#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
// TODO
#endif

class Version final
{
public:
  Version(quint8 major_version,
          quint8 minor_version,
          quint16 build_version); /**< Create version to compare with logical
                                     Operators */
  quint8 majorVersion(); /**< Observer to check the Major Version Code */

protected:
#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  explicit Version(
    SANE_Int version_code = 0); /**< Create version with default 0 */
#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
  explicit Version(int version_code = 0); /**< Create version with default 0 */
#endif

  quint8 m_major_version;  /**< Major Version */
  quint8 m_minor_version;  /**< Minor Version */
  quint16 m_build_version; /**< Build Version */

  friend class SaneLibrary; /**< friend declaration for using version in
                             * sanelibrary
                             */
  friend bool operator>(
    const Version& lhs,
    const Version& rhs); /**< friend declaration for logical Operators */
  friend bool operator==(
    const Version& lhs,
    const Version&
      rhs); /**< friend declaration for compare equivalend and antivalent */
};
/// Logical Operators to compare the versioncode
bool
operator<=(const Version& lhs, const Version& rhs);
bool
operator<(const Version& lhs, const Version& rhs);
bool
operator>(const Version& lhs, const Version& rhs);
bool
operator>=(const Version& lhs, const Version& rhs);
bool
operator==(const Version& lhs, const Version& rhs);
bool
operator!=(const Version& lhs, const Version& rhs);

#endif // VERSION_H
