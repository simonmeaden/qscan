#ifndef SCANSANE_H
#define SCANSANE_H

#include <QObject>

class ScanSane : public QObject
{
Q_OBJECT
public:
explicit ScanSane(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SCANSANE_H