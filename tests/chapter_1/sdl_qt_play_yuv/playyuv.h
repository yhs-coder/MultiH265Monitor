#pragma once

#include <QtWidgets/QWidget>
#include "ui_playyuv.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PlayYUVClass; };
QT_END_NAMESPACE

class PlayYUV : public QWidget
{
    Q_OBJECT

public:
    PlayYUV(QWidget *parent = nullptr);
    ~PlayYUV();
    void timerEvent(QTimerEvent* ev) override;

private:
    Ui::PlayYUVClass *ui;
};
