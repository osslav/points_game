#ifndef CAPTUREDAREA_H
#define CAPTUREDAREA_H

#include "mainwindow.h"
#include <QGraphicsItem>
#include <QPainter>

struct Area
{
    QPolygon* isArea;
    Area* next;

    point team;

    ~Area() { delete isArea; };
};

class CapturedAreas : public QGraphicsItem
{
private:
    MapCell ***mapGame_;
    Area *areas_;

public:
    CapturedAreas(MapCell ***mapGame);
    void searchNewArea(int FirstPointByX, int FirstPointByY, point player);
    void clear();

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // CAPTUREDAREA_H
