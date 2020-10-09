// Qt Biblioteker
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
#include <algorithm>
#include <Windows.h>

// Konstanter og globale variabler
using namespace std;

// Erstatter alt tekst i en streng med noget andet
void string_replace_all_instances (std::string &string_used, std::string string_search, std::string string_replace) {
    size_t index = 0;
    int search_length = string_search.length();

    while (true) {
        index = string_used.find(string_search, index);

        if (index == std::string::npos) {
            break;
        }

        else {
            string_used.replace(index, search_length, string_replace);
            index += search_length;
        }
    }
}

// Primær kode
class text_rpg {

    // Overordnet variabler
    std::string player_name, player_area = "Test";
    short window_size_x = 800, window_size_y = 600;
    int player_life = 100, player_gold = 50, player_armour = 0, player_mana = 0, enemy_life = 100, current_choice = 0, rng = rand() % 100 + 1;

    // Qt indhold
    QFlags<Qt::WindowType> window_flags = Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint;
    QWidget* window = new QWidget;
    QPushButton* window_button_close = new QPushButton("Luk vindue");
    QTabWidget* tabs_actionsmenu = new QTabWidget;
    QWidget* tab_inventory = new QWidget;
    QWidget* tab_stats = new QWidget; 
    QBoxLayout* tab_stats_layout = new QHBoxLayout();
    QLabel* tab_stats_player_life = new QLabel;
    QWidget* tab_combat = new QWidget;
    QWidget* tab_settings = new QWidget;
    QWidget* tab_achievements = new QWidget;
    QWidget* player_name_widget = new QWidget; 
    QBoxLayout* player_name_layout = new QHBoxLayout;
    QLineEdit* player_name_line = new QLineEdit;
    QPushButton* player_name_accept = new QPushButton("Accept");
    QLabel* label_player_lifes = new QLabel;
    QLabel* label_player_gold = new QLabel;
    QLabel* label_player_armour = new QLabel;
    QLabel* label_player_mana = new QLabel;
    QLabel* label_player_area = new QLabel;
    QBoxLayout* label_player_layout = new QHBoxLayout;
    QBoxLayout* label_browser_layout = new QHBoxLayout;
    QTextEdit* text_browser = new QTextEdit;
    QPushButton* choice_button_01 = new QPushButton("Valg 1");
    QPushButton* choice_button_02 = new QPushButton("Valg 2");
    QBoxLayout* choice_button_layout = new QHBoxLayout;
    QGroupBox* gbox_l = new QGroupBox("Spilområde"); 
    QBoxLayout* gbox_l_layout = new QVBoxLayout;
    QGroupBox* gbox_h = new QGroupBox("Handlinger"); 
    QBoxLayout* gbox_h_layout = new QVBoxLayout;
    QBoxLayout* layout_global = new QHBoxLayout;

    void text_browser_append(std::string append_string) {
        this->text_browser->append(QString::fromStdString(append_string));
    }

    template <typename TypeLambda>
    void connect_choice_button(QPushButton* choice_button, TypeLambda &lambda) {
        QObject::connect(choice_button, &QPushButton::clicked, [=](){ lambda(); });
    }

    void disconnect_choice_button(QPushButton* choice_button) {
        QObject::disconnect(choice_button, 0, 0, 0);
    }

    void set_label_text_colour(QLabel* streng_label, std::string streng_front, std::string &streng_content, std::string streng_colour){
        streng_label->setText(QString::fromStdString(streng_front + "<font color=\"" + streng_colour + "\">" + streng_content + "</font>"));
    }

    void qt_setup() {
        window->setFixedSize(window_size_x, window_size_y); window->setWindowFlags(window_flags); // QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
        tab_stats->setLayout(tab_stats_layout); tab_stats_layout->addWidget(tab_stats_player_life);
        tabs_actionsmenu->insertTab(0, tab_settings, "Indstillinger"); tabs_actionsmenu->insertTab(0, tab_achievements, "Præsentationer"); tabs_actionsmenu->insertTab(0, tab_combat, "Kamp");
        tabs_actionsmenu->insertTab(0, tab_stats, "Statistikker"); tabs_actionsmenu->insertTab(0, tab_inventory, "Inventar");
        tabs_actionsmenu->setCurrentIndex(0);
        text_browser->setReadOnly(true);
        choice_button_layout->addWidget(choice_button_01); choice_button_layout->addWidget(choice_button_02);
        label_player_layout->addWidget(label_player_lifes); label_player_layout->addWidget(label_player_gold); label_player_layout->addWidget(label_player_armour); label_player_layout->addWidget(label_player_mana);
        label_browser_layout->addWidget(label_player_area); label_browser_layout->addLayout(label_player_layout); set_label_text_colour(label_player_area, "Område: ", player_area, "black");
        set_label_text_colour(label_player_lifes, "Liv: ", std::to_string(player_life), "red"); set_label_text_colour(label_player_gold, "Guld: ", std::to_string(player_gold), "orange");
        set_label_text_colour(label_player_armour, "Armor: ", std::to_string(player_armour), "darkgreen"); set_label_text_colour(label_player_mana, "Mana: ", std::to_string(player_mana), "blue");
        gbox_l_layout->addLayout(label_browser_layout); gbox_l_layout->addWidget(text_browser); gbox_l_layout->addLayout(choice_button_layout); gbox_l->setLayout(gbox_l_layout);
        gbox_h_layout->addWidget(tabs_actionsmenu); gbox_h_layout->addWidget(window_button_close); gbox_h->setLayout(gbox_h_layout);
        layout_global->addWidget(gbox_l); layout_global->addWidget(gbox_h);
        window->setLayout(layout_global);
        player_name_widget->setWindowTitle("Vælg Navn!"); player_name_widget->setLayout(player_name_layout); player_name_layout->addWidget(player_name_line); player_name_layout->addWidget(player_name_accept);
        player_name_widget->setWindowFlags(window_flags); player_name_widget->show();

        QObject::connect(window_button_close, &QPushButton::clicked, [=]() {window->close(); });
        QObject::connect(player_name_accept, &QPushButton::clicked, [=]() {player_name = player_name_line->text().toLocal8Bit().constData(); player_name_widget->close(); window->show(); storyLine(); });
    }

    // Andet indhold
    int damage() {
        return (rng <= 5 ? rng * 2 : rng); // critical damage hvis rng under eller lig fem
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
            }
            
            else if (enemy_life == 0) {
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
        disconnect_choice_button(choice_button_01);
        disconnect_choice_button(choice_button_02);

        switch (current_choice) {

        // Vil du sove videre? || Vil du stå op?
        case 0:
            readLine("-- Valg 1 --", "-- Valg 2 --");
            readLine("-- Valg 2 --", "-- Valg 3 --");

            choice_button_01->setText("Sove videre");
            choice_button_02->setText("Stå op");

            connect_choice_button(choice_button_01, [=](){ readLine("-- Story 2 --", "-- Story 3 --"); current_choice = 1; storyChoice(); });
            connect_choice_button(choice_button_02, [=](){ readLine("-- Story 3 --", "-- Story 4 --"); current_choice = 2; storyChoice(); });
            break;

        // Springer op af sengen og løber || Griber riffel
        case 1:
            readLine("-- Valg 3 --", "-- Valg 4 --");
            readLine("-- Valg 4 --", "-- Valg 5 --");

            choice_button_01->setText("Spring op af seng");
            choice_button_02->setText("Grib riffel");

            connect_choice_button(choice_button_01, [=](){ readLine("-- Story 4 --", "-- Story 5 --"); current_choice = 3; storyChoice(); });
            connect_choice_button(choice_button_02, [=](){ readLine("-- Story 5 --", "-- Story 6 --"); current_choice = 4; storyChoice(); });
            break;

        // Vil du stå op? - JA -> Griber riffel || Du spurter ud af bagdøren
        case 2:
            readLine("-- Valg 4 --", "-- Valg 5 --");
            readLine("-- Valg 5 --", "-- Valg 6 --");

            choice_button_01->setText("Grib riffel");
            choice_button_02->setText("Spurt ud af bagdøren");

            connect_choice_button(choice_button_01, [=](){ readLine("-- Story 5 --", "-- Story 6 --"); current_choice = 4; storyChoice(); }); //current_choice skal ikke være 1
            connect_choice_button(choice_button_02, [=](){ readLine("-- Story 4 --", "-- Story 5 --"); current_choice = 3; storyChoice(); });
            break;
        
        // Finder økse || løber til bil
        case 3:
            readLine("-- Valg 6 --", "-- Valg 7 --");
            readLine("-- Valg 7 --", "-- Valg 8 --");

            choice_button_01->setText("Tag den skarbeste øske fra skuret");
            choice_button_02->setText("Kør ind mod byen");

            connect_choice_button(choice_button_01, [=](){ readLine("-- Story 6 --", "-- Story 7 --"); current_choice = 5; storyChoice(); });
            connect_choice_button(choice_button_02, [=](){ readLine("-- Story 7.1 --", "-- Story 8 --"); current_choice = 6; storyChoice(); });

        // printer en del tekst, derefter - Du går nærmere || Vender bilen og kører væk
        case 4:
            // Burde meget gerne printe noget tekst der ikke er blevet printet
            readLine("-- Story 7 --", "-- Story 7.1 --");
            readLine("-- Story 7.1 --", "-- Story 8 --");

            readLine("-- Valg 10 --", "-- Valg 11 --");
            readLine("-- Valg 11 --", "-- Valg 12 --");

            choice_button_01->setText("Stig ud af blien og gå lidt nærmere");
            choice_button_02->setText("Vend blien og kør med fuld fart væk");

            connect_choice_button(choice_button_01, [=](){ readLine("-- Story 8 --", "-- Story 9 --"); current_choice = 7; storyChoice(); });
            connect_choice_button(choice_button_02, [=](){ readLine("-- Story 9 --", "-- Story 10 --"); current_choice = 8; storyChoice(); });

            // Mangler case 5, 6, 7, 8
        default:
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

                        string_replace_all_instances(line, "$name", player_name);
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