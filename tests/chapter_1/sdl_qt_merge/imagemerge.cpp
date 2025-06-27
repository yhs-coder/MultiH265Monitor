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

	// ������ͼ��
	QImage img1("1.jpeg");
	QImage img2("2.jpg");
	// �ж�ͼ���Ƿ�򿪳ɹ�
	if (img1.isNull() || img2.isNull()) {
		QMessageBox::information(this, "error", "open image failed!");
		return;
	}

	// ���´��ڵĿ�Ⱥ͸߶�
	int out_w = img1.width() + img2.width();
	int out_h = img1.height();
	// ȡ����ͼ��߶ȵ����ֵ
	if (out_h < img2.height()) {
		out_h = img2.height();
	}
	sdl_width = out_w;
	sdl_height = out_h;

	// ���ô��ڴ�С
	resize(sdl_width, sdl_height);
	ui.label->move(0, 0);
	// ��Ҫ�޸�label��С
	ui.label->resize(sdl_width, sdl_height);

	// ��ʼ��SDL video��
	SDL_Init(SDL_INIT_VIDEO);

	// ��sdl��Ⱦ��ͼ�����ݣ��󶨵�qt�ؼ���
	sdl_win = SDL_CreateWindowFrom((void*)ui.label->winId());
	
	// ������Ⱦ��
	sdl_render = SDL_CreateRenderer(sdl_win, -1, SDL_RENDERER_ACCELERATED);

	// ��������
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

	// ��������ͼ���С�Ŀռ�
	rgb = new unsigned char[sdl_width * sdl_height * pix_size];

	// Ĭ������Ϊ͸�� 
	memset(rgb, 0, sdl_width * sdl_height * pix_size);
	// ��ʼ�ϲ�����ͼ��
	for (int i = 0; i < sdl_height; i++) {
		int gap = i * sdl_width * pix_size;
		if (i < img1.height()) {
			// ����ͼ��1���е�ͼ������
			memcpy(rgb + gap, img1.scanLine(i), img1.width() * pix_size);
		}
		// ����gap��ָ�����ͼ��2����ʼλ��
		gap += img1.width() * pix_size;
		if (i < img2.height()) {
			// ����ͼ��2���е�ͼ������
			memcpy(rgb + gap, img2.scanLine(i), img2.width() * pix_size);
		}
	}
	// ע���ʽҪ��SDL��ʽһ��
	// ����ϲ�����ļ�������
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

	// ���ڴ�����д�����
	SDL_UpdateTexture(sdl_texture, NULL, rgb, sdl_width * pix_size);
	// ������Ļ
	SDL_RenderClear(sdl_render);
	SDL_Rect sdl_rect;
	sdl_rect.x = 0;
	sdl_rect.y = 0;
	sdl_rect.w = sdl_width;
	sdl_rect.h = sdl_height;
	// ���������ݸ��Ƶ���Ⱦ
	SDL_RenderCopy(sdl_render, sdl_texture,
		NULL,		// ԭͼλ�úͳߴ�
		&sdl_rect	// Ŀ��λ�úͳߴ�
	);
	// ������Ļ��ʾ
	SDL_RenderPresent(sdl_render);
}
