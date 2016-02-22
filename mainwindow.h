#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql/QSql>
#include <QtSql/QSqlDatabase>
#include <QtSql/QtSql>
#include <QFileInfo>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

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

    bool connectDB(QString pathToDB);
    void cleaner();

private slots:
    void saveInfo();
    void clearForm();
    void help();
    void programInfo();
};

#endif // MAINWINDOW_H
