#include "scanoptions.h"

ScanOptions::ScanOptions(QObject* parent)
  : QObject(parent)
{}

int
ScanOptions::id() const
{
  return m_id;
}

void
ScanOptions::setId(int id)
{
  m_id = id;
}

QString
ScanOptions::name() const
{
  return m_name;
}

void
ScanOptions::setName(const QString& name)
{
  m_name = name;
}

QString
ScanOptions::title() const
{
  return m_title;
}

void
ScanOptions::setTitle(const QString& title)
{
  m_title = title;
}

QString
ScanOptions::description() const
{
  return m_description;
}

void
ScanOptions::setDescription(const QString& description)
{
  m_description = description;
}
