#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , database_("/home/matija/MtexApp/firme.xml")
    , document_("/home/matija/MtexApp/firme")
{
    //initialize UI
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    readFromDatabaseFile();
    //connect signals and slots
    QWidget::connect(ui->addFirmButton, &QPushButton::clicked, this, &MainWindow::onPushedAddFirm);
    QWidget::connect(ui->addIncomeButton, &QPushButton::clicked, this, &MainWindow::onPushedAddIncome);
    QWidget::connect(ui->addPayedButton, &QPushButton::clicked, this, &MainWindow::onPushedAddPayed);
    QWidget::connect(ui->addFrimToDb, &QPushButton::clicked, this, &MainWindow::onPushedAddFirmToDB);
    QWidget::connect(ui->importIncomeButton, &QPushButton::clicked, this, &MainWindow::onPushedImportIncome);
    QWidget::connect(ui->importPayedButton, &QPushButton::clicked, this, &MainWindow::onPushedImportPayed);
    QWidget::connect(ui->overviewButton, &QPushButton::clicked, this, &MainWindow::onPushedButtonOverview);
    QWidget::connect(ui->showFirmOverviewButton, &QPushButton::clicked, this, &MainWindow::onPushedButtonFirmOverview);
    QWidget::connect(ui->backButtonPage2, &QPushButton::clicked, this, &MainWindow::onPushedBackPage2);
    QWidget::connect(ui->backButtonPage3, &QPushButton::clicked, this, &MainWindow::onPushedBackPage3);
    QWidget::connect(ui->backButtonPage4, &QPushButton::clicked, this, &MainWindow::onPushedBackPage4);
    QWidget::connect(ui->backButtonPage5, &QPushButton::clicked, this, &MainWindow::onPushedBackPage5);
    //open database for reading and writting
//    if(!database_.open())
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::readFromDatabaseFile()
{
    if(!database_.open(QIODevice::ReadWrite))
    {
        std::cerr << "Unable to create empty file firme.xml\n =>" << strerror(errno) << "\n" << std::endl;
        database_.close();
        exit(1);
    }else{

        if(!document_.setContent(&database_))
            qDebug() << "Unable to initalize document array." << strerror(errno);

        QDomElement root = document_.documentElement();

        QDomElement component = root.firstChildElement("Firm");

        while(!component.isNull())
        {

            QString firmName =  component.attribute("Name", "");
            Firm pomFirm(firmName);

            QDomElement firmState = component.firstChildElement();

            QString date;
            float money;

            while(!firmState.isNull() && (firmState.tagName() != "Firm"))
            {
                if(firmState.tagName() == "Income")
                {
                    date = firmState.attribute("Date", "01.01.1000");
                    money = firmState.text().toFloat();
                    pomFirm.putInIncomeMap(QDate::fromString(date, "dd.MM.yyyy"), money);

                }else if(firmState.tagName() == "Payed")
                {
                    date = firmState.attribute("Date", "01.01.1000");
                    money = firmState.text().toFloat();
                    pomFirm.putInPaidMap(QDate::fromString(date, "dd.MM.yyyy"), money);
                }
                firmState = firmState.nextSibling().toElement();
            }
            firmsVector.append(pomFirm);

            component = component.nextSibling().toElement();
        }

        for (int i = 0; i < firmsVector.size(); ++i) {
            qDebug() << firmsVector[i].getFirmName();
        }

        database_.close();
    }
}

void MainWindow::onPushedAddFirm()
{
    ui->stackedWidget->setCurrentIndex(1);
    ui->firmLineEdit->clear();
}

void MainWindow::onPushedAddIncome()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->listWidgetIncome->clear();
    for(auto i : firmsVector)
    {
        ui->listWidgetIncome->addItem(i.getFirmName());
    }

}

void MainWindow::onPushedAddPayed()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->listWidgetPayed->clear();
    for(auto i : firmsVector)
    {
        ui->listWidgetPayed->addItem(i.getFirmName());
    }
}

void MainWindow::onPushedButtonOverview()
{
    ui->stackedWidget->setCurrentIndex(4);
    ui->overviewListFirms->clear();
    ui->overviewListPayed->clear();
    ui->overviewListIncome->clear();
    ui->diffLineEdit->clear();
    for(auto i : firmsVector)
    {
        ui->overviewListFirms->addItem(i.getFirmName());
    }
}

void MainWindow::onPushedAddFirmToDB()
{
    //dodavanje firme u lokalnu promenjivu koja sadrzi baze
    QString firmName = ui->firmLineEdit->text();

    Firm pomFirm(firmName);

    firmsVector.append(pomFirm);

    QDomElement firm = document_.createElement("Firm");
    firm.setAttribute("Name", firmName);
    document_.elementsByTagName("Firms").at(0).appendChild(firm);

    //upisivanje u database fajl
    if(database_.open(QFile::Text | QFile::ReadWrite | QFile::Truncate))
    {
        QTextStream stream(&database_);
        stream << document_.toString();
    }else{
        std::cerr << "Unable to open file for importing new firm" <<std::endl;
        exit(1);
    }
    database_.close();
    QMessageBox msgBox;
    msgBox.setText("Firma uneta u bazu");
    msgBox.exec();
    ui->firmLineEdit->setText("");
}

void MainWindow::onPushedImportIncome()
{
    //kupljenje podataka iz forme
    QDate selectedDate = ui->calendarIncome->selectedDate();
    QString text = ui->incomeLine->text();
    float income = text.toFloat();

    auto selected = ui->listWidgetIncome->selectedItems();
    QString selectedFirmName = selected[0]->text();

    for(auto i : firmsVector){
        if(i.getFirmName() == selectedFirmName){
            i.putInIncomeMap(selectedDate, income);
        }
    }

    QDomElement root = document_.documentElement();
    auto components = root.elementsByTagName("Firm");

    auto componentsLength = components.length();

    //dodavanje u promenjjivu
    for(auto i = 0; i < componentsLength; i++)
    {
        auto node = components.at(i);
        auto attributes = node.attributes();
        if(!attributes.isEmpty())
        {
            if(attributes.item(0).nodeValue() == selectedFirmName)
            {
                QDomElement newIncome = document_.createElement("Income");
                newIncome.setAttribute("Date", selectedDate.toString("dd.MM.yyyy"));
                QDomText newIncomeValue = document_.createTextNode(text);
                newIncome.appendChild(newIncomeValue);
                node.appendChild(newIncome);
            }
        }
    }

    //dodavanje u database fajl
    if(!database_.open(QFile::Text | QFile::ReadWrite | QFile::Truncate))
    {
        std::cerr << "Error opening file for writting income" <<std::endl;
    }else{
        QTextStream stream(&database_);
        stream << document_.toString();
    }
    database_.close();
    QMessageBox msgBox;
    msgBox.setText("Podaci o prodaji uneti u bazu");
    msgBox.exec();
    ui->incomeLine->setText("");
}

void MainWindow::onPushedImportPayed()
{
    //kupljenje podataka iz forme
    QDate selectedDate = ui->calendarPayed->selectedDate();
    QString text = ui->payedLine->text();
    float payed = text.toFloat();

    auto selected = ui->listWidgetPayed->selectedItems();
    QString selectedFirmName = selected[0]->text();

    for(auto i : firmsVector){
        if(i.getFirmName() == selectedFirmName){
            i.putInPaidMap(selectedDate, payed);
        }
    }

    QDomElement root = document_.documentElement();
    auto components = root.elementsByTagName("Firm");

    auto componentsLength = components.length();

    for(auto i = 0; i < componentsLength; i++)
    {
        auto node = components.at(i);
        auto attributes = node.attributes();
        if(!attributes.isEmpty())
        {
            if(attributes.item(0).nodeValue() == selectedFirmName)
            {
                QDomElement newPayed = document_.createElement("Payed");
                newPayed.setAttribute("Date", selectedDate.toString("dd.MM.yyyy"));
                QDomText newPayedValue = document_.createTextNode(text);
                newPayed.appendChild(newPayedValue);
                node.appendChild(newPayed);
            }
        }
    }

    if(!database_.open(QFile::Text | QFile::ReadWrite | QFile::Truncate))
    {
        std::cerr << "Error opening file for writting payed" <<std::endl;
    }else{
        QTextStream stream(&database_);
        stream << document_.toString();
    }
    database_.close();
    QMessageBox msgBox;
    msgBox.setText("Podaci o placanju uneti u bazu");
    msgBox.exec();
    ui->payedLine->setText("");
}

void MainWindow::onPushedButtonFirmOverview()
{
    auto selected = ui->overviewListFirms->selectedItems();
    QString selectedFirmName = selected[0]->text();

    ui->overviewListPayed->clear();
    ui->overviewListIncome->clear();

    for(Firm& firm : firmsVector)
    {
        if(firm.getFirmName() == selectedFirmName)
        {
            QList<QDate> dates = firm.getIncomeMap().keys();
            for(QDate& date : dates)
            {
                ui->overviewListIncome->addItem(date.toString() + ": " + QString::number(firm.getIncomeMap().value(date)));
            }

            dates = firm.getPaidMap().keys();
            for(QDate& date : dates)
            {
                ui->overviewListPayed->addItem(date.toString() + ": " + QString::number(firm.getPaidMap().value(date)));
            }
            ui->diffLineEdit->setText(QString::number(firm.calculateDifference()));
        }

    }


}

void MainWindow::onPushedBackPage2()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::onPushedBackPage3()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::onPushedBackPage4()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::onPushedBackPage5()
{
    ui->stackedWidget->setCurrentIndex(0);
}
