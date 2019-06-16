#ifndef QDOUBLESLIDER_H
#define QDOUBLESLIDER_H

#include <cmath>
#include <QSlider>
#include <QWidget>

class QDoubleSlider : public QSlider
{
  Q_OBJECT Q_PROPERTY(qreal minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(qreal maximum READ maximum WRITE setMaximum)
      Q_PROPERTY(qreal singleStep READ singleStep WRITE setSingleStep)
        Q_PROPERTY(qreal pageStep READ pageStep WRITE setPageStep)
          Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged USER true)
            Q_PROPERTY(
              qreal sliderPosition READ sliderPosition WRITE setSliderPosition NOTIFY sliderMoved)

              public : QDoubleSlider(QWidget *parent = nullptr);
  QDoubleSlider(Qt::Orientation orientation, QWidget *parent = nullptr);

  int places()const ;
  void setPlaces(int places);

  qreal maximum() const;
  void setMaximum(int max);
  void setMaximum(qreal max);
  qreal minimum() const;
  void setMinimum(int min);
  void setMinimum(qreal min);
  void setRange(int min, int max);
  void setRange(qreal min, qreal max);

  qreal singleStep() const;
  void setSingleStep(const qreal &singleStep);

  qreal pageStep() const;
  void setPageStep(const qreal &pageStep);

  qreal value();
  void setValue(int value);
  void setValue(qreal value);
  qreal sliderPosition();
  void setSliderPosition(int value);
  void setSliderPosition(qreal value);

  signals:
  void valueChanged(qreal);
  void sliderMoved(qreal);

  protected:
  int m_multiplier, m_places;
  qreal m_maximum{}, m_minimum{};
  qreal m_single_step, m_page_step;

  void valueHasChanged(int value);
  void sliderHasMoved(int value);
};

#endif // QDOUBLESLIDER_H
