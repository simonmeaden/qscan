#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QAbstractListModel>
#include <QListView>
#include <QVariant>
#include <QWidget>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QPixmap>

using ImageList = QList<QImage>;

class ImageDelegate : public QStyledItemDelegate
{
  Q_OBJECT
public:
  explicit ImageDelegate(QObject* parent = nullptr);
  ~ImageDelegate() override = default;

public:
  void paint(QPainter* painter,
             const QStyleOptionViewItem& option,
             const QModelIndex& index) const override;
};

class ImageListModel : public QAbstractListModel
{
  Q_OBJECT
public:
  explicit ImageListModel(QObject* parent = nullptr);

  void setCover(const QImage& image);
  bool addThumbnail(const QImage& image, bool has_text);
  bool insertThumbnail(int row, const QImage& image, bool has_text);
  bool removeThumbnail(int row);
  bool moveThumbnail(int source, int destination);
  void replaceThumbnail(int row, const QImage& image, bool has_text);
  void setHasText(int index, bool has_text);
  bool hasText(int row);

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;
  QVariant data(const QModelIndex& index, int role) const override;
  Qt::DropActions supportedDropActions() const override;
  Qt::DropActions supportedDragActions() const override;
  Qt::ItemFlags flags(const QModelIndex& index) const override;
  bool removeRows(int row,
                  int count,
                  const QModelIndex& parent = QModelIndex()) override;
  bool moveRows(const QModelIndex&,
                int sourceRow,
                int count, const QModelIndex&,
                int destinationChild) override;
  bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;

protected:
  ImageList m_images;
  QList<bool> m_has_text;
  QStringList m_headers;

};

class ImageView : public QListView
{
public:
  explicit ImageView(QWidget* parent = nullptr);

  void setCover(const QImage& image);
  void addThumbnail(const QImage& image, bool has_text = false);
  void insertThumbnail(int row, const QImage& image, bool has_text = false);
  void removeThumbnail(int row);
  void moveThumbnail(int source, int destination);
  void replaceThumbnail(int row, const QImage& image, bool has_text);
  void setHasText(int index, bool has_text);

protected:
  ImageListModel* m_model;
};

#endif // IMAGEVIEW_H
