#ifndef SCANIMAGE_H
#define SCANIMAGE_H

#include <QApplication>
#include <QClipboard>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QPainter>
#include <QPoint>
#include <QtMath>

#include "qscanplugin_global.h"
#include "basescanimage.h"

class ScanEditor;

class QSCANPLUGINSHARED_EXPORT ScanImage : public BaseScanImage
{
  Q_OBJECT
public:
    explicit ScanImage(QWidget *parent);
    explicit ScanImage(QString datadir, QWidget *parent);

    //  void rotateBy(int angle);
    void cropToContent();
    void scaleBy();
    void save();
    void saveAs();
    QImage saveAsCover();
    void rescan();
    void rescale();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    QRect selection();
    void copySelection();
    void selectAll();
    void setDefaultPageCropSize();
    QPair<QImage, QImage> splitPages();
    QImage splitLeftPage();
    QImage splitRightPage();
    QImage makePage();

signals:
    //  void sendImage(QImage);
    //  void sendImages(QImage, QImage);
    //  void sendCover(const QImage&);

protected:
    QString m_datadir;
    QMatrix m_matrix;
    qreal m_rotation;

    QString m_filename;

    void contextMenuEvent(QContextMenuEvent *event) override;

    QPointF getRotatedPoint(QPointF p, QPointF center, qreal angleRads);
    QRectF getMinimumBoundingRect(QRect r, qreal angleRads);
    qreal getMax(qreal a, qreal b, qreal c, qreal d);
    void enableSetDefaultCropSize();
    void disableSetDefaultCropSize();
    void enableSelectionActions();
    void disableSelectionActions();
    void enableNoSelectionActions();
    void disableNoSelectionActions();

    //  QAction* m_cut_act;
    QAction *m_copy_selection_act{};
    QAction *m_crop_to_selection_act{};
    QAction *m_clear_selection_act{};
    QAction *m_crop_to_content_act{};
    QAction *m_rotate_cw_act{};
    QAction *m_rotate_acw_act{};
    QAction *m_rotate_180_act{};
    QAction *m_rotate_by_angle_act{};
    QAction *m_rotate_by_edge_act{};
    QAction *m_rescan_act{};
    QAction *m_scale_act{};
    QAction *m_selectall_act{};
    QAction *m_save_act{};
    QAction *m_save_as_act{};
    //  QAction* m_save_as_cover_act{};
    QAction *m_set_def_crop_act{};
    //  QAction* m_split_pages_act{};
    //  QAction* m_split_right_act{};
    //  QAction* m_split_left_act{};
    //  QAction* m_make_page_act{};

    void initActions();

};

#endif // SCANIMAGE_H
