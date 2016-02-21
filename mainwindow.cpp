#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ------------------------- Всякая красота ----------------------------

    this->setWindowTitle(tr("Запись в объединения"));

    // --------------------------- Main ToolBar ----------------------------

    // Иконки: http://www.flaticon.com/packs/web-application-ui/4

    ui->mainToolBar->addAction(QIcon(":/icons/Icons/save.png"), tr("Сохранить"), this, SLOT(saveInfo()));
    ui->mainToolBar->addAction(QIcon(":/icons/Icons/clear.png"), tr("Очистить форму"), this, SLOT(clearForm()));
    ui->mainToolBar->addAction(QIcon(":/icons/Icons/help.png"), tr("Помощь"), this, SLOT(help()));
    ui->mainToolBar->addAction(QIcon(":/icons/Icons/info.png"), tr("О программе"), this, SLOT(programInfo()));

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

void MainWindow::saveInfo()
{
    if (myDB.isOpen())
    {
        // Проверяем заполнение обязательных полей


        // Если обязательные поля заполнены
        // Создаём окно, запрашивающее подтверждение действия
        QMessageBox messageBox(QMessageBox::Question,
                    tr("Сохранение"),
                    tr("Сохранить введённые данные?"),
                    QMessageBox::Yes | QMessageBox::No,
                    this);

        messageBox.setButtonText(QMessageBox::Yes, tr("Да"));
        messageBox.setButtonText(QMessageBox::No, tr("Нет"));

        // Если действие подтверждено
        if (messageBox.exec() == QMessageBox::Yes)
        {

        }

    }
    else
    {
        ui->lblStatus->setText(tr("Ошибка сохранения: отсутсвует соединение с базой"));
    }

}

void MainWindow::clearForm()
{
    // Создаём окно, запрашивающее подтверждение действия
    QMessageBox messageBox(QMessageBox::Question,
                tr("Очистка формы"),
                tr("Вы действительно хотите очистить форму? <br /> Все данные будут потеряны!"),
                QMessageBox::Yes | QMessageBox::No,
                this);

    messageBox.setButtonText(QMessageBox::Yes, tr("Да"));
    messageBox.setButtonText(QMessageBox::No, tr("Нет"));

    // Если действие подтверждено
    if (messageBox.exec() == QMessageBox::Yes)
    {
        ui->surname->clear();
        ui->name->clear();
        ui->patrname->clear();
        ui->docNum->clear();
        ui->schoolArea->clear();
        ui->schoolNum->clear();
        ui->classNum->clear();
        ui->phone->clear();
        ui->mail->clear();

        ui->parents->clear();
        ui->address->clear();
        ui->courses->clear();

        ui->docType->setCurrentIndex(0);
        ui->gender->setCurrentIndex(0);
        ui->bday->setCurrentIndex(0);
        ui->bmon->setCurrentIndex(0);

        ui->byear->clear();
    }

}

void MainWindow::help()
{

}

void MainWindow::programInfo()
{

}
