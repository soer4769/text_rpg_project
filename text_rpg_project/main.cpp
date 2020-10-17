// Qt Biblioteker
#include <QtWidgets/QApplication>
#include <QObject>
#include <QPushButton>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTabWidget>
#include <QDebug>
#include <QTextEdit>
#include <QLabel>
#include <QString>
#include <QCheckBox>
#include <QTextCodec>
#include <QComboBox>
#include <QSpinBox>

// Standard C++ Biblioteker
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdarg>
#include <Windows.h>

// Konstanter og globale variabler
using namespace std;
class text_rpg;

class text_rpg_qt {

    // Overordnet variabler
    int window_size_x, window_size_y, setting_current_row = 0;
    std::vector<std::pair<QString, float>> player_inventory, player_stats, player_achievements;

    // Qt - Window rpg game
    QFlags<Qt::WindowType> window_flags = Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint;
    QWidget* window = new QWidget;
    QPushButton* window_button_close = new QPushButton("Luk vindue");
    
    // Qt - Window player name
    QWidget* player_name_widget = new QWidget;
    QBoxLayout* player_name_layout = new QHBoxLayout;
    QLineEdit* player_name_line = new QLineEdit;
    QPushButton* player_name_accept = new QPushButton("Vælg");

    // Qt - Tabs
    QTabWidget* tabs_actionsmenu = new QTabWidget;
    QWidget* tab_inventory = new QWidget;
    QWidget* tab_stats = new QWidget;
    QWidget* tab_combat = new QWidget;
    QWidget* tab_settings = new QWidget;
    QWidget* tab_achievements = new QWidget;

    // Qt - Tab content
    QLayout* layout_tab_inventory = new QHBoxLayout;
    QLabel* tab_inventory_label = new QLabel;
    QLayout* layout_tab_stats = new QHBoxLayout;
    QLabel* tab_stats_label = new QLabel;
    QGridLayout* layout_tab_settings = new QGridLayout;
    QLayout* layout_tab_achievements = new QHBoxLayout;
    QLabel* tab_achievements_label = new QLabel;

    // Qt - Text browser area
    QBoxLayout* label_player_layout = new QHBoxLayout;
    QBoxLayout* label_browser_layout = new QHBoxLayout;
    QLabel* label_player_lifes = new QLabel;
    QLabel* label_player_gold = new QLabel;
    QLabel* label_player_armour = new QLabel;
    QLabel* label_player_mana = new QLabel;
    QLabel* label_player_area = new QLabel;
    QTextEdit* text_browser = new QTextEdit;
    QBoxLayout* choice_button_layout = new QHBoxLayout;

    // Qt - Global box layout
    QGroupBox* gbox_l = new QGroupBox("Spilområde");
    QBoxLayout* gbox_l_layout = new QVBoxLayout;
    QGroupBox* gbox_h = new QGroupBox("Handlinger");
    QBoxLayout* gbox_h_layout = new QVBoxLayout;
    QBoxLayout* layout_global = new QHBoxLayout;

    void qt_setup() {

        // Window - rpg game
        window->setFixedSize(window_size_x, window_size_y);
        window->setWindowFlags(window_flags);
        window->setLayout(layout_global);

        // Window - player name
        player_name_widget->setWindowTitle("Vælg Navn!");
        player_name_widget->setLayout(player_name_layout);
        player_name_layout->addWidget(player_name_line);
        player_name_layout->addWidget(player_name_accept);
        player_name_widget->setWindowFlags(window_flags);
        player_name_widget->show();

        // Tabs
        tabs_actionsmenu->insertTab(0, tab_settings, "Indstillinger");
        tabs_actionsmenu->insertTab(0, tab_achievements, "Præsentationer");
        tabs_actionsmenu->insertTab(0, tab_combat, "Kamp");
        tabs_actionsmenu->insertTab(0, tab_stats, "Statistikker");
        tabs_actionsmenu->insertTab(0, tab_inventory, "Inventar");
        tabs_actionsmenu->setCurrentIndex(0);

        // Tab content
        tab_inventory->setLayout(layout_tab_inventory); // inventory tab
        layout_tab_inventory->addWidget(tab_inventory_label);
        tab_inventory_label->setAlignment(Qt::AlignTop);
        tab_stats->setLayout(layout_tab_stats); // stats tab
        layout_tab_stats->addWidget(tab_stats_label);
        tab_stats_label->setAlignment(Qt::AlignTop);
        tab_settings->setLayout(layout_tab_settings); // settings tab
        layout_tab_settings->setColumnStretch(1, 1);
        layout_tab_settings->setAlignment(Qt::AlignTop);
        layout_tab_settings->setVerticalSpacing(20);
        tab_achievements->setLayout(layout_tab_achievements); // achievements tab
        layout_tab_achievements->addWidget(tab_achievements_label);
        tab_achievements_label->setAlignment(Qt::AlignTop);

        // Text browser & choice buttons
        text_browser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        text_browser->setReadOnly(true);
        choice_button_layout->addWidget(choice_button_01);
        choice_button_layout->addWidget(choice_button_02);

        // Text browser labels
        label_player_layout->addWidget(label_player_lifes);
        label_player_layout->addWidget(label_player_gold);
        label_player_layout->addWidget(label_player_armour);
        label_player_layout->addWidget(label_player_mana);
        label_browser_layout->addWidget(label_player_area);
        label_browser_layout->addLayout(label_player_layout);

        // Layout 'global box left'
        gbox_l_layout->addLayout(label_browser_layout);
        gbox_l_layout->addWidget(text_browser);
        gbox_l_layout->addLayout(choice_button_layout);
        gbox_l->setLayout(gbox_l_layout);
        layout_global->addWidget(gbox_l);

        // Layout 'global box right'
        gbox_h_layout->addWidget(tabs_actionsmenu);
        gbox_h_layout->addWidget(window_button_close);
        gbox_h->setLayout(gbox_h_layout);
        layout_global->addWidget(gbox_h);

        // Button connections
        QObject::connect(window_button_close, &QPushButton::clicked, [=]() {window->close(); });
    }

    void set_label_text_colour(QLabel* streng_label, std::string streng_front, std::string& streng_content, std::string streng_colour) {
        streng_label->setText(QString::fromStdString(streng_front + "<font color=\"" + streng_colour + "\">" + streng_content + "</font>"));
    }

    void update_tab_items(std::vector<std::pair<QString, float>>& tab_object) {
        QString tab_table = "";

        for (std::pair<QString, float>& tab_item : tab_object) {
            if (tab_object == player_inventory || tab_object == player_stats) {
                tab_table.append("<tr><td style=\"font-weight: bold;\">" + tab_item.first + ": </td><td>" + QString::number(tab_item.second) + "</td></tr>");
            } else if (tab_object == player_achievements) {
                bool achievement_state = tab_item.second;
                QString achievement_text = QString::number(tab_item.second) == true ? "sandt" : "falsk";
                QString achievement_colour = achievement_state ? "green" : "red";

                tab_table.append("<tr><td style=\"font-weight: bold;\">" + tab_item.first + ": </td><td style=\"color:" + achievement_colour + ";\">" + achievement_text + "</td></tr>");
            }
        }

        if (tab_object == player_inventory) {
            tab_inventory_label->setText("<table>" + tab_table + "</table>");
            tab_inventory_label->show();
        }
        else if (tab_object == player_stats) {
            tab_stats_label->setText("<table>" + tab_table + "</table>");
            tab_stats_label->show();
        }
        else if (tab_object == player_achievements) {
            tab_achievements_label->setText("<table>" + tab_table + "</table>");
            tab_achievements_label->show();
        }
    }

    std::vector<std::pair<QString, float>>& get_tab_label_object(QString tab_label) {
        if(tab_label == "inventar") {
            return player_inventory;
        } else if (tab_label == "statistikker") {
            return player_stats;
        } else if (tab_label == "præsentationer") {
            return player_achievements;
        }
    }

    public:
        QPushButton* choice_button_01 = new QPushButton;
        QPushButton* choice_button_02 = new QPushButton;

        void text_browser_append(std::string append_string, std::string string_colour = "black") {
            text_browser->insertHtml("<p style=\"color:" + QString::fromStdString(string_colour) + ";\">" + QString::fromStdString(append_string) + "</p>");
            text_browser->append("");
        }

        template <typename TypeLambda> void connect_choice_button(QPushButton* choice_button, TypeLambda& lambda) {
            QObject::connect(choice_button, &QPushButton::clicked, [=]() { lambda(); });
        }

        void disconnect_choice_button(QPushButton* choice_button) {
            QObject::disconnect(choice_button, 0, 0, 0);
        }

        void set_label_player_area(std::string& player_area) {
            set_label_text_colour(label_player_area, "Område: ", player_area, "black");
        }

        void set_label_player_lifes(int& player_lifes) {
            set_label_text_colour(label_player_lifes, "Liv: ", std::to_string(player_lifes), "red");
        }

        void set_label_player_gold(int& player_gold) {
            set_label_text_colour(label_player_gold, "Guld: ", std::to_string(player_gold), "orange");
        }

        void set_label_player_armour(int& player_armour) {
            set_label_text_colour(label_player_armour, "Armor: ", std::to_string(player_armour), "darkgreen");
        }

        void set_label_player_mana(int& player_mana) {
            set_label_text_colour(label_player_mana, "Mana: ", std::to_string(player_mana), "darkgreen");
        }

        void add_tab_item(QString item_label, QString item_name, float item_amount) {
            std::vector<std::pair<QString, float>>& tab_object = get_tab_label_object(item_label);

            for (std::pair<QString, float>& tab_item : tab_object) {
                if (tab_item.first == item_name) {
                    tab_item.second += item_amount;
                    update_tab_items(tab_object);
                    return;
                }
            }

            tab_object.push_back({ item_name, item_amount });
            update_tab_items(tab_object);
        }

        template <typename TypeLambda> void create_setting_connection(QString setting_type, QString setting_label, TypeLambda& lambda, QStringList setting_values = {}) {
            QLabel* setting_type_label = new QLabel(setting_label);
            
            // Adds a checkbox setting to the panel
            if (setting_type == "checkbox") {
                QCheckBox* setting_type_interactable = new QCheckBox;

                QObject::connect(setting_type_interactable, &QCheckBox::clicked, [=]() { lambda(setting_type_interactable->isChecked()); });
                layout_tab_settings->addWidget(setting_type_interactable, setting_current_row, 0, Qt::AlignTop | Qt::AlignCenter);
                layout_tab_settings->setRowMinimumHeight(setting_current_row, 25);

            // Adds a dropdown setting to the panel
            } else if (setting_type == "dropdown") {
                QComboBox* setting_type_interactable = new QComboBox;

                setting_type_interactable->setFixedWidth(100);
                setting_type_interactable->addItems(setting_values);

                QObject::connect(setting_type_interactable, qOverload<int>(&QComboBox::currentIndexChanged), [=]() { lambda(setting_type_interactable->currentIndex()); });
                layout_tab_settings->addWidget(setting_type_interactable, setting_current_row, 0, Qt::AlignTop | Qt::AlignCenter);

            // Adds a slider setting to the panel
            } else if (setting_type == "slider") {
                QSlider* setting_type_interactable = new QSlider(Qt::Horizontal);

                setting_type_interactable->setFixedWidth(100);
                setting_type_interactable->setMinimum(QString(setting_values[0]).toInt());
                setting_type_interactable->setMaximum(QString(setting_values[1]).toInt());
                setting_type_interactable->setValue(QString(setting_values[2]).toInt());

                QObject::connect(setting_type_interactable, &QSlider::valueChanged, [=]() { lambda(setting_type_interactable->value()); });
                layout_tab_settings->addWidget(setting_type_interactable, setting_current_row, 0, Qt::AlignTop | Qt::AlignCenter);

            // Adds a number setting to the panel
            } else if (setting_type == "number") {
                QSpinBox* setting_type_interactable = new QSpinBox;

                setting_type_interactable->setFixedWidth(100);
                setting_type_interactable->setRange(QString(setting_values[0]).toInt(), QString(setting_values[1]).toInt());
                setting_type_interactable->setSingleStep(QString(setting_values[2]).toInt());
                setting_type_interactable->setValue(QString(setting_values[3]).toInt());

                QObject::connect(setting_type_interactable, &QSpinBox::textChanged, [=]() { lambda(setting_type_interactable->value()); });
                layout_tab_settings->addWidget(setting_type_interactable, setting_current_row, 0, Qt::AlignTop | Qt::AlignCenter);
            }

            layout_tab_settings->addWidget(setting_type_label, setting_current_row, 1, Qt::AlignTop | Qt::AlignLeft);
            setting_current_row += 2;
        }

        void remove_tab_item(QString item_label, QString item_name, float item_amount, bool delete_if_empty = false) {
            std::vector<std::pair<QString, float>>& tab_object = get_tab_label_object(item_label);

            for (std::pair<QString, float>& tab_item : tab_object) {
                if (tab_item.first == item_name) {
                    tab_item.second -= item_amount;

                    if (tab_item.second <= 0 && delete_if_empty) {
                        tab_object.erase(std::remove(tab_object.begin(), tab_object.end(), tab_item), tab_object.end());
                    }

                    update_tab_items(tab_object);
                    return;
                }
            }
        }

        void on_player_name_accept(text_rpg* FuncClass, void(text_rpg::*f)(void), std::string* var_player_name) {
            QObject::connect(player_name_accept, &QPushButton::clicked, [=]() {
                std::string player_name = player_name_line->text().toStdString();

                if (player_name != "") {
                    (*var_player_name) = player_name;
                    player_name_widget->close();
                    window->show();
                    (FuncClass->*f)();
                } else {
                    player_name_line->setStyleSheet("border: 1px solid red;");
                }
            });
        }

        text_rpg_qt(int wsize_x, int wsize_y) {
            window_size_x = wsize_x;
            window_size_y = wsize_y;

            qt_setup();
        }
};

// Primær kode
class text_rpg : public text_rpg_qt {

    // Overordnet variabler
    std::string player_name, player_area = "Test";
    int player_life = 100, player_gold = 50, player_armour = 0, player_mana = 0, enemy_life = 100, current_choice = 0, rng = rand() % 100 + 1;

    // Erstatter alt tekst i en streng med noget andet
    void string_replace_all_instances(std::string& string_used, std::string string_search, std::string string_replace) {
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

        // eksempel - tilføj fane indhold
        //add_tab_item("inventar", "Æbler", 3);
        //add_tab_item("statistikker", "Dræbt", 0);
        //add_tab_item("præsentationer", "Vundet", false);
        //text_browser_append("Du modtog 3 æbler!", "red");

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

            choice_button_01->setText("Tag øske fra skuret");
            choice_button_02->setText("Kør mod byen");

            connect_choice_button(choice_button_01, [=](){ readLine("-- Story 6 --", "-- Story 7 --"); current_choice = 5; storyChoice(); });
            connect_choice_button(choice_button_02, [=](){ readLine("-- Story 7.1 --", "-- Story 8 --"); current_choice = 6; storyChoice(); });

        // printer en del tekst, derefter - Du går nærmere || Vender bilen og kører væk
        case 4:
            // Burde meget gerne printe noget tekst der ikke er blevet printet
            readLine("-- Story 7 --", "-- Story 7.1 --");
            readLine("-- Story 7.1 --", "-- Story 8 --");

            readLine("-- Valg 10 --", "-- Valg 11 --");
            readLine("-- Valg 11 --", "-- Valg 12 --");

            choice_button_01->setText("Stig ud og gå nærmere");
            choice_button_02->setText("Vend bilen og kør væk");

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
                        line != "" ? text_browser_append(line) : NULL;
                    }
                }

                line_number++;
            }

            myfile.close();
        }
        
        qDebug("Unable to open file");
    }

public:
    // Nedenfor ses eksempler for hvordan man kan sætte indstilliner op
    // > create_setting_connection(type, tekst, [=](variabler){ funktionalitet })
    // > type: checkbox, dropdown, slider, number
    // > variable returns: bool checked (checkbox), int selected_id (dropdown), int slider_val (slider), int number_val (number)
    //create_setting_connection("checkbox", "kage tilgængelighed", [=](bool checked_status) { qDebug() << checked_status; });
    //create_setting_connection("dropdown", "kage farve", [=](int farve_id) { qDebug() << farve_id; }, {"rød", "grøn", "blå"});
    //create_setting_connection("slider", "kage mængde", [=](int slider_val) { qDebug() << slider_val; }, {"0", "100", "25"});
    //create_setting_connection("number", "kage antal", [=](int number_val) { qDebug() << number_val; }, { "0", "100", "5", "25" });

    text_rpg() : text_rpg_qt(800, 600) {
        on_player_name_accept(this, &text_rpg::storyLine, &player_name);
        set_label_player_area(player_area);
        set_label_player_lifes(player_life);
        set_label_player_gold(player_gold);
        set_label_player_armour(player_armour);
        set_label_player_mana(player_mana);

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