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


    names = new QRegularExpression("^[А-ЯЁ]{1}[а-яё]*(-[А-ЯЁ]{1}[а-яё]*)?$");
    words = new QRegularExpression();


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

    if (myDB.isOpenError())
    {
        ConnectionDialog cd;
        cd.exec();
    }
}

MainWindow::~MainWindow()
{
    if (myDB.isOpen())
        myDB.close();

    delete names;
    delete words;
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

        QString surname = ui->surname->text().simplified().replace(QRegularExpression("-{2,}"), "-");
        QString name = ui->name->text().simplified().replace(QRegularExpression("-{2,}"), "-");
        QString docNum = ui->docNum->text().simplified().replace(QRegularExpression("-{2,}"), "-");
        QString schoolNum = ui->schoolNum->text().simplified().replace(QRegularExpression("-{2,}"), "-");
        QString phone = ui->phone->text().simplified().replace(QRegularExpression("-{2,}"), "-");

        QString docType = ui->docType->currentText();
        QString gender = ui->gender->currentText();

        if (surname == "" || name == "" || docNum == "" || schoolNum == "" || phone == "" || docType == "" || gender == "")
        {
            // Если одно или несколько обязательных полей не заполнены
            // Сообщаем об этом пользователю
            QMessageBox messageBox(QMessageBox::Information,
                                   tr("Сохранение"),
                                   tr("Ошибка сохранения: Не заполнено одно или несколько обязательных полей!"),
                                   QMessageBox::Yes,
                                   this);
            messageBox.setButtonText(QMessageBox::Yes, tr("ОК"));
            messageBox.exec();
            // Выходим
            return;
        }

        QString patrname = ui->patrname->text().simplified().replace(QRegularExpression("-{2,}"), "-");
        QString schoolArea = ui->schoolArea->text().simplified().replace(QRegularExpression("-{2,}"), "-");
        QString classNum = ui->classNum->text().simplified().replace(QRegularExpression("-{2,}"), "-");
        QString mail = ui->mail->text().simplified().replace(QRegularExpression("-{2,}"), "-");
        QString parents = ui->parents->toPlainText().simplified().replace(QRegularExpression("-{2,}"), "-");
        QString address = ui->address->toPlainText().simplified().replace(QRegularExpression("-{2,}"), "-");
        QString courses = ui->courses->toPlainText().simplified().replace(QRegularExpression("-{2,}"), "-");
        QString bday = ui->bday->currentText();
        QString bmon = ui->bmon->currentText();
        QString byear = QString::number(ui->byear->value());

        bool isName = true;
        bool isSurname = true;
        bool isPatrName = true;

        QRegularExpressionMatch match = names->match(name);
        isName = match.hasMatch();

        match = names->match(surname);
        isSurname = match.hasMatch();

        if (!patrname.isEmpty())
        {
            match = names->match(patrname);
            isPatrName = match.hasMatch();
        }

        if (!isName || !isSurname || !isPatrName)
        {
            QMessageBox messageBox(QMessageBox::Information,
                                   tr("Сохранение"),
                                   tr("Ошибка сохранения: Одно или несколько полей заполнены неверно!"),
                                   QMessageBox::Yes,
                                   this);
            messageBox.setButtonText(QMessageBox::Yes, tr("ОК"));
            messageBox.exec();
            return;
        }

        QString birthday = bday + "." + bmon + "." + byear;

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
            QString strQuery = "INSERT INTO Учащийся (";
            strQuery.append("'Фамилия', 'Имя', 'Отчество', 'Тип документа', 'Номер документа', 'Пол', 'Год рождения', ");
            strQuery.append("'Район школы', 'Школа', 'Класс', 'Родители', 'Домашний адрес', 'Телефон', 'e-mail') VALUES ('");
            strQuery.append(surname + "', '" + name  + "', '" + patrname  + "', '" + docType  + "', '" + docNum  + "', '" + gender  + "', '" + birthday  + "', '");
            strQuery.append(schoolArea  + "', '" + schoolNum  + "', '" + classNum  + "', '" + parents  + "', '" + address  + "', '" + phone  + "', '" + mail  + "');");

            QSqlQuery query;
            query.exec(strQuery);
            strQuery.clear();

            QStringList qsl = courses.split("/n", QString::SkipEmptyParts);
            for (QString & newCours : qsl)
            {
                newCours = newCours.trimmed(); // обрезаем лишние пробелы
                if (!newCours.isEmpty())
                {
                    strQuery.append("INSERT INTO Запись ('Тип документа', 'Номер документа', 'Объединение') VALUES ('");
                    strQuery.append(docType  + "', '" + docNum  + "', '" + newCours + "');");
                    query.exec(strQuery);
                    strQuery.clear();
                }
            }

            cleaner();
        }

    }
    else
    {
        // Выводим сообщение с инфой
        // Вероятно, имеет смысл иметь возможность вызвать отсюда окна с настройками соединения


        ui->lblStatus->setText(tr("Ошибка сохранения: отсутсвует соединение с базой"));
        QMessageBox messageBox(QMessageBox::Information,
                               tr("Сохранение"),
                               tr("Сохранение невозможно: отсутсвует соединение с базой."),
                               QMessageBox::Yes,
                               this);
        messageBox.setButtonText(QMessageBox::Yes, tr("ОК"));
        messageBox.exec();
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
        cleaner();
    }

}

void MainWindow::cleaner()
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

void MainWindow::help()
{

}

void MainWindow::programInfo()
{

}
