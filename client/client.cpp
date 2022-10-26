#include <iostream>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "ChatClientApp.h"
#include "Logger.h"

int main(int argc, char **argv)
{
  core::Logger::init("server_log");

  std::string host = "127.0.0.1", port = "4026";
  if (argc == 3)
  {
    host = argv[1];
    port = argv[2];
  }

  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;

  qmlRegisterType<client::ChatModel>("models", 1, 0, "ChatModel");

  qRegisterMetaType<core::ChatEvent>();
  qmlRegisterUncreatableType<core::ChatEvent>("enums", 1, 0, "ChatEvent",
                                              "Not creatable as it is an enum type.");

  qmlRegisterType<client::ChatClientApp>("client", 1, 0, "ChatClientApp");


  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                   &app, [url](QObject *obj, const QUrl &objUrl)
                   {
                     if (!obj && url == objUrl)
                       QCoreApplication::exit(-1);
                   }, Qt::QueuedConnection);


  engine.load(url);

  return app.exec();
}
