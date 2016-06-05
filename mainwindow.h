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
#include <QSettings>
#include <QDate>
#include <QDialogButtonBox>
#include <QDebug>

#include <info.h>

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

    info *MyInfo;
    QDialog *help;

    void cleaner();
    void getDataAss(QStringList* qsl, QComboBox* comboBox);
    QString getDataCheckBox(QCheckBox* checkBox);

    void replaceBoolData(QString* mainstr, QString dataname, QString data);

private slots:
    void saveInfo();
    void clearForm();

};

#endif // MAINWINDOW_H
