#include "mainwindow.h"
#include "win_mainwindow.h"

#include <QtDebug>
#include <QListWidget>

MainWindow::~MainWindow()
{
    delete win;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , win(new Ui::MainWindow)
{
    win->setupUi(this);
}


void MainWindow::on_addButton_clicked()
{
    QListWidget *left = win->left;
    QString input = win->textEdit->text();
    bool flag = true;
    for(int i = 0; i < left->count(); ++i)
    {
        QListWidgetItem* item = left->item(i);
        if (item->text() == input) {
            flag = false;
            break;
        }
    }
    if(flag) {
        left->addItem(input);
        win->textEdit->clear();
    }
}

void MainWindow::on_clearButton_clicked()
{
    clear();
}

void clear()
{
    win->left->clear();
    win->right->clear();
}

void MainWindow::on_deleteButton_clicked()
{
    buttonDelete();
}

void buttonDelete()
{
    if(win->left->currentItem() != nullptr) {
        win->left->removeItemWidget(win->left->currentItem());
        delete win->left->currentItem();
    }
    if(win->right->currentItem() != nullptr) {
        win->right->removeItemWidget(win->right->currentItem());
        delete win->right->currentItem();
    }
}


void MainWindow::on_toRightButton_clicked()
{
    buttonRight();
}

void buttonRight()
{
    if(win->left->currentItem() != nullptr) {
        win->right->addItem( win->left->currentItem()->text() );
        win->left->removeItemWidget(win->left->currentItem());
        delete win->left->currentItem();
    }
}
