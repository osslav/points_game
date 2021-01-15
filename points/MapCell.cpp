#include "MapCell.h"
#include <QPainter>
//*
MapCell::MapCell(MainWindow *parent, int x, int y, point cellPoint, int sizeCell)
    //: QGraphicsItem()
{
    parent_ = parent;
    size_ = sizeCell;
    point_ = cellPoint;

    locationByX_ = x;
    locationByY_ = y;

    setCursor(Qt::PointingHandCursor);
}


QRectF MapCell::boundingRect() const
{
    //qreal penWight = 1;
    //return QRectF(penWight / 2, penWight / 2, size_ + penWight, size_ + penWight);
    return QRectF(0, 0, size_, size_);
}


void MapCell::paint(QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget)
{
    painter->setPen(Qt::gray);
    painter->drawLine(size_/2, 0, size_/2, size_);
    painter->drawLine(0, size_/2, size_, size_/2);

    if (point_ == bluePlayer)
    {
        painter->setBrush(QBrush(Qt::blue, Qt::SolidPattern));
        painter->drawEllipse(size_/4, size_/4, size_/2, size_/2);
    }
    if (point_ == redPlayer)
    {
        painter->setBrush(QBrush(Qt::red, Qt::SolidPattern));
        painter->drawEllipse(size_/4, size_/4, size_/2, size_/2);
    }
}
//*
void MapCell::setPoint(point newPoint)
{
    point_ = newPoint;
    if (point_ == empty) setCursor(Qt::PointingHandCursor);
    else  setCursor(Qt::ArrowCursor);

    update();
}
//*/

void MapCell::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (point_ == empty)
    {
        if (parent_->getTurn()) point_ = bluePlayer;
        else point_ = redPlayer;
        setCursor(Qt::ArrowCursor);


        parent_->nextTurn();
        parent_->createArea(locationByX_ ,locationByY_);


        update();
    }

}
//*/

