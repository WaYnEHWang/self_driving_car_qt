#include "widget.h"
#include "workthread.h"
#include <QMessageBox>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QByteArray>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    eraseDockerLog();
    initialUI();
    initialDocker();
}

void Widget::onRound() {

    eraseRoundLog();
    if(process->state() == QProcess::Running) {
        process->close();
    }
    roundValue = roundTimesBox->value();
    qDebug() << roundCmd + QString::number(roundValue);
    process->setStandardOutputFile("round_output.txt");
    process->start(roundCmd + QString::number(roundValue));
    if (!process->waitForStarted()) qDebug() << "process starting failure!";
    roundBtn->setEnabled(false);
    statusData->setText(tr("Setting..."));
}

void Widget::closeEvent(QCloseEvent *event)
{
    switch(QMessageBox::warning(NULL,tr("Warning"),
       tr("Are you sure to leave the App?"),QMessageBox::Yes|QMessageBox::Cancel)) {
        case QMessageBox::Yes:
            if(process->state() == QProcess::Running) {
                process->close();
            }
            qDebug() << stopCmd;
            statusData->setText(tr("Stopping modules now..."));
            process->setStandardOutputFile("stop_output.txt");
            process->start(stopCmd);
            process->waitForFinished(6000);
            process->close();
            event->accept();
            break;
    case QMessageBox::Cancel:
        event->ignore();
            break;
    }
}

void Widget::initialUI()
{
    //window
    if (deskWgt) {
        screenHeight = deskWgt->height();
        screenWidth = deskWgt->width();
    }
    resize(windowWidth,windowHeight);
    move(screenWidth/2 - windowWidth/2, screenHeight/2 - windowHeight/2);
    setWindowTitle(tr("Auto Set Appliction"));

    //label
    statusLabel = new QLabel(this);
    contentLabel = new QLabel(this);
    timesLabel = new QLabel(this);
    statusData = new QLabel(this);
    contentData = new QLabel(this);

    statusLabel->setText(tr("Status:"));
    //contentLabel->setText(tr("Contents:"));
    timesLabel->setText(tr("Times:"));

    statusLabel->setFont(QFont("Times",12));
    contentLabel->setFont(QFont("Times",12));
    timesLabel->setFont(QFont("Times",12));
    statusData->setFont(QFont("Times",12));

    statusLabel->setGeometry(0,0,60,20);
    contentLabel->setGeometry(0,100,80,20);
    timesLabel->setGeometry(250,200,60,20);
    statusData->setGeometry(60,0,windowWidth-120,80);
    contentData->setGeometry(80,100,windowWidth-160,60);

    //statusLabel->setStyleSheet("border:2px solid red;");
    //contentLabel->setStyleSheet("border:2px solid red;");
    //timesLabel->setStyleSheet("border:2px solid red;");
    //statusData->setStyleSheet("border:2px solid red;");
    //contentData->setStyleSheet("border:2px solid red;");
    //l1->setAlignment(Qt::AlignCenter);   //label文字置中

    //button

    roundBtn = new QPushButton(this);
    roundBtn->setGeometry(250,top,button_width,button_hight);
    roundBtn->setIcon(QIcon("run.png"));
    roundBtn->setIconSize(QSize(150,150));
    connect(roundBtn,SIGNAL(clicked()),this,SLOT(onRound()));
    roundBtn->setEnabled(false);

    //spinbox
    roundTimesBox = new QSpinBox(this);
    roundTimesBox->setGeometry(320,195,50,30);
    roundTimesBox->setRange(1, 10);
    roundTimesBox->setValue(1);

    // process
    process = new QProcess();

    //thread
    roundThread = new WorkerThread();
    dockerThread = new CheckDockerThread();
    dockerThread->start();
    connect(roundThread,SIGNAL(done()),this,SLOT(checkRoundFileDone()));
    connect(dockerThread,SIGNAL(done()),this,SLOT(checkDockerFileDone()));
    connect(roundBtn,SIGNAL(clicked()),roundThread,SLOT(start()));
    connect(roundThread,SIGNAL(switchNotification()),this,SLOT(switchNotificationFuc()));

}

void Widget::initialDocker()
{

    qDebug() << dockerCmd;
    process->setStandardOutputFile("launch_output.txt");
    process->start(dockerCmd);
    if (!process->waitForStarted()) qDebug() << "process starting failure!";
    statusData->setText(tr("Initialize..."));
}

void Widget::checkRoundFileDone()
{
    if(roundThread->isRunning()){
        roundThread->terminate();
    }
    roundBtn->setEnabled(true);
    process->close();
    statusData->setText(tr("Car is stopped. You can turn off the switch of VCU."));

}

void Widget::checkDockerFileDone()
{
    if(dockerThread->isRunning()){
        dockerThread->terminate();
    }
    roundBtn->setEnabled(true);
    process->close();
    statusData->setText(tr("You can press the button now."));

}

void Widget::eraseRoundLog(){

    QFile roundFile("round_output.txt");
    roundFile.open(QFile::WriteOnly|QFile::Truncate);
    roundFile.close();
}

void Widget::eraseDockerLog(){
    QFile launchFile("launch_output.txt");
    launchFile.open(QFile::WriteOnly|QFile::Truncate);
    launchFile.close();
}

void Widget::switchNotificationFuc(){
statusData->setText(tr("Now, You can turn on the switch of VCU and car will go."));
}
