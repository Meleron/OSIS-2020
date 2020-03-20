#include "secondwindow.h"
#include "ui_secondwindow.h"
#include <QPainter>

static bool toDraw = true;

SecondWindow::~SecondWindow()
{
    delete interface;
}

SecondWindow::SecondWindow(QWidget *parent) :
    QDialog(parent),
    interface(new Ui::SecondWindow)
{
    interface->setupUi(this);
}

void SecondWindow::GetMessage(int pic, QColor* color, bool _toDraw){
    this->pic = pic;
    this->color = color;
    toDraw = _toDraw;
    repaint();
}

void movePoints(QPointF *points, int count, QPointF qpoint){
    for(int i=0; i < count; i++){
        points[i] = points[i] + qpoint;
    }
}

void SecondWindow::paintEvent(QPaintEvent *event) {
    if(toDraw)
    {
        QPainter *painter = new QPainter(this);

        QPointF diamond[4] = {
              QPointF(65, 0),
              QPointF(130, 115),
              QPointF(65, 225),
              QPointF(0, 115)
        };
        QPointF star[10] = {
                QPointF(90, 0),
                QPointF(114, 70),
                QPointF(182, 71),
                QPointF(129, 112),
                QPointF(148, 181),
                QPointF(90, 142),
                QPointF(35, 181),
                QPointF(55, 112),
                QPointF(0, 71),
                QPointF(71, 71)
        };
        movePoints(star, 10, QPointF(100, 100));
        movePoints(diamond, 4, QPointF(120, 100));
        painter->setPen(QPen(Qt::black, 0));
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setBrush(QBrush(*color, Qt::SolidPattern));
        switch(pic){
            case 1: painter->drawPolygon(diamond, 4); break;
            case 2: painter->drawEllipse(150, 150, 100, 100); break;
            case 3: painter->drawRect(150, 150, 100, 100); break;
            case 4: painter->drawPolygon(star, 10); break;
        }

        painter->end();
    }
}
