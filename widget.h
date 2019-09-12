#ifndef WIDGET_H
#define WIDGET_H

#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QDesktopWidget>
#include <QSpinBox>
#include <QDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QProcess>
#include <QLineEdit>
#include "workthread.h"
#include "checkdockerthread.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
private:
    int button_width = 150;
    int button_hight = 150;
    int top = 225;
    int screenHeight = 0;
    int screenWidth = 0;
    int windowWidth = 650;
    int windowHeight = 400;
    int roundValue = 0;
    QString cmd;
    QString dockerCmd = "bash /home/wayne/apollo_0807/apollo/modules/autoset/auto_launch_qt.sh";
    QString roundCmd = "docker exec -u wayne apollo_dev_wayne /bin/bash /apollo/modules/autoset/auto_round.sh ";
    QString stopCmd = "docker exec -u wayne apollo_dev_wayne /bin/bash /apollo/modules/autoset/auto_stop_modules.sh";

    QLabel *statusLabel, *contentLabel, *timesLabel, *statusData, *contentData;
    QPushButton *launchBtn, *roundBtn, *stopBtn;
    QDesktopWidget *deskWgt = QApplication::desktop();
    QSpinBox *roundTimesBox;
    QMessageBox msgBox;
    QProcess *process;
    QLineEdit *userName;
    WorkerThread *roundThread;
    CheckDockerThread *dockerThread;

private slots:
    void onRound();
    void checkRoundFileDone();
    void checkDockerFileDone();
    void switchNotificationFuc();

protected:
    void closeEvent(QCloseEvent *event);
    void initialUI();
    void initialDocker();
    void eraseRoundLog();
    void eraseDockerLog();

};

#endif // WIDGET_H
