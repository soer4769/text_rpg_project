#include <QtWidgets/QApplication>
#include <QObject>
#include <QSlider>
#include <QProgressBar>

int main (int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget* window = new QWidget; window->setFixedSize(500, 200); window->show();
    QSlider *slider = new QSlider(window); slider->show();
    QProgressBar* progress = new QProgressBar(window); progress->show();

    QObject::connect(slider, SIGNAL(valueChanged(int)), progress, SLOT(setValue(int)));

    return a.exec();
}