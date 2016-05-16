#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QTextStream>
#include <QComboBox>

#include "connectiondialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSqlDatabase myDB;

    QRegularExpression *names;
    QRegularExpression *words;

    ConnectionDialog *dialog;

    void cleaner();
    void getDataAss(QStringList* qsl, QComboBox* comboBox);

private slots:
    void saveInfo();
    void clearForm();
    void help();
    void programInfo();
    void changeConfig();

    bool connectDB();

};

#endif // MAINWINDOW_H
