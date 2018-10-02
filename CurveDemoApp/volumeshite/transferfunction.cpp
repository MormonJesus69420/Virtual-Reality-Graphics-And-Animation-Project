#include "transferfunction.h"

#include <QDataStream>
#include <QDebug>
#include <QFile>
#include <QtAlgorithms>
#define UNUSED(expr) \
  do {               \
    (void)(expr);    \
  } while (0)

TransferFunction::TransferFunction()
{
  _type = 1;
  _box_height = 255;
  _box_width = 256;
  _red_points.append(QPoint(0, 0));
  _red_points.append(QPoint(_box_width, 0));
  //    _red_points.append(QPoint(_box_width/2,_box_height));
  //    _red_points.append(QPoint(_box_width/2,_box_height/2));
  _green_points.append(QPoint(0, 0));
  _green_points.append(QPoint(_box_width, 0));
  //    _green_points.append(QPoint(_box_width/2,_box_height/2));
  _blue_points.append(QPoint(0, 0));
  _blue_points.append(QPoint(_box_width, 0));
  //    _blue_points.append(QPoint(_box_width/2,_box_height/2));
  _alpha_points.append(QPoint(0, 0));
  _alpha_points.append(QPoint(_box_width, 0));
  _alpha_points.append(QPoint(_box_width / 2, _box_height));
  updateFunction();
}

std::vector<GMlib::Vector<float, 4>> TransferFunction::getTransferValues()
{
  return transfunc;
}

void TransferFunction::setTransferValues(std::vector<GMlib::Vector<float, 4>> value)
{
  transfunc.clear();
  transfunc = value;
}

void TransferFunction::insertPoint(int x, int y, int width, int height, int box, bool insert)
{

  if (box <= 4 && box > 0) {
    int xPoint = int(float(x) / float(width) * 255.0);
    int yPoint = 255 - int(float(y) / float(height) * 255.0);

    if (insert) {
      //  qDebug() << "transfer entered" << x << ":" << xPoint << y <<":" << yPoint << width << height << box;
      qDebug() << "Inserting transfer point " << xPoint << yPoint;

      QPoint p(xPoint, yPoint);
      if (box == 1)
        _red_points.append(p);
      if (box == 2)
        _green_points.append(p);
      if (box == 3)
        _blue_points.append(p);
      if (box == 4)
        _alpha_points.append(p);

      updateFunction();
    }
    else {

      qDebug() << "Removing transfer point " << xPoint << yPoint;
      QPoint p(xPoint, yPoint);
      if (box == 1 && _red_points.contains(p))
        _red_points.remove(_red_points.indexOf(p));
      if (box == 2 && _green_points.contains(p))
        _green_points.remove(_green_points.indexOf(p));
      if (box == 3 && _blue_points.contains(p))
        _blue_points.remove(_blue_points.indexOf(p));
      if (box == 4 && _alpha_points.contains(p))
        _alpha_points.remove(_alpha_points.indexOf(p));

      updateFunction();
    }
  }
}

void TransferFunction::toggleTransfer()
{
  if (_type > 4)
    _type = 1;

  insertPoint(_box_width / 2, 0, _box_width, _box_height, 1, false);
  insertPoint(_box_width / 2, 0, _box_width, _box_height, 2, false);
  insertPoint(_box_width / 2, 0, _box_width, _box_height, 3, false);
  insertPoint(_box_width / 2, 0, _box_width, _box_height, 4, false);

  insertPoint(_box_width / 2, 0, _box_width, _box_height, _type++, true);
  //    updateFunction();
}

//Fredrik dra data
void TransferFunction::linearInterpolation(QVector<QPointF>& interp_points, QVector<QPoint>& points)
{
  interp_points.clear();
  QVector<QPoint> scaledPoints = scalePoints(points);

  QVectorIterator<QPoint> i(scaledPoints);
  while (i.hasNext()) {
    QPointF curr_point = i.next();

    if (i.hasNext()) {
      QPointF next_point = i.peekNext();
      float y = 0.0f;
      int x0 = curr_point.x();
      int x1 = next_point.x();
      float y0 = curr_point.y();
      float y1 = next_point.y();

      for (int i = curr_point.x(); i < next_point.x(); i++) {
        y = y0 + (i - x0) * ((y1 - y0) / (x1 - x0));
        QPointF function_point(i, y);

        interp_points.append(function_point);
      }
    }
  }
}

void TransferFunction::sort(QVector<QPoint>& list)
{
  qSort(list.begin(), list.end(), xLess);

  // Remove duplicates (points that are too close to each other)
  //  QMutableVectorIterator<QPoint> i(list);
  //  i.next(); // Move the iterator forward
  //  while(i.hasNext())
  //  {
  //    const QPoint &curr = i.next();
  //    if (i.hasNext() && (std::abs(curr.x() - i.next().x()) < 5))
  //      i.remove();
  //  }
}

void TransferFunction::removeClosestPoint(ColorComponent comp, const QPoint& pos)
{
}

void TransferFunction::moveClosestPoint(ColorComponent comp, const QPoint& pos)
{
}

void TransferFunction::updateFunction()
{
  sort(_red_points);
  sort(_blue_points);
  sort(_green_points);
  sort(_alpha_points);

  linearInterpolation(_red_points_function, _red_points);
  linearInterpolation(_blue_points_function, _blue_points);
  linearInterpolation(_green_points_function, _green_points);
  linearInterpolation(_alpha_points_function, _alpha_points);

  makeTransferFunction();
}

void TransferFunction::makeTransferFunction()
{
  transfunc.clear();
  for (int i = 0; i < 256; i++) {
    transfunc.push_back(GMlib::Vector<float, 4>((float)_red_points_function.at(i).y() / 255.0,
        (float)_green_points_function.at(i).y() / 255.0,
        (float)_blue_points_function.at(i).y() / 255.0,
        (float)_alpha_points_function.at(i).y() / 255.0));
  }
}

QVector<QPoint> TransferFunction::scalePoints(QVector<QPoint>& points)
{
  QVector<QPoint> scaledpoints;
  QVectorIterator<QPoint> i(points);
  while (i.hasNext()) {
    QPoint curr_point = i.next();
    //    QPoint point;
    //    point.setX(((float)curr_point.x() /(float)_box_width * 256 ));
    //    point.setY(255 - ((float)curr_point.y() /(float)_box_height * 255));
    scaledpoints.append(curr_point);
  }
  return scaledpoints;
}

void TransferFunction::saveTransferFunction(const QString& file_name)
{
  QVector<QVector<QPoint>> interpolation_points;

  interpolation_points.append(_red_points);
  interpolation_points.append(_green_points);
  interpolation_points.append(_blue_points);
  interpolation_points.append(_alpha_points);

  QFile file(file_name);
  file.open(QIODevice::WriteOnly);
  QDataStream out(&file);

  out << interpolation_points;

  file.close();
}

void TransferFunction::loadTransferFunction(const QString& file_name)
{
  QVector<QVector<QPoint>> interpolation_points;

  QFile file(file_name);
  file.open(QIODevice::ReadOnly);
  QDataStream in(&file);

  in >> interpolation_points;

  _red_points = interpolation_points.at(0);
  _green_points = interpolation_points.at(1);
  _blue_points = interpolation_points.at(2);
  _alpha_points = interpolation_points.at(3);

  file.close();

  updateComponent(All);
}

void TransferFunction::updateComponent(TransferFunction::ColorComponent comp)
{
  QVector<QPoint>* interp_points;
  QVector<QPointF>* function_points;

  UNUSED(interp_points);
  UNUSED(function_points);

  switch (comp) {
  case Red:
    interp_points = &_red_points;
    function_points = &_red_points_function;
    break;

  case Green:
    interp_points = &_green_points;
    function_points = &_green_points_function;
    break;

  case Blue:
    interp_points = &_blue_points;
    function_points = &_blue_points_function;
    break;

  case Alpha:
    interp_points = &_alpha_points;
    function_points = &_alpha_points_function;
    break;

  case All:
    sort(_red_points);
    sort(_green_points);
    sort(_blue_points);
    sort(_alpha_points);

    linearInterpolation(_red_points_function, _red_points);
    linearInterpolation(_green_points_function, _green_points);
    linearInterpolation(_blue_points_function, _blue_points);
    linearInterpolation(_alpha_points_function, _alpha_points);

    updateFunction();

    return;

  default:
    break;
  }

  ////  sort(*interp_points);
  //  linearInterpolation(*interp_points, *function_points);

  //  updateFunction();
}
