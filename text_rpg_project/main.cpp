// QT Biblioteker
#include <QtWidgets/QApplication>
#include <QObject>
#include <QSlider>
#include <QPushButton>
#include <QProgressBar>
#include <QMenu>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QGroupBox>
#include <QTabWidget>
#include <QDebug>
#include <QTextEdit>
#include <QLabel>
#include <QString>
#include <QTextCodec>
#include <QtQml/QQmlEngine>

// Standard C++ Biblioteker
#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>

// Konstanter og globale variabler
using namespace std;

// Primær kode
class text_rpg {

    // Qt indhold
    QFlags<Qt::WindowType> window_flags = Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint;
    QWidget* window = new QWidget;
    QPushButton* window_button_close = new QPushButton("Luk vindue");
    QTabWidget* tabs_actionsmenu = new QTabWidget;
    QWidget* tab_inventory = new QWidget;
    QWidget* tab_stats = new QWidget; QBoxLayout* tab_stats_layout = new QHBoxLayout(); QLabel* tab_stats_player_life = new QLabel;
    QWidget* tab_combat = new QWidget;
    QWidget* tab_settings = new QWidget;
    QWidget* player_name_widget = new QWidget; QBoxLayout* player_name_layout = new QHBoxLayout;
    QLineEdit* player_name_line = new QLineEdit;
    QPushButton* player_name_accept = new QPushButton("Accept");
    QTextEdit* text_browser = new QTextEdit;
    QPushButton* choice_button_01 = new QPushButton("Valg 1");
    QPushButton* choice_button_02 = new QPushButton("Valg 2");
    QBoxLayout* choice_button_layout = new QHBoxLayout;
    QGroupBox* gbox_l = new QGroupBox("Vindue"); QBoxLayout* gbox_l_layout = new QVBoxLayout;
    QGroupBox* gbox_h = new QGroupBox("Actions"); QBoxLayout* gbox_h_layout = new QVBoxLayout;
    QBoxLayout* layout_l = new QVBoxLayout;
    QBoxLayout* layout_h = new QVBoxLayout;
    QBoxLayout* layout_global = new QHBoxLayout;

    void text_browser_append(std::string append_string) {
        this->text_browser->append(QString::fromStdString(append_string));
    }

    void set_choice_button_method(QPushButton* choice_button, QString execute_string) {
        QObject::connect(choice_button, &QPushButton::clicked, [=]() {QJSEngine engine; engine.evaluate(execute_string); });
    }

    void qt_setup() {
        window->setFixedSize(800, 600); window->setWindowFlags(window_flags); // QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
        tab_stats->setLayout(tab_stats_layout); tab_stats_layout->addWidget(tab_stats_player_life); tab_stats_player_life->setText("Health: " + QString::number(player_life));
        tabs_actionsmenu->insertTab(0, tab_settings, "Settings"); tabs_actionsmenu->insertTab(0, tab_combat, "Combat");
        tabs_actionsmenu->insertTab(0, tab_stats, "Stats"); tabs_actionsmenu->insertTab(0, tab_inventory, "Inventory");
        tabs_actionsmenu->setCurrentIndex(0);
        text_browser->setReadOnly(true);
        choice_button_layout->addWidget(choice_button_01); choice_button_layout->addWidget(choice_button_02);
        gbox_l_layout->addWidget(text_browser); gbox_l_layout->addLayout(choice_button_layout); gbox_l->setLayout(gbox_l_layout);
        gbox_h_layout->addWidget(tabs_actionsmenu); gbox_h_layout->addWidget(window_button_close); gbox_h->setLayout(gbox_h_layout);
        layout_l->addWidget(gbox_l); layout_l->addWidget(gbox_h);
        layout_global->addLayout(layout_l); layout_global->addLayout(layout_h);
        window->setLayout(layout_global);
        player_name_widget->setWindowTitle("Vælg Navn!"); player_name_widget->setLayout(player_name_layout); player_name_layout->addWidget(player_name_line); player_name_layout->addWidget(player_name_accept);
        player_name_widget->setWindowFlags(window_flags); player_name_widget->show();

        QObject::connect(window_button_close, &QPushButton::clicked, [=]() {window->close(); });
        QObject::connect(player_name_accept, &QPushButton::clicked, [=]() {player_name = player_name_line->text().toLocal8Bit().constData(); player_name_widget->close(); window->show(); text_browser_append(player_name); storyLine(); });
    }

    // Andet indhold
    int player_life = 100;
    int enemy_life = 100;
    int current_choice = 0;
    string player_name = "test";
    int rng = rand() % 100 + 1;

    int damage() {
        if (rng <= 5) {
            rng = rng * 2; // critical damage
            return rng;
        } else if (rng >= 6) {
            return rng;
        }
    }

    void combat_system() {
        
        while (player_life != 0 || enemy_life != 0) {
            cout << player_name << " angriber! Du forårsager " << damage() << " skade!" << endl;
            enemy_life -= rng;
            Sleep(2);
            cout << "fjende angriber! Den forårsager " << (damage()/65) << " skade!" << endl;
            player_life -= rng;
            Sleep(2);
            combat_system();

        if (player_life == 0) {
            cout << "You died - GAME OVER" << endl;
            // Her skal historien genstartestest
        } else if (enemy_life == 0) {
            cout << "Enemy has been slain! YOU WON" << endl;
                }
            }
    }

    void storyLine() {
        readLine("||-- Velkomst --||", "||-- Introduktion --||");
        readLine("||-- Introduktion --||", "||-- Acquired Items --||");
        readLine("-- Story 1 --", "-- Story 2 --");
        storyChoice();
    }

    void storyChoice() {
        switch (current_choice) {
        case 0:
            readLine("-- Valg 1 --", "-- Valg 2 --");
            readLine("-- Valg 2 --", "-- Valg 3 --");
            choice_button_01->setText("Sove videre");
            choice_button_02->setText("Stå op");
            QObject::connect(choice_button_01, &QPushButton::clicked, [=]() { readLine("-- Story 2 --", "-- Story 3 --"); current_choice = 1; storyChoice(); });
            QObject::connect(choice_button_02, &QPushButton::clicked, [=]() { readLine("-- Story 3 --", "-- Story 4 --"); current_choice = 2; storyChoice(); });
            break;
        case 1:
            readLine("-- Valg 3 --", "-- Valg 4 --");
            readLine("-- Valg 4 --", "-- Valg 5 --");
            choice_button_01->setText("Spring op af seng");
            choice_button_02->setText("Grib riffel");
            QObject::connect(choice_button_01, &QPushButton::clicked, [=]() { readLine("-- Story 4 --", "-- Story 5 --"); current_choice = 3; storyChoice(); });
            QObject::connect(choice_button_02, &QPushButton::clicked, [=]() { readLine("-- Story 5 --", "-- Story 6 --"); current_choice = 4; storyChoice(); });
            break;
        case 2:
            text_browser_append("kage!");
            break;
        }
    }

    void readLine(string read_from, string read_to) {
        int text_file_size = 200;
        int start_stop = 0;
        string line;
        ifstream myfile("dialog.txt");
        if (myfile.is_open()) {

            int line_number = 1;
            int line_number2 = 1;

            while (line_number != text_file_size && getline(myfile, line)) {
                if (line == read_from) {
                    start_stop = 1;
                }

                if (start_stop == 1) {
                    while (line_number2 != text_file_size && getline(myfile, line)) {
                        if (line == read_to) {
                            start_stop = 0;
                            break;
                        }
                        text_browser_append(line);
                    }
                }

                line_number++;
            }

            myfile.close();
        }
        
        qDebug("Unable to open file");
    }


public:
    text_rpg() {
        qt_setup();
        // text_browser_append(readLine([Indsæt linje]))
        //readLine(2);
    }
};

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
* https://doc.qt.io/qt-5/qtwidgets-index.html
* https://wiki.qt.io/New_Signal_Slot_Syntax
* https://doc.qt.io/qt-5/stylesheet-reference.html
* https://doc.qt.io/Qt-5/stylesheet-syntax.html
*/

// BACKUP KODE - SLET IKKE
//QWidget* window = new QWidget; window->setFixedSize(500, 200);
//QSlider* slider = new QSlider(window); slider->setGeometry(0, 50, 50, 50); slider->show();
//QProgressBar* progress = new QProgressBar(window); progress->show();
//QLineEdit* textline = new QLineEdit(window); textline->setReadOnly(true); textline->setPlaceholderText("KAGE!"); textline->show();
//QMenu* button_menu = new QMenu("&File"); button_menu->addAction("Item 1", [=]() {textline->setPlaceholderText("MIN KAGE!"); }); button_menu->addAction("Item 2");
//QPushButton* button = new QPushButton("Klik mig!", window); button->setGeometry(100, 50, 100, 20); /*button->setMenu(button_menu);*/ button->show();
//QBoxLayout* layout = new QHBoxLayout; layout->addWidget(button); layout->addWidget(textline); window->setLayout(layout); window->show();
//QMenuBar* menubar = new QMenuBar(window); menubar->addMenu(button_menu); menubar->show();

//QObject::connect(slider, &QSlider::valueChanged, progress, &QProgressBar::setValue);
//QObject::connect(button, &QPushButton::clicked, [=]() {textline->setPlaceholderText("MIN KAGE!");});

int main (int argc, char *argv[])
{
    QApplication a(argc, argv);
    text_rpg game;
    return a.exec();
}