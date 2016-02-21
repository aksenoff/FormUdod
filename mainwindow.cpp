#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ------------------------- Всякая красота ----------------------------

    this->setWindowTitle(tr("Запись в объединения"));


    // ----------------------------- DataBase ------------------------------

        // Временная строчка - указан путь к базе.
        if (connectDB("D:/Domerk/GUAP/Diplom/kcttTempDB.sqlite"))
        {
            // работаем с базой
        }
        else
        {
            // просим указать параметры соединения
        }
}

MainWindow::~MainWindow()
{
    if (myDB.isOpen())
        myDB.close();
    delete ui;
}

// ============================================================
// ============== Установка соединения с базой ================
// ============================================================

bool MainWindow::connectDB(QString pathToDB)
{
    myDB = QSqlDatabase::addDatabase("QSQLITE");    // Указываем СУБД
    myDB.setDatabaseName(pathToDB);                 // Задаём полное имя базы

    QFileInfo checkFile(pathToDB);                  // Информация о файле базы

    if (checkFile.isFile())                         // Если такой файл существует
    {
        if (myDB.open())                            // Открываем соединение
        {
            ui->lblStatus->setText(tr("Соединение установлено")); // Выводим сообщение
            return true;                 // Возвращаем true
        }
        else
            ui->lblStatus->setText(tr("Ошибка соединения: соединение не установлено"));
    }
    else
    {
        ui->lblStatus->setText(tr("Ошибка соединения: отсутсвует файл базы данных"));
    }
    return false;
}
