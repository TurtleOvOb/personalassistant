#include "ui/personalassistant.h"
#include <QtWidgets/QApplication>
#include<qstylefactory.h>
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("fusion"));
    QFont font("Microsoft YaHei UI");
    //QFont font("PingFang SC");
    qSetMessagePattern("[%{file}:%{line}] %{message}");
    app.setFont(font);
    personalassistant window;
    window.show();
    return app.exec();
}
