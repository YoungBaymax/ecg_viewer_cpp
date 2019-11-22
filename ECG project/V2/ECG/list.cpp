#include "list.h"
#include "ui_list.h"

QString HNECG;
QStringList list;
QMap<QString,QString> map;


List::List(QWidget *parent, QString hn) :
    QWidget(parent),
    ui(new Ui::List)
{
    HNECG = hn;

    getECGlist(hn);

    ui->setupUi(this);
    ui->lineEdit->insert(hn);
}

List::~List()
{
    delete ui;
}

void List::on_lineEdit_2_textChanged(const QString &arg1)
{
    QList<QListWidgetItem*> item = ui->listWidget->findItems(arg1,Qt::MatchContains);
    QRegExp reg(arg1, Qt::CaseInsensitive, QRegExp::Wildcard);
    ui->listWidget->clear();
    ui->listWidget->addItems(list.filter(reg));
    ui->listWidget->sortItems(Qt::DescendingOrder);
}

void List::getECGlist(QString HN)
{
    manager.get(QNetworkRequest(QUrl(IPADDRESS+"/ekg/ekg_get_info.php?hn="+HN)));
    connect(&manager, SIGNAL(finished(QNetworkReply*)),
            SLOT(downloadFinished(QNetworkReply*)));
}

void List::downloadFinished(QNetworkReply *reply)
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

void List::parseXML(QString xml)
{
    QDomDocument doc;
    doc.setContent(xml);

    QDomNodeList ecglist = doc.elementsByTagName("data").at(0).toElement().elementsByTagName("id");
    list.clear();
    ui->listWidget->clear();
    map.clear();
    for(int i=0;i<ecglist.length();i++)
    {
        QString key = ecglist.at(i).attributes().namedItem("value").nodeValue();
        QString value = ecglist.at(i).toElement().elementsByTagName("dtm_record").at(0).toElement().text();
        map.insert(value,key);
        list.append(value);
        ui->listWidget->addItem(value);
    }

    ui->listWidget->sortItems(Qt::DescendingOrder);
}

void List::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    QString id = map.value(item->text());
    ECG *w = new ECG(this,HNECG,id);
    w->showMaximized();
    w->show();
}

void List::on_pushButton_clicked()
{
    HNECG = ui->lineEdit->text();
    getECGlist(HNECG);
}

void List::on_lineEdit_editingFinished()
{
    HNECG = ui->lineEdit->text();
    getECGlist(HNECG);
}
