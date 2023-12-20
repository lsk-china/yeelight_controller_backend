//
// Created by lsk on 12/20/23.
//

#ifndef YEELIGHT_BACKEND_MICLOUDCLIENT_H
#define YEELIGHT_BACKEND_MICLOUDCLIENT_H

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QJsonObject>

struct DeviceInfo {
    QString name;
    QString did;
    QString token;
    QString ip;
    bool isOnline;
};

class MiCloudClient : QObject{
    Q_OBJECT
public:
    explicit MiCloudClient(QObject *parent = nullptr);
    ~MiCloudClient() override;

    int login(QString userID, QString password, QString region);
    QList<DeviceInfo> queryDevices(QString region);

private:
    QNetworkAccessManager *networkAccessManager;
    QString userID;
    QString serviceToken;
    QString ssecurity;
    QString cuserId;
    QString passToken;
    bool success = false;

    QString loginStep1(QString userID);
    QString loginStep2(QString sign, QString username, QString password);
    QString loginStep3(QString region);
    QString countryToServerURL(QString region);
    QJsonObject get(QString url, std::initializer_list<QPair<QString, QString>> args);
    QJsonObject post(QString url, std::initializer_list<QPair<QString, QString>> args);
};


#endif //YEELIGHT_BACKEND_MICLOUDCLIENT_H
