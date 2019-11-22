#ifndef INTERPRET_H
#define INTERPRET_H

#include <ecg.h>

#include <QWidget>
#include <QListWidgetItem>
#include <QtDebug>

//Network
#include <QtNetwork/QNetworkAccessManager>
#include <QUrl>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

namespace Ui {
class Interpret;
}

class Interpret : public QWidget
{
    Q_OBJECT
    
public:
    explicit Interpret(QWidget *parent = 0);
    ~Interpret();
    
private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void downloadInterpretationFinished(QNetworkReply *reply);
    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

private:
    Ui::Interpret *ui;
    QNetworkAccessManager manager;

public:
    void setInterpretation();
};

#endif // INTERPRET_H
