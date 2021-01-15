#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "MapCell.h"
#include "CapturedAreas.h"

#include <QToolBar>
#include <QIcon>
#include <QAction>

#include <QGraphicsScene>


#include <QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    turn_ = redPlayer;

    QPixmap quitPix("exit.png");                                            //create toolbar
    QPixmap restartPix("restart.png");

    QToolBar *toolbar = addToolBar("main toolbar");

    labelShowTurn = new QLabel(" red   ");
    labelShowTurn->setStyleSheet("color: rgb(200, 0, 0)");
    QFont font = labelShowTurn->font();
    font.setPointSize(15);
    labelShowTurn->setFont(font);

    toolbar->addWidget(labelShowTurn);
    labelShowTurn->setToolTip("player's turn is displayed here");


    QAction *restart = toolbar->addAction(QIcon(restartPix), "restart game");
    connect(restart, &QAction::triggered, this, &MainWindow::restartGame);

    toolbar->addSeparator();

    QAction *quit = toolbar->addAction(QIcon(quitPix), "exit game");
    connect(quit, &QAction::triggered, qApp, &QApplication::quit);


    ui->button1->setText("next turn");                                      //create button "next turn"
    ui->button1->setCursor(Qt::PointingHandCursor);

    connect(ui->button1, SIGNAL(clicked()), this, SLOT(nextTurn()));


//*
    QGraphicsScene* q = new QGraphicsScene(this);                           //create map game

    mapGame_ = new MapCell**[WEIGTH_GAME_MAP];

    for(int i = 0; i <= WEIGTH_GAME_MAP; i++)
    {
        mapGame_[i] = new MapCell*[HEIGHT_GAME_MAP];

        for(int j = 0; j <= HEIGHT_GAME_MAP; j++)
        {
            mapGame_[i][j] = new MapCell(this, i, j);
            q->addItem(mapGame_[i][j]);
            mapGame_[i][j]->setPos(i * (mapGame_[i][j]->boundingRect()).width() + (mapGame_[i][j]->boundingRect()).width()/2,
                            j * (mapGame_[i][j]->boundingRect()).height() + (mapGame_[i][j]->boundingRect()).height()/2);
        }
    }

    mapAreas_ = new  CapturedAreas(mapGame_);
    q->addItem(mapAreas_);
    mapAreas_->setPos(10, 10);

    ui->graphicsView->setScene(q);

}

void MainWindow::createArea(int FirstPointByX, int FirstPointByY)
{
    if (!turn_) mapAreas_->searchNewArea(FirstPointByX, FirstPointByY, bluePlayer);
    else mapAreas_->searchNewArea(FirstPointByX, FirstPointByY, redPlayer);
}



void MainWindow::nextTurn()
{
    turn_ = !turn_;
    if (turn_)
    {
        labelShowTurn->setText(" blue ");
        labelShowTurn->setStyleSheet("color: rgb(0, 0, 200)");
    }
    else
    {
        labelShowTurn->setText(" red  ");
        labelShowTurn->setStyleSheet("color: rgb(200, 0, 0)");
    }
}

void MainWindow::restartGame()
{
    for(int i = 0; i <= WEIGTH_GAME_MAP; i++)
        for(int j = 0; j <= HEIGHT_GAME_MAP; j++)
        {

            mapGame_[i][j]->setPoint(empty);
        }

    mapAreas_->clear();
}

void MainWindow::show()
{
    ui->graphicsView->setSceneRect(0, 0, WEIGTH_GAME_MAP * DEFAULT_SIZE_CELL + 40, HEIGHT_GAME_MAP * DEFAULT_SIZE_CELL + 40);//ui->graphicsView->viewport()->width(), ui->graphicsView->viewport()->height());
    QWidget::show();
}


MainWindow::~MainWindow()
{
    for(int i = 0; i <= WEIGTH_GAME_MAP; i++)
        for(int j = 0; j <= HEIGHT_GAME_MAP; j++)
        {
            delete mapGame_[i][j];
        }

    for(int i = 0; i <= WEIGTH_GAME_MAP; i++)
        delete []mapGame_[i];

    delete []mapGame_;


    delete ui;
}





