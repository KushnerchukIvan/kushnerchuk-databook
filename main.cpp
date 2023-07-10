#include "mainwindow.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QIcon icon(":/images/addressbook.png");
    app.setWindowIcon(icon);
    Q_INIT_RESOURCE(databook);
    MainWindow mainWin;
    mainWin.resize(600,400);
    mainWin.show();
    return app.exec();
}
