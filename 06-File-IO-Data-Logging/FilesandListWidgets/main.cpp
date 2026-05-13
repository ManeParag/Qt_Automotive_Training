
#include "widget.h"

#include <QApplication>
#include <QtSql/QSqlDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return QCoreApplication::exec();
}
