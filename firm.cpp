#include "firm.h"
#include <iostream>

Firm::Firm(const QString& firmName)
    :firmName(firmName)
{
    std::cout<< "Created " << firmName.toStdString() << " firm" <<std::endl;
}

void Firm::putInIncomeMap(QDate date, int incomeValue)
{
    incomeMap[date] = incomeValue;
}


void Firm::putInPaidMap(QDate date, int paidValue)
{
    paidMap[date] = paidValue;
}

int Firm::findIncomeByDate(QDate date) const
{
    return incomeMap[date];
}

int Firm::findPaidByDate(QDate date) const
{
    return paidMap[date];
}

QMap<QDate, int> Firm::getIncomeMap() const
{
    return incomeMap;
}

QMap<QDate, int> Firm::getPaidMap() const
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
