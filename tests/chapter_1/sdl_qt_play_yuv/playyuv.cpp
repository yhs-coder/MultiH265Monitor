#include "playyuv.h"
#include <sdl/SDL.h>
#include <fstream>
#include <QMessageBox>

#pragma comment(lib, "SDL2.lib")

static  SDL_Window* sdl_win = nullptr;
static SDL_Renderer* sdl_render = nullptr;
static SDL_Texture* sdl_texture = nullptr;
static int sdl_width = 0;
static int sdl_height = 0;
static unsigned char* yuv = nullptr;
static int pix_size = 2;
static std::ifstream yuv_file;

PlayYUV::PlayYUV(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::PlayYUVClass())
{
    ui->setupUi(this);

	// 打开yuv格式文件
	yuv_file.open("400_300_25.yuv", std::ios::binary);
	if (!yuv_file) {
		QMessageBox::information(this, "error", "open yuv file failed!");
		exit(-1);
	}

	sdl_width = 400;
	sdl_height =300;
	ui->label->move(20, 20);
	ui->label->resize(sdl_width, sdl_height);

	// 初始化SDL video库
	SDL_Init(SDL_INIT_VIDEO);

	// 将sdl渲染的图像数据，绑定到qt控件上
	sdl_win = SDL_CreateWindowFrom((void*)ui->label->winId());

	// 创建渲染器
	sdl_render = SDL_CreateRenderer(sdl_win, -1, SDL_RENDERER_ACCELERATED);

	// 创建材质,支持yuv格式
	sdl_texture = SDL_CreateTexture(sdl_render,
		SDL_PIXELFORMAT_IYUV,		// SDL采用yuv格式
		SDL_TEXTUREACCESS_STREAMING,
		sdl_width,
		sdl_height
	);
	yuv = new unsigned char[sdl_width * sdl_height * pix_size];
	startTimer(10);
}

PlayYUV::~PlayYUV()
{
    delete ui;
}

void PlayYUV::timerEvent(QTimerEvent* ev)
{
	// 读取文件内容，这里1.5字节是因为使用yuv420格式采样，4个y分量共用一个u、v分量，每个像素占1.5字节
	yuv_file.read((char*)yuv, sdl_width * sdl_height * 1.5);

	// yuv是平面存储格式，不像rgb像素格式交叉存储，而是一行存储的都是y，u，v
	// yyyyyyyy
	// uu
	// vv
	// 将内存数据写入材质
	SDL_UpdateTexture(sdl_texture, NULL, yuv, 
		sdl_width		// 一行y的字节数
	);
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
