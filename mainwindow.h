#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include "firm.h"
#include <iostream>
#include <map>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QtXml>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void readFromDatabaseFile();

public slots:
    void onPushedAddFirm();
    void onPushedAddIncome();
    void onPushedAddPayed();
    void onPushedAddFirmToDB();
    void onPushedImportIncome();
    void onPushedImportPayed();


private:
    Ui::MainWindow *ui;
    QString fileName = "C:\\Users\\matij\\OneDrive\\Documents\\GitHub\\MtexApp\\firme.xml";
    QFile database_;
    QDomDocument document_;

    QVector<Firm> firmsVector;
};
#endif // MAINWINDOW_H
