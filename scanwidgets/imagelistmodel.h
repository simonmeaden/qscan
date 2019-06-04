#ifndef IMAGELISTMODEL_H
#define IMAGELISTMODEL_H

#include <QAbstractListModel>
#include <QImage>
#include <QMimeData>

#include "logger.h"

using ImageList = QList<QImage>;

class ImageListModel : public QAbstractListModel
{
  Q_OBJECT
public:
  explicit ImageListModel(QObject* parent = nullptr);

  void setCover(const QImage& image);
  int appendThumbnail(const QImage &image, bool has_text,
                      bool internal_image = false);
  bool insertThumbnail(int row, const QImage& image, bool has_text, bool is_internal_image);
  bool removeThumbnail(int row);
  bool moveThumbnail(int source, int destination);
  void replaceThumbnail(int row,
                        const QImage& image,
                        bool has_text,
                        bool is_internal_image);
  void setHasText(int row, bool has_text);
  bool hasText(int row);
  bool isEmpty();
  bool isInternalImage(int row);
  void setInternalImage(int row, bool value);
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

protected:
  Log4Qt::Logger* m_logger;
  ImageList m_images;
  ImageList m_extra_images; // used to store internal images.
  QList<bool> m_has_text;
  QList<bool> m_internal_image;
  QStringList m_headers;

  static const QString MIMETYPE;

};
#endif // IMAGELISTMODEL_H