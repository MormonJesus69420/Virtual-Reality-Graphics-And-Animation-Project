#ifndef TRANSFERFUNCTION_H
#define TRANSFERFUNCTION_H

#include <QList>
#include <QObject>
#include <QPoint>
#include <QVector>
#include <vector>

#include <gmCoreModule>


/**
 *Based on fredrik
 *
 *
 **/

class TransferFunction {
  public:
  enum ColorComponent { Red,
    Green,
    Blue,
    Alpha,
    All };
  TransferFunction();

  std::vector<GMlib::Vector<float, 4>> getTransferValues();
  void setTransferValues(std::vector<GMlib::Vector<float, 4>> value);

  void saveTransferFunction(const QString& file_name);
  void loadTransferFunction(const QString& file_name);

  void insertPoint(int x, int y, int width, int height, int box, bool insert);
  void toggleTransfer();

  private:
  QVector<QPoint> _red_points;
  QVector<QPoint> _blue_points;
  QVector<QPoint> _green_points;
  QVector<QPoint> _alpha_points;

  QVector<QPointF> _red_points_function;
  QVector<QPointF> _blue_points_function;
  QVector<QPointF> _green_points_function;
  QVector<QPointF> _alpha_points_function;
  std::vector<GMlib::Vector<float, 4>> transfunc;

  int _box_height;
  int _box_width;
  int _type;

  static bool xLess(const QPoint& point_one, const QPoint& point_two)
  {
    return (point_one.x() < point_two.x());
  }

  void linearInterpolation(QVector<QPointF>& interp_points, QVector<QPoint>& points);
  void sort(QVector<QPoint>& list);
  void removeClosestPoint(ColorComponent comp, const QPoint& pos);
  void moveClosestPoint(ColorComponent comp, const QPoint& pos);
  void updateFunction();
  void makeTransferFunction();
  QVector<QPoint> scalePoints(QVector<QPoint>& points);

  void updateComponent(ColorComponent comp);
};

#endif // TRANSFERFUNCTION_H
