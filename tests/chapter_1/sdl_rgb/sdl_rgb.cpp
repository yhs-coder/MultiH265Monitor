#include <sdl/SDL.h>
#include <iostream>
#include <memory>

// 导入SDL2库
#pragma comment(lib, "SDL2.lib")
#undef main
using namespace std;

int main(int argc, char* argv[])
{
	int width = 800;
	int height = 600;
	// 1. 初始化SDL video库，（即初始化SDL的视频子系统)
	// 初始化成功返回0
	if (SDL_Init(SDL_INIT_VIDEO)) {
		cout << SDL_GetError() << endl;
		return -1;
	}

	// 2. 生成SDL 窗口
	auto screen = SDL_CreateWindow("test-sdl",
		SDL_WINDOWPOS_CENTERED, // 窗口位置
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	if (!screen) {
		cout << SDL_GetError() << endl;
		return -2;
	}

	// 3. 生成一个硬件加速的渲染器
	auto render = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED);	
	//auto render = SDL_CreateRenderer(screen, -1, SDL_RENDERER_SOFTWARE);
	if (!render) {
		cout << SDL_GetError() << endl;
		return -3;
	}

	// 4. 生成材质
	auto texture = SDL_CreateTexture(render,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,	// 频繁修改，可加锁 
		width,
		height);
	if (!texture) {
		cout << SDL_GetError() << endl;
		return -4;
	}

	// 存放图像的数据
	shared_ptr<unsigned char> rgb(new unsigned char[width * height * 4]);
	auto data = rgb.get();
	unsigned char tmp = 255;

	// 事件循环处理
	for (;;) {
		//判断退出
		SDL_Event ev;
		// 事件超时时间
		SDL_WaitEventTimeout(&ev, 10);
		if (ev.type == SDL_QUIT)
		{
			break;
		}
		tmp--;

		// 数据处理
		for (int i = 0; i < height; i++) {
			int gap = i * width * 4;
			for (int j = 0; j < width * 4; j += 4) {
				// SDL 的像素格式字节序会根据平台的字节序而变化
				// 在小端系统上，如 x86/x64，其实际存储顺序为 BGRA，最低有效字节存储在最低地址处
				// 而Qt字节序固定为 RGB，与平台字节序无关。只要按照 RGB 的顺序填充数据即可
				data[gap + j] = tmp;		// B
				data[gap + j + 1] = 150;	// G
				data[gap + j + 2] = tmp;	// R
				data[gap + j + 3] = 0;		// A
			}
		}
		// 5. 将内存数据写入材质
		SDL_UpdateTexture(texture, NULL, data, width * 4);


		// 6. 清理屏幕
		SDL_RenderClear(render);
		SDL_Rect sdl_rect;
		sdl_rect.x = 0;
		sdl_rect.y = 0;
		sdl_rect.w = width;
		sdl_rect.h = height;

		// 7. 复制材质到渲染
		SDL_RenderCopy(render, texture,
			NULL,		// 原图位置和尺寸
			&sdl_rect	// 目标位置和尺寸
		);
		// 8. 更新屏幕显示
		SDL_RenderPresent(render);
	}

	// 释放资源
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(screen);
	SDL_Quit();
	//getchar();
	return 0;
}