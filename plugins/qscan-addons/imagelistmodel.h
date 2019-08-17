#ifndef IMAGELISTMODEL_H
#define IMAGELISTMODEL_H

#include <QAbstractListModel>
#include <QImage>
#include <QMimeData>
#include <QFont>
#include <QFontMetrics>
#include <QtDebug>

#include "qscanlogging.h"

namespace QScanner {

using ImageList = QList<QImage>;

class ImageListModel : public QAbstractListModel
{
  Q_OBJECT
public:
  explicit ImageListModel(QObject* parent = nullptr);

  void setCover(const QImage& image);
  int appendThumbnail(const QImage& image);
  int appendCompleted(const bool completed);
  bool insertThumbnail(int row, const QImage& image);
  bool removeThumbnail(int row);
  bool moveThumbnail(int source, int destination);
  void replaceThumbnail(int row, const QImage& image);
  QImage thumbnail(int row);
  void setCompleted(int row, bool has_text);
  bool isCompleted(int row);
  bool isEmpty();
  //  bool isInternalImage(int row);
  //  void setInternalImage(int row, bool value);
  QString tooltip(int row);

  QStringList mimeTypes() const override;
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role) const override;
  bool setData(const QModelIndex& index,
               const QVariant& value,
               int role) override;
  Qt::DropActions supportedDropActions() const override;
  Qt::DropActions supportedDragActions() const override;
  QMimeData* mimeData(const QModelIndexList& indexes) const override;
  bool dropMimeData(const QMimeData* data,
                    Qt::DropAction action,
                    int row,
                    int /*column*/,
                    const QModelIndex& parent) override;
  bool canDropMimeData(const QMimeData* data,
                       Qt::DropAction,
                       int,
                       int column,
                       const QModelIndex& parent) const override;
  Qt::ItemFlags flags(const QModelIndex& index) const override;
  bool removeRows(int row,
                  int count,
                  const QModelIndex& parent = QModelIndex()) override;
  bool moveRows(const QModelIndex& sourceParent,
                int sourceRow,
                int count,
                const QModelIndex& destinationParent,
                int destinationChild) override;
  bool insertRows(int row,
                  int count,
                  const QModelIndex& parent = QModelIndex()) override;

  void setFont(const QFont& font);

protected:
  QFont m_font;
  ImageList m_images;
  ImageList m_internal_images; // used to store internal images.
  QList<bool> m_is_completed;
  //  QList<bool> m_internal_image;
  QStringList m_headers;

  static const QString MIMETYPE;

};

} // end of namespace QScanner

#endif // IMAGELISTMODEL_H
