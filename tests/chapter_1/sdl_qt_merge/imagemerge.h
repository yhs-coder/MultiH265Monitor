#pragma once

#include <QtWidgets/QWidget>
#include "ui_imagemerge.h"

class ImageMerge : public QWidget
{
    Q_OBJECT

public:
    ImageMerge(QWidget *parent = nullptr);
    ~ImageMerge();
    void timerEvent(QTimerEvent* ev) override;

private:
    Ui::ImageMergeClass ui;
};
