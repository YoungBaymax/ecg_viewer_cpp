#include "ecg.h"
#include "list.h"
#include <QApplication>

QString IPADDRESS = "http://sddn.med.cmu.ac.th";
//QString IPADDRESS = "http://10.0.2.71";

int A4WIDTH = 297*4;
int A4HEIGHT = 210*4;
int VERTICALMARGIN = 2;
int HORIZONTALMARGIN = 15;
int LINESPACE = 15;
int INFOSPACE = 200;
int FONTSIZE1 = 30;
int FONTSIZE2 = 18;

int main(int argc, char *argv[])
{
    /*
    QApplication a(argc, argv);
    ECG w;
    w.showMaximized();
    w.show();

    return a.exec();
    */

    QApplication a(argc, argv);
    //List w;
    //w.showMaximized();
    //w.show();
    List *w;
    if(argc>1)
    {
        w = new List(0, argv[1]);
    }
    else
    {
        w = new List(0, "");
    }
    w->show();

    return a.exec();
}
