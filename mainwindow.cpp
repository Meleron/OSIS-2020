#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , interface(new Ui::MainWindow)
{
    interface->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete interface;
}


void MainWindow::on_pushButton_clicked()
{
    scene1 = new QGraphicsScene(interface->graphicsView);
    scene2 = new QGraphicsScene(interface->graphicsView_2);
    scene3 = new QGraphicsScene(interface->graphicsView_3);
    scene4 = new QGraphicsScene(interface->graphicsView_4);
    MyClass *class1 = new MyClass(scene1, 1000);
    QThread *thread2= new QThread;
    MyClass *class2 = new MyClass(scene2, 500);
    QThread *thread3= new QThread;
    MyClass *class3 = new MyClass(scene3, 700);
    QThread *thread4= new QThread;
    MyClass *class4 = new MyClass(scene4, 300);
    interface->graphicsView->setScene(scene1);
    interface->graphicsView_2->setScene(scene2);
    interface->graphicsView_3->setScene(scene3);
    interface->graphicsView_4->setScene(scene4);
    QThread *thread= new QThread;
    class1->moveToThread(thread);
    class2->moveToThread(thread2);
    class3->moveToThread(thread3);
    class4->moveToThread(thread4);
    connect(thread, SIGNAL(started()), class1, SLOT(doWork())); connect(class1, SIGNAL(send(int)), this, SLOT(update(int)));
    connect(thread2, SIGNAL(started()), class2, SLOT(doWork())); connect(class2, SIGNAL(send(int)), this, SLOT(update(int)));
    connect(thread3, SIGNAL(started()), class3, SLOT(doWork())); connect(class3, SIGNAL(send(int)), this, SLOT(update(int)));
    connect(thread4, SIGNAL(started()), class4, SLOT(doWork())); connect(class4, SIGNAL(send(int)), this, SLOT(update(int)));
    thread->start();
    thread2->start();
    thread3->start();
    thread4->start();
    interface->pushButton->setEnabled(false);
}


void MainWindow::update (int i)
{
    repaint();
    qDebug() << i;
}
