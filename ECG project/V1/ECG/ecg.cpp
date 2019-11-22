#include "ecg.h"
#include "ui_ecg.h"

QString ECGID;

//Patient Information
QString NAME = "";
QString SURNAME = "";
QString GENDER = "";
QString AGEYEAR = "";
QString AGEMONTH = "";
QDateTime BIRTH;
QString RECORDDATE = "";
QString HN = "";

//ECG type
QString ECGTYPE;
QString PHILLIPS = "phillips";
QString GE = "ge";
QString E4L = "e4l";


//ECG parameters
QString RATE = "";
QString RR = "";
QString PR = "";
QString QRSdur = "";
QString QT = "";
QString QTCB = "";
QString QTCF = "";
QString Paxis = "";
QString QRSaxis = "";
QString Taxis = "";

QList<QString> INTERPRETATION;
QList<QString> FINDING;

QString SEVERITY = "";
QString DIAGNOSIS = "";

QString PAPERSPEED = "";
QString AMPLIFICATION = "";

QList<int> leadpositionx;
QList<int> leadpositiony;
QList<QList<int> > leads;

int id;
int bitCount;
uint buffer;
QByteArray previous;
int nextcode = 256;
QByteArray current;
int position = -1;

ECG::ECG(QWidget *parent, QString hn, QString ecgid) : QMainWindow(parent),

    ui(new Ui::ECG)
{
    ui->setupUi(this);

    HN = hn;
    ECGID = ecgid;

    leadpositionx.append(0);
    leadpositionx.append(A4WIDTH*(1%3)/3);
    leadpositionx.append(A4WIDTH*(2%3)/3);
    leadpositionx.append(0);
    leadpositionx.append(A4WIDTH*(1%3)/3);
    leadpositionx.append(A4WIDTH*(2%3)/3);
    leadpositionx.append(0);
    leadpositionx.append(A4WIDTH*(1%3)/3);
    leadpositionx.append(A4WIDTH*(2%3)/3);
    leadpositionx.append(0);
    leadpositionx.append(A4WIDTH*(1%3)/3);
    leadpositionx.append(A4WIDTH*(2%3)/3);

    leadpositiony.append(INFOSPACE);
    leadpositiony.append(INFOSPACE);
    leadpositiony.append(INFOSPACE);
    leadpositiony.append(INFOSPACE+((A4HEIGHT-INFOSPACE)/5));
    leadpositiony.append(INFOSPACE+((A4HEIGHT-INFOSPACE)/5));
    leadpositiony.append(INFOSPACE+((A4HEIGHT-INFOSPACE)/5));
    leadpositiony.append(INFOSPACE+((A4HEIGHT-INFOSPACE)*2/5));
    leadpositiony.append(INFOSPACE+((A4HEIGHT-INFOSPACE)*2/5));
    leadpositiony.append(INFOSPACE+((A4HEIGHT-INFOSPACE)*2/5));
    leadpositiony.append(INFOSPACE+((A4HEIGHT-INFOSPACE)*3/5));
    leadpositiony.append(INFOSPACE+((A4HEIGHT-INFOSPACE)*3/5));
    leadpositiony.append(INFOSPACE+((A4HEIGHT-INFOSPACE)*3/5));



    scene = new QGraphicsScene;
    ui->graphicsView->setRenderHints(QPainter::Antialiasing);
    QApplication::setOverrideCursor(Qt::WaitCursor);


    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->graphicsView->verticalScrollBar()->setValue(ui->graphicsView->verticalScrollBar()->minimum());

    //ECG parameters
    RATE = "";
    RR = "";
    PR = "";
    QRSdur = "";
    QT = "";
    QTCB = "";
    QTCF = "";
    Paxis = "";
    QRSaxis = "";
    Taxis = "";

    INTERPRETATION.clear();
    FINDING.clear();

    SEVERITY = "";
    DIAGNOSIS = "";

    PAPERSPEED = "";
    AMPLIFICATION = "";

    leads.clear();


    getPatientdata();
    getECG();

}

ECG::~ECG()
{
    delete ui;
}

void ECG::drawGrid()
{
    int n = 0;
    for (int i=0;i<A4WIDTH;i=i+4)
    {
        if(n == 9)
        {
            scene->addLine(i,INFOSPACE,i,A4HEIGHT,QPen(QBrush(Qt::red),1));
            n=0;
        }
        else if(n == 4)
        {
            scene->addLine(i,INFOSPACE,i,A4HEIGHT,QPen(QBrush(Qt::red),0.3));
            n++;
        }
        else
        {
            scene->addLine(i,INFOSPACE,i,A4HEIGHT,QPen(QBrush(Qt::red),0.15));
            n++;
        }
    }

    n = 0;
    for (int i=INFOSPACE;i<A4HEIGHT;i=i+4)
    {
        if(n == 9)
        {
            scene->addLine(0,i,A4WIDTH,i,QPen(QBrush(Qt::red),1));
            n=0;
        }
        else if(n == 4)
        {
            scene->addLine(0,i,A4WIDTH,i,QPen(QBrush(Qt::red),0.3));
            n++;
        }
        else
        {
            scene->addLine(0,i,A4WIDTH,i,QPen(QBrush(Qt::red),0.15));
            n++;
        }
    }

    //line = scene->addLine(0,40,Ui::A4WIDTH,40,QPen(QBrush(Qt::red),2));
    //line->setFlag(QGraphicsItem::ItemIsMovable);

    scene->addLine(0,40,A4WIDTH,40,QPen(QBrush(Qt::red),2));
    scene->addLine(116,40,116,INFOSPACE,QPen(QBrush(Qt::red),1));
    scene->addLine(0,INFOSPACE,A4WIDTH,INFOSPACE,QPen(QBrush(Qt::red),2));

    scene->addLine(0,0,A4WIDTH,0,QPen(QBrush(Qt::black),2));
    scene->addLine(0,0,0,A4HEIGHT,QPen(QBrush(Qt::black),2));
    scene->addLine(A4WIDTH,0,A4WIDTH,A4HEIGHT,QPen(QBrush(Qt::black),2));
    scene->addLine(0,A4HEIGHT,A4WIDTH,A4HEIGHT,QPen(QBrush(Qt::black),2));

    QFont font;
    font.setPixelSize(FONTSIZE2);
    font.setBold(true);
    font.setFamily("TH Sarabun New");

    QGraphicsTextItem *parameter1 = new QGraphicsTextItem;
    setTextproperties1(parameter1, font, "Rate",HORIZONTALMARGIN,35);
    scene->addItem(parameter1);

    QGraphicsTextItem *parameter2 = new QGraphicsTextItem;
    setTextproperties1(parameter2, font, "RR",HORIZONTALMARGIN,35+(LINESPACE*1));
    scene->addItem(parameter2);

    QGraphicsTextItem *parameter3 = new QGraphicsTextItem;
    setTextproperties1(parameter3, font, "PR",HORIZONTALMARGIN,35+(LINESPACE*2));
    scene->addItem(parameter3);

    QGraphicsTextItem *parameter4 = new QGraphicsTextItem;
    setTextproperties1(parameter4, font, "QRS dur",HORIZONTALMARGIN,35+(LINESPACE*3));
    scene->addItem(parameter4);

    QGraphicsTextItem *parameter5 = new QGraphicsTextItem;
    setTextproperties1(parameter5, font, "QT",HORIZONTALMARGIN,35+(LINESPACE*4));
    scene->addItem(parameter5);

    QGraphicsTextItem *parameter6 = new QGraphicsTextItem;
    setTextproperties1(parameter6, font, "QTCB",HORIZONTALMARGIN,35+(LINESPACE*5));
    scene->addItem(parameter6);

    QGraphicsTextItem *parameter7 = new QGraphicsTextItem;
    setTextproperties1(parameter7, font, "QTCF",HORIZONTALMARGIN,35+(LINESPACE*6));
    scene->addItem(parameter7);

    QGraphicsTextItem *parameter8 = new QGraphicsTextItem;
    setTextproperties1(parameter8, font, "P axis",HORIZONTALMARGIN,35+(LINESPACE*7));
    scene->addItem(parameter8);

    QGraphicsTextItem *parameter9 = new QGraphicsTextItem;
    setTextproperties1(parameter9, font, "QRS axis",HORIZONTALMARGIN,35+(LINESPACE*8));
    scene->addItem(parameter9);

    QGraphicsTextItem *parameter10 = new QGraphicsTextItem;
    setTextproperties1(parameter10, font, "T axis",HORIZONTALMARGIN,35+(LINESPACE*9));
    scene->addItem(parameter10);

    QGraphicsTextItem *subtopic1 = new QGraphicsTextItem;
    setTextproperties1(subtopic1, font, "Interpretation",130,35);
    scene->addItem(subtopic1);

    QGraphicsTextItem *subtopic2 = new QGraphicsTextItem;
    setTextproperties1(subtopic2, font, "Finding",A4WIDTH/2,35);
    scene->addItem(subtopic2);

    QGraphicsTextItem *subtopic3 = new QGraphicsTextItem;
    setTextproperties1(subtopic3, font, "Diagnosis:",130,35+(LINESPACE*9));
    scene->addItem(subtopic3);

    QGraphicsTextItem *subtopic4 = new QGraphicsTextItem;
    setTextproperties1(subtopic4, font, "Approved by:",A4WIDTH/2,35+(LINESPACE*9));
    scene->addItem(subtopic4);

    QGraphicsTextItem *subtopic5 = new QGraphicsTextItem;
    setTextproperties1(subtopic5, font, "Date:",A4WIDTH-154-HORIZONTALMARGIN,35+(LINESPACE*9));
    scene->addItem(subtopic5);

    font.setBold(false);

    QString limbampi = "";
    if(ui->comboBox->currentText()=="0.5")
    {
        limbampi = " X 0.5";
    }
    else if(ui->comboBox->currentText()=="2.0")
    {
        limbampi = " X 2.0";
    }

    QString chestampi = "";
    if(ui->comboBox_2->currentText()=="0.5")
    {
        chestampi = " X 0.5";
    }
    else if(ui->comboBox_2->currentText()=="2.0")
    {
        chestampi = " X 2.0";
    }

    QGraphicsTextItem *lead1 = new QGraphicsTextItem;
    setTextproperties1(lead1, font, "I"+limbampi, HORIZONTALMARGIN,INFOSPACE);
    scene->addItem(lead1);

    QGraphicsTextItem *lead2 = new QGraphicsTextItem;
    setTextproperties1(lead2, font, "II"+limbampi, HORIZONTALMARGIN+(A4WIDTH/3),INFOSPACE);
    scene->addItem(lead2);

    QGraphicsTextItem *lead3 = new QGraphicsTextItem;
    setTextproperties1(lead3, font, "III"+limbampi, HORIZONTALMARGIN+(A4WIDTH*2/3),INFOSPACE);
    scene->addItem(lead3);

    QGraphicsTextItem *lead4 = new QGraphicsTextItem;
    setTextproperties1(lead4, font, "aVR"+limbampi, HORIZONTALMARGIN,INFOSPACE+((A4HEIGHT-INFOSPACE)/5));
    scene->addItem(lead4);

    QGraphicsTextItem *lead5 = new QGraphicsTextItem;
    setTextproperties1(lead5, font, "aVL"+limbampi, HORIZONTALMARGIN+(A4WIDTH/3),INFOSPACE+((A4HEIGHT-INFOSPACE)/5));
    scene->addItem(lead5);

    QGraphicsTextItem *lead6 = new QGraphicsTextItem;
    setTextproperties1(lead6, font, "aVF"+limbampi, HORIZONTALMARGIN+(A4WIDTH*2/3),INFOSPACE+((A4HEIGHT-INFOSPACE)/5));
    scene->addItem(lead6);

    QGraphicsTextItem *lead7 = new QGraphicsTextItem;
    setTextproperties1(lead7, font, "V1"+chestampi, HORIZONTALMARGIN,INFOSPACE+((A4HEIGHT-INFOSPACE)*2/5));
    scene->addItem(lead7);

    QGraphicsTextItem *lead8 = new QGraphicsTextItem;
    setTextproperties1(lead8, font, "V2"+chestampi, HORIZONTALMARGIN+(A4WIDTH/3),INFOSPACE+((A4HEIGHT-INFOSPACE)*2/5));
    scene->addItem(lead8);

    QGraphicsTextItem *lead9 = new QGraphicsTextItem;
    setTextproperties1(lead9, font, "V3"+chestampi, HORIZONTALMARGIN+(A4WIDTH*2/3),INFOSPACE+((A4HEIGHT-INFOSPACE)*2/5));
    scene->addItem(lead9);

    QGraphicsTextItem *lead10 = new QGraphicsTextItem;
    setTextproperties1(lead10, font, "V4"+chestampi, HORIZONTALMARGIN,INFOSPACE+((A4HEIGHT-INFOSPACE)*3/5));
    scene->addItem(lead10);

    QGraphicsTextItem *lead11 = new QGraphicsTextItem;
    setTextproperties1(lead11, font, "V5"+chestampi, HORIZONTALMARGIN+(A4WIDTH/3),INFOSPACE+((A4HEIGHT-INFOSPACE)*3/5));
    scene->addItem(lead11);

    QGraphicsTextItem *lead12 = new QGraphicsTextItem;
    setTextproperties1(lead12, font, "V6"+chestampi, HORIZONTALMARGIN+(A4WIDTH*2/3),INFOSPACE+((A4HEIGHT-INFOSPACE)*3/5));
    scene->addItem(lead12);

    QGraphicsTextItem *lead2l = new QGraphicsTextItem;
    setTextproperties1(lead2l, font, "II", HORIZONTALMARGIN,INFOSPACE+((A4HEIGHT-INFOSPACE)*4/5));
    scene->addItem(lead2l);

    QGraphicsTextItem *paperspeed = new QGraphicsTextItem;
    setTextproperties1(paperspeed, font, "Paper speed:", HORIZONTALMARGIN,A4HEIGHT-40);
    scene->addItem(paperspeed);

    QGraphicsTextItem *amplification = new QGraphicsTextItem;
    setTextproperties1(amplification, font, "Amplification:", HORIZONTALMARGIN+200,A4HEIGHT-40);
    scene->addItem(amplification);

    QGraphicsTextItem *hospitalname = new QGraphicsTextItem;
    setTextproperties1(hospitalname, font, "โรงพยาบาลมหาราชนครเชียงใหม่ คณะแพทยศาสตร์ มหาวิทยาลัยเชียงใหม่", HORIZONTALMARGIN+750,A4HEIGHT-40);
    scene->addItem(hospitalname);

    QGraphicsSvgItem *logo = new QGraphicsSvgItem(QLatin1String(":/icon/icon/medlogo.svg"));
    scene->addItem(logo);
    logo->setPos(A4WIDTH-HORIZONTALMARGIN-30,A4HEIGHT-40);
    logo->setScale(0.02);
}

void ECG::addECGvalues()
{
    QFont font;
    font.setPixelSize(FONTSIZE1);
    font.setBold(true);
    font.setFamily("TH Sarabun New");

    QGraphicsTextItem *text1 = new QGraphicsTextItem;
    setTextproperties1(text1 , font, NAME+" "+SURNAME+" เพศ "+GENDER+" "+AGEYEAR+" ปี "+AGEMONTH+" เดือน", HORIZONTALMARGIN, VERTICALMARGIN);
    scene->addItem(text1);

    QGraphicsTextItem *text2 = new QGraphicsTextItem;
    setTextproperties1(text2, font, "วันที่บันทึก: "+RECORDDATE,A4WIDTH/2,VERTICALMARGIN);
    scene->addItem(text2);

    QGraphicsTextItem *text3 = new QGraphicsTextItem;
    setTextproperties1(text3, font, "HN "+HN,A4WIDTH-154-HORIZONTALMARGIN,VERTICALMARGIN);
    scene->addItem(text3);

    font.setPixelSize(FONTSIZE2);
    font.setBold(false);
    font.setFamily("TH Sarabun New");

    QGraphicsTextItem *parameter1 = new QGraphicsTextItem;
    setTextproperties1(parameter1, font, RATE,HORIZONTALMARGIN+60,35);
    scene->addItem(parameter1);

    QGraphicsTextItem *parameter2 = new QGraphicsTextItem;
    setTextproperties1(parameter2, font, RR,HORIZONTALMARGIN+60,35+(LINESPACE*1));
    scene->addItem(parameter2);

    QGraphicsTextItem *parameter3 = new QGraphicsTextItem;
    setTextproperties1(parameter3, font, PR,HORIZONTALMARGIN+60,35+(LINESPACE*2));
    scene->addItem(parameter3);

    QGraphicsTextItem *parameter4 = new QGraphicsTextItem;
    setTextproperties1(parameter4, font, QRSdur,HORIZONTALMARGIN+60,35+(LINESPACE*3));
    scene->addItem(parameter4);

    QGraphicsTextItem *parameter5 = new QGraphicsTextItem;
    setTextproperties1(parameter5, font, QT,HORIZONTALMARGIN+60,35+(LINESPACE*4));
    scene->addItem(parameter5);

    QGraphicsTextItem *parameter6 = new QGraphicsTextItem;
    setTextproperties1(parameter6, font, QTCB,HORIZONTALMARGIN+60,35+(LINESPACE*5));
    scene->addItem(parameter6);

    QGraphicsTextItem *parameter7 = new QGraphicsTextItem;
    setTextproperties1(parameter7, font, QTCF,HORIZONTALMARGIN+60,35+(LINESPACE*6));
    scene->addItem(parameter7);

    QGraphicsTextItem *parameter8 = new QGraphicsTextItem;
    setTextproperties1(parameter8, font, Paxis,HORIZONTALMARGIN+60,35+(LINESPACE*7));
    scene->addItem(parameter8);

    QGraphicsTextItem *parameter9 = new QGraphicsTextItem;
    setTextproperties1(parameter9, font, QRSaxis,HORIZONTALMARGIN+60,35+(LINESPACE*8));
    scene->addItem(parameter9);

    QGraphicsTextItem *parameter10 = new QGraphicsTextItem;
    setTextproperties1(parameter10, font, Taxis,HORIZONTALMARGIN+60,35+(LINESPACE*9));
    scene->addItem(parameter10);

    for(int i=0;i<INTERPRETATION.size();i++)
    {
        if(INTERPRETATION.size()<8)
        {
            QGraphicsTextItem *inter = new QGraphicsTextItem;
            setTextproperties1(inter, font, INTERPRETATION.at(i),130,35+(LINESPACE*(i+1)));
            scene->addItem(inter);
        }
    }

    for(int i=0;i<FINDING.size();i++)
    {
        if(FINDING.size()<8)
        {
            QGraphicsTextItem *finding = new QGraphicsTextItem;
            setTextproperties1(finding, font, FINDING.at(i),A4WIDTH/2,35+(LINESPACE*(i+1)));
            scene->addItem(finding);
        }
    }

    QGraphicsTextItem *severity = new QGraphicsTextItem;
    setTextproperties1(severity, font, SEVERITY, A4WIDTH-154-HORIZONTALMARGIN,35);
    scene->addItem(severity);

    QGraphicsTextItem *paperspeed = new QGraphicsTextItem;
    setTextproperties1(paperspeed, font, PAPERSPEED + " mm/s", HORIZONTALMARGIN+80,A4HEIGHT-40);
    scene->addItem(paperspeed);

    QGraphicsTextItem *amplification = new QGraphicsTextItem;
    setTextproperties1(amplification, font, AMPLIFICATION + " mm/mV", HORIZONTALMARGIN+200+80,A4HEIGHT-40);
    scene->addItem(amplification);
}

void ECG::on_horizontalSlider_valueChanged(int value)
{
    ui->graphicsView->setTransform(QTransform::fromScale(value*0.5,value*0.5));
}


void ECG::getPatientdata()
{
    manager.get(QNetworkRequest(QUrl(IPADDRESS+"/report&program/program/sp_api/search.php?hn="+HN)));
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(downloadFinished(QNetworkReply*)));
}

void ECG::downloadFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error())
    {
        fprintf(stderr, "Download of %s failed: %s\n", url.toEncoded().constData(), qPrintable(reply->errorString()));
    }
    else
    {
        QByteArray dataResult= reply->readAll();
        QString string(dataResult);
        //printf("Download of %s succeeded (saved to %s)\n", url.toEncoded().constData(), qPrintable(string));
        if(string != "")
        {
            //qDebug()<<string;
            parseXML(string);
        }
    }

    reply->deleteLater();
}
void ECG::parseXML(QString xml)
{
    QDomDocument doc;
    doc.setContent(xml);

    QDomNodeList list = doc.elementsByTagName("data");
    QDomNodeList hn = list.at(0).toElement().elementsByTagName("hn");

    QString pid = hn.at(0).toElement().elementsByTagName("pid").at(0).toElement().text();
    QString firstname = hn.at(0).toElement().elementsByTagName("fnm").at(0).toElement().text();
    NAME = firstname;
    QString lastname = hn.at(0).toElement().elementsByTagName("lnm").at(0).toElement().text();
    SURNAME = lastname;
    QString gender = hn.at(0).toElement().elementsByTagName("sex").at(0).toElement().text();
    if(gender == "ช")
    {
        gender = "ชาย";
    }
    else if (gender == "ญ")
    {
        gender = "หญิง";
    }
    else
    {
        gender = "-";
    }
    GENDER = gender;
    QString birthday = hn.at(0).toElement().elementsByTagName("dob").at(0).toElement().text();
    QDateTime today = QDateTime::currentDateTime();
    QDateTime date;
    QString year = "";
    QString month = "";
    try
    {
        date = QDateTime::fromString(birthday,"yyyy-MM-dd");
        BIRTH = date;
        year = QString::number(today.date().year()-date.date().year());
        month = QString::number(today.date().month()-date.date().month());

    }
    catch(...)
    {

    }

}

void ECG::getECG()
{
    //QString hn = "2166018";
    //Phillips ECG
    //QString ecgid = "426";

    //E4L ECG
    //QString ecgid = "384";

    //GE ECG
    //QString ecgid = "218";

    QString url = IPADDRESS+"/ekg/get_ekg_file.php?id="+ECGID;

    manager2.get(QNetworkRequest(QUrl(url)));
    connect(&manager2, SIGNAL(finished(QNetworkReply*)),
            SLOT(downloadECGFinished(QNetworkReply*)));
}
void ECG::downloadECGFinished(QNetworkReply *reply)
{
    QUrl url = reply->url();
    if (reply->error())
    {
        fprintf(stderr, "Download of %s failed: %s\n", url.toEncoded().constData(), qPrintable(reply->errorString()));
    }
    else
    {
        QByteArray dataResult= reply->readAll();
        QString string(dataResult);
        //printf("Download of %s succeeded (saved to %s)\n", url.toEncoded().constData(), qPrintable(string));
        if(string != "")
        {
            //qDebug()<<string;
            parseECG(string);
        }
    }
}

void ECG::parseECG(QString xml)
{
    QDomDocument doc;
    doc.setContent(xml);

    QDomNodeList list = doc.elementsByTagName("data");
    QString textencodebase64 = list.at(0).toElement().text();
    /*
     textencodebase64:

         <?xml version="1.0" encoding="UTF-8"?>
         <data value="0" desc="data was Encode by base64 FUNCTION">
            base64data
         </data>
     */
    QByteArray bytearray;
    QByteArray text = QByteArray::fromBase64(bytearray.append(textencodebase64));

    /*
     text:

        <?xml version="1.0" encoding="UTF-8"?>
        <ecg>
            <hn>2803589</hn>
            <txn>329400</txn>
            <typ>ipd</typ>
            <dtm_record>2013-04-30 14:19:31</dtm_record>
            <dtm_add>2013-05-15 10:32:20</dtm_add>
            <sender_id>2803589</sender_id>
            <version>1.0</version>
            <encoding>UTF-8</encoding>
            <content>
                base64data
            </content>
        </ecg>
     */

     doc.setContent(text);
     list = doc.elementsByTagName("ecg");
     QString hn = list.at(0).toElement().elementsByTagName("hn").at(0).toElement().text();
     QString txn = list.at(0).toElement().elementsByTagName("txn").at(0).toElement().text();
     QString type = list.at(0).toElement().elementsByTagName("typ").at(0).toElement().text();
     QString dtm_record = list.at(0).toElement().elementsByTagName("dtm_record").at(0).toElement().text();
     QString dtm_add = list.at(0).toElement().elementsByTagName("dtm_add").at(0).toElement().text();
     QString senderid = list.at(0).toElement().elementsByTagName("sender_id").at(0).toElement().text();
     QString version = list.at(0).toElement().elementsByTagName("version").at(0).toElement().text();
     QString encoding = list.at(0).toElement().elementsByTagName("encoding").at(0).toElement().text();
     QString contentencodebase64 = list.at(0).toElement().elementsByTagName("content").at(0).toElement().text();

     bytearray.clear();
     QByteArray content = QByteArray::fromBase64(bytearray.append(contentencodebase64));
     //qDebug()<<content;
     doc.setContent(content);
     QString ecgtype = getType(doc.firstChildElement().nodeName());
     ECGTYPE = ecgtype;

     if(ecgtype==PHILLIPS)
     {
        parsePhillips(doc);
     }
     else if(ecgtype==E4L)
     {
         parseE4L(doc);
     }
     else if(ecgtype==GE)
     {
         parseGE(doc);
     }

     if(ecgtype!="")
     {
         RECORDDATE = dtm_record;
         QDateTime daterecord = QDateTime::fromString(dtm_record,"yyyy-MM-dd HH:mm:ss");
         //qDebug()<<QString::number(daterecord.date().year());
         int year = daterecord.date().year()-BIRTH.date().year();
         int month = daterecord.date().month()-BIRTH.date().month();
         if(month<0)
         {
             year--;
             month = 12+month;
         }

         AGEYEAR = QString::number(year);
         AGEMONTH = QString::number(month);

         scene->clear();
         drawGrid();
         addECGvalues();
         drawLead(true);
     }
     else
     {
         scene->clear();
         QGraphicsTextItem *noecg = new QGraphicsTextItem;
         QFont font;
         font.setPixelSize(FONTSIZE1);
         font.setBold(true);
         font.setFamily("TH Sarabun New");
         setTextproperties1(noecg,font,"ECG data are not available!",500,500);
         scene->addItem(noecg);

     }
     ui->graphicsView->setScene(scene);
     QApplication::restoreOverrideCursor();
}

QString ECG::getType(QString name)
{
    if(name=="restingecgdata")
    {
        return PHILLIPS;
    }
    else if(name=="ClinicalDocument")
    {
        return E4L;
    }
    else if(name=="sapphire")
    {
        return GE;
    }
    else
    {
        return "";
    }
}

void ECG::parsePhillips(QDomDocument doc)
{
    QDomNodeList list = doc.elementsByTagName("restingecgdata");
    QDomNodeList interpretations = list.at(0).toElement().elementsByTagName("interpretations");
    QDomNodeList interpretation = interpretations.at(0).toElement().elementsByTagName("interpretation");
    QDomNodeList globalmeasurements = interpretation.at(0).toElement().elementsByTagName("globalmeasurements");

    RATE = globalmeasurements.at(0).toElement().elementsByTagName("heartrate").at(0).toElement().text();
    RR = globalmeasurements.at(0).toElement().elementsByTagName("rrint").at(0).toElement().text();
    PR = globalmeasurements.at(0).toElement().elementsByTagName("print").at(0).toElement().text();
    QRSdur = globalmeasurements.at(0).toElement().elementsByTagName("qrsdur").at(0).toElement().text();
    QT = globalmeasurements.at(0).toElement().elementsByTagName("qtint").at(0).toElement().text();
    QTCB = globalmeasurements.at(0).toElement().elementsByTagName("qtcb").at(0).toElement().text();
    QTCF = globalmeasurements.at(0).toElement().elementsByTagName("qtcf").at(0).toElement().text();
    Paxis = globalmeasurements.at(0).toElement().elementsByTagName("pfrontaxis").at(0).toElement().text();
    QRSaxis = globalmeasurements.at(0).toElement().elementsByTagName("qrsfrontaxis").at(0).toElement().text();
    Taxis = globalmeasurements.at(0).toElement().elementsByTagName("tfrontaxis").at(0).toElement().text();

    SEVERITY = interpretation.at(0).toElement().elementsByTagName("severity").at(0).toElement().text();

    QDomNodeList statement = interpretation.at(0).toElement().elementsByTagName("statement");

    for(int i=0;i<statement.size();i++)
    {
        QString leftstatement = statement.at(i).toElement().elementsByTagName("leftstatement").at(0).toElement().text();
        QString rightstatement = statement.at(i).toElement().elementsByTagName("rightstatement").at(0).toElement().text();
        if(leftstatement!="")
        {
            INTERPRETATION.append(leftstatement);
        }
        if(rightstatement!="")
        {
            FINDING.append(rightstatement);
        }
    }

    QDomNodeList reportinfo = list.at(0).toElement().elementsByTagName("reportinfo");
    QDomNodeList reportgain = reportinfo.at(0).toElement().elementsByTagName("reportgain");
    QDomNodeList amplitudegain = reportgain.at(0).toElement().elementsByTagName("amplitudegain");
    AMPLIFICATION = amplitudegain.at(0).toElement().elementsByTagName("overallgain").at(0).toElement().text();
    PAPERSPEED = reportgain.at(0).toElement().elementsByTagName("timegain").at(0).toElement().text();

    QDomNodeList waveforms = list.at(0).toElement().elementsByTagName("waveforms");
    QDomNodeList parsedwaveforms = waveforms.at(0).toElement().elementsByTagName("parsedwaveforms");
    QString encoding = parsedwaveforms.at(0).attributes().namedItem("dataencoding").nodeValue();
    QString compression = parsedwaveforms.at(0).attributes().namedItem("compression").nodeValue();

    QByteArray bytearray;
    QByteArray wave;
    if(encoding=="Base64")
    {
        wave = QByteArray::fromBase64(bytearray.append(parsedwaveforms.at(0).toElement().text()));
    }
    else
    {
        wave.append(parsedwaveforms.at(0).toElement().text());
    }

    if(compression=="XLI")
    {
    //XLI decompressor

    //Size|Unk|Delta|LZW compressed deltas (10-bit code)

    // Each chunk begins with an 8 byte header consisting of:
    //   - 32-bit integer describing the length of the chunk
    //   - 16-bit integer (unknown payload, seemingly always 1)
    //   - 16-bit integer describing the first delta code
    //
    // After this header exists LZW-compressed delta codes, using 10-bit code words:
    // 0        2        4        6        8  ...
    // +--------+--------+--------+--------+--------+--------+--------+--------+--------+
    // | Size            |  Unk.  | Delta  | LZW compressed deltas (10-bit codes)       |
    // +--------+--------+--------+--------+                                            |
    // | ...                                                               [Size bytes] |
    // +--------+--------+--------+--------+--------+--------+--------+--------+--------+

    double total = 0;
    leads.clear();
    for(int l=0;l<12;l++)
    {
    if(wave.length()<total+8)
    {
        break;
    }
    //Get size of LZW compressed data
    QByteArray sizearray;
    sizearray.reserve(4);
    for(int i=0+total;i<4+total;i++)
    {
        sizearray.append(wave.at(i));
    }
    int size = 0;
    memcpy(&size,sizearray,sizeof(qint32));
    //qDebug()<<QString::number(size);

    //Get delta

    QByteArray startarray;
    startarray.clear();
    startarray.reserve(2);
    for(int i=6+total;i<8+total;i++)
    {
        startarray.append(wave.at(i));
    }

    short initvalue = *(reinterpret_cast<short*>(startarray.data()));
    //qDebug()<<QString::number(initvalue);

    //Get LZW compressed data
    QByteArray chunk;
    if(wave.length()<total+size+8)
    {
        break;
    }
    for(int i=8+total;i<size+8+total;i++)
    {
        chunk.append(wave.at(i));
    }

    total = total+size+8;


    //Create a dictionary
    map.clear();
    for(int i=0;i<256;i++)
    {
        unsigned int var = i;
        char a = var & 0xFF;
        QByteArray integer;
        integer.append(a);
        map.insert(i,integer);
    }

    //Init values
    int maxCode = (1 << 10) - 2;
    id = 0;
    bitCount = 0;
    buffer = 0;
    previous.clear();
    nextcode = 256;
    current.clear();
    position = -1;

    //Decompress LZW 10 bits compressed data
    QByteArray bytes;
    int b;
    id = 0;
    while(-1 != (b=read(chunk,maxCode)))
    {
        unsigned int var = b;
        char a = var & 0xFF;
        bytes.append(a);
        if(id==chunk.size())
        {
            break;
        }

    }

    //Joint data between hiword and lowword [HIWORD1,LOWWORD1,HIWORD2,LOWWORD2,....]
    QList<short> output = unpack(bytes);

    //Decode the delta compression
    QList<int> data = decodedeltas(output,initvalue);
    leads.append(data);
    }


    }
}

void ECG::parseE4L(QDomDocument doc)
{
    QDomNodeList list = doc.elementsByTagName("ClinicalDocument");

    QDomNodeList globalmeasurements = list.at(0).toElement().elementsByTagName("GlobalMeasurement");

    RATE = globalmeasurements.at(0).toElement().elementsByTagName("item").at(0).attributes().namedItem("value").nodeValue();
    RR = globalmeasurements.at(0).toElement().elementsByTagName("item").at(10).attributes().namedItem("value").nodeValue();
    PR = globalmeasurements.at(0).toElement().elementsByTagName("item").at(1).attributes().namedItem("value").nodeValue();
    QRSdur = globalmeasurements.at(0).toElement().elementsByTagName("item").at(2).attributes().namedItem("value").nodeValue();
    QT = globalmeasurements.at(0).toElement().elementsByTagName("item").at(3).attributes().namedItem("value").nodeValue();
    QTCB = getQTCB(QT,RR);
    QTCF = getQTCF(QT,RR);
    Paxis = globalmeasurements.at(0).toElement().elementsByTagName("item").at(5).attributes().namedItem("value").nodeValue();
    QRSaxis = globalmeasurements.at(0).toElement().elementsByTagName("item").at(6).attributes().namedItem("value").nodeValue();
    Taxis = globalmeasurements.at(0).toElement().elementsByTagName("item").at(7).attributes().namedItem("value").nodeValue();

    SEVERITY = "";

    QDomNodeList statement = list.at(0).toElement().elementsByTagName("AutomaticTypification").at(0).toElement().elementsByTagName("item");

    for(int i=0;i<statement.size();i++)
    {
        QString leftstatement = statement.at(i).attributes().namedItem("text").nodeValue();
        if(leftstatement!="")
        {
            if(i==0)
            {
                SEVERITY = leftstatement;
            }
            INTERPRETATION.append(leftstatement);
        }
    }

    AMPLIFICATION = "";
    PAPERSPEED = "";

    leads.clear();
    QDomNodeList waveformdata = list.at(0).toElement().elementsByTagName("ECGWave").at(0).toElement().elementsByTagName("WaveFormData");
    for(int i=0;i<waveformdata.length();i++)
    {
        if(i<12)
        {
            QString value = waveformdata.at(i).attributes().namedItem("value").nodeValue();
            QStringList list = value.split(" ", QString::SkipEmptyParts);
            QList<int> data;
            bool ok;
            for(int j=0;j<list.length();j++)
            {
                float f = list.at(j).toFloat(&ok)/5;
                data.append(static_cast<int>(f));
            }
            leads.append(data);
        }
    }
}

void ECG::parseGE(QDomDocument doc)
{
    QDomNodeList list = doc.elementsByTagName("sapphire");

    QDomNodeList dcarrecord = list.at(0).toElement().elementsByTagName("dcarRecord");
    QDomNodeList patientinfo = dcarrecord.at(0).toElement().elementsByTagName("patientInfo");
    QDomNodeList visit = patientinfo.at(0).toElement().elementsByTagName("visit");
    QDomNodeList order = visit.at(0).toElement().elementsByTagName("order");
    QDomNodeList ecgresting = order.at(0).toElement().elementsByTagName("ecgResting");
    QDomNodeList params = ecgresting.at(0).toElement().elementsByTagName("params");
    QDomNodeList ecg = params.at(0).toElement().elementsByTagName("ecg");
    QDomNodeList num = ecg.at(0).toElement().elementsByTagName("num");
    QDomNodeList var = ecg.at(0).toElement().elementsByTagName("var");
    QDomNodeList mediantemplate = var.at(0).toElement().elementsByTagName("medianTemplate");
    QDomNodeList measurements = mediantemplate.at(0).toElement().elementsByTagName("measurements");
    QDomNodeList global = measurements.at(0).toElement().elementsByTagName("global");

    RATE = num.at(0).toElement().elementsByTagName("ventricularRate").at(0).attributes().namedItem("V").nodeValue();
    RR = global.at(0).toElement().elementsByTagName("aveRRInterval").at(0).attributes().namedItem("V").nodeValue();
    PR = global.at(0).toElement().elementsByTagName("PR_Interval").at(0).attributes().namedItem("V").nodeValue();
    QRSdur = global.at(0).toElement().elementsByTagName("QRS_Duration").at(0).attributes().namedItem("V").nodeValue();
    QT = global.at(0).toElement().elementsByTagName("QT_Interval").at(0).attributes().namedItem("V").nodeValue();
    QTCB = getQTCB(QT,RR);
    QTCF = getQTCF(QT,RR);
    Paxis = global.at(0).toElement().elementsByTagName("P_Axis").at(0).attributes().namedItem("V").nodeValue();
    QRSaxis = global.at(0).toElement().elementsByTagName("R_Axis").at(0).attributes().namedItem("V").nodeValue();
    Taxis = global.at(0).toElement().elementsByTagName("T_Axis").at(0).attributes().namedItem("V").nodeValue();

    SEVERITY = "";

    QDomNodeList statement = ecg.at(0).toElement().elementsByTagName("interpretation").at(0).toElement().elementsByTagName("statement");

    for(int i=0;i<statement.size();i++)
    {
        QString leftstatement = statement.at(i).attributes().namedItem("V").nodeValue();
        if(leftstatement!="")
        {
            INTERPRETATION.append(leftstatement);
        }
    }
    if(INTERPRETATION.length()>0)
    {
        SEVERITY = INTERPRETATION.at(INTERPRETATION.length()-1);
    }
    QDomNodeList cfg = ecg.at(0).toElement().elementsByTagName("cfg");
    QDomNodeList reportconfiguration = cfg.at(0).toElement().elementsByTagName("reportConfiguration");

    AMPLIFICATION = reportconfiguration.at(0).toElement().elementsByTagName("frontalLeadGain").at(0).attributes().namedItem("V").nodeValue();
    PAPERSPEED = reportconfiguration.at(0).toElement().elementsByTagName("writerSpeed").at(0).attributes().namedItem("V").nodeValue();

    leads.clear();
    QDomNodeList wav = ecg.at(0).toElement().elementsByTagName("wav");
    QDomNodeList ecgwaveformmxg = wav.at(0).toElement().elementsByTagName("ecgWaveformMXG");
    QDomNodeList ecgwaveform = ecgwaveformmxg.at(0).toElement().elementsByTagName("ecgWaveform");

    for(int i=0;i<ecgwaveform.length();i++)
    {
        if(i<12)
        {
            QString value = ecgwaveform.at(i).attributes().namedItem("V").nodeValue();
            QStringList list = value.split(" ", QString::SkipEmptyParts);
            QList<int> data;
            bool ok;
            for(int j=0;j<list.length();j++)
            {
                data.append(list.at(j).toInt(&ok,10));
            }
            leads.append(data);
        }
    }
}

QString ECG::getQTCB(QString qt, QString rr)
{
    if(qt==""||rr=="")
    {
        return "";
    }
    else
    {
        bool ok;
        int qtnum = qt.toInt(&ok,10);
        double rrnum = rr.toDouble(&ok);

        int num = qtnum/(sqrt(rrnum/1000));
        return QString::number(num);
    }
}

QString ECG::getQTCF(QString qt, QString rr)
{
    if(qt==""||rr=="")
    {
        return "";
    }
    else
    {
        bool ok;
        int qtnum = qt.toInt(&ok,10);
        double rrnum = rr.toDouble(&ok);

        int num = qtnum*10/(cbrt(rrnum));
        return QString::number(num);
    }
}

void ECG::drawLead(bool newlead)
{
    float s = 0.2f;
    int half = (A4HEIGHT-INFOSPACE)/(5*2);

    QPen pen;
    pen.setWidth(1.2);
    pen.setBrush(Qt::black);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);

    if(!leads.isEmpty())
    {
        if(ECGTYPE==PHILLIPS&&newlead)
        {
            reconstituteLeads();
        }

        for(int i=50;i<leads.at(1).length()-1;i++)
        {
            if(i==6000)
            {
                break;
            }
            else
            {
                QGraphicsLineItem *line = scene->addLine(0+(i*s),
                                                         INFOSPACE+((A4HEIGHT-INFOSPACE)*4/5)+half-(leads.at(1).at(i)*s),
                                                         0+((i+1)*s),
                                                         INFOSPACE+((A4HEIGHT-INFOSPACE)*4/5)+half-(leads.at(1).at(i+1)*s),
                                                         QPen(QBrush(Qt::black),1.2));
                line->setPen(pen);
            }
        }

        bool ok;
        for(int l=0;l<leads.length();l++)
        {
            QList<int> data = leads.at(l);
            float ampi;
            if(l<6)
            {
                ampi = ui->comboBox->currentText().toFloat(&ok);
            }
            else
            {
                ampi = ui->comboBox_2->currentText().toFloat(&ok);
            }
            for(int i=50;i<1950;i++)
            {
               QGraphicsLineItem *line = scene->addLine(leadpositionx.at(l)+(i*s),
                                                        leadpositiony.at(l)+half-(data.at(i)*s*ampi),
                                                        leadpositionx.at(l)+((i+1)*s),
                                                        leadpositiony.at(l)+half-(data.at(i+1)*s*ampi),
                                                        QPen(QBrush(Qt::black),1.2));
               line->setPen(pen);
            }
        }
    }
}
void ECG::baselinewander()
{

}

void ECG::reconstituteLeads()
{
    //Reconstitute leads III, aVR, aVL, and aVF
    if(leads.length()>=12)
    {
        QList<int> leadI = leads.at(0);
        QList<int> leadII = leads.at(1);
        QList<int> leadIII = leads.at(2);
        QList<int> leadaVR = leads.at(3);
        QList<int> leadaVL = leads.at(4);
        QList<int> leadaVF = leads.at(5);

        //lead III
        for(int i=0;i<leadIII.length();i++)
        {
            int v = leadII.at(i) - leadI.at(i) - leadIII.at(i);
            leadIII.replace(i,v);
        }
        leads.replace(2,leadIII);

        //lead aVR
        for(int i=0;i<leads.at(3).length();i++)
        {
            int v = -leads.at(3).at(i) - ((leadI.at(i)+leadII.at(i))/2);

            leadaVR.replace(i,v);
        }
        leads.replace(3,leadaVR);

        //lead aVL
        for(int i=0;i<leads.at(4).length();i++)
        {
            int v = ((leadI.at(i)-leadIII.at(i))/2) - leads.at(4).at(i);
            leadaVL.replace(i,v);
        }
        leads.replace(4,leadaVL);

        //lead aVF
        for(int i=0;i<leads.at(5).length();i++)
        {
            int v = ((leadII.at(i)+leadIII.at(i))/2) - leads.at(5).at(i);
            leadaVF.replace(i,v);
        }
        leads.replace(5,leadaVF);
    }
}

int ECG::read(QByteArray chunk, int maxCode)
{
    if(current.isNull() || (position == current.length()))
    {
        current = readInternal(chunk,maxCode);
        position = 0;
    }

    if(current.length()>0)
    {
        return current.at(position++) & 0xFF;
    }
    else
    {
        return -1;
    }
}

QByteArray ECG::readInternal(QByteArray chunk, int maxCode)
{

    QByteArray data;
    int code;
    while(-1 != (code = readCodeword(chunk)))
    {
        if(code >= maxCode + 1)
        {
            break;
        }
        if(!map.contains(code))
        {
            for(int j=0;j<previous.size();j++)
            {
                data.append(previous.at(j));
            }
            data.append(previous.at(0));
            //qDebug()<<"code "+QString::number(code);
            map.insert(code,data);
        }
        else
        {
            data.append(map.value(code));
        }

        if(previous.length() > 0 && nextcode <= maxCode)
        {
            QByteArray nextdata;
            for(int j=0;j<previous.size();j++)
            {
                nextdata.append(previous.at(j));
            }
            nextdata.append(data.at(0));
            map.insert(nextcode++,nextdata);
        }
        previous = data;
        return data;
    }
    return data.append(map.value(0));
}

int ECG::readCodeword(QByteArray chunk)
{
    while(bitCount <= 24)
    {
        if(id>chunk.length()-1)
        {
            return -1;
        }
        buffer |= ((chunk.at(id) & 0xFF) << (24 - bitCount)) & 0XFFFFFFFF;
        bitCount += 8;
        id++;
        //qDebug()<<"ID "+QString::number(id)+"buff "+QString::number(buffer);
    }

    int code = (buffer >> (32 - 10)) & 0x0000FFFF;
    buffer =((buffer & 0xFFFFFFFF) << 10) & 0xFFFFFFFF;
    bitCount -= 10;

    return code;
}

QList<short> ECG::unpack(QByteArray bytes)
{
    QList<short> output;
    int l = (bytes.length()+1)/2;
    for(int i = 0, j = l; i < l; i++,j++)
    {
        output.append((short)((bytes.at(i) << 8) | (j < bytes.length() ? (int)bytes.at(j) : 0)));
    }
    return output;
}

QList<int> ECG::decodedeltas(QList<short> input, short initvalue)
{
    QList<int> output;

    for(int i=0; i<input.length();i++)
    {
        output.append(input.at(i));
    }

    int x = output.at(0);
    int y = output.at(1);
    int lastvalue = initvalue;

    for(int i=2;i<output.length();i++)
    {
        int z = (y+y)-x-lastvalue;
        lastvalue = output.at(i) - 64;
        output.replace(i,z);//at(i) = z;
        x = y;
        y = z;
    }

    return output;
}

void ECG::setTextproperties1(QGraphicsTextItem *text, QFont font, QString string, int x, int y)
{
    text->setPos(x,y);
    text->setPlainText(string);
    text->setFont(font);
}

void ECG::on_pushButton_2_clicked()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Landscape);
    QPrintDialog dlg(&printer);
    if(dlg.exec()==QDialog::Accepted)
    {
        QPainter p(&printer);
        scene->render(&p);
        p.end();
    }
}

void ECG::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export PDF",QString(), "*.pdf");
    if (!fileName.isEmpty())
    {
        QPrinter printer(QPrinter::HighResolution);
        printer.setPageSize(QPrinter::A4);
        printer.setOrientation(QPrinter::Landscape);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);

        QPainter p(&printer);
        scene->render(&p);
        p.end();
    }

}

void ECG::on_pushButton_3_clicked()
{
    scene->clear();
    drawGrid();
    addECGvalues();
    drawLead(false);
}
