#include <iostream>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "ChatClient.h"

int main(int argc, char ** argv) {
    std::string host = "127.0.0.1", port = "4026";
    if (argc == 3) {
        host = argv[1];
        port = argv[2];
    }
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    ChatClient::getInstance();
//    QObject::connect(g_auth, &Auth::loggingOut,
//                     g_xmpp, &Xmpp::logout);

    qmlRegisterType<ChatModel>("models", 1, 0, "ChatModel");

    qmlRegisterUncreatableType<ChatEvent>("enums", 1, 0, "ChatEvent",
                                          "Not creatable as it is an enum type.");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
                if (!obj && url == objUrl)
                    QCoreApplication::exit(-1);
            }, Qt::QueuedConnection);


    engine.load(url);

    return app.exec();
}
