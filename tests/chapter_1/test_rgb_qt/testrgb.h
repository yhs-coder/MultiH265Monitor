#pragma once

#include <QtWidgets/QWidget>
#include "ui_testrgb.h"

class TestRGB : public QWidget
{
    Q_OBJECT

public:
    TestRGB(QWidget *parent = nullptr);
    ~TestRGB();

    // 重载窗口绘制事件
    void paintEvent(QPaintEvent* ev) override;

private:
    Ui::TestRGBClass ui;
};
