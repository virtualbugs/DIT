#include "src/mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QStringList() << "tr_TR" << "en_GB";
    for (const QString &language : uiLanguages) {
        const QString baseName = "DIT_" + language;
        if (translator.load(baseName, "://languages/")) {
            //app.installTranslator(&translator);
            break;
        }
    }
    MainWindow w;
    w.show();
    return app.exec();
}
