#ifndef BASELINEWANDER_H
#define BASELINEWANDER_H

#include <QList>

class baselinewander
{
public:
    float median(QList<int> data);
    QList<int> getShiftpoint(QList<int> data);
    QList<int> getShift(QList<int> data);
    QList<int> normalizeBaseline(QList<int> data);
};

#endif // BASELINEWANDER_H
