//
// Created by lsk on 12/20/23.
//

#ifndef YEELIGHT_BACKEND_YEELIGHTDBUSSERVICE_H
#define YEELIGHT_BACKEND_YEELIGHTDBUSSERVICE_H

#include <QObject>

class YeeLightDBusService : public QObject{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.lsk.yeelightcontroller")
public:
    YeeLightDBusService(QObject *parent = nullptr);
    ~YeeLightDBusService();
public slots:
    int login(QString username, QString password, QString region);
    int isLogin();

};


#endif //YEELIGHT_BACKEND_YEELIGHTDBUSSERVICE_H
