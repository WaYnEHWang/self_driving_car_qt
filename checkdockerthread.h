#ifndef CHECKDOCKERTHREAD_H
#define CHECKDOCKERTHREAD_H

#include <QApplication>
#include <QThread>
#include <QFile>
#include <QDebug>
#include <QTime>
#include <QElapsedTimer>

class CheckDockerThread : public QThread
{
    Q_OBJECT
public:
    CheckDockerThread(QObject *parent = 0) : QThread(parent){ }
protected:
    void run(){

        while(1)
        {
            QElapsedTimer t;
            t.start();
            while(t.elapsed()<1000) QCoreApplication::processEvents();

            QFile file("launch_output.txt");
            if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                qDebug() << "Can't open the file.";
            }
            while(!file.atEnd())
            {
                QByteArray line = file.readLine();
                QString str(line);
                stopped = str.contains("turn Canbus on");
                if(stopped) {
                    qDebug() << str;
                    emit done();
                }
            }
            file.close();

        }
    }
signals:
    void done();
private:
    bool stopped = false;
};
#endif // CHECKDOCKERTHREAD_H
