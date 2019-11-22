#ifndef ECG_H
#define ECG_H

#include <QMainWindow>
#include <QGraphicsTextItem>

#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QTextStream>
#include <QScrollBar>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QtDebug>

#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QImageReader>
#include <QByteArray>

#include <QDomDocument>
#include <QDateTime>
#include <QDateEdit>
#include <QFontMetrics>
#include <QGraphicsSvgItem>
#include <QMap>
#include <QList>
#include <QtCore>

#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintDialog>
#include <QFileDialog>


/*
static int A4WIDTH = 297*4;
static int A4HEIGHT = 210*4;
static int VERTICALMARGIN = 5;
static int HORIZONTALMARGIN = 20;
static int LINESPACE = 15;
static int INFOSPACE = 200;
static int FONTSIZE1 = 30;
*/
extern QString IPADDRESS;
extern int A4WIDTH;
extern int A4HEIGHT;
extern int VERTICALMARGIN;
extern int HORIZONTALMARGIN;
extern int LINESPACE;
extern int INFOSPACE;
extern int FONTSIZE1;
extern int FONTSIZE2;

namespace Ui {
class ECG;
//static int A4WIDTH = 297*4;
//static int A4HEIGHT = 210*4;
}

class ECG : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit ECG(QWidget *parent = 0, QString hn = "", QString ecgid = "");
    ~ECG();

private slots:
    void on_horizontalSlider_valueChanged(int value);
    void downloadFinished(QNetworkReply *reply);
    void downloadECGFinished(QNetworkReply *reply);
    //void slotClicked();
    //void onRequestComplete();

    //void finishedSlot(QNetworkReply* reply);

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::ECG *ui;
    //QGraphicsLineItem *line;
    //QNetworkAccessManager *networkManager;
    //QNetworkAccessManager::QNetworkAccessManager(QObject *parent);
    //QNetworkAccessManager* nam;
    QGraphicsScene *scene;
    QNetworkAccessManager manager;
    QNetworkAccessManager manager2;
    QMap<int,QByteArray> map;
    //QNetworkReply* reply;
    //QList<QNetworkReply *> currentDownloads;

public:
    void drawGrid();
    void addECGvalues();
    void getPatientdata();
    void parseXML(QString xml);
    void getECG();
    QString getType(QString name);
    void parsePhillips(QDomDocument doc);
    void parseE4L(QDomDocument doc);
    void parseGE(QDomDocument doc);
    void parseECG(QString xml);
    int read(QByteArray chunk, int maxCode);
    QByteArray readInternal(QByteArray chunk, int maxCode);
    int readCodeword(QByteArray chunk);
    QList<short> unpack(QByteArray bytes);
    QList<int> decodedeltas(QList<short> input, short initvalue);
    void drawLead(bool newlead);
    void reconstituteLeads();
    QString getQTCB(QString qt, QString rr);
    QString getQTCF(QString qt, QString rr);
    void setTextproperties1(QGraphicsTextItem *text, QFont font, QString string, int x, int y);
};

#endif // ECG_H
