#ifndef LIST_H
#define LIST_H

#include <QWidget>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QtDebug>

#include <QDomDocument>
#include <QMap>
#include <QListWidgetItem>
#include <ecg.h>

namespace Ui {
class List;
}

class List : public QWidget
{
    Q_OBJECT
    
public:
    explicit List(QWidget *parent = 0, QString hn = "");
    ~List();
    
private slots:
    void on_lineEdit_2_textChanged(const QString &arg1);
    void downloadFinished(QNetworkReply *reply);

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_clicked();

    void on_lineEdit_editingFinished();

private:
    Ui::List *ui;
    QNetworkAccessManager manager;

public:
    void getECGlist(QString HN);
    void parseXML(QString xml);
};

#endif // LIST_H
