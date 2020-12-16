#ifndef FIRM_H
#define FIRM_H
#include <QVector>
#include <QMap>
#include <QDate>
#include <ostream>

using namespace std;

class Firm
{
public:
    Firm(const QString& firmName);

    void putInIncomeMap(QDate date, float incomeValue);
    void putInPaidMap(QDate date, float paidValue);

    float findIncomeByDate(QDate date) const;
    float findPaidByDate(QDate date) const;

    QMap<QDate, float> getIncomeMap() const;
    QMap<QDate, float> getPaidMap() const;
    QString getFirmName() const;
    void setFirmName(const QString& newFirmName);

    float calculateDifference() const;

    friend ostream& operator<<(ostream& stream, const Firm& firm);

private:
    QString firmName;
    QMap<QDate, float> incomeMap;
    QMap<QDate, float> paidMap;
};

#endif // FIRM_H
