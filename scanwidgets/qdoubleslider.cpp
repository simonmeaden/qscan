#include "qdoubleslider.h"

QDoubleSlider::QDoubleSlider(QWidget* parent)
  : QSlider(parent)
  , m_places(0)
  , m_multiplier(1)
  , m_maximum(100.0)
  , m_minimum(1.0)
  , m_single_step(1.0)
  , m_page_step(10.0)
{
  connect(this, &QSlider::valueChanged, this, &QDoubleSlider::valueHasChanged);
  connect(this, &QSlider::sliderMoved, this, &QDoubleSlider::sliderHasMoved);
}

QDoubleSlider::QDoubleSlider(Qt::Orientation orientation, QWidget* parent)
  : QSlider(orientation, parent)
  , m_places(0)
  , m_multiplier(1)
  , m_maximum(100.0)
  , m_minimum(1.0)
  , m_single_step(1.0)
  , m_page_step(10.0)
{

}

int QDoubleSlider::places() const
{
  return m_places;
}

void QDoubleSlider::setPlaces(int places)
{
  m_places = places;
  m_multiplier = int(pow(qreal(10), places));
  setMaximum(m_maximum);
  setMinimum(m_minimum);
}

qreal QDoubleSlider::minimum() const
{
  return m_minimum;
}

void QDoubleSlider::setMinimum(int min)
{
  setMinimum(qreal(min));
}

void QDoubleSlider::setMinimum(qreal min)
{
  m_minimum = min;
  QSlider::setMinimum(int(m_minimum * m_multiplier));
}

qreal QDoubleSlider::maximum() const
{
  return m_maximum;
}

void QDoubleSlider::setMaximum(int max)
{
  setMaximum(qreal(max));
}

void QDoubleSlider::setMaximum(qreal max)
{
  m_maximum = max;
  QSlider::setMaximum(int(m_maximum * m_multiplier));
}

void QDoubleSlider::setRange(int min, int max)
{
  setMinimum(min);
  setMaximum(max);
}

void QDoubleSlider::setRange(qreal min, qreal max)
{
  setMinimum(min);
  setMaximum(max);
}

qreal QDoubleSlider::singleStep() const
{
  return m_single_step;
}

void QDoubleSlider::setSingleStep(const qreal& step)
{
  m_single_step = step;
  QSlider::setSingleStep(int(m_single_step * m_multiplier));
}

qreal QDoubleSlider::pageStep() const
{
  return m_page_step;
}

void QDoubleSlider::setPageStep(const qreal& page_step)
{
  m_page_step = page_step;
}

qreal QDoubleSlider::value()
{
  return qreal(QSlider::value()) / qreal(m_multiplier);
}

void QDoubleSlider::setValue(int value)
{
  QSlider::setValue(int(qreal(value) * qreal(m_multiplier)));
}

void QDoubleSlider::setValue(qreal value)
{
  QSlider::setValue(int(value * qreal(m_multiplier)));
}

qreal QDoubleSlider::sliderPosition()
{
  return qreal(QSlider::sliderPosition()) / qreal(m_multiplier);
}

void QDoubleSlider::setSliderPosition(int value)
{
  QSlider::setSliderPosition(int(qreal(value) * qreal(m_multiplier)));
}

void QDoubleSlider::setSliderPosition(qreal value)
{
  QSlider::setSliderPosition(int(value * qreal(m_multiplier)));
}

void QDoubleSlider::valueHasChanged(int value)
{
  qreal result = qreal(value) / qreal(m_multiplier);
  emit valueChanged(result);
}

void QDoubleSlider::sliderHasMoved(int value)
{
  qreal result = qreal(value) / qreal(m_multiplier);
  emit sliderMoved(result);
}
