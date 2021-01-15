#ifndef MAPCELL_H
#define MAPCELL_H

#include "mainwindow.h"
#include <QGraphicsItem>

//*

class MapCell : public QGraphicsItem
{

private:
    int locationByX_;
    int locationByY_;

    point point_;
    int size_;
    MainWindow *parent_;

public:
    MapCell(MainWindow *parent, int x, int y, point cellPoint = empty, int sizeCell = DEFAULT_SIZE_CELL);
    QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget = 0) override;
    void setPoint(point newPoint);
    point getPoint() { return point_; };


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

};//*/

#endif // MAPCELL_H

