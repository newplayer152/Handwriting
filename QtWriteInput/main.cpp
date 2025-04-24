
#include "mainwindow.h"
#include <QApplication>
#include <QWidget>
#include <QObject>
#include <QDebug>
#include <QMouseEvent>



int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.showAt(QPoint(600, 300)); //浮动位置
    window.show();


    return app.exec();

}


