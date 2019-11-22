#include "interpret.h"
#include "ui_interpret.h"

Interpret::Interpret(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Interpret)
{
    ui->setupUi(this);
}

Interpret::~Interpret()
{
    delete ui;
}

void Interpret::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    item->setFlags(item->flags () | Qt::ItemIsEditable);
}

void Interpret::on_pushButton_2_clicked()
{
    ui->listWidget->takeItem(ui->listWidget->row(ui->listWidget->currentItem()));
}

void Interpret::on_pushButton_3_clicked()
{
    //Add item to the interpretation list
    ui->listWidget->addItem(ui->lineEdit->text());
    ui->lineEdit->clear();
}

void Interpret::on_pushButton_clicked()
{
    //Save an interpretation
    //Template
    /*
     <?xml version = "1.0" encoding = "UTF-8"?>
        <ecg>
            <id>25</id>
            <verifier>10103</verifier>
            <verification>true</verification>
            <finding>
                <text1>ทดสอบ1</text1>
                <text2>ทดสอบ2</text2>
                <text3>ทดสอบ3</text3>
                <text4>ทดสอบ4</text4>
                <text5>ทดสอบ5</text5>
                <text6>ทดสอบ6</text6>
                <text7>ทดสอบ7</text7>
                <text8>ทดสอบ8</text8>
            </finding>
            <diagnosis>Normal sinus rhythm</diagnosis>
            <signature>10103</signature>
        </ecg>
     */
    setInterpretation();
}
void Interpret::setInterpretation()
{
    QString url = IPADDRESS+"/ekg/get_interpret.php";
    QByteArray data;
    QString string;

    string.append("<?xml version = \"1.0\" encoding = \"UTF-8\"?>");
    string.append("<ecg>");
                string.append("<id>53</id>");
                string.append("<verifier>10103</verifier>");
                string.append("<verification>true</verification>");
                string.append("<finding>");

                for(int i=0;i<8;i++)
                {

                    if(i<ui->listWidget->count())
                    {
                        string.append("<text");
                        string.append(QString::number(i+1));
                        string.append(">");
                        string.append(ui->listWidget->item(i)->text());
                        string.append("</text");
                        string.append(QString::number(i+1));
                        string.append(">");
                    }
                    else
                    {
                        string.append("<text");
                        string.append(QString::number(i+1));
                        string.append("></text");
                        string.append(QString::number(i+1));
                        string.append(">");
                    }
                }
                string.append("</finding>");
                string.append("<diagnosis>");
                string.append(ui->lineEdit_2->text());
                string.append("</diagnosis>");
                string.append("<signature>10103</signature>");
            string.append("</ecg>");
    data.append(string);


    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded;charset=UTF-8");
    req.setHeader(QNetworkRequest::ContentLengthHeader, data.length());

    manager.post(req,data);
    connect(&manager, SIGNAL(finished(QNetworkReply*)), SLOT(downloadInterpretationFinished(QNetworkReply*)));

    //QNetworkRequest request(url);
    //request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

}
void Interpret::downloadInterpretationFinished(QNetworkReply *reply)
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
        printf("Download of %s succeeded (saved to %s)\n", url.toEncoded().constData(), qPrintable(string));
        if(string != "")
        {
            qDebug()<<string;
            //parseECG(string);
        }
    }
}
