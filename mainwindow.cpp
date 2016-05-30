#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    MyInfo = new info();

    // ------------------------- Всякая красота ----------------------------

    QSettings settings ("Other/config.ini", QSettings::IniFormat);
    settings.beginGroup("Settings");
    this->setWindowTitle(settings.value("windowtile", "Запись в объединения").toString());
    settings.endGroup();

    // --------------------------- Main ToolBar ----------------------------

    // Иконки: http://www.flaticon.com/packs/web-application-ui/4

    ui->mainToolBar->addAction(QIcon(":/icons/Icons/save.png"), tr("Сохранить"), this, SLOT(saveInfo()));
    ui->mainToolBar->addAction(QIcon(":/icons/Icons/clear.png"), tr("Очистить форму"), this, SLOT(clearForm()));
    ui->mainToolBar->addSeparator();
    ui->mainToolBar->addAction(QIcon(":/icons/Icons/help.png"), tr("Помощь"), this, SLOT(help()));
    ui->mainToolBar->addAction(QIcon(":/icons/Icons/info.png"), tr("О программе"), MyInfo, SLOT(show()));

    names = new QRegularExpression("^[А-ЯЁ]{1}[а-яё]*(-[А-ЯЁ]{1}[а-яё]*)?$");
    words = new QRegularExpression();

    // Получение инфы обобъединениях

    QFile file ("Other/association.txt");
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        if(stream.status() == QTextStream::Ok)
        {
            QStringList qsl;
            while (!stream.atEnd())
            {
                QString str = stream.readLine().simplified().replace(QRegularExpression("-{2,}"), "-");
                if (!str.isEmpty() && str.at(0) != '#')
                    qsl.append(str);
            }

            qsl.sort();
            qsl.prepend("- Не выбрано -");
            ui->ass1->addItems(qsl);
            ui->ass2->addItems(qsl);
            ui->ass3->addItems(qsl);
        }
        file.close();
    }

    // ----------------------------- DataBase ------------------------------

    myDB = QSqlDatabase::addDatabase("QSQLITE");    // Указываем СУБД
    settings.beginGroup("Database");
    myDB.setHostName(settings.value("hostname", "localhost").toString());
    myDB.setDatabaseName(settings.value("dbname", "kcttTempDB").toString());
    myDB.setUserName(settings.value("username").toString());
    myDB.setPassword(settings.value("password").toString());
    settings.endGroup();

    if (myDB.open() && !myDB.isOpenError() && myDB.isValid())                            // Открываем соединение
        ui->lblStatus->setText(tr("Соединение установлено")); // Выводим сообщение
    else
        ui->lblStatus->setText(tr("Ошибка соединения: соединение не установлено"));
}

MainWindow::~MainWindow()
{
    if (myDB.isOpen())
        myDB.close();

    delete MyInfo;

    delete names;
    delete words;
    delete ui;
}

// ============================================================
// ============================================================
// ============================================================

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

        QDate currentDate;
        QString strCurrentDate = currentDate.currentDate().toString(Qt::SystemLocaleShortDate);
        // В данный момент дата устанавливается в формате ДД.ММ.ГГГГ
        // Подумать над тем, как её нужно задавать
        qDebug()<<strCurrentDate;

        if (surname == "" || name == "" || docNum == "" || schoolNum == "" || phone == "" || docType == "" || gender == "" || (ui->ass1->currentIndex() == 0 && ui->ass2->currentIndex() == 0 && ui->ass3->currentIndex() == 0))
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
        QString parents;
        if (ui->parentType1->currentIndex() != 0)
            parents.append(ui->parentType1->currentText() + ": " + ui->parent1->text().simplified().replace(QRegularExpression("-{2,}"), "-") + "; ");

        if (ui->parentType2->currentIndex() != 0)
            parents.append(ui->parentType2->currentText() + ": " + ui->parent2->text().simplified().replace(QRegularExpression("-{2,}"), "-")  + "; ");

        QString address = ui->address->toPlainText().simplified().replace(QRegularExpression("-{2,}"), "-");

        QString birthday;

        if (ui->bday->currentIndex() != 0 && ui->bmon->currentIndex() != 0)
        {
            QDate bdate;
            bdate.setDate(ui->byear->value(), ui->bmon->currentText().toInt(), ui->bday->currentText());
            birthday.append(bdate.toString(Qt::SystemLocaleShortDate));
        }

        QString police = getDataCheckBox(ui->police);
        QString inv = getDataCheckBox(ui->inv);
        QString large = getDataCheckBox(ui->large);
        QString migr = getDataCheckBox(ui->migr);
        QString incom = getDataCheckBox(ui->incom);
        QString needy = getDataCheckBox(ui->needy);
        QString health = getDataCheckBox(ui->health);
        QString orph = getDataCheckBox(ui->orph);



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
            strQuery.append("`Фамилия`, `Имя`, `Отчество`, `Тип документа`, `Номер документа`, `Пол`, `Дата рождения`, `Район школы`, `Школа`, `Класс`, `Родители`, `Домашний адрес`, `Телефон`, ");
            strQuery.append("`e-mail`, `С ослабленным здоровьем`, `Сирота`, `Инвалид`, `На учёте в полиции`, `Многодетная семья`, `Неполная семья`, `Малообеспеченная семья`, `Мигранты`, `Дата заявления`) VALUES ('");
            strQuery.append(surname + "', '" + name  + "', '" + patrname  + "', '" + docType  + "', '" + docNum  + "', '" + gender  + "', '" + birthday  + "', '");
            strQuery.append(schoolArea  + "', '" + schoolNum  + "', '" + classNum  + "', '" + parents  + "', '" + address  + "', '" + phone  + "', '" + mail + "', '");
            strQuery.append(health  + "', '" + orph  + "', '" + inv  + "', '" + police  + "', '" + large  + "', '" + incom  + "', '" + needy  + "', '" + migr + "', '" + strCurrentDate + "');");

            QSqlQuery query;
            query.exec(strQuery);

            qDebug() << strQuery;
            //qDebug() << query.lastError().toString();

            strQuery.clear();

            QStringList qsl;
            getDataAss(&qsl, ui->ass1);
            getDataAss(&qsl, ui->ass2);
            getDataAss(&qsl, ui->ass3);
            for (QString & newCours : qsl)
            {
                strQuery.append("INSERT INTO Запись (`Тип документа`, `Номер документа`, `Объединение`) VALUES ('");
                strQuery.append(docType  + "', '" + docNum  + "', '" + newCours + "');");
                query.exec(strQuery);
                strQuery.clear();
            }

            // ------------------------------------------
            // Создание файла для печати
            // шаблон: Other/template.txt

            QFile file("Other/template.txt");
            QString declaration;
            if(file.open(QIODevice::ReadOnly))
            {
                QTextStream stream(&file);
                declaration = stream.readAll();
                file.close();
            }

            declaration.replace("%surname%", surname);
            declaration.replace("%name%", name);
            declaration.replace("%patrname%", patrname);

            declaration.replace("%parent1_type%", ui->parentType1->currentText());
            declaration.replace("%parent1%", ui->parent1->text().simplified());
            declaration.replace("%parent2_type%", ui->parentType2->currentText());
            declaration.replace("%parent2%", ui->parent2->text().simplified());

            declaration.replace("%birthday%", birthday);
            declaration.replace("%classNum%", classNum);
            declaration.replace("%schoolNum%", schoolNum);
            declaration.replace("%schoolArea%", schoolArea);
            declaration.replace("%address%", address);
            declaration.replace("%phone%", phone);
            declaration.replace("%email%", mail);
            declaration.replace("%doctype%", docType);
            declaration.replace("%docnum%", docNum);
            declaration.replace("%сurrentDate%", strCurrentDate);

            QString ass;
            for (int i = 1; i<=qsl.size(); i++)
                ass.append(QString::number(i) + ".&nbsp;<u>" + qsl.at(i-1) +"</u><br />");

            declaration.replace("%ass%", ass);

            replaceBoolData(&declaration, "%large%", large);
            replaceBoolData(&declaration, "%incom%", incom);
            replaceBoolData(&declaration, "%needy%", needy);
            replaceBoolData(&declaration, "%orph%", orph);
            replaceBoolData(&declaration, "%migr%", migr);

            if (inv == "true" || health == "true")
                declaration.replace("%health%", "Да");
            else
                declaration.replace("%health%", "Нет");

            qDebug() << declaration;

            QString filename = surname + "_" + name + "_" + docNum + ".html";
            QFile file2(filename);
            if(file2.open(QIODevice::WriteOnly))
            {
                QTextStream stream(&file2);
                stream << declaration;
                file2.close();
            }

            // ------------------------------------------

            cleaner();
        }

    }
    else
    {
        // Выводим сообщение с инфой
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

// ============================================================
// ============================================================
// ============================================================

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

void MainWindow::getDataAss(QStringList* qsl, QComboBox* comboBox)
{
    if (comboBox->currentIndex() != 0)
        qsl->append(comboBox->currentText());
}

QString MainWindow::getDataCheckBox(QCheckBox* checkBox)
{
    if (checkBox->isChecked())
        return "true";
    else
        return "false";
}

void MainWindow::replaceBoolData(QString* mainstr, QString dataname, QString data)
{
    if (data == "true")
        mainstr->replace(dataname, "Да");
    else
        mainstr->replace(dataname, "Нет");
}

// ============================================================
// ============================================================
// ============================================================

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

    ui->address->clear();

    ui->docType->setCurrentIndex(0);
    ui->gender->setCurrentIndex(0);
    ui->bday->setCurrentIndex(0);
    ui->bmon->setCurrentIndex(0);

    ui->byear->clear();

    ui->ass1->setCurrentIndex(0);
    ui->ass2->setCurrentIndex(0);
    ui->ass3->setCurrentIndex(0);

    ui->parentType1->setCurrentIndex(0);
    ui->parentType2->setCurrentIndex(0);

    ui->parent1->clear();
    ui->parent2->clear();

    ui->incom->setChecked(false);
    ui->inv->setChecked(false);
    ui->migr->setChecked(false);
    ui->large->setChecked(false);
    ui->police->setChecked(false);
    ui->orph->setChecked(false);
    ui->needy->setChecked(false);
    ui->health->setChecked(false);
}

// ============================================================
// ============================================================
// ============================================================

void MainWindow::help()
{

}

// ============================================================
// ============================================================
// ============================================================

