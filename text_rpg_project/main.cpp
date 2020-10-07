#include <QtWidgets/QApplication>
#include <QObject>
#include <QSlider>
#include <QPushButton>
#include <QProgressBar>
#include <QMenu>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QMenuBar>

// Skab nyt objekt, ændre dens værdier og vise den på skærmen f.eks. knap:
// QPushButton* button = new QPushButton("Klik mig!", window); button->setGeometry(100, 50, 100, 20); button->show();

// Forbind objekt med objekt f.eks. slider og progress bar:
// QObject::connect(slider, &QSlider::valueChanged, progress, &QProgressBar::setValue);

// Forbind objekt med funktion f.eks. knap og input felt:
// QObject::connect(button, &QPushButton::clicked, [=]() {textline->setPlaceholderText("MIN KAGE!");});

/* Brugbare kilder som kan hjælpe med at lære omkring QT programmering:
* https://wiki.qt.io/Qt_for_Beginners
* https://doc.qt.io/qt-5/qtwidgets-mainwindows-menus-example.html
* https://doc.qt.io/qt-5/qtwidgets-module.html
* https://wiki.qt.io/New_Signal_Slot_Syntax
*/

int main (int argc, char *argv[])
{
    QApplication a(argc, argv);
    QWidget* window = new QWidget; window->setFixedSize(500, 200);
    QSlider* slider = new QSlider(window); slider->setGeometry(0, 50, 50, 50); slider->show();
    QProgressBar* progress = new QProgressBar(window); progress->show();
    QLineEdit* textline = new QLineEdit(window); textline->setReadOnly(true); textline->setPlaceholderText("KAGE!"); textline->show();
    QMenu* button_menu = new QMenu("&File"); button_menu->addAction("Item 1", [=]() {textline->setPlaceholderText("MIN KAGE!"); }); button_menu->addAction("Item 2");
    QPushButton* button = new QPushButton("Klik mig!", window); button->setGeometry(100, 50, 100, 20); /*button->setMenu(button_menu);*/ button->show();
    QBoxLayout* layout = new QHBoxLayout; layout->addWidget(button); layout->addWidget(textline); window->setLayout(layout); window->show();
    QMenuBar* menubar = new QMenuBar(window); menubar->addMenu(button_menu); menubar->show();

    QObject::connect(slider, &QSlider::valueChanged, progress, &QProgressBar::setValue);
    QObject::connect(button, &QPushButton::clicked, [=]() {textline->setPlaceholderText("MIN KAGE!");});

    return a.exec();
}