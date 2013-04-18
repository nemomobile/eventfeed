
#include <stdio.h>
#include <stdlib.h>

#include <QCoreApplication>
#include "eventfeedservice.h"

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
#define PRINTABLE(a) a
void myMessageOutput(QtMsgType type, const char *msg)
#else
#define PRINTABLE(a) qPrintable(a)
void myMessageOutput(QtMsgType type, const QMessageLogContext&, const QString& msg)
#endif
{
    switch (type) {
     case QtDebugMsg:
         fprintf(stderr, "Debug: %s\n", PRINTABLE(msg));
         break;
     case QtWarningMsg:
         fprintf(stderr, "Warning: %s\n", PRINTABLE(msg));
         break;
     case QtCriticalMsg:
         fprintf(stderr, "Critical: %s\n", PRINTABLE(msg));
         break;
     case QtFatalMsg:
         fprintf(stderr, "Fatal: %s\n", PRINTABLE(msg));
         abort();
     }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
    qInstallMsgHandler(myMessageOutput);
#else
    qInstallMessageHandler(myMessageOutput);
#endif

    EventFeedService* service = new EventFeedService;

    QObject::connect(service, SIGNAL(idle()), &app, SLOT(quit()));

    return app.exec();
}
