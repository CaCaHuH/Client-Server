#ifndef INPUTWIDGET_H
#define INPUTWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QVector>
#include <QUdpSocket>
#include "udpclient.h"

namespace Ui {
class inputWidget;
}

class inputWidget : public QWidget
{
    Q_OBJECT

public:
    explicit inputWidget(QWidget *parent = nullptr);
     ~inputWidget();
    void create();
    void readvalues(QDoubleSpinBox);
    void setSettings(QDoubleSpinBox*);

signals:
    void stopall();

public slots:
    void SendClick();
    void ClearClick();
    void displayError();
    void dataRecieved(QString datagram);
    void callNumOfCalculate();
    void setReceivingPort(quint16 portR);

private:
    Ui::inputWidget *ui;
    QPushButton my_btn;
    QPushButton my_btn_clear;
    QPushButton my_btn_call;
    int minw = 200;
    int minh = 0;
    int decimals = 4;
    QDoubleSpinBox my_spinbox1;
    QDoubleSpinBox my_spinbox2;
    QDoubleSpinBox my_spinbox3;
    QDoubleSpinBox my_spinbox4;
    QDoubleSpinBox my_spinbox5;
    QDoubleSpinBox my_spinbox6;
    QDoubleSpinBox my_spinbox7;
    QDoubleSpinBox my_spinbox8;
    QDoubleSpinBox my_spinbox9;
    QDoubleSpinBox my_spinbox10;
    QDoubleSpinBox my_spinbox11;
    QDoubleSpinBox my_spinbox12;
    QSpinBox my_spinbox13;
    QVector<QDoubleSpinBox*> spinboxvec;
    QUdpSocket *m_socketWrite;
    QDataStream in;
    quint16 m_receivingPort;
    int counter_zap = 1;

};

#endif // INPUTWIDGET_H
