#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , database_(fileName)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    readFromDatabaseFile();
    QWidget::connect(ui->addFirmButton, &QPushButton::clicked, this, &MainWindow::onPushedAddFirm);
    QWidget::connect(ui->addIncomeButton, &QPushButton::clicked, this, &MainWindow::onPushedAddIncome);
    QWidget::connect(ui->addPayedButton, &QPushButton::clicked, this, &MainWindow::onPushedAddPayed);
    QWidget::connect(ui->addFrimToDb, &QPushButton::clicked, this, &MainWindow::onPushedAddFirmToDB);
    QWidget::connect(ui->importIncomeButton, &QPushButton::clicked, this, &MainWindow::onPushedImportIncome);
    QWidget::connect(ui->importPayedButton, &QPushButton::clicked, this, &MainWindow::onPushedImportPayed);
}

MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::readFromDatabaseFile()
{
    if(!database_.open(QFile::Text | QFile::ReadWrite))
    {
        std::cerr << "Unable to create empty file firme.xml\n =>" << strerror(errno) << "\n" << std::endl;
        database_.close();
        exit(1);
    }else{
        document_.setContent(&database_);

        QDomElement root = document_.documentElement();

        QDomElement component = root.firstChildElement("Firm");

        while(!component.isNull())
        {

            QString firmName =  component.attribute("Name", "");
            Firm pomFirm(firmName);

            QDomElement firmState = component.firstChildElement();

            QString date;
            int money;

            while(!firmState.isNull() && (firmState.tagName() != "Firm"))
            {
                if(firmState.tagName() == "Income")
                {
                    date = firmState.attribute("Date", "01.01.1000");
                    money = firmState.text().toInt();
                    pomFirm.putInIncomeMap(QDate::fromString(date, "dd.MM.yyyy"), money);

                }else if(firmState.tagName() == "Payed")
                {
                    date = firmState.attribute("Date", "01.01.1000");
                    money = firmState.text().toInt();
                    pomFirm.putInPaidMap(QDate::fromString(date, "dd.MM.yyyy"), money);
                }
                firmState = firmState.nextSibling().toElement();
            }
            firmsVector.append(pomFirm);

            component = component.nextSibling().toElement();
        }

        database_.close();
    }
}

void MainWindow::onPushedAddFirm()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::onPushedAddIncome()
{
    ui->stackedWidget->setCurrentIndex(2);
    for(auto i : firmsVector)
    {
        ui->listWidgetIncome->addItem(i.getFirmName());
    }

}

void MainWindow::onPushedAddPayed()
{
    ui->stackedWidget->setCurrentIndex(3);
    for(auto i : firmsVector)
    {
        ui->listWidgetIncome->addItem(i.getFirmName());
    }
}

void MainWindow::onPushedAddFirmToDB()
{
    ui->stackedWidget->setCurrentIndex(0);
    QString firmName = ui->firmLineEdit->text();

    Firm pomFirm(firmName);

    firmsVector.append(pomFirm);

    QDomElement firm = document_.createElement("Firm");
    firm.setAttribute("Name", firmName);
    document_.elementsByTagName("Firms").at(0).appendChild(firm);

    if(database_.open(QFile::Text | QFile::ReadWrite | QFile::Truncate))
    {
        QTextStream stream(&database_);
        stream << document_.toString();
    }else{
        std::cerr << "Unable to open file for importing new firm" <<std::endl;
        exit(1);
    }
    database_.close();


}

void MainWindow::onPushedImportIncome()
{
    ui->stackedWidget->setCurrentIndex(0);

    QDate selectedDate = ui->calendarIncome->selectedDate();
    QString text = ui->incomeLine->text();
    int income = text.toInt();

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

    if(!database_.open(QFile::Text | QFile::ReadWrite | QFile::Truncate))
    {
        std::cerr << "Error opening file for writting income" <<std::endl;
    }else{
        QTextStream stream(&database_);
        stream << document_.toString();
    }
    database_.close();
}

void MainWindow::onPushedImportPayed()
{
    ui->stackedWidget->setCurrentIndex(0);

    QDate selectedDate = ui->calendarPayed->selectedDate();
    QString text = ui->payedLine->text();
    int payed = text.toInt();

    auto selected = ui->listWidgetIncome->selectedItems();
    QString selectedFirmName = selected[0]->text();

    for(auto i : firmsVector){
        if(i.getFirmName() == selectedFirmName){
            i.putInIncomeMap(selectedDate, payed);
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
                QDomElement newIncome = document_.createElement("Payed");
                newIncome.setAttribute("Date", selectedDate.toString("dd.MM.yyyy"));
                QDomText newIncomeValue = document_.createTextNode(text);
                newIncome.appendChild(newIncomeValue);
                node.appendChild(newIncome);
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
}
