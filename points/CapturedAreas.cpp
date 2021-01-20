#include "CapturedAreas.h"
#include <QPainter>
#include "MapCell.h"

CapturedAreas::CapturedAreas(MapCell ***mapGame, int sizeCell, int heightGameMap, int weigthGameMap)
{
    heightGameMap_ = heightGameMap;
    weigthGameMap_ = weigthGameMap;
    sizeCell_ = sizeCell;

    mapGame_ = mapGame;
    areas_ = nullptr;
}

QRectF CapturedAreas::boundingRect() const
{
    //qreal penWight = 0;
    //return QRectF(penWight / 2, penWight / 2, (WEIGTH_GAME_MAP * DEFAULT_SIZE_CELL) + penWight, (HEIGHT_GAME_MAP * DEFAULT_SIZE_CELL) + penWight);
    return QRectF(0, 0, weigthGameMap_ * sizeCell_, heightGameMap_ * sizeCell_);
}


void CapturedAreas::paint(QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget)
{
    //QPolygon polygon;

    //polygon << QPoint(30,10) << QPoint(10,30) << QPoint(30,10);
    Area* term = areas_;
    while (term != nullptr)
    {
        if (term->team == redPlayer) painter->setBrush(QBrush(Qt::red, Qt::SolidPattern));
        else painter->setBrush(QBrush(Qt::blue, Qt::SolidPattern));
        painter->setOpacity(0.3);
        painter->drawPolygon(*term->isArea);

        term = term->next;
    }
}
//*

//функция ищет вокруг заданной точки первую точку(по порядку от левой верхней до правой нижней, начиная с точки FirstPoint)
QPoint CapturedAreas::checkingCellAround(QPoint CentralPoint, QPoint FirstPoint, point player)
{
    int PointByX = (CentralPoint.x() - (sizeCell_ / 2)) / sizeCell_,  PointByY = (CentralPoint.y() - (sizeCell_ / 2)) / sizeCell_;

    int x = (FirstPoint.x() - (sizeCell_ / 2)) / sizeCell_, y = (FirstPoint.y() - (sizeCell_ / 2)) / sizeCell_;
    if (x == PointByX + 2)
    {
        x = PointByX - 1;
        y++;
    }

    //Q_ASSERT(y <= PointByY + 2);
    while(y != PointByY + 2)
    {
        if ((x >= 0) && (y >= 0) && (x < weigthGameMap_) && (y < heightGameMap_) && (!mapGame_[x][y]->getCellInAreaFlag()) &&
                (mapGame_[x][y]->getPoint() ==  player)) return QPoint((x * sizeCell_) + (sizeCell_ / 2), (y * sizeCell_) + (sizeCell_ / 2));

        x++;

        if ((x == PointByX) && (y == PointByY)) x++;

        if (x == PointByX + 2)
        {
            x = PointByX - 1;
            y++;
        }

    };

    return QPoint();
}

//проверка есть ли точка в полигоне
bool checkPointInPolygon(QPoint point, QPolygon* polygon)
{
    for (int i = polygon->size() - 1;  i >= 0; i--)
        if (point == polygon->point(i)) return true;

    return false;
}
//*/

//проверка есть ли чужие точки внутри полигона
bool CapturedAreas::checkEnemyPointInPolygon(QPolygon *checkingPolygon, point enemyPlayer)
{
    for (int y = 1; y < heightGameMap_; y++)
        for (int x = 1; x < weigthGameMap_; x++)
        {
            if (mapGame_[x][y]->getPoint() == enemyPlayer)
            {
                QPolygon polygonAroundArea(1);
                polygonAroundArea.setPoint(0, (x * sizeCell_) + (sizeCell_ / 2), (y * sizeCell_) + (sizeCell_ / 2));
                if (checkingPolygon->intersects(polygonAroundArea)) return true;
            }

        }

    return false;

}


void CapturedAreas::capturedPointsInPolygon(QPolygon *checkingPolygon, point enemyPlayer)
{
    for (int y = 1; y < heightGameMap_; y++)
        for (int x = 1; x < weigthGameMap_; x++)
        {
            QPoint nextPoint((x * sizeCell_) + (sizeCell_ / 2), (y * sizeCell_) + (sizeCell_ / 2));

            if (!checkingPolygon->contains(nextPoint))
            {
                QPolygon polygonAroundArea;
                polygonAroundArea << nextPoint;
                if (checkingPolygon->intersects(polygonAroundArea)) mapGame_[x][y]->cellCaptured();
            }
        }
}


void CapturedAreas::searchNewArea(int FirstPointByX, int FirstPointByY, point player)
{

    QPolygon* newPolygon = new QPolygon;

    //for check:
    //*newPolygon << QPoint(((FirstPointByX + 1) * sizeCell_) + (sizeCell_ / 2), ((FirstPointByY) * sizeCell_) + (sizeCell_ / 2))
    //            << QPoint(((FirstPointByX) * sizeCell_) + (sizeCell_ / 2), ((FirstPointByY - 1) * sizeCell_) + (sizeCell_ / 2))
    //            << QPoint(((FirstPointByX - 1) * sizeCell_) + (sizeCell_ / 2), ((FirstPointByY) * sizeCell_) + (sizeCell_ / 2))
    //            << QPoint(((FirstPointByX) * sizeCell_) + (sizeCell_ / 2), ((FirstPointByY + 1) * sizeCell_) + (sizeCell_ / 2));


//*


    QPoint currentPoint((FirstPointByX * sizeCell_) + (sizeCell_ / 2), (FirstPointByY * sizeCell_) + (sizeCell_ / 2));                                  //точка, вокруг которой будем искать соседние точки
    QPoint firstPointBySearchNewPoint(((FirstPointByX - 1) * sizeCell_) + (sizeCell_ / 2), ((FirstPointByY - 1) * sizeCell_) + (sizeCell_ / 2));        //первая точка для поиска соседних точек

    *newPolygon << currentPoint;
    int countPointInPolygon = 1;
    bool flagExit = false;

    do
    {
        QPoint nextPoint = checkingCellAround(currentPoint, firstPointBySearchNewPoint, player);              //ищем соседнюю точку


        if (nextPoint.isNull())
        {
            if (countPointInPolygon > 1)
            {
                firstPointBySearchNewPoint.setX(newPolygon->point(countPointInPolygon - 1).x() + sizeCell_);
                firstPointBySearchNewPoint.setY(newPolygon->point(countPointInPolygon - 1).y());

                currentPoint = newPolygon->point(countPointInPolygon - 2);
            }

            newPolygon->removeLast();
            countPointInPolygon--;
        }


        //если эта точка уже есть в полигоне то возможно 2 варианта:
        // 1) эта точка из которой мы пришли
        // 2) найден цикл


        while ((newPolygon->contains(nextPoint)) && ((countPointInPolygon < 3) || (nextPoint != newPolygon->point(0))))
        {
            // чтобы выяснить что именно произошло проверим есть ли еще соседние точки(после найденной)
            nextPoint.rx() += sizeCell_;
            nextPoint = checkingCellAround(currentPoint, nextPoint, player);


            if (nextPoint.isNull())                         //если таких больше нет => то эта точка из которой мы пришли => мы зашли в тупик
            {
                firstPointBySearchNewPoint.setX(newPolygon->point(countPointInPolygon - 1).x() + sizeCell_);
                firstPointBySearchNewPoint.setY(newPolygon->point(countPointInPolygon - 1).y());

                currentPoint = newPolygon->point(countPointInPolygon - 2);

                newPolygon->removeLast();                   //удаляем точку, в которой тупик
                countPointInPolygon--;
            }

            //if ((nextPoint == newPolygon->point(0)) && (countPointInPolygon > 2)) break;
        }


        //если была найдена точка и ее нет в полигоне то мы добавляем ее в полигон
        if (!nextPoint.isNull())
        {
            *newPolygon << nextPoint;
            countPointInPolygon++;

            currentPoint = nextPoint;
            firstPointBySearchNewPoint.setX(currentPoint.x() - sizeCell_);
            firstPointBySearchNewPoint.setY(currentPoint.y() - sizeCell_);
        }



        //если последняя и первая точка полигона совпадают то мы
        //либо нашли цикл, проходящий через начальную точку
        //либо не нашли цикла и вернулись в начало

        //в обоих случаях конец алгоритма
        if ((newPolygon->isEmpty()) || (newPolygon->point(countPointInPolygon - 1) == newPolygon->point(0)))  flagExit = true;


    } while (!flagExit);

//*/
    point enemyPlayer;
    if (player == bluePlayer) enemyPlayer = redPlayer;
    else enemyPlayer = bluePlayer;

    if ((newPolygon->size() > 1) && (checkEnemyPointInPolygon(newPolygon, enemyPlayer)))
    {
        capturedPointsInPolygon(newPolygon, enemyPlayer);

        Area* newArea = new Area;

        newArea->isArea = newPolygon;
        newArea->next = areas_;
        newArea->team = player;

        areas_ = newArea;

        update();
    }
    else
    {
        delete newPolygon;
    }
}


void CapturedAreas::clear()
{
    Area* area, *previousArea;
    area = areas_;

    while (area != nullptr)
    {
        previousArea = area;
        area = area->next;
        delete previousArea;
    }

    areas_ = nullptr;
    update();
}
