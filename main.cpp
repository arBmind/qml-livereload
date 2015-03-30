#include <QtGui/QGuiApplication>
#include <QFileSystemWatcher>
#include <QtQuick/QQuickView>
#include <QDebug>
#include <QQmlApplicationEngine>
#include <QThread>
#include <QTimer>

#define QML_FILE "./main.qml"

class LiveReload: public QGuiApplication {
    Q_OBJECT
public:
    LiveReload(int argc, char* argv[]);
    QScopedPointer<QQmlApplicationEngine> engine;
    QFileSystemWatcher watcher;
    QTimer timer;
private slots:
    void fileChanged(const QString & path) {
        qDebug() << "file changed: " << path;
        timer.start();
    };
    void timeout() {
        engine.reset(new QQmlApplicationEngine(QUrl(QML_FILE), this));
    }
};

LiveReload::LiveReload(int argc, char* argv[])
    : QGuiApplication(argc,argv) {
    timer.setSingleShot(true);
    timer.setInterval(50);
    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()));
    watcher.addPath("./");
    watcher.addPath(QML_FILE);
    connect(&watcher, SIGNAL(directoryChanged(const QString &)),
      this, SLOT(fileChanged(const QString &)));
    connect(&watcher, SIGNAL(fileChanged(const QString &)),
      this, SLOT(fileChanged(const QString &)));
    engine.reset(new QQmlApplicationEngine(QUrl(QML_FILE), this));
}

int main(int argc, char* argv[]) {
    LiveReload app(argc,argv);
    return app.exec();
}

#include "main.moc"
