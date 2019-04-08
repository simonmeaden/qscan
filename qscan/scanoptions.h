#ifndef SCANOPTIONS_H
#define SCANOPTIONS_H

#include <QObject>

class ScanOptions : public QObject
{
public:
  ScanOptions(QObject* parent = nullptr);

  int id() const;
  void setId(int id);

  QString name() const;
  void setName(const QString& name);

  QString title() const;
  void setTitle(const QString& title);

  QString description() const;
  void setDescription(const QString& description);

protected:
  int m_id;
  QString m_name;
  QString m_title;
  QString m_description;
};
typedef QSharedPointer<ScanOptions> Options;
typedef QMap<QString, ScanOptions> OptionsMap;

#endif // SCANOPTIONS_H
