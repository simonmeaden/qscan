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
#ifndef VERSION_H
#define VERSION_H

#include <QObject>

#if defined(Q_OS_UNIX) || defined(Q_OS_LINUX)
  #include <sane/sane.h>
#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
  // TODO
#endif

namespace QScanner {

class Version final
{
public:
  Version(quint8 major_version,
          quint8 minorVersion,
          quint16 buildVersion); /**< Create version to compare with logical
                                     Operators */
  quint8 majorVersion() const; /**< Observer to check the Major Version Code */
  quint8 minorVersion() const; /**< Observer to check the Minor Version Code */
  quint16 buildVersion() const; /**< Observer to check the Build Version Code */

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
                               sanelibrary
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
bool operator<=(const Version& lhs, const Version& rhs);
bool operator<(const Version& lhs, const Version& rhs);
bool operator>(const Version& lhs, const Version& rhs);
bool operator>=(const Version& lhs, const Version& rhs);
bool operator==(const Version& lhs, const Version& rhs);
bool operator!=(const Version& lhs, const Version& rhs);

} // end of namespace QScanner

#endif // VERSION_H
