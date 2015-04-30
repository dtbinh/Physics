#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#include "mainwindow.h"
#include <GL/glut.h>

int main(int argc, char *argv[])
{

    glutInit(&argc, argv);
    //glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
