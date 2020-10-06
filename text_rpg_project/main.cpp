#include "text_rpg_project.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    text_rpg_project w;
    w.show();
    return a.exec();
}
