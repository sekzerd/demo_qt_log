#include <QCoreApplication>
#include <QThread>
#include <QtConcurrent>
#include <QtDebug>

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QFile logFile("log.txt");
    if (!logFile.isOpen())
        logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream out(&logFile);
    QString buffer;
    switch (type) {
    case QtDebugMsg:
        buffer += "[Debug]: ";
        break;
    case QtInfoMsg:
        buffer += "[Info]: ";
        break;
    case QtWarningMsg:
        buffer += "[Warning]: ";
        break;
    case QtCriticalMsg:
        buffer += "[Critical]: ";
        break;
    case QtFatalMsg:
        buffer += "[Fatal]: ";
        break;
    }

    buffer += QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");

    buffer += msg;
    buffer += " (";
    buffer += QString(context.file);
    buffer += ":";
    buffer += QString::number(context.line);
    buffer += ") ";
    out << buffer << endl;
    qDebug() << buffer;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qInstallMessageHandler(customMessageHandler);

    QtConcurrent::run([]() {
        while (true) {
            qDebug() << "running";
            QThread::msleep(1000);
        }
    });
    return a.exec();
}
