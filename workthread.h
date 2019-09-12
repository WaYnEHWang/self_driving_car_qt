#ifndef WORKTHREAD_H
#define WORKTHREAD_H
#include <QApplication>
#include <QThread>
#include <QFile>
#include <QDebug>
#include <QTime>
#include <QElapsedTimer>

class WorkerThread : public QThread
{
    Q_OBJECT
public:
    WorkerThread(QObject *parent = 0) : QThread(parent){ }
protected:
    void run(){
        QFile roundFile("round_output.txt");
        roundFile.open(QFile::WriteOnly|QFile::Truncate);
        roundFile.close();
        firstTime = false;
        roundOne = false;
        autoPilot = false;
        while(1)
        {
            QElapsedTimer t;
            t.start();
            while(t.elapsed()<1000) QCoreApplication::processEvents();

            QFile file("round_output.txt");
            if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                qDebug() << "Can't open the file.";
            }
            while(!file.atEnd())
            {
                QByteArray line = file.readLine();
                QString str(line);
                if(str.contains("finish")) {
                    qDebug() << str;
                    emit done();
                }else if (str.contains("Round 1")) {
                    roundOne = true;
                }else if (str.contains("auto pilot start done")) {
                    autoPilot = true;
                }
            }
            file.close();

            if (!firstTime && roundOne && autoPilot) {
                emit switchNotification();
                firstTime = true;
                roundOne = false;
                autoPilot = false;
                qDebug() << "send switch Notification.";
            }
        }
    }
signals:
    void done();
    void switchNotification();
private:
    bool roundOne, autoPilot, firstTime;
};

#endif // WORKTHREAD_H
