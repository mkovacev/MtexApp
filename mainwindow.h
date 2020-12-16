#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <map>
#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QtXml>
#include <QDebug>
#include "firm.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace std;

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
    void onPushedButtonOverview();
    void onPushedButtonFirmOverview();
    void onPushedBackPage2();
    void onPushedBackPage3();
    void onPushedBackPage4();
    void onPushedBackPage5();


private:
    Ui::MainWindow *ui;
    QString fileName = "firme.xml";
    QFile database_;
    QDomDocument document_;

    QVector<Firm> firmsVector;
};
#endif // MAINWINDOW_H
