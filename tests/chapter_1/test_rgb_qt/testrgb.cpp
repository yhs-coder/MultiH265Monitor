#include "testrgb.h"
#include <QPainter>
#include <QImage>

// 全局变量命名时，尽量避免和QWidget类中成员同名，否则该变量和QWidget类内变量有冲突！
// 如果一定要使用该命名，就将其定义为类内成员变量，或者通过::width明确指定全局变量
// 和编译器的作用域解析机制和变量查找规则有关，即先 当前类->基类->全局作用域的变量和函数
//static int width = 1280; // !error
//static int height = 720; // !error


static int w = 1280;
static int h = 720;

TestRGB::TestRGB(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    // 重置窗口大小
    resize(w, h);
}

TestRGB::~TestRGB()
{
}

void TestRGB::paintEvent(QPaintEvent* ev)
{
    // QImage对象，存储RGB数据
    QImage img(w, h, QImage::Format_RGB888);
    // bits() 返回指向图像像素数据的指针，用于直接操作图像的 RGB 数据
    auto data = img.bits();
    unsigned char r = 255;
    // 设置RGB，填充图像数据
    for (int j = 0; j < h; j++) {
        r--;
        int b = j * w * 3;
        for (int i = 0; i < w * 3; i += 3) {
            data[b + i] = r;          // R
            data[b + i + 1] = 100;    // G
            data[b + i + 2] = 100;    // B
        }
    }
    // 创建一个绘制对象
    QPainter p;
    // 将绘制目标设置为当前窗口
    p.begin(this);
    // 将 QImage 对象 img 绘制到窗口的左上角 (0, 0)
    p.drawImage(0, 0, img);
    // 结束绘制操作，释放资源
    p.end();
}
