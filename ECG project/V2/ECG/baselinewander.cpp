#include "baselinewander.h"

float baselinewander::median(QList<int> data)
{
    qSort(data.begin(),data.end());
    int middle = ((data.length())/2);
    if(data.length()%2==0)
    {
        int medianA = data.at(middle);
        int medianB = data.at(middle-1);
        float median = (medianA+medianB)/2;
        return median;
    }
    else
    {
        float median = data.at(middle+1);
        return median;
    }
}

QList<int> baselinewander::getShiftpoint(QList<int> data)
{
    QList<int> d;
    d.append(0);
    int upgrade = 1;
    int increase = 0;
    for(int i=1;i<data.length();i++)
    {
        int diff = abs(data.at(i)-data.at(i-1));
        if(diff>((256*upgrade)-30))
        {
            int dub = diff/256;
            if(diff%256>128)
            {
                dub++;
            }
            upgrade=upgrade+dub-increase;
            increase = dub;
            d.append(i);
        }
    }
    d.append(data.length());
    return d;
}

QList<int> baselinewander::getShift(QList<int> data)
{
    QList<int> d;
    d.append(0);
    int upgrade = 1;
    int increase = 0;
    bool positive = true;

    for(int i=1;i<data.length();i++)
    {
        int diff = data.at(i)-data.at(i-1);
        if(diff>((256*upgrade)-30))
        {
            d.append(upgrade*256);
            int dub = diff/256;
            if(diff%256>128)
            {
                dub++;
            }
            upgrade = upgrade + dub - increase;
            increase = dub;
            positive = true;
        }
        else if(diff<((-256*upgrade)+30))
        {
            d.append(upgrade*-256);
            int dub = -1*diff/256;
            if((-1*diff)%256>128)
            {
                dub++;
            }
            upgrade = upgrade + dub - increase;
            increase = dub;
            positive = false;
        }
        else
        {
            if(positive)
            {
                d.append((upgrade-1)*256);
            }
            else
            {
                d.append((upgrade-1)*-256);
            }
        }

    }
    return d;
}
QList<int> baselinewander::normalizeBaseline(QList<int> data)
{
    int median = this->median(data);
    for(int i=0;i<data.length();i++)
    {
        data.replace(i,data.at(i)-median);
    }
    return data;
}
