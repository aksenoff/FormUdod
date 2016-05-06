#include "connectiondialog.h"
#include "ui_connectiondialog.h"

ConnectionDialog::ConnectionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectionDialog)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("Применить"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Отмена"));

    this->setWindowTitle(tr("Настройка соединения"));

    QSettings settings ("Kctt", "KcttTempDB");
    ui->tempHost->setText(settings.value("hostname", "localhost").toString());
    ui->tempBase->setText(settings.value("dbname", "kcttTempDB").toString());
    ui->tempUser->setText(settings.value("username").toString());
    ui->tempPasswd->setText(settings.value("password").toString());
}

ConnectionDialog::~ConnectionDialog()
{
    delete ui;
}

void ConnectionDialog::on_buttonBox_accepted()
{

    // Создаём окно, запрашивающее подтверждение действия
    QMessageBox messageBox(QMessageBox::Question,
                tr("Изменение настроек подключения"),
                tr("Вы уверены, что хотите изменить настройки подключения? <br /> "
                   "Обратите внимание, что некорректный ввод одного или нескольких<br />"
                   "параметров может привести к потере соединения и некорректной работе приложения."),
                QMessageBox::Yes | QMessageBox::Cancel,
                this);

    messageBox.setButtonText(QMessageBox::Yes, tr("Да"));
    messageBox.setButtonText(QMessageBox::Cancel, tr("Отмена"));

    // Если действие подтверждено
    if (messageBox.exec() == QMessageBox::Yes)
    {
        QSettings settings ("Kctt", "KcttTempDB");
        settings.setValue("hostname", ui->tempHost->text());
        settings.setValue("dbname", ui->tempBase->text());
        settings.setValue("username", ui->tempUser->text());
        settings.setValue("password", ui->tempPasswd->text());

        emit connectReconfig();

    }
}