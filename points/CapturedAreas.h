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
    int sizeCell_;
    int heightGameMap_;
    int weigthGameMap_;

    MapCell ***mapGame_;
    Area *areas_;

    QPoint checkingCellAround(QPoint CentralPoint, QPoint FirstPoint, point player);
    bool checkEnemyPointInPolygon(QPolygon *checkingPolygon, point enemyplayer);
    void capturedPointsInPolygon(QPolygon *checkingPolygon, point enemyPlayer);

public:
    CapturedAreas(MapCell ***mapGame, int sizeCell, int heightGameMap, int weigthGameMap);
    void searchNewArea(int FirstPointByX, int FirstPointByY, point enemyPlayer);
    void clear();

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
};

#endif // CAPTUREDAREA_H
