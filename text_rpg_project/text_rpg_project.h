#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_text_rpg_project.h"

class text_rpg_project : public QMainWindow
{
    Q_OBJECT

public:
    text_rpg_project(QWidget *parent = Q_NULLPTR);

private:
    Ui::text_rpg_projectClass ui;
};
