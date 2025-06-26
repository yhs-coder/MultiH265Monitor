#pragma once

#include <QtWidgets/QWidget>
#include "ui_sdlqtrgb.h"

class SdlQtRGB : public QWidget
{
    Q_OBJECT

public:
    SdlQtRGB(QWidget *parent = nullptr);
    ~SdlQtRGB();
    void timerEvent(QTimerEvent* ev) override;
private:
    Ui::SdlQtRGBClass ui;
};
