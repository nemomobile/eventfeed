
#include <stdio.h>
#include <stdlib.h>

#include <QCoreApplication>
#include "eventfeedservice.h"

void myMessageOutput(QtMsgType type, const char *msg)
{
    switch (type) {
     case QtDebugMsg:
         fprintf(stderr, "Debug: %s\n", msg);
         break;
     case QtWarningMsg:
         fprintf(stderr, "Warning: %s\n", msg);
         break;
     case QtCriticalMsg:
         fprintf(stderr, "Critical: %s\n", msg);
         break;
     case QtFatalMsg:
         fprintf(stderr, "Fatal: %s\n", msg);
         abort();
     }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    qInstallMsgHandler(myMessageOutput);

    EventFeedService* service = new EventFeedService;

    QObject::connect(service, SIGNAL(idle()), &app, SLOT(quit()));

    return app.exec();
}
