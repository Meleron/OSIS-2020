#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QListWidget>

static int i = 0;

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    connect(ui->actionStart, &QAction::triggered, this, [this] { onStart(timer); });
    connect(ui->actionPause, &QAction::triggered, this, [this] { onPause(timer); });
    connect(ui->actionQuit, &QAction::triggered, this, [] { exit(0); });
    timer->start(25);
}


void MainWindow::paintEvent(QPaintEvent *event) {
    QPainter *paint = new QPainter(this);
    QTransform transform;
    paint->setPen(Qt::pink);
    transform.translate(500, 350);
    transform.rotate(i);

    paint->setTransform(transform);
    paint->drawText(100, 300, "Some text");
    paint->rotate(0);
    paint->end();
}

void MainWindow::onTimeout(){
    i = (i + 1) % 360;
    repaint();
}

void MainWindow::onStart(QTimer *timer)
{
    timer->start(50);
}

void MainWindow::onPause(QTimer *timer)
{
    timer->stop();
}
