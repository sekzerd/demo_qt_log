#include <QCoreApplication>
#include <QMutex>
#include <QThread>
#include <QtConcurrent>
#include <QtDebug>

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();
    static QFile logFile("log.txt");
    if (!logFile.isOpen()) {
        logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    }
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
    mutex.unlock();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qInstallMessageHandler(customMessageHandler);
    QThreadPool::globalInstance()->setMaxThreadCount(4);
    QtConcurrent::run(QThreadPool::globalInstance(), []() {
        while (true) {
            qDebug() << "running ID:" << QThread::currentThreadId();
            QThread::msleep(1);
        }
    });
    QtConcurrent::run(QThreadPool::globalInstance(), []() {
        while (true) {
            qDebug() << "running ID:" << QThread::currentThreadId();
            QThread::msleep(1);
        }
    });
    QtConcurrent::run(QThreadPool::globalInstance(), []() {
        while (true) {
            qDebug() << "running ID:" << QThread::currentThreadId();
            QThread::msleep(1);
        }
    });
    QtConcurrent::run(QThreadPool::globalInstance(), []() {
        while (true) {
            qDebug() << "running ID:" << QThread::currentThreadId();
            QThread::msleep(1);
        }
    });
    return a.exec();
}
