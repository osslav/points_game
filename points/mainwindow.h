#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QWidget>
#include <QApplication>
#include <QPushButton>
#include <QLabel>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


const int DEFAULT_SIZE_CELL = 20;
const int HEIGHT_GAME_MAP = 40;
const int WEIGTH_GAME_MAP = 30;



enum point
{
    empty = -1,
    redPlayer,
    bluePlayer,
};



class MapCell;
class CapturedAreas;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    void show();

    bool getTurn() { return turn_;} ;
    void createArea(int FirstPointByX, int FirstPointByY);

    ~MainWindow();

public slots:
    void nextTurn();
    void restartGame();


private:
    MapCell*** mapGame_;
    CapturedAreas* mapAreas_;

    QLabel *labelShowTurn;
    bool turn_;

    Ui::MainWindow *ui;
};



#endif // MAINWINDOW_H
