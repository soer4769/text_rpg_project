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
#include <QTimer>
#include <QScrollBar>

// Standard C++ Biblioteker
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cstdarg>
#include <chrono>

// Konstanter og globale variabler
using namespace std;
class text_rpg;

///////////////////////////////////////////////////////////////
// Qt klassen indeholder alt Qt releteret kode, således at det ikke er en del af selve tekst RPG koden så den kan bruges som en template to at bygge brugerdefineret udgaver
///////////////////////////////////////////////////////////////
class text_rpg_qt {

    // Overordnet variabler
    int window_size_x, window_size_y, setting_current_row = 0, combat_button_active = 0;
    std::vector<std::pair<QString, float>> player_inventory, player_stats, player_achievements;

    // Qt - Vindue rpg game
    QFlags<Qt::WindowType> window_flags = Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint;
    QWidget* window = new QWidget;
    QPushButton* window_button_close = new QPushButton("Luk vindue");
    QTimer* timer = new QTimer;
    
    // Qt -Vindue player name
    QWidget* player_name_widget = new QWidget;
    QBoxLayout* player_name_layout = new QHBoxLayout;
    QLineEdit* player_name_line = new QLineEdit;
    QPushButton* player_name_accept = new QPushButton("Vælg");

    // Qt - Faner
    QTabWidget* tabs_actionsmenu = new QTabWidget;
    QWidget* tab_inventory = new QWidget;
    QWidget* tab_stats = new QWidget;
    QWidget* tab_combat = new QWidget;
    QWidget* tab_settings = new QWidget;
    QWidget* tab_achievements = new QWidget;

    // Qt - Faner indhold
    QLayout* layout_tab_inventory = new QHBoxLayout;
    QLabel* tab_inventory_label = new QLabel;
    QLayout* layout_tab_stats = new QHBoxLayout;
    QLabel* tab_stats_label = new QLabel;
    QGridLayout* layout_tab_settings = new QGridLayout;
    QLayout* layout_tab_achievements = new QHBoxLayout;
    QLabel* tab_achievements_label = new QLabel;
    QLayout* layout_tab_combat = new QVBoxLayout;
    QLabel* tab_combat_label = new QLabel;
    QPushButton* tab_combat_button_kick = new QPushButton("Spark");
    QPushButton* tab_combat_button_slash = new QPushButton("Skyd");
    QPushButton* tab_combat_button_slam = new QPushButton("Slag");

    // Qt - tekst felt område
    QBoxLayout* label_player_layout = new QHBoxLayout;
    QBoxLayout* label_browser_layout = new QHBoxLayout;
    QLabel* label_player_lifes = new QLabel;
    QLabel* label_player_gold = new QLabel;
    QLabel* label_player_armour = new QLabel;
    QLabel* label_player_mana = new QLabel;
    QLabel* label_player_area = new QLabel;
    QTextEdit* text_browser = new QTextEdit;
    QBoxLayout* choice_button_layout = new QHBoxLayout;
    QPushButton* choice_button_01 = new QPushButton;
    QPushButton* choice_button_02 = new QPushButton;

    // Qt - Global boks layout
    QGroupBox* gbox_l = new QGroupBox("Spilområde");
    QBoxLayout* gbox_l_layout = new QVBoxLayout;
    QGroupBox* gbox_h = new QGroupBox("Handlinger");
    QBoxLayout* gbox_h_layout = new QVBoxLayout;
    QBoxLayout* layout_global = new QHBoxLayout;

    // Skaber og opsætter Qt vinduet 
    void qt_setup() {
        ///////////////////////////////////////////////////////////////
        // Window - rpg spil vinduet
        ///////////////////////////////////////////////////////////////
        window->setFixedSize(window_size_x, window_size_y);
        window->setWindowFlags(window_flags);
        window->setLayout(layout_global);

        ///////////////////////////////////////////////////////////////
        // Window - spiller navn vindue
        ///////////////////////////////////////////////////////////////
        player_name_widget->setWindowTitle("Vælg Navn!");
        player_name_widget->setLayout(player_name_layout);
        player_name_layout->addWidget(player_name_line);
        player_name_layout->addWidget(player_name_accept);
        player_name_widget->setWindowFlags(window_flags);
        player_name_widget->show();

        ///////////////////////////////////////////////////////////////
        // Faner - skaber fanerne og sætter 'inventar' fanen som aktiv
        ///////////////////////////////////////////////////////////////
        tabs_actionsmenu->insertTab(0, tab_settings, "Indstillinger");
        tabs_actionsmenu->insertTab(0, tab_achievements, "Præstationer");
        tabs_actionsmenu->insertTab(0, tab_combat, "Kamp");
        tabs_actionsmenu->insertTab(0, tab_stats, "Statistikker");
        tabs_actionsmenu->insertTab(0, tab_inventory, "Inventar");
        tabs_actionsmenu->setCurrentIndex(0);

        ///////////////////////////////////////////////////////////////
        // Faner indhold - tilføjer layout og widets til de individuelle faner
        ///////////////////////////////////////////////////////////////
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

        tab_combat->setLayout(layout_tab_combat); // combat tab
        layout_tab_combat->addWidget(tab_combat_button_kick);
        layout_tab_combat->addWidget(tab_combat_button_slash);
        layout_tab_combat->addWidget(tab_combat_button_slam);
        set_combat_button_active(0);

        ///////////////////////////////////////////////////////////////
        // Tekst felt og valg knapper - viser scrollbar, sætter feltets tilstand til 'read only' og tilføjer to valg knapper under feltet
        ///////////////////////////////////////////////////////////////
        text_browser->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        text_browser->setReadOnly(true);
        choice_button_layout->addWidget(choice_button_01);
        choice_button_layout->addWidget(choice_button_02);

        ///////////////////////////////////////////////////////////////
        // Tekst felt etiketter - tilføjer etiketter og dets layout ovenover tekst feltet
        ///////////////////////////////////////////////////////////////
        label_player_layout->addWidget(label_player_lifes);
        label_player_layout->addWidget(label_player_gold);
        label_player_layout->addWidget(label_player_armour);
        label_player_layout->addWidget(label_player_mana);
        label_browser_layout->addWidget(label_player_area);
        label_browser_layout->addLayout(label_player_layout);

        ///////////////////////////////////////////////////////////////
        // Layout 'global boks venstre'
        ///////////////////////////////////////////////////////////////
        gbox_l_layout->addLayout(label_browser_layout);
        gbox_l_layout->addWidget(text_browser);
        gbox_l_layout->addLayout(choice_button_layout);
        gbox_l->setLayout(gbox_l_layout);
        layout_global->addWidget(gbox_l);

        ///////////////////////////////////////////////////////////////
        // Layout 'global box right'
        ///////////////////////////////////////////////////////////////
        gbox_h_layout->addWidget(tabs_actionsmenu);
        gbox_h_layout->addWidget(window_button_close);
        gbox_h->setLayout(gbox_h_layout);
        layout_global->addWidget(gbox_h);

        ///////////////////////////////////////////////////////////////
        // Button connections
        ///////////////////////////////////////////////////////////////
        QObject::connect(window_button_close, &QPushButton::clicked, [=]() {window->close(); });
        QObject::connect(tab_combat_button_kick, &QPushButton::clicked, [=]() { set_combat_button_active(0); });
        QObject::connect(tab_combat_button_slash, &QPushButton::clicked, [=]() { set_combat_button_active(1); });
        QObject::connect(tab_combat_button_slam, &QPushButton::clicked, [=]() { set_combat_button_active(2); });
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
                QString achievement_text = achievement_state ? "sandt" : "falsk";
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
        } else if (tab_label == "præstationer") {
            return player_achievements;
        }
    }

    void set_combat_button_active(int new_active_button) {
        combat_button_active = new_active_button;
        tab_combat_button_kick->setEnabled(true);
        tab_combat_button_slash->setEnabled(true);
        tab_combat_button_slam->setEnabled(true);

        switch (new_active_button) {
            case 0:
                tab_combat_button_kick->setEnabled(false);
                break;
            case 1:
                tab_combat_button_slash->setEnabled(false);
                break;
            case 2:
                tab_combat_button_slam->setEnabled(false);
                break;
        }
    }

    public:
        ///////////////////////////////////////////////////////////////
        // Misc
        ///////////////////////////////////////////////////////////////
        int get_combat_button_active() {
            return combat_button_active;
        }

        // Klades når spilleren har klikket på akkepter knappen efter at have indtastet sit navn i vindue boksen
        void on_player_name_accept(text_rpg* FuncClass, void(text_rpg::* f)(void), std::string* var_player_name) {
            QObject::connect(player_name_accept, &QPushButton::clicked, [=]() {
                std::string player_name = player_name_line->text().toStdString();

                // Hvis spilleren har indtastet sit navn, gem navnet og fortsæt til spillet
                if (player_name != "") {
                    (*var_player_name) = player_name;
                    player_name_widget->close();
                    window->show();
                    (FuncClass->*f)();
                }
                else {
                    player_name_line->setStyleSheet("border: 1px solid red;"); // Spilleren har ikke skrevet noget, advar om dette ved at gøre tekstboksen kant rød
                }
            });
        }

        ///////////////////////////////////////////////////////////////
        // Text browser
        ///////////////////////////////////////////////////////////////
        void text_browser_append(std::string append_string, std::string string_colour = "black") {
            text_browser->insertHtml("<p style=\"color:" + QString::fromStdString(string_colour) + ";\">" + QString::fromStdString(append_string) + "</p><br>");
            text_browser->verticalScrollBar()->setValue(text_browser->verticalScrollBar()->maximum());
        }

        void text_browser_clear() {
            text_browser->clear();
        }

        ///////////////////////////////////////////////////////////////
        // Timer
        ///////////////////////////////////////////////////////////////
        template <typename TypeLambda> void method_timer(TypeLambda& lambda) {
            QObject::disconnect(timer, 0, 0, 0);
            QObject::connect(timer, &QTimer::timeout, [=]() { lambda(); });
            timer->setSingleShot(true);
        }

        void method_timer_start(int timer_interval) {
            timer->start(timer_interval);
        }

        ///////////////////////////////////////////////////////////////
        // Choice buttons
        ///////////////////////////////////////////////////////////////
        template <typename TypeLambda> void connect_choice_button(int choice_button, TypeLambda& lambda) {
            QObject::connect(choice_button == 0 ? choice_button_01 : choice_button_02, &QPushButton::clicked, [=]() { lambda(); });
        }

        void disconnect_choice_button(int choice_button) {
            QObject::disconnect(choice_button == 0 ? choice_button_01 : choice_button_02, 0, 0, 0);
        }

        void set_choice_button_text(int choice_button, QString choice_text) {
            (choice_button == 0 ? choice_button_01 : choice_button_02)->setText(choice_text);
        }

        void set_choice_button_state(int choice_button, bool choice_state) {
            (choice_button == 0 ? choice_button_01 : choice_button_02)->setEnabled(choice_state);
        }

        ///////////////////////////////////////////////////////////////
        // Text browser labels
        ///////////////////////////////////////////////////////////////
        void set_label_player_area(std::string& player_area) {
            text_browser_append("> Du er nu i området: " + player_area + " <", "blue");
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

        ///////////////////////////////////////////////////////////////
        // Tabs
        ///////////////////////////////////////////////////////////////
        void add_tab_item(QString item_label, QString item_name, int item_amount, std::string item_pickup_message = "", std::string item_pickup_message_colour = "magenta") {
            std::vector<std::pair<QString, float>>& tab_object = get_tab_label_object(item_label);

            if (item_pickup_message == "" && item_label == "inventar") {
                item_pickup_message = "Du opsamlede " + std::to_string(item_amount) + " " + item_name.toStdString() + "!";
            } else if (item_pickup_message == "" && item_label == "præstationer" && item_amount != 0) {
                item_pickup_message = "Du fik præstationen '" + item_name.toStdString() + "'!";
            }

            text_browser_append(item_pickup_message, item_pickup_message_colour);

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

        void set_active_tab(int tab_index) {
            tabs_actionsmenu->setCurrentIndex(tab_index);
        }

        void empty_player_inventory() {
            player_inventory.clear();
        }

        ///////////////////////////////////////////////////////////////
        // Tabs - settings tab
        ///////////////////////////////////////////////////////////////
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

        void set_settings_window_size(int w, int h) {
            if (w == -1 && h == -1) {
                window->showFullScreen();
            }

            else {
                window->showNormal();
                window->setFixedSize(w, h);
            }
        }

        ///////////////////////////////////////////////////////////////
        // Constructor
        ///////////////////////////////////////////////////////////////
        text_rpg_qt(int wsize_x, int wsize_y) {
            window_size_x = wsize_x;
            window_size_y = wsize_y;

            qt_setup();
        }
};

///////////////////////////////////////////////////////////////
// Primær kode
///////////////////////////////////////////////////////////////
class text_rpg : public text_rpg_qt {

    // Overordnet variabler
    std::string player_name, player_area = "Hjemme";
    int player_life = 100, player_gold = 50, player_armor = 100, player_mana = 0, enemy_life = 80, current_choice = 0,
        total_damage, rng_crit_player, rng_crit_enemy, rng_hit_player, rng_hit_enemy, total_damage_enemy;

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
    
    void damage_enemy(){							// Funktion for fjendens skade
        rng_crit_enemy = rand() % 100 + 1;			// Tilfældighed beregnes for kritisk skade (fra 1 til 100)
        rng_hit_enemy = rand() % 100 + 1;			// Tilfældighed beregnes til chancen for at ramme (fra 1 til 100)

        if (rng_hit_enemy >= 20) {					// Hvis den tilfældige værdi for at ramme er over 20, så skal koden udføres (80% chance)
            total_damage_enemy = rand() % 15 + 11;	// Tilfældig skade (fra 10 til 25)
        }
        
        if (rng_crit_enemy <= 5) {					// (95% chance)
            total_damage_enemy = total_damage_enemy * 1.5; // Ganger fjendens totale skade med halvanden i tilfælde af crit
        }
    }

    // Andet indhold
    void damage_player() {
        rng_hit_player = rand() % 100 + 1;			// Tilfældighed beregnes til chancen for at ramme (fra 1 til 100)
        rng_crit_player = rand() % 100 + 1;			// Tilfældighed beregnes for kritisk skade (fra 1 til 100)
        total_damage = rand() % 100 + 1;			// Tilfældig skade (fra 1 til 100)

        switch(get_combat_button_active()) {
            case 0: // kick 
                if (rng_hit_player >= 5) {			// 95% chance for at ramme
                    total_damage = total_damage/7.0; // Modifikation til skaden (fra 1 til 30 skade uden crit)
                    break;
                }
                else {
                    total_damage = 0;				// 0 skade ved miss
                    break;
                }
            case 1: // shoot 
                if (rng_hit_player >= 70) {			// 30% chance for at ramme)
                    total_damage = total_damage/2.0 + 9; // Modifikation til skaden (fra 10 til 60 uden crit)
                    break;
                }
                else {
                    total_damage = 0;				// 0 skade ved miss
                    break;
                }
            case 2: // slam
                if (rng_hit_player >= 50) {			// 50% chance for at ramme
                    total_damage = total_damage/4.0 + 9;	// Modifikation til skaden (fra 10 til 35 skade)
                } else {
                    total_damage = 0;				// 0 skade ved miss
                    break;
                }
        }

        if (rng_crit_player <= 5) {					// Beregner den kritiske skade for spilleren (5% chance for crit)
            total_damage = total_damage * 1.5;		// Total skade ganget med halvanden ved crit
        }
    }

    ///////////////////////////////////////////////////////////////
    // combat_system -  Kampsystemet i spillet. Spilleren er oppe imod ét monster/ en fjende, hvor man enten kan vinde eller tabe
    ///////////////////////////////////////////////////////////////
    void combat_system() { 
        set_choice_button_state(0, false);			// Deaktiverer valgknapperne
        set_choice_button_state(1, false);

        text_browser_append("------------------------------------------------------------", "red");
        text_browser_append("DU ER I EN KAMP!", "red"); // Meddeler kampen starter i rød skrift
        set_active_tab(2);							    // Skifter tabben til menuen for kamp

        method_timer([=]() {
            damage_player();						    // beregner skaden for spilleren
            text_browser_append(player_name + " angriber! Du forårsager " + to_string(total_damage) + " skade!", "blue"); // Meddeler brugeren om spillerens skade
            enemy_life = (enemy_life > total_damage ? enemy_life - total_damage : 0); // Trækker spillerens skade fra fjendens liv. Den overgår aldrig total skade

            damage_enemy();							    // Beregner skaden for fjenden
            text_browser_append("Fjende angriber! Den forårsager " + to_string(total_damage_enemy) + " skade!", "green"); // Meddeler brugeren om fjendens skade
            text_browser_append("Fjende liv: " + to_string(enemy_life)); // Meddeler brugeren om fjendens resterende liv
            
            if (player_armor != 0) { // Så længe spilleren ikke har 0 armor, skal armor trækkes fra, ellers skal livet trækkes fra
                player_armor = (player_armor > total_damage_enemy ? player_armor - total_damage_enemy : 0);
				set_label_player_armour(player_armor); // Opdaterer indikatoren for armor
            } else {
                player_life = (player_life > total_damage_enemy ? player_life - total_damage_enemy : 0);
				 set_label_player_lifes(player_life); // Opdaterer indikatoren for liv
            }

            if (player_life <= 0 || enemy_life <= 0) {
                set_choice_button_state(0, true); // aktiverer valgknapperne
                set_choice_button_state(1, true);

                if (player_life <= 0) { // Hvis spilleren dør
                    text_browser_append("!Du døde - GAME OVER!", "red"); 
                    text_browser_append("------------------------------------------------------------", "red");
                    add_tab_item("statistikker", "Antal død", 1); // Opdaterer statistikker om antal døde
                    game_over(); // Spillet genstartes 
                }

                else if (enemy_life <= 0) { // Hvis fjenden dør
                    text_browser_append("Fjenden blev dræbt - YOU WON!", "red");
                    text_browser_append("------------------------------------------------------------", "red");
                    add_tab_item("statistikker", "Monstre dræbt", 1); // Opdaterer statistikker om antal monstre dræbt
                    storyChoice(); // Historien fortsætter
                }
            }

            else {
                method_timer_start(3000); // Hvis ingen af betingelserne er opfyldt, genstarter kampsystemet; 3000 = 3 sekunders runde
            }
        });

        method_timer_start(3000); // Initierer timeren for runden (3 sekunder)
    }

    ///////////////////////////////////////////////////////////////
    // StoryLine - starter en ny historie med introduktion, inden valgene bliver givet
    ///////////////////////////////////////////////////////////////
    void storyLine() {
        set_label_player_area(player_area);
        set_label_player_lifes(player_life);
        set_label_player_gold(player_gold);
        set_label_player_armour(player_armor);
        set_label_player_mana(player_mana);
        text_browser_clear(); // Fjerner den tekst der bliver printet idet man opretter præstationer
        add_tab_item("inventar", "Riffel", 1); // Tilføjer en "Riffel" til spillerens inventar

        // Start på spillet
        readLine("||-- Velkomst --||", "||-- Introduktion --||");
        readLine("||-- Introduktion --||", "||-- Acquired Items --||");
        readLine("-- Story 1 --", "-- Story 2 --");
        storyChoice();
    }

    ///////////////////////////////////////////////////////////////
    // storyChoice - printer tekst, giver spilleren ting og starter kampe afhængig af ruten spilleren tager gennem historien
    ///////////////////////////////////////////////////////////////
    void storyChoice() {
        disconnect_choice_button(0);
        disconnect_choice_button(1);

        // Et switch case med alle de forskellige muligheder. De forskellige cases leder til både forskellige andre cases men også de kan også lede til de samme cases
        switch (current_choice) {

        case 0:
        // Starter efter introduktion
        // Vil du sove videre? || Vil du stå op?

            // Printer valgmuligheder
            readLine("-- Vil du --", "-- Eller --");
            readLine("-- Valg (1) --", "-- Valg (2) --");
            readLine("-- Eller --", "-- Slut --");
            readLine("-- Valg (2) --", "-- Valg (3) --");
            text_browser_append("------------------------------------------------------------");

            // Ændre knappernes tekst
            set_choice_button_text(0, "Sov videre");
            set_choice_button_text(1, "Stå op");

            // Alt efter om man trykker på knap 0 eller 1 printes den tilhørene historie - Den tekst der står under valg x på flowchart
            // De 2 knapper leder til hver deres case
            connect_choice_button(0, [=]() { readLine("-- Story 2 --", "-- Story 3 --"); current_choice = 1; storyChoice(); });
            connect_choice_button(1, [=]() { set_label_player_area(player_area = "Køkken"); readLine("-- Story 3 --", "-- Story 4 --"); current_choice = 2; storyChoice(); });
            // Knapper føre til:
            // case 1
            // case 2
            break;

        case 1:
        // Kan startes af case 0
        // Springer op af sengen og løber || Griber riffel
            readLine("-- Vil du --", "-- Eller --");
            readLine("-- Valg (3) --", "-- Valg (4) --");
            readLine("-- Eller --", "-- Slut --");
            readLine("-- Valg (4) --", "-- Valg (5) --");
            text_browser_append("------------------------------------------------------------");

            set_choice_button_text(0, "Løb ud i baghaven");
            set_choice_button_text(1, "Grib riffel");

            connect_choice_button(0, [=]() { set_label_player_area(player_area = "Baghave"); readLine("-- Story 4 --", "-- Story 5 --"); current_choice = 3; storyChoice(); });
            connect_choice_button(1, [=]() { set_label_player_area(player_area = "Byen"); readLine("-- Story 5 --", "-- Story 6 --"); readLine("-- Story 7 --", "-- Story 7.1 --"); readLine("-- Story 7.1 --", "-- Story 8 --"); current_choice = 4; storyChoice(); });
            // case 3
            // case 4
            break;

        case 2:
        // Kan startes af case 0
        // Griber riffel || Du spurter ud af bagdøren
            readLine("-- Vil du --", "-- Eller --");
            readLine("-- Valg (4) --", "-- Valg (5) --");
            readLine("-- Eller --", "-- Slut --");
            readLine("-- Valg (5) --", "-- Valg (6) --");
            text_browser_append("------------------------------------------------------------");

            set_choice_button_text(0, "Grib riffel");
            set_choice_button_text(1, "Spurt ud af bagdøren");

            connect_choice_button(0, [=]() { set_label_player_area(player_area = "Byen"); readLine("-- Story 5 --", "-- Story 6 --"); readLine("-- Story 7 --", "-- Story 7.1 --"); readLine("-- Story 7.1 --", "-- Story 8 --"); current_choice = 4; storyChoice(); });
            connect_choice_button(1, [=]() { set_label_player_area(player_area = "Baghave"); readLine("-- Story 4 --", "-- Story 5 --"); current_choice = 5; storyChoice(); });
            // case 4
            // case 5
            break;

        case 3:
        // Kan startes af case 1
        // Finder økse || løber til bil
            readLine("-- Vil du --", "-- Eller --");
            readLine("-- Valg (6) --", "-- Valg (7) --");
            readLine("-- Eller --", "-- Slut --");
            readLine("-- Valg (7) --", "-- Valg (8) --");
            text_browser_append("------------------------------------------------------------");

            set_choice_button_text(0, "Tag øske fra skuret");
            set_choice_button_text(1, "Kør mod byen");

            connect_choice_button(0, [=]() { add_tab_item("inventar", "Økse", 1, "", "orange"); readLine("-- Story 6 --", "-- Story 7 --"); current_choice = 6; combat_system(); });
            connect_choice_button(1, [=]() { set_label_player_area(player_area = "Byen"); readLine("-- Story 7.1 --", "-- Story 8 --"); current_choice = 7; storyChoice(); });
            // case 6
            // case 7
            break;

        case 4:
        // Kan startes af case 1 og case 2
        // Du går nærmere || Vender bilen og kører væk
            readLine("-- Vil du --", "-- Eller --");
            readLine("-- Valg (10) --", "-- Valg (11) --");
            readLine("-- Eller --", "-- Slut --");
            readLine("-- Valg (11) --", "-- Valg (12) --");
            text_browser_append("------------------------------------------------------------");

            set_choice_button_text(0, "Stig ud og gå nærmere");
            set_choice_button_text(1, "Vend bilen og kør væk");

            connect_choice_button(0, [=]() { readLine("-- Story 8 --", "-- Story 9 --"); current_choice = 10; storyChoice(); });
            connect_choice_button(1, [=]() { readLine("-- Story 9 --", "-- Story 10 --"); current_choice = 11; storyChoice(); });
            // case 10
            // case 11
            break;

        case 5:
        // Kan startes af case 2
        // Find økse || Kører til byen
            readLine("-- Vil du --", "-- Eller --");
            readLine("-- Valg (6) --", "-- Valg (7) --");
            readLine("-- Eller --", "-- Slut --");
            readLine("-- Valg (7) --", "-- Valg (8) --");
            text_browser_append("------------------------------------------------------------");
            
            set_choice_button_text(0, "Find skarp økse");
            set_choice_button_text(1, "Kør ind mod byen");

            connect_choice_button(0, [=]() { add_tab_item("inventar", "Økse", 1, "", "orange"); readLine("-- Story 6 --", "-- Story 7 --"); current_choice = 6; combat_system(); });
            connect_choice_button(1, [=]() { set_label_player_area(player_area = "Byen"); readLine("-- Story 7.1 --", "-- Story 8 --"); current_choice = 7; storyChoice(); });
            // case 6
            // case 7
            break;
        
        case 6:
        // Kan startes af case 3 og case 5
        // LOOT monster - Ja || Nej
            add_tab_item("præstationer", "MONSTER KREATUR NEDLAGT", true);
            readLine("-- Vil du --", "-- Eller --");
            readLine("-- Valg (8) --", "-- Valg (9) --");
            readLine("-- Eller --", "-- Slut --");
            readLine("-- Valg (9) --", "-- Valg (10) --");
            text_browser_append("------------------------------------------------------------");

            set_choice_button_text(0, "Nej");
            set_choice_button_text(1, "Ja");

            connect_choice_button(0, [=]() { set_label_player_area(player_area = "Byen"); readLine("-- Story 7 --", "-- Story 7.1 --"); readLine("-- Story 7.1 --", "-- Story 8 --"); current_choice = 8; storyChoice(); });
            connect_choice_button(1, [=]() { set_label_player_area(player_area = "Byen"); add_tab_item("inventar", "Armor plate", 1); readLine("-- Story 7 --", "-- Story 7.1 --"); readLine("-- Story 7.1 --", "-- Story 8 --"); current_choice = 9; storyChoice(); });
            // case 8
            // case 9
            break;

        case 7:
        // Kan startes af case 3 og case 5
        // Du går nærmere || Vender bilen og kører væk
        // Minder meget om case 4, men printer lidt anden tekst
            readLine("-- Vil du --", "-- Eller --");
            readLine("-- Valg (10) --", "-- Valg (11) --");
            readLine("-- Eller --", "-- Slut --");
            readLine("-- Valg (11) --", "-- Valg (12) --");
            text_browser_append("------------------------------------------------------------");

            set_choice_button_text(0, "Gå nærmere");
            set_choice_button_text(1, "Kør VÆK!");

            connect_choice_button(0, [=]() { set_label_player_area(player_area = "Motorvejen"); readLine("-- Story 7 --", "-- Story 7.1 --"); readLine("-- Story 7.1 --", "-- Story 8 --"); current_choice = 10; storyChoice(); });
            connect_choice_button(1, [=]() { set_label_player_area(player_area = "Motorvejen"); readLine("-- Story 9 --", "-- Story 10 --"); current_choice = 11; storyChoice(); });
            // case 10
            // case 11
            break;
            
        case 8:
        // Kan startes af case 6
        // Du går nærmere || Vender bilen og kører væk
        // Minder meget om især case 9, men også case 4 og mindre case 7
            readLine("-- Vil du --", "-- Eller --");
            readLine("-- Valg (10) --", "-- Valg (11) --");
            readLine("-- Eller --", "-- Slut --");
            readLine("-- Valg (11) --", "-- Valg (12) --");
            text_browser_append("------------------------------------------------------------");

            set_choice_button_text(0, "Gå nærmere");
            set_choice_button_text(1, "Kør VÆK!");

            connect_choice_button(0, [=]() { set_label_player_area(player_area = "Motorvejen"); readLine("-- Story 8 --", "-- Story 9 --"); current_choice = 10; storyChoice(); });
            connect_choice_button(1, [=]() { set_label_player_area(player_area = "Motorvejen"); readLine("-- Story 9 --", "-- Story 10 --"); current_choice = 11; storyChoice(); });
            // case 10
            // case 11
            break;

        case 9:
        // Kan startes af case 6
        // Du går nærmere || Vender bilen og kører væk
        // Minder meget om især case 8, men også case 4 og mindre case 7
            readLine("-- Vil du --", "-- Eller --");
            readLine("-- Valg (10) --", "-- Valg (11) --");
            readLine("-- Eller --", "-- Slut --");
            readLine("-- Valg (11) --", "-- Valg (12) --");
            text_browser_append("------------------------------------------------------------");

            set_choice_button_text(0, "Gå nærmere");
            set_choice_button_text(1, "Kør VÆK!");

            connect_choice_button(0, [=]() { set_label_player_area(player_area = "Motorvejen"); readLine("-- Story 8 --", "-- Story 9 --"); current_choice = 10; storyChoice(); });
            connect_choice_button(1, [=]() { set_label_player_area(player_area = "Motorvejen"); readLine("-- Story 9 --", "-- Story 10 --"); current_choice = 11; storyChoice(); });
            // case 10
            // case 11
            break;

        case 10:
        // Kan startes af case 4, case 7, case 8 og case 9
        // Kører hjem og samler ting || Efterlader alt og køre til ven
        // Samme valg som case 11
            readLine("-- Vil du --", "-- Eller --");
            readLine("-- Valg (12) --", "-- Valg (13) --");
            readLine("-- Eller --", "-- Slut --");
            readLine("-- Valg (13) --", "-- Valg (14) --");
            text_browser_append("------------------------------------------------------------");

            set_choice_button_text(0, "Kør hjem");
            set_choice_button_text(1, "Efterlad alt");

            connect_choice_button(0, [=]() { readLine("-- Story 10 --", "-- Story 11 --"); add_tab_item("præstationer", "KØRTE HURTIGERE END 200 KM/T", true); readLine("-- Story 11 --", "-- Story 12 --"); add_tab_item("inventar", "Tandbørste", 1, "", "Orange"); add_tab_item("inventar", "Ekstra tøj", 1, "", "Orange"); add_tab_item("inventar", "Alt mad fra køleskabet", 1, "", "Orange"); readLine("-- Story 12 --", "-- Story 13 --"); current_choice = 12; storyChoice(); });
            connect_choice_button(1, [=]() { readLine("-- Story 12 --", "-- Story 13 --"); current_choice = 13; storyChoice(); });
            // case 12
            // case 13
            break;

        case 11:
        // Kan startes af case 4, case 7, case 8 og case 9
        // Kører hjem og samler ting || Efterlader alt og køre til ven
        // Samme valg som case 10
            readLine("-- Vil du --", "-- Eller --");
            readLine("-- Valg (12) --", "-- Valg (13) --");
            readLine("-- Eller --", "-- Slut --");
            readLine("-- Valg (13) --", "-- Valg (14) --");
            text_browser_append("------------------------------------------------------------");

            set_choice_button_text(0, "Kør hjem");
            set_choice_button_text(1, "Efterlad alt");

            connect_choice_button(0, [=]() { readLine("-- Story 10 --", "-- Story 11 --"); add_tab_item("præstationer", "KØRTE HURTIGERE END 200 KM/T", true); readLine("-- Story 11 --", "-- Story 12 --"); add_tab_item("inventar", "Tandbørste", 1, "", "Orange"); add_tab_item("inventar", "Ekstra tøj", 1, "", "Orange"); add_tab_item("inventar", "Alt mad fra køleskabet", 1, "", "Orange"); readLine("-- Story 12 --", "-- Story 13 --"); current_choice = 12; storyChoice(); });
            connect_choice_button(1, [=]() { readLine("-- Story 12 --", "-- Story 13 --"); current_choice = 13; storyChoice(); });
            // case 12
            // case 13
            break;

        case 12:
        // Kan startes af case 10 og case 11
        // Speederen i bund || Kører økonomisk
        // Næsten sammen udkom som case 13, bare mere tekst og får flere ting
            readLine("-- Vil du --", "-- Eller --");
            readLine("-- Valg (14) --", "-- Valg (15) --");
            readLine("-- Eller --", "-- Slut --");
            readLine("-- Valg (15) --", "-- Valg (16) --");
            text_browser_append("------------------------------------------------------------");

            set_choice_button_text(0, "Speederen i bund");
            set_choice_button_text(1, "Kør mere økonomisk");

            connect_choice_button(0, [=]() { set_label_player_area(player_area = "Tankstation"); readLine("-- Story 13 --", "-- Story 13.1 --"); readLine("-- Story 13.1 --", "-- Story 14 --"); current_choice = 14; storyChoice(); });
            connect_choice_button(1, [=]() { set_label_player_area(player_area = "Tankstation"); readLine("-- Story 14 --", "-- Story 15 --"); readLine("-- Story 13.1 --", "-- Story 14 --"); current_choice = 15; storyChoice(); });
            // case 14
            // case 15
            break;

        case 13:
        // Kan startes af case 11
        // Speederen i bund || Kører økonomisk
        // Næsten sammen udkom som case 12, bare mindre tekst og får færer ting
            readLine("-- Vil du --", "-- Eller --");
            readLine("-- Valg (14) --", "-- Valg (15) --");
            readLine("-- Eller --", "-- Slut --");
            readLine("-- Valg (15) --", "-- Valg (16) --");
            text_browser_append("------------------------------------------------------------");

            set_choice_button_text(0, "Speederen i bund");
            set_choice_button_text(1, "Kør mere økonomisk");

            connect_choice_button(0, [=]() { set_label_player_area(player_area = "Tankstation"); readLine("-- Story 13 --", "-- Story 13.1 --"); readLine("-- Story 13.1 --", "-- Story 14 --"); current_choice = 14; storyChoice(); });
            connect_choice_button(1, [=]() { set_label_player_area(player_area = "Tankstation"); readLine("-- Story 14 --", "-- Story 15 --"); readLine("-- Story 13.1 --", "-- Story 14 --"); current_choice = 15; storyChoice(); });
            // case 14
            // case 15
            break;

        case 14:
        // Kan startes af case 12 og case 13
        // Fylder benzin || Spiser og venter til i morgen
            readLine("-- Vil du --", "-- Eller --");
            readLine("-- Valg (16) --", "-- Valg (17) --");
            readLine("-- Eller --", "-- Slut --");
            readLine("-- Valg (17) --", "-- Valg (18) --");
            text_browser_append("------------------------------------------------------------");

            set_choice_button_text(0, "Fyld 5 liter benzin");
            set_choice_button_text(1, "Spis mad");

            connect_choice_button(0, [=]() { readLine("-- Story 15 --", "-- Story 16 --"); add_tab_item("inventar", "L. Benzin", 5, "", "Orange"); readLine("-- Story 16 --", "-- Story 17 --"); current_choice = 16; storyChoice(); });
            connect_choice_button(1, [=]() { readLine("-- Story 20 --", "-- Story 21 --"); current_choice = 17; storyChoice(); });
            // case 16
            // case 17
            break;

        case 15:
        // Kan startes af case 12 og case 13
        // Fylder benzin || Spiser og venter til i morgen
            readLine("-- Vil du --", "-- Eller --");
            readLine("-- Valg (16) --", "-- Valg (17) --");
            readLine("-- Eller --", "-- Slut --");
            readLine("-- Valg (17) --", "-- Valg (18) --");
            text_browser_append("------------------------------------------------------------");

            set_choice_button_text(0, "Fyld 5 liter benzin");
            set_choice_button_text(1, "Spis mad");

            connect_choice_button(0, [=]() { readLine("-- Story 15 --", "-- Story 16 --"); add_tab_item("inventar", "L. Benzin", 1, "", "Orange"); readLine("-- Story 16 --", "-- Story 17 --"); current_choice = 16; storyChoice(); });
            connect_choice_button(1, [=]() { readLine("-- Story 20 --", "-- Story 21 --"); current_choice = 17; storyChoice(); });
            // case 16
            // case 17
            break;

        case 16:
        // Kan startes af case 14 og case 15
        // Snack med til turen? - Ja || Nej
            readLine("-- Vil du --", "-- Eller --");
            readLine("-- Valg (18) --", "-- Valg (19) --");
            readLine("-- Eller --", "-- Slut --");
            readLine("-- Valg (19) --", "-- Valg (20) --");
            text_browser_append("------------------------------------------------------------");

            set_choice_button_text(0, "Ja");
            set_choice_button_text(1, "Nej");

            connect_choice_button(0, [=]() { readLine("-- Story 17 --", "-- Story 18 --"); add_tab_item("præstationer", "SPISTE EN SNICKERS", true); add_tab_item("inventar", "Benzin", 1); set_label_player_area(player_area = "Bilen"); readLine("-- Story 18 --", "-- Story 19 --"); add_tab_item("præstationer", "FYLDTE BENZIN PÅ BILEN", true); readLine("-- Story 19 --", "-- Story 20 --"); readLine("-- Story 20 --", "-- Story 21 --"); current_choice = 18; storyChoice(); });
            connect_choice_button(1, [=]() { readLine("-- Story 18 --", "-- Story 19 --"); add_tab_item("præstationer", "FYLDTE BENZIN PÅ BILEN", true); readLine("-- Story 19 --", "-- Story 20 --"); readLine("-- Story 20 --", "-- Story 21 --"); current_choice = 19; storyChoice(); });
            // case 18
            // case 19
            break;

        case 17:
        // Kan startes af case 14 og case 15
        // Sov - Bag desken || Baglokalet
            readLine("-- Vil du --", "-- Eller --");
            readLine("-- Valg (20) --", "-- Valg (21) --");
            readLine("-- Eller --", "-- Slut --");
            readLine("-- Valg (21) --", "-- Vil du --");
            text_browser_append("------------------------------------------------------------");

            set_choice_button_text(0, "Sov bag desken");
            set_choice_button_text(1, "Sov i baglokalet");

            connect_choice_button(0, [=]() { set_label_player_area(player_area = "hjemme"); readLine("-- Story 21 --", "||-- Choices --||"); current_choice = 20; storyChoice(); });
            connect_choice_button(1, [=]() { set_label_player_area(player_area = "hjemme"); readLine("-- Story 21 --", "||-- Choices --||"); current_choice = 20; storyChoice(); });
            // case 20
            // case 20
            break;

        case 18:
        // Kan startes af case 16
        // Sov - Bag desken || Baglokalet
            readLine("-- Vil du --", "-- Eller --");
            readLine("-- Valg (20) --", "-- Valg (21) --");
            readLine("-- Eller --", "-- Slut --");
            readLine("-- Valg (21) --", "-- Vil du --");
            text_browser_append("------------------------------------------------------------");

            set_choice_button_text(0, "Sov bag desken");
            set_choice_button_text(1, "Sov i baglokalet");

            connect_choice_button(0, [=]() { set_label_player_area(player_area = "hjemme"); readLine("-- Story 21 --", "||-- Choices --||"); current_choice = 20; storyChoice(); });
            connect_choice_button(1, [=]() { set_label_player_area(player_area = "hjemme"); readLine("-- Story 21 --", "||-- Choices --||"); current_choice = 20; storyChoice(); });
            // case 20
            // case 20
            break;

        case 19:
        // Kan startes af case 16
        // Sov - Bag desken || Baglokalet
            readLine("-- Vil du --", "-- Eller --");
            readLine("-- Valg (20) --", "-- Valg (21) --");
            readLine("-- Eller --", "-- Slut --");
            readLine("-- Valg (21) --", "-- Vil du --");
            text_browser_append("------------------------------------------------------------");

            set_choice_button_text(0, "Sov bag desken");
            set_choice_button_text(1, "Sov i baglokalet");

            connect_choice_button(0, [=]() { set_label_player_area(player_area = "hjemme"); readLine("-- Story 21 --", "||-- Choices --||"); current_choice = 20; storyChoice(); });
            connect_choice_button(1, [=]() { set_label_player_area(player_area = "hjemme"); readLine("-- Story 21 --", "||-- Choices --||"); current_choice = 20; storyChoice(); });
            // case 20
            // case 20
            break;

        case 20:
        // Kan startes af case 17, case 18 og case 19
        // Spillet er nu færdigt og man kan genstarte det hvis man har lyst og prøve at få de sidste præstationer
            add_tab_item("statistikker", "Spil gennemført", 1);
            game_over();
            break;
        }
    }

    ///////////////////////////////////////////////////////////////
    // readline - Denne funktion kan læse og printe et tekst stykke i dialog.txt. Man skal bare indsætte overskriften 
    //            over den øsnkede teskt der skal printes og overskiften under den ønskede tekst
    ///////////////////////////////////////////////////////////////
    void readLine(string read_from, string read_to) {
        int text_file_size = 220;       // Programmet læser til denne linje i .txt - Skal være længere end .txt filen
        int start_stop = 0;             // Bruges til at bestemme hvilken del af teksten der skal printes. Skipper når = 0, printer når = 1
        string line;                    // Bruges til at stoppe med at kigge resten af .txt filen igennem når der er blevet printet det der skal printes
        ifstream myfile("dialog.txt");  // Definere hvilken fil vi vil kigge på

        if (myfile.is_open()) {

            int line_number = 1;         // Linje nummer den tjekker - Bliver til sidst 1 stører så den tjekker alle linjer igennem

            // Begynder at læse filen igennem
            while (line_number != text_file_size && getline(myfile, line)) {

                // Når den kommer til første input i funktionen trigger den næste if-statement
                if (line == read_from) {
                    start_stop = 1;
                }

                // Her begynder den at printe indtil den trigger næste if-statement
                if (start_stop == 1) {
                    while (getline(myfile, line)) {
                        
                        // Når den kommer til sidste input i funktionen stopper den med at printe
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
            return;
        }
        
        qDebug("Unable to open file");
    }

    ///////////////////////////////////////////////////////////////
    // game_over - Afslutter et spil og tillader spilleren at genstarte historien
    //             og kaldes når historien er blevet gennemført eller spilleren dør til monsteret
    ///////////////////////////////////////////////////////////////
    void game_over() {
        set_choice_button_text(0, "Genstart");
        set_choice_button_text(1, "");

        disconnect_choice_button(0);
        disconnect_choice_button(1);
        connect_choice_button(0, [=]() { 
            player_life = 100;
            player_gold = 50; 
            player_armor = 100; 
            player_mana = 0; 
            enemy_life = 80; 
            current_choice = 0;
            set_label_player_area(player_area = "hjemme");
            empty_player_inventory();
            storyLine(); 
        });
    }

public:
    // Nedenfor ses eksempler for hvordan man kan sætte indstillinger op
    // > create_setting_connection(type, tekst, [=](variabler){ funktionalitet })
    // > type: checkbox, dropdown, slider, number
    // > variabler: bool checked (checkbox), int selected_id (dropdown), int slider_val (slider), int number_val (number)
    //
    //create_setting_connection("checkbox", "kage tilgængelighed", [=](bool checked_status) { qDebug() << checked_status; });
    //create_setting_connection("dropdown", "kage farve", [=](int farve_id) { qDebug() << farve_id; }, {"rød", "grøn", "blå"});
    //create_setting_connection("slider", "kage mængde", [=](int slider_val) { qDebug() << slider_val; }, {"0", "100", "25"});
    //create_setting_connection("number", "kage antal", [=](int number_val) { qDebug() << number_val; }, { "0", "100", "5", "25" });
    
    // Tilføj. ændre og fjern tekst til fanerne:
    //add_tab_item("inventar", "kager", 10, "Du opsamlede et æble", "orange"); // vil skabe 10 kager
    //add_tab_item("inventar", "kager", 2); // vil tilføje (ændre) 2 kager oveni
    //remove_tab_item("inventar", "kager", 12, true); // vil fjerne alle kager
    //add_tab_item("inventar", "Æbler", 3);
    //add_tab_item("statistikker", "Dræbt", 0);
    //add_tab_item("præstationer", "Vundet", false); // definere og sætter præstationen til falsk
    //add_tab_item("præstationer", "Vundet", true); // ændre præstationens tilstand til sandt
    //text_browser_append("Du modtog 3 æbler!", "red");

    text_rpg() : text_rpg_qt(800, 600) {
        // Tilføjer præstationer så man kan se hvilket man ikke har opnået 
        add_tab_item("præstationer", "MONSTER KREATUR NEDLAGT", false);
        add_tab_item("præstationer", "KØRTE HURTIGERE END 200 KM/T", false);
        add_tab_item("præstationer", "SPISTE EN SNICKERS", false);
        add_tab_item("præstationer", "FYLDTE BENZIN PÅ BILEN", false);
        add_tab_item("statistikker", "Monstre dræbt", 0);
        add_tab_item("statistikker", "Antal død", 0);
        add_tab_item("statistikker", "Spil gennemført", 0);

        // SKRIV KOMMENTARER
        on_player_name_accept(this, &text_rpg::storyLine, &player_name);
        create_setting_connection("dropdown", "window size", [=](int window_size_id) { 
            switch (window_size_id) {
            case 0:
                set_settings_window_size(800,600);
                break;
            case 1:
                set_settings_window_size(1280,1024);
                break;
            case 2:
                set_settings_window_size(1600,1200);
                break;
            case 3:
                set_settings_window_size(-1, -1);
                break;
            }
        }, {"800x600", "1280x1024", "1600x1200", "Fuld skærm"});
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