//
// Created by lsk on 12/20/23.
//

#include "MiCloudClient.h"

#include <QEventLoop>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QCryptographicHash>

MiCloudClient::MiCloudClient(QObject *parent) : QObject(parent) {
    this->networkAccessManager = new QNetworkAccessManager(this);
}

MiCloudClient::~MiCloudClient() {
    delete this->networkAccessManager;
}

QString MiCloudClient::loginStep1(QString userID) {
    QNetworkCookie cookie;
    cookie.setName("userId");
    cookie.setValue(userID.toUtf8());
    this->networkAccessManager->cookieJar()->insertCookie(cookie);
    QJsonObject resp = get("https://account.xiaomi.com/pass/serviceLogin", {
        QPair<QString, QString>("sid", "xiaomiio"),
        QPair<QString, QString>("_json", "true"),
        QPair<QString, QString>("userId", userID)
    });
    if (!success) {
        return {};
    }
    return resp.find("_sign").value().toString();
}

QString MiCloudClient::loginStep2(QString sign, QString username, QString password) {
    QString passwordHash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toUpper();
    std::initializer_list<QPair<QString, QString>> params;
    if ((!sign.isNull()) && (!sign.isEmpty())) {
        params = {
                QPair<QString, QString>("sid", "xiaomiio"),
                QPair<QString, QString>("hash", passwordHash),
                QPair<QString, QString>("callback", "https://sts.api.io.mi.com/sts"),
                QPair<QString, QString>("qs", "%3Fsid%3Dxiaomiio%26_json%3Dtrue"),
                QPair<QString, QString>("user", username),
                QPair<QString, QString>("_json", "true"),
                QPair<QString, QString>("sign", sign)
        };
    } else {
        params = {
            QPair<QString, QString>("sid", "xiaomiio"),
                    QPair<QString, QString>("hash", passwordHash),
                    QPair<QString, QString>("callback", "https://sts.api.io.mi.com/sts"),
                    QPair<QString, QString>("qs", "%3Fsid%3Dxiaomiio%26_json%3Dtrue"),
                    QPair<QString, QString>("user", username),
                    QPair<QString, QString>("_json", "true"),

        };
    }
    QJsonObject response = post("https://account.xiaomi.com/pass/serviceLoginAuth2", params);
}

QString MiCloudClient::loginStep3(QString region) {
    return QString();
}

QString MiCloudClient::countryToServerURL(QString region) {
    return QString();
}

int MiCloudClient::login(QString userID, QString password, QString region) {
    QString sign = loginStep1(userID);
    if (!success) {
        return -1;
    }
    QString location;
    if (!sign.startsWith("http")) {
        location = loginStep2(sign, userID, password);
    } else {
        location = sign;
    }
    return 0;
}

QList<DeviceInfo> MiCloudClient::queryDevices(QString region) {
    return QList<DeviceInfo>();
}

QJsonObject MiCloudClient::get(QString url,  std::initializer_list<QPair<QString, QString>> args) {
    QJsonDocument doc;
    QUrl qUrl(url);
    qUrl.setQuery(QUrlQuery(args));
    QNetworkRequest request(qUrl);
    QNetworkReply *reply = networkAccessManager->get(request);
    auto *eventLoop = new QEventLoop();
    connect(reply, &QNetworkReply::finished, eventLoop, &QEventLoop::quit);
    if (reply->error() != QNetworkReply::NoError) {
        success = false;
        delete eventLoop;
        delete reply;
        return {};
    }
    doc = QJsonDocument::fromJson(reply->readAll().replace("&&&START&&&", ""));
    if (doc.isNull()) {
        success = false;
        delete eventLoop;
        delete reply;
        return {};
    }
    success = true;
    eventLoop->exec();
    delete eventLoop;
    delete reply;
    return doc.object();
}

QJsonObject MiCloudClient::post(QString url, std::initializer_list<QPair<QString, QString>> args) {
    QJsonDocument doc;
    QUrl qUrl(url);
    QUrlQuery query(args);
    QNetworkRequest request(qUrl);
    QNetworkReply *reply = networkAccessManager->post(request, query.toString(QUrl::FullyEncoded).toUtf8());
    auto *eventLoop = new QEventLoop();
    connect(reply, &QNetworkReply::finished, eventLoop, &QEventLoop::quit);
    if (reply->error() != QNetworkReply::NoError) {
        success = false;
        delete eventLoop;
        delete reply;
        return {};
    }
    doc = QJsonDocument::fromJson(reply->readAll().replace("&&&START&&&", ""));
    if (doc.isNull()) {
        success = false;
        delete eventLoop;
        delete reply;
        return {};
    }
    success = true;
    eventLoop->exec();
    delete eventLoop;
    delete reply;
    return doc.object();
}

