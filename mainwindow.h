#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAction;
//class QListWidget;
class QMenu;
class QTextEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
private slots:
    void open();
    void save();
    void newEntry();
    //void print();
    void edit();
    void del();
    void about();
private:
    void createActions();
    void createStatusBar();
    QTextEdit   *textEdit;
    //QListWidget *customerList;
    QMenu       *viewMenu;
};
#endif // MAINWINDOW_H
