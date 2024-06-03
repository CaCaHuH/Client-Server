#include "inputwidget.h"
#include "ui_inputwidget.h"
#include <QDebug>
#include <QString>
#include <QTime>
#include <unistd.h>
#include <QThread>

inputWidget::inputWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::inputWidget)
{
    ui->setupUi(this);

    UdpClient* client = new UdpClient();
    QThread* thread = new QThread;
    client->moveToThread(thread);

    connect(thread,SIGNAL(started()),client,SLOT(process()));

    connect(client,SIGNAL(finished()),thread,SLOT(quit()));

    connect(this,SIGNAL(stopall()),client,SLOT(stop()));

    connect(client,SIGNAL(finished()),client,SLOT(deleteLater()));

    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));

    connect(client,SIGNAL(dataRecieved(QString)),this,SLOT(dataRecieved(QString)));

    connect(client,SIGNAL(receivingPort(quint16)),this,SLOT(setReceivingPort(quint16)));

    thread->start();

    m_socketWrite = nullptr;
}

inputWidget::~inputWidget()
{
    delete ui;
}

void inputWidget::setSettings(QDoubleSpinBox *spinbox_ptr)
{
    spinbox_ptr->setMinimumSize(QSize(minw, minh));
    spinbox_ptr->setMaximumSize(QSize(300, 30));
    spinbox_ptr->setDecimals(decimals);
    spinbox_ptr->setMinimum(-10000);
    spinbox_ptr->setMaximum(10000);
    spinbox_ptr->setFont(QFont ("Courier New", 12));
}

void inputWidget::create()
{
    qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") << "Create Client.";

    my_btn_call.setText("Запросить количество расчетов");
    my_btn_call.setFont(QFont ("Courier New", 12));
    ui->horizontalLayout_7s->addWidget(&my_btn_call);
    connect(&my_btn_call, SIGNAL(clicked()), this, SLOT(callNumOfCalculate()));

    my_btn_clear.setText("Очистить текстовое поле");
    my_btn_clear.setFont(QFont ("Courier New", 12));
    ui->verticalLayout1->addWidget(&my_btn_clear,7);
    connect(&my_btn_clear, SIGNAL(clicked()), this, SLOT(ClearClick()));

    ui->horizontalLayout_14s->addWidget(&my_spinbox13, 1);
    my_spinbox13.setMinimumSize(QSize(minw, minh));
    my_spinbox13.setMaximumSize(QSize(300, 30));
    my_spinbox13.setMinimum(1);
    my_spinbox13.setMaximum(100);
    my_spinbox13.setFont(QFont ("Courier New", 12));

    my_btn.setText("Отправить данные");
    my_btn.setFont(QFont ("Courier New", 12));
    ui->verticalLayout2->addWidget(&my_btn,7);
    connect(&my_btn, SIGNAL(clicked()), this, SLOT(SendClick()));

    //Adding spinboxes
    ui->horizontalLayout_1s->addWidget(&my_spinbox1, 1);
    setSettings(&my_spinbox1);
    my_spinbox1.setValue(0.1);
    spinboxvec.push_back(&my_spinbox1);
    ui->horizontalLayout_2s->addWidget(&my_spinbox2, 1);
    setSettings(&my_spinbox2);
    my_spinbox2.setValue(0);
    spinboxvec.push_back(&my_spinbox2);
    ui->horizontalLayout_3s->addWidget(&my_spinbox3, 1);
    setSettings(&my_spinbox3);
    my_spinbox3.setValue(300);
    spinboxvec.push_back(&my_spinbox3);
    ui->horizontalLayout_4s->addWidget(&my_spinbox4, 1);
    setSettings(&my_spinbox4);
    my_spinbox4.setValue(45);
    spinboxvec.push_back(&my_spinbox4);
    ui->horizontalLayout_5s->addWidget(&my_spinbox5, 1);
    setSettings(&my_spinbox5);
    my_spinbox5.setValue(300);
    spinboxvec.push_back(&my_spinbox5);
    ui->horizontalLayout_6s->addWidget(&my_spinbox11, 1);
    setSettings(&my_spinbox11);
    my_spinbox6.setValue(0.8);
    spinboxvec.push_back(&my_spinbox11);

    ui->horizontalLayout_8s->addWidget(&my_spinbox6, 1);
    setSettings(&my_spinbox6);
    my_spinbox7.setValue(0.05);
    spinboxvec.push_back(&my_spinbox6);
    ui->horizontalLayout_9s->addWidget(&my_spinbox7, 1);
    setSettings(&my_spinbox7);
    my_spinbox8.setValue(0.1);
    spinboxvec.push_back(&my_spinbox7);
    ui->horizontalLayout_10s->addWidget(&my_spinbox8, 1);
    setSettings(&my_spinbox8);
    my_spinbox9.setValue(20);
    spinboxvec.push_back(&my_spinbox8);
    ui->horizontalLayout_11s->addWidget(&my_spinbox9, 1);
    setSettings(&my_spinbox9);
    my_spinbox10.setValue(0.5);
    spinboxvec.push_back(&my_spinbox9);
    ui->horizontalLayout_12s->addWidget(&my_spinbox10, 1);
    setSettings(&my_spinbox10);
    my_spinbox11.setValue(35);
    spinboxvec.push_back(&my_spinbox10);
    ui->horizontalLayout_13s->addWidget(&my_spinbox12, 1);
    setSettings(&my_spinbox12);
    my_spinbox12.setValue(0.3);
    spinboxvec.push_back(&my_spinbox12);

}

void inputWidget::SendClick()
{
    qDebug() << "------------------------------------------------------------------------";
    qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") << "Push SendButton.";
    float VALUES[spinboxvec.length()];
    for(int i = 0; i < spinboxvec.length(); i++){
    VALUES[i] = spinboxvec.at(i)->value();
    }

    if (m_socketWrite == nullptr)
        m_socketWrite = new QUdpSocket(this);

    ui->textEdit->append(QString("-------------------------------------"));
    if (m_socketWrite){
        ui->textEdit->append(QString(QVariant(counter_zap).toString() + ") Client connected to socket."));
    } else{
        ui->textEdit->append(QString(QVariant(counter_zap).toString() + ") Client is not connected to socket."));
        return;
    }
    ui->textEdit->append("Current time: " + QTime::currentTime().toString("hh:mm:ss:zzz"));
    QString init = QString("Initial data:\n") +
            QString("h = ") + QVariant(VALUES[0]).toString() + QString("\n") +
            QString("y0 = ") + QVariant(VALUES[1]).toString() + QString("\n") +
            QString("v0 = ") + QVariant(VALUES[2]).toString() + QString("\n") +
            QString("tetta0 = ") + QVariant(VALUES[3]).toString() + QString("\n") +
            QString("T = ") + QVariant(VALUES[4]).toString() + QString("\n") +
            QString("m0 = ") + QVariant(VALUES[5]).toString() + QString("\n") +
            QString("cx0 = ") + QVariant(VALUES[6]).toString() + QString("\n") +
            QString("cya = ") + QVariant(VALUES[7]).toString() + QString("\n") +
            QString("A = ") + QVariant(VALUES[8]).toString() + QString("\n") +
            QString("tr = ") + QVariant(VALUES[9]).toString() + QString("\n") +
            QString("mc = ") + QVariant(VALUES[10]).toString() + QString("\n") +
            QString("d = ") + QVariant(VALUES[11]).toString() + QString("\n") +
            QString("Number of calculate: ") + QVariant(my_spinbox13.value()).toString();

    ui->textEdit->append(init);

    QByteArray dataOut;
    QDataStream out(&dataOut, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    QString msg = "Client message";
    out << qint64(0);
    out << qint8(2);
    out << quint16(m_receivingPort);
    out << qint64(counter_zap);
    out << float(VALUES[0]); // 1
    out << float(VALUES[1]); // 2
    out << float(VALUES[2]); // 3
    out << float(VALUES[3]); // 4
    out << float(VALUES[4]); // 5
    out << float(VALUES[5]); // 6
    out << float(VALUES[6]); // 7
    out << float(VALUES[7]); // 8
    out << float(VALUES[8]); // 9
    out << float(VALUES[9]); // 10
    out << float(VALUES[10]); // 11
    out << float(VALUES[11]); // 12
    out << qint8(my_spinbox13.value());
    out << msg;
    out.device()->seek(qint64(0));
    out << qint64(dataOut.size() - sizeof(qint64));
    qDebug()<< "size" << dataOut.size();
    m_socketWrite->writeDatagram(dataOut, QHostAddress::LocalHost, 36001);
    counter_zap = counter_zap + 1;
}

void inputWidget::ClearClick()
{
    ui->textEdit->clear();
    counter_zap = 1;
}

void inputWidget::displayError()
{
    qDebug() <<"ERROR";

}

void inputWidget::dataRecieved(QString datagram)
{
    ui->textEdit->append(QString("-------------------------------------"));
    ui->textEdit->append("Current time: " + QTime::currentTime().toString("hh:mm:ss:zzz"));
    ui->textEdit->append(datagram);    
}

void inputWidget::callNumOfCalculate()
{
    qDebug() << "------------------------------------------------------------------------";
    qDebug() << QTime::currentTime().toString("hh:mm:ss:zzz") << "Push callNumOfCalculate.";

    if (m_socketWrite == nullptr)
        m_socketWrite = new QUdpSocket(this);

    ui->textEdit->append(QString("-------------------------------------"));
    ui->textEdit->append("Current time: " + QTime::currentTime().toString("hh:mm:ss:zzz"));
    if (m_socketWrite){
        ui->textEdit->append("Client connected to socket.");
    } else{
        ui->textEdit->append("Client is not connected to socket.");
        return;
    }

    ui->textEdit->append(QString("The request for the number of calculations has been sent to the server..."));

    QByteArray dataOut;
    QDataStream out(&dataOut, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << qint64(0);
    out << qint8(3);
    out << quint16(m_receivingPort);
    out.device()->seek(qint64(0));
    out << qint64(dataOut.size() - sizeof(qint64));
    qDebug()<< "size" << dataOut.size();
    m_socketWrite->writeDatagram(dataOut, QHostAddress::LocalHost, 36001);
}

void inputWidget::setReceivingPort(quint16 portR)
{
    m_receivingPort = portR;
}
