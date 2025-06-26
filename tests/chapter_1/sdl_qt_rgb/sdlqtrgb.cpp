#include "sdlqtrgb.h"
#include <sdl/SDL.h>
#include <iostream>

#pragma comment(lib, "SDL2.lib")

static  SDL_Window* sdl_win = nullptr;
static SDL_Renderer* sdl_render = nullptr;
static SDL_Texture* sdl_texture = nullptr;
static int sdl_width = 0;
static int sdl_height = 0;
static unsigned char* rgb = nullptr;
static int pix_size = 4;

SdlQtRGB::SdlQtRGB(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	sdl_width = ui.label->width();
	sdl_height = ui.label->height();
	// 初始化SDL video库
	SDL_Init(SDL_INIT_VIDEO);


	// 将sdl渲染的图像数据，绑定到qt控件上
	sdl_win = SDL_CreateWindowFrom((void*)ui.label->winId());

	// 创建渲染器
	sdl_render = SDL_CreateRenderer(sdl_win, -1, SDL_RENDERER_ACCELERATED);

	// 创建材质
	sdl_texture = SDL_CreateTexture(sdl_render,
		SDL_PIXELFORMAT_ABGR8888,
		SDL_TEXTUREACCESS_STREAMING,
		sdl_width,
		sdl_height
	);
	if (!sdl_texture) {
		std::cout << SDL_GetError() << std::endl;
		return;
	}
	rgb = new unsigned char[sdl_width * sdl_height * pix_size];
	startTimer(10);

}

SdlQtRGB::~SdlQtRGB()
{}

void SdlQtRGB::timerEvent(QTimerEvent* ev)
{
	static unsigned char tmp = 255;
	tmp--;
	for (int i = 0; i < sdl_height; i++) {
		int gap = i * sdl_width * pix_size;
		for (int j = 0; j < sdl_width * pix_size; j += pix_size) {
			rgb[gap + j] = tmp;			// B
			rgb[gap + j + 1] = 150;		// G
			rgb[gap + j + 2] = tmp;		// R
			rgb[gap + j + 3] = 0;		// A
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
