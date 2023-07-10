#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
    #include <QtPrintSupport/qtprintsupportglobal.h>
    #if QT_CONFIG(printdialog)
        #include <QtPrintSupport>
    #endif
#endif
#include "mainwindow.h"

MainWindow::MainWindow()
{
    QTableWidget* tableWidget = new QTableWidget;
    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderLabels(QStringList()<<tr("Ім'я")<<tr("Номер телефону")<<tr("E-mail")<<tr("Адреса"));
    tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    setCentralWidget(tableWidget);
    createActions();
    createStatusBar();
    setWindowTitle(tr("Адресна книга"));
}

void MainWindow::newEntry()
{
    QString name    = QInputDialog::getText(this, tr("Введіть ім'я"), tr("Ім'я:"));
    QString phone   = QInputDialog::getText(this, tr("Введіть номер телефона"), tr("Номер телефона:"));
    QString email   = QInputDialog::getText(this, tr("Введіть E-mail"), tr("E-mail:"));
    QString address = QInputDialog::getText(this, tr("Введіть адресу"), tr("Адреса:"));
    QTableWidget* tableWidget = qobject_cast<QTableWidget*>(centralWidget());

    int row = tableWidget->rowCount();
    tableWidget->insertRow(row);

    QTableWidgetItem* nameItem  = new QTableWidgetItem(name);
    QTableWidgetItem* phoneItem = new QTableWidgetItem(phone);
    QTableWidgetItem* emailItem = new QTableWidgetItem(email);
    QTableWidgetItem* addressItem = new QTableWidgetItem(address);
    tableWidget->setItem(row, 0, nameItem);
    tableWidget->setItem(row, 1, phoneItem);
    tableWidget->setItem(row, 2, emailItem);
    tableWidget->setItem(row, 3, addressItem);
}
/*
void MainWindow::print()
{
#if defined(QT_PRINTSUPPORT_LIB) && QT_CONFIG(printdialog)
    QTextDocument *document = textEdit->document();
    QPrinter printer;

    QPrintDialog dlg(&printer, this);
    if (dlg.exec() != QDialog::Accepted) {
        return;
    }
    document->print(&printer);
    statusBar()->showMessage(tr("Готово"), 2000);
#endif
}
*/
void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Відкрити файл"),
        QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).value(0), tr("Текстові файли (*.txt)"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, tr("Помилка"), tr("Не вдалося відкрити файл."));
        return;
    }
    QTableWidget* tableWidget = qobject_cast<QTableWidget*>(centralWidget());
    tableWidget->clearContents();
    tableWidget->setRowCount(0);
    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList fields = line.split(',');
        int row = tableWidget->rowCount();
        tableWidget->insertRow(row);
        for (int column = 0; column < fields.size(); ++column)
        {
            QTableWidgetItem* item = new QTableWidgetItem(fields[column]);
            tableWidget->setItem(row, column, item);
        }
    }
    file.close();
}

void MainWindow::save()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Зберегти файл"),
        QStandardPaths::standardLocations(QStandardPaths::DesktopLocation).value(1), tr("Текстові файли (*.txt)"));
    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
        QMessageBox::critical(this, tr("Помилка"), tr("Не вдалося відкрити файл для запису."));
        return;
        }
    QTextStream out(&file);
    QTableWidget* tableWidget = qobject_cast<QTableWidget*>(centralWidget());
    int rowCount    = tableWidget->rowCount();
    int columnCount = tableWidget->columnCount();
    for (int row = 0; row < rowCount; ++row)
        {
        QStringList rowData;
        for (int column = 0; column < columnCount; ++column)
            {
            QTableWidgetItem* item = tableWidget->item(row, column);
            if (item)
                rowData.append(item->text());
            else
                rowData.append("");
            }
        QString line = rowData.join(",");
        out << line << Qt::endl;
        }
    file.close();
}

void MainWindow::edit()
{
   QTableWidget* tableWidget = qobject_cast<QTableWidget*>(centralWidget());
   int currentRow    = tableWidget->currentRow();
   int currentColumn = tableWidget->currentColumn();
   if (currentRow >= 0 && currentColumn >= 0)
   {
       tableWidget->editItem(tableWidget->item(currentRow, currentColumn));
   }
}

void MainWindow::del()
{
   QTableWidget* tableWidget = qobject_cast<QTableWidget*>(centralWidget());
   int currentRow = tableWidget->currentRow();
   if (currentRow >= 0)
   {
        tableWidget->removeRow(currentRow);
   }
}


void MainWindow::createActions()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&Файл"));
    QToolBar *fileToolBar = addToolBar(tr("File"));
    QMenu *oppMenu = menuBar()->addMenu(tr("&Операції"));
    QToolBar *oppToolBar = addToolBar(tr("Операції"));

    const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/add.png"));
    QAction *newLetterAct = new QAction(newIcon, tr("&Додати"), this);
    newLetterAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    newLetterAct->setStatusTip(tr("Додати нового користувача"));
    connect(newLetterAct, &QAction::triggered, this, &MainWindow::newEntry);
    oppMenu->addAction(newLetterAct);
    oppToolBar->addAction(newLetterAct);

    const QIcon editIcon = QIcon::fromTheme("document-edit", QIcon(":/images/edit.png"));
    QAction *editAct = new QAction(editIcon, tr("&Редагувати"), this);
    editAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
    editAct->setStatusTip(tr("Редагувати дані"));
    connect(editAct, &QAction::triggered, this, &MainWindow::edit);
    oppMenu->addAction(editAct);
    oppToolBar->addAction(editAct);

    const QIcon deleteIcon = QIcon::fromTheme("document-edit", QIcon(":/images/delete.png"));
    QAction *deleteAct = new QAction(deleteIcon, tr("&Видалити"), this);
    deleteAct->setShortcuts(QKeySequence::Delete);
    deleteAct->setStatusTip(tr("Видалити користувача"));
    connect(deleteAct, &QAction::triggered, this, &MainWindow::del);
    oppMenu->addAction(deleteAct);
    oppToolBar->addAction(deleteAct);

    const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
    QAction *saveAct = new QAction(saveIcon, tr("&Зберегти"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Зберегти поточний"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::save);
    fileMenu->addAction(saveAct);
    fileToolBar->addAction(saveAct);
/*
    const QIcon printIcon = QIcon::fromTheme("document-print", QIcon(":/images/print.png"));
    QAction *printAct = new QAction(printIcon, tr("&Друкувати..."), this);
    printAct->setShortcuts(QKeySequence::Print);
    printAct->setStatusTip(tr("Друкувати поточний"));
    connect(printAct, &QAction::triggered, this, &MainWindow::print);
    fileMenu->addAction(printAct);
    fileToolBar->addAction(printAct);
*/
    const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
    QAction *openAct = new QAction(openIcon, tr("&Відкрити..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Відкрити файл"));
    connect(openAct, &QAction::triggered, this, &MainWindow::open);
    fileMenu->addAction(openAct);
    fileToolBar->addAction(openAct);

    fileMenu->addSeparator();

    QMenu *helpMenu = menuBar()->addMenu(tr("&Допомога"));
    QToolBar *helpToolBar = addToolBar(tr("Допомога"));

    const QIcon aboutIcon = QIcon::fromTheme("document-about", QIcon(":/images/help.png"));
    QAction *aboutAct = new QAction(aboutIcon, tr("&Довідка"), this);
    aboutAct->setStatusTip(tr("Довідка"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);
    helpMenu->addAction(aboutAct);
    helpToolBar->addAction(aboutAct);
    /* варіант 1
    const QIcon exitIcon = QIcon::fromTheme("document-exit", QIcon(":/images/exit.png"));
    QAction *quitAct = fileMenu->addAction(exitIcon,tr("&Вихід"), qApp, &QCoreApplication::quit);
    //quitAct->setShortcuts(QKeySequence::Quit);
    connect(quitAct, &QAction::triggered, this, &QCoreApplication::quit);
    fileMenu->addAction(quitAct);
    fileToolBar->addAction(quitAct);
    quitAct->setStatusTip(tr("Вихід з програми"));
    */
    //  варіант 2
    const QIcon exitIcon = QIcon::fromTheme("document-close", QIcon(":/images/exit.png"));
    QAction *quitAct = fileMenu->addAction(exitIcon,tr("&Вихід"), qApp, &QCoreApplication::exit);
    quitAct->setShortcuts(QKeySequence::Close);
    connect(quitAct, &QAction::triggered, this, &QCoreApplication::quit);
    fileMenu->addAction(quitAct);
    fileToolBar->addAction(quitAct);
    quitAct->setStatusTip(tr("Вихід з програми"));
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Готово"));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("Про програму"),
                       tr("<b>Адресна книга -</b> це електронний записник, що містить сукупність контактів."));
}
