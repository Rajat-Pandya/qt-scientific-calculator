#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    // Set application metadata
    a.setApplicationName("Scientific Calculator");
    a.setOrganizationName("QtCalculator");
    a.setApplicationVersion("1.0.0");
    
    MainWindow w;
    w.show();
    
    return a.exec();
}
