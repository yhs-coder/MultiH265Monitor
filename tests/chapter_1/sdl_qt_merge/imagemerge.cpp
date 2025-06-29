#include "imagemerge.h"
#include <sdl/SDL.h>
#include <iostream>
#include <QImage>
#include <QMessageBox>

#pragma comment(lib, "SDL2.lib")

static  SDL_Window* sdl_win = nullptr;
static SDL_Renderer* sdl_render = nullptr;
static SDL_Texture* sdl_texture = nullptr;
static int sdl_width = 0;
static int sdl_height = 0;
static unsigned char* rgb = nullptr;
static int pix_size = 4;

ImageMerge::ImageMerge(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	// 打开两幅图像
	QImage img1("1.jpeg");
	QImage img2("2.jpg");
	// 判断图像是否打开成功
	if (img1.isNull() || img2.isNull()) {
		QMessageBox::information(this, "error", "open image failed!");
		return;
	}

	// 更新窗口的宽度和高度
	int out_w = img1.width() + img2.width();
	int out_h = img1.height();
	// 取两幅图像高度的最大值
	if (out_h < img2.height()) {
		out_h = img2.height();
	}
	sdl_width = out_w;
	sdl_height = out_h;

	// 重置窗口大小
	resize(sdl_width, sdl_height);
	ui.label->move(0, 0);
	// 需要修改label大小
	ui.label->resize(sdl_width, sdl_height);

	// 初始化SDL video库
	SDL_Init(SDL_INIT_VIDEO);

	// 将sdl渲染的图像数据，绑定到qt控件上
	sdl_win = SDL_CreateWindowFrom((void*)ui.label->winId());
	
	// 创建渲染器
	sdl_render = SDL_CreateRenderer(sdl_win, -1, SDL_RENDERER_ACCELERATED);

	// 创建材质
	sdl_texture = SDL_CreateTexture(sdl_render,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		sdl_width,
		sdl_height
	);
	if (!sdl_texture) {
		std::cout << SDL_GetError() << std::endl;
		return;
	}

	// 开辟两幅图像大小的空间
	rgb = new unsigned char[sdl_width * sdl_height * pix_size];

	// 默认设置为透明 
	memset(rgb, 0, sdl_width * sdl_height * pix_size);
	// 开始合并两幅图像
	for (int i = 0; i < sdl_height; i++) {
		int gap = i * sdl_width * pix_size;
		if (i < img1.height()) {
			// 拷贝图像1该行的图像数据
			memcpy(rgb + gap, img1.scanLine(i), img1.width() * pix_size);
		}
		// 更新gap，指向该行图像2的起始位置
		gap += img1.width() * pix_size;
		if (i < img2.height()) {
			// 拷贝图像2该行的图像数据
			memcpy(rgb + gap, img2.scanLine(i), img2.width() * pix_size);
		}
	}
	// 注意格式要和SDL格式一致
	// 保存合并后的文件到本地
	QImage out(rgb, sdl_width, sdl_height, QImage::Format_ARGB32);
	out.save("out.png");

	startTimer(10);
}

ImageMerge::~ImageMerge()
{}

void ImageMerge::timerEvent(QTimerEvent * ev)
{

	static unsigned char tmp = 255;
	tmp--;
	for (int i = 0; i < sdl_height; i++) {
		int gap = i * sdl_width * pix_size;
		for (int j = 0; j < sdl_width * pix_size; j += pix_size) {
			//rgb[gap + j] = 0;			// B
			//rgb[gap + j + 1] = 0;		// G
			rgb[gap + j + 2] = tmp;		// R
			//rgb[gap + j + 3] = 0;		// A
		}
	}

	// 将内存数据写入材质
	SDL_UpdateTexture(sdl_texture, NULL, rgb, sdl_width * pix_size);
	// 清理屏幕
	SDL_RenderClear(sdl_render);
	SDL_Rect sdl_rect;
	sdl_rect.x = 0;
	sdl_rect.y = 0;
	sdl_rect.w = sdl_width;
	sdl_rect.h = sdl_height;
	// 将材质数据复制到渲染
	SDL_RenderCopy(sdl_render, sdl_texture,
		NULL,		// 原图位置和尺寸
		&sdl_rect	// 目标位置和尺寸
	);
	// 更新屏幕显示
	SDL_RenderPresent(sdl_render);
}
