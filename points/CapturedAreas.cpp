#include "CapturedAreas.h"
#include <QPainter>
#include "MapCell.h"

CapturedAreas::CapturedAreas(MapCell ***mapGame)
{
    mapGame_ = mapGame;
    areas_ = nullptr;
}

QRectF CapturedAreas::boundingRect() const
{
    //qreal penWight = 0;
    //return QRectF(penWight / 2, penWight / 2, (WEIGTH_GAME_MAP * DEFAULT_SIZE_CELL) + penWight, (HEIGHT_GAME_MAP * DEFAULT_SIZE_CELL) + penWight);
    return QRectF(0, 0, WEIGTH_GAME_MAP * DEFAULT_SIZE_CELL, HEIGHT_GAME_MAP * DEFAULT_SIZE_CELL);
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
QPoint checkingCellAround(MapCell ***mapGame, QPoint CentralPoint, QPoint FirstPoint, point player)
{
    int PointByX = (CentralPoint.x() - 10) / 20,  PointByY = (CentralPoint.y() - 10) / 20;

    int x = (FirstPoint.x() - 10) / 20, y = (FirstPoint.y() - 10) / 20;
    if (x == PointByX + 2)
    {
        x = PointByX - 1;
        y++;
    }


    do
    {
        if (mapGame[x][y]->getPoint() ==  player) return QPoint((x * 20) + 10, (y * 20) + 10);

        x++;

        if ((x == PointByX) && (y == PointByY)) x++;

        if (x == PointByX + 2)
        {
            x = PointByX - 1;
            y++;
        }

    } while(y != PointByY + 2);

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

void CapturedAreas::searchNewArea(int FirstPointByX, int FirstPointByY, point player)
{

    QPolygon* newPolygon = new QPolygon;

    //for check:
    //*newPolygon << QPoint(((FirstPointByX + 1) * 20) + 10, ((FirstPointByY) * 20) + 10) << QPoint(((FirstPointByX) * 20) + 10, ((FirstPointByY - 1) * 20) + 10)
    //               << QPoint(((FirstPointByX - 1) * 20) + 10, ((FirstPointByY) * 20) + 10) << QPoint(((FirstPointByX) * 20) + 10, ((FirstPointByY + 1) * 20) + 10);


//*


    QPoint currentPoint((FirstPointByX * 20) + 10, (FirstPointByY * 20) + 10);                                  //точка, вокруг которой будем искать соседние точки
    QPoint firstPointBySearchNewPoint(((FirstPointByX - 1) * 20) + 10, ((FirstPointByY - 1) * 20) + 10);        //первая точка для поиска соседних точек

    *newPolygon << currentPoint;
    int countPointInPolygon = 1;
    bool flagExit = false;

    do
    {
        QPoint nextPoint = checkingCellAround(mapGame_, currentPoint, firstPointBySearchNewPoint, player);              //ищем соседнюю точку


        //если эта точка уже есть в полигоне то возможно 2 варианта:
        // 1) эта точка из которой мы пришли
        // 2) найден цикл

        if (checkPointInPolygon(nextPoint, newPolygon))
        {
            // чтобы выяснить что именно произошло проверим есть ли еще соседние точки(после найденной)
            nextPoint.rx()++;
            nextPoint = checkingCellAround(mapGame_, currentPoint, nextPoint, player);


            if (nextPoint.isNull())                         //если таких больше нет => то эта точка из которой мы пришли => мы зашли в тупик
            {
                firstPointBySearchNewPoint.setX(newPolygon->point(countPointInPolygon - 1).x() + 20);
                firstPointBySearchNewPoint.setY(newPolygon->point(countPointInPolygon - 1).y());

                currentPoint = newPolygon->point(countPointInPolygon - 2);

                newPolygon->removeLast();                   //удаляем точку, в которой тупик
                countPointInPolygon--;
            }
        }


        //если была найдена точка и ее нет в полигоне то мы добавляем ее в полигон
        if (!nextPoint.isNull())
        {
            *newPolygon << nextPoint;
            countPointInPolygon++;

            currentPoint = nextPoint;
            firstPointBySearchNewPoint.setX(currentPoint.x() - 20);
            firstPointBySearchNewPoint.setY(currentPoint.y() - 20);
        }


        //если последняя и первая точка полигона совпадают то мы
        //либо нашли цикл, проходящий через начальную точку
        //либо не нашли цикла и вернулись в начало

        //в обоих случаях конец алгоритма
        if (newPolygon->point(countPointInPolygon - 1) == newPolygon->point(0))  flagExit = true;


    } while (!flagExit);

//*/
    Area* newArea = new Area;

    newArea->isArea = newPolygon;
    newArea->next = areas_;
    newArea->team = player;

    areas_ = newArea;

    update();
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
