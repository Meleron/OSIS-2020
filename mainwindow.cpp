#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <thread>

static bool toDraw = true;
static QPoint points[20];

MainWindow::~MainWindow()
{
    delete interface;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , interface(new Ui::MainWindow)
{
    interface->setupUi(this);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer->setInterval(40);
    timer->start();
}

void MainWindow::paintEvent(QPaintEvent *event) {
    if(toDraw)
    {
        QPainter *painter = new QPainter(this);
        painter->setPen(QPen(Qt::red, 0));
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setBrush(QBrush(Qt::darkRed, Qt::SolidPattern));
        for ( int i =0; i < 20; i++){
            painter->drawEllipse(points[i].x(), points[i].y(), 30, 30);
        }
        painter->end();
    }
}

void MainWindow::onTimeout(){
    QPoint pos = QWidget::mapFromGlobal(QCursor::pos()) - QPoint(16,16);
    for (int i = 19; i >= 1; i--){
        points[i] = points[i-1];
    }
    points[0] = pos;
    repaint();
}