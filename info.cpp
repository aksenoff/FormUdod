#include "info.h"
#include "ui_info.h"

info::info(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::info)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Информация о программе"));
    this->setWindowFlags(Qt::Window | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);

    QString orgName;
    QSettings settings ("Other/config.ini", QSettings::IniFormat);
    settings.beginGroup("Settings");
    orgName.append(settings.value("orgname").toString());
    settings.endGroup();

    if(!orgName.isEmpty())
        orgName.prepend(" ");

    ui->programText->setHtml("<strong>FormUdod</strong><br /><br />Программа предназначена для записи в объединения, входящие в состав учреждения дополнительного образования" + orgName + ".<br /><br />"
                           "Программа представляет собой анкету, через заполнение которой можно осуществить запись ребёнка в предложенные объединения. "
                           "На основе введённых пользователем данных формируется документ в формате HTML, представляющий собой официальное заявление. Это заявление доступно для печати и уже содержит все необходимые данные — пользователю остаётся только поставить свою подпись.<br /><br />"
                           "Подробная инструкция по работе с программной находится в разделе Помощь. Если у Вас остались вопросы или Вы хотите помочь в развитии данного проекта, "
                           "посетите его <a href=https://github.com/Domerk/KcttTempDB/wiki>домашнюю страницу</a> или обратитесь к администрации образовательного учреждения.<br /><br />"
                           "Разработано по заказу Центра детского (юношеского) технического творчества Московского района Санкт-Петербурга.<br /><br />"
                           "Программа основана на Qt 5.4.1 (MSVC 2010, 32 бита).<br /><br />"
                           "<strong>Разработчик:</strong><br />"
                           "<a href=https://github.com/Domerk>Domerk</a><br /><br />"
                           "<strong>Благодарности:</strong><br />"
                           "<a href=https://github.com/aksenoff>Alex Aksenoff</a>, <a href=https://github.com/aizenbit>Alex Aizenbit</a>");

    ui->licenseText->setHtml(tr("The MIT License (MIT)"
                                "<br /><br />Copyright © 2016 Domerk"
                                "<br /><br />Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the &quot;Software&quot;), to deal in the Software without restriction, including without limitation the rights"
                                "to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:"
                                "<br /><br />The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software."
                                "<br /><br />THE SOFTWARE IS PROVIDED &quot;AS IS&quot;, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS "
                                "OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,"
                                "OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE."
                                "<br /><br />Translations into Russian:"
                                "<br /><br />Copyright © 2016 Domerk"
                                "<br /><br />Данная лицензия разрешает лицам, получившим копию данного программного обеспечения и сопутствующей документации (в дальнейшем именуемыми «Программное Обеспечение»), безвозмездно использовать Программное Обеспечение без ограничений, включая "
                                "неограниченное право на использование, копирование, изменение, добавление, публикацию, распространение, сублицензирование и/или продажу копий Программного Обеспечения, а также лицам, которым предоставляется данное Программное Обеспечение, при соблюдении следующих условий:"
                                "<br /><br />Указанное выше уведомление об авторском праве и данные условия должны быть включены во все копии или значимые части данного Программного Обеспечения."
                                "<br /><br />ДАННОЕ ПРОГРАММНОЕ ОБЕСПЕЧЕНИЕ ПРЕДОСТАВЛЯЕТСЯ «КАК ЕСТЬ», БЕЗ КАКИХ-ЛИБО ГАРАНТИЙ, ЯВНО ВЫРАЖЕННЫХ ИЛИ ПОДРАЗУМЕВАЕМЫХ, ВКЛЮЧАЯ ГАРАНТИИ ТОВАРНОЙ ПРИГОДНОСТИ, СООТВЕТСТВИЯ ПО ЕГО КОНКРЕТНОМУ НАЗНАЧЕНИЮ И ОТСУТСТВИЯ НАРУШЕНИЙ, НО НЕ  ОГРАНИЧИВАЯСЬ ИМИ. "
                                "НИ В КАКОМ СЛУЧАЕ АВТОРЫ ИЛИ ПРАВООБЛАДАТЕЛИ НЕ НЕСУТ  ОТВЕТСТВЕННОСТИ ПО КАКИМ-ЛИБО ИСКАМ, ЗА УЩЕРБ ИЛИ ПО ИНЫМ ТРЕБОВАНИЯМ, В ТОМ ЧИСЛЕ, ПРИ ДЕЙСТВИИ КОНТРАКТА, ДЕЛИКТЕ ИЛИ ИНОЙ СИТУАЦИИ, ВОЗНИКШИМ ИЗ-ЗА ИСПОЛЬЗОВАНИЯ ПРОГРАММНОГО ОБЕСПЕЧЕНИЯ ИЛИ ИНЫХ ДЕЙСТВИЙ С ПРОГРАММНЫМ ОБЕСПЕЧЕНИЕМ."
                                "<br /><br /><hr /><br />Иконки панели инструментов предоставлены <a href=http://www.flaticon.com title=Flaticon>www.flaticon.com</a> "
                                "по лицензии <a href=http://creativecommons.org/licenses/by/3.0/ title=Creative Commons BY 3.0>CC BY 3.0</a>.<br />"));
}

info::~info()
{
    delete ui;
}
