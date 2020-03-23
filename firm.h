#ifndef FIRM_H
#define FIRM_H
#include <QVector>
#include <QMap>
#include <QDate>

class Firm
{
public:
    Firm(const QString& firmName);

    void putInIncomeMap(QDate date, int incomeValue);
    void putInPaidMap(QDate date, int paidValue);

    int findIncomeByDate(QDate date) const;
    int findPaidByDate(QDate date) const;

    QMap<QDate, int> getIncomeMap() const;
    QMap<QDate, int> getPaidMap() const;
    QString getFirmName() const;
    void setFirmName(const QString& newFirmName);


private:
    QString firmName;
    QMap<QDate, int> incomeMap;
    QMap<QDate, int> paidMap;
};

#endif // FIRM_H
