#include "firm.h"
#include <iostream>

Firm::Firm(const QString& firmName)
    :firmName(firmName)
{
    std::cout<< "Created " << firmName.toStdString() << " firm" <<std::endl;
}

void Firm::putInIncomeMap(QDate date, float incomeValue)
{
    incomeMap[date] = incomeValue;
}


void Firm::putInPaidMap(QDate date, float paidValue)
{
    paidMap[date] = paidValue;
}

float Firm::findIncomeByDate(QDate date) const
{
    return incomeMap[date];
}

float Firm::findPaidByDate(QDate date) const
{
    return paidMap[date];
}

QMap<QDate, float> Firm::getIncomeMap() const
{
    return incomeMap;
}

QMap<QDate, float> Firm::getPaidMap() const
{
    return paidMap;
}

QString Firm::getFirmName() const
{
    return firmName;
}

void Firm::setFirmName(const QString& newFirmName)
{
    firmName = newFirmName;
}

float Firm::calculateDifference() const
{
    float incomeSum = 0.0;
    float paidSum = 0.0;

    QList<QDate> dates = incomeMap.keys();
    for(auto& date : dates)
    {
        incomeSum += incomeMap.value(date);
    }

    dates = paidMap.keys();
    for(auto& date : dates)
    {
        paidSum += paidMap.value(date);
    }

    return incomeSum - paidSum;
}

ostream& operator<<(ostream& stream, const Firm& firm)
{
    stream << firm.firmName << endl;
    stream << "Income" << endl;
    QList<QDate> dates = firm.incomeMap.keys();
    for(QDate& d : dates)
    {
        stream << d.toString()
               << ": "
               << firm.incomeMap.value(d)
               << endl;
    }

    stream << "Paid" << endl;
    dates = firm.paidMap.keys();
    for(QDate& d : dates)
    {
        stream << d.toString()
               << ": "
               << firm.paidMap.value(d)
               << endl;
    }

    return stream;
}
