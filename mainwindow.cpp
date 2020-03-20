#include "mainwindow.h"
#include "ui_mainwindow.h"

static QColor color = Qt::white;
static bool toDraw = false;
static int pic=0;

MainWindow::~MainWindow()
{
    delete interface;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , interface(new Ui::MainWindow)
{
    interface->setupUi(this);
    nextWindow = new SecondWindow();
    connect(this, &MainWindow::sendMessage, nextWindow, &SecondWindow::GetMessage);
    nextWindow->show();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer->setInterval(30);
    timer->start();
}


void MainWindow::onTimeout(){
    else if(interface->starButton->isChecked())
        pic = 4;
    else if(interface->squareButton->isChecked())
        pic = 3;
    else if(interface->circleButton->isChecked())
        pic = 2;
    if(interface->diamondButton->isChecked())
        pic = 1;

    if(interface->redButton->isChecked())
        color = Qt::red;
    else if(interface->greenButton->isChecked())
        color = Qt::green;
    else if(interface->blueButton->isChecked())
        color = Qt::blue;

    toDraw = interface->toDraw->isChecked();

    emit sendMessage(pic, &color, toDraw);
}
