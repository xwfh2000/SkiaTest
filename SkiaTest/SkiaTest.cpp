// SkiaTest.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"



#include <SkBitmap.h>
#include <SkCanvas.h>
#include <SkTypeface.h>
#include <SkPath.h>
#include <SkDiscretePathEffect.h>
#include <SkGradientShader.h>
#include "include/core/SkTypes.h"

#ifdef _WIN32

#include <SDL.h>

#else

#include <SDL2/SDL.h>

#endif

//创建RGBA结构体
struct RGBA {
	//设置Red
	Uint32 rmask = 0x00ff0000;
	//设置Green
	Uint32 gmask = 0x0000ff00;
	//设置Blue
	Uint32 bmask = 0x000000ff;
	//设置Alpha
	Uint32 amask = 0xff000000;
};

SkPath star() {
	const SkScalar R = 60.0f, C = 128.0f;
	SkPath path;
	path.moveTo(C + R, C);
	for (int i = 1; i < 15; ++i) {
		SkScalar a = 0.44879895f * i;
		SkScalar r = R + R * (i % 2);
		path.lineTo(C + r * cos(a), C + r * sin(a));
	}
	return path;
}
void draw(SkCanvas* canvas) {
	SkPaint paint;
	paint.setPathEffect(SkDiscretePathEffect::Make(10.0f, 4.0f));
	SkPoint points[2] = { SkPoint::Make(0.0f, 0.0f), SkPoint::Make(256.0f, 256.0f) };
	SkColor colors[2] = { SkColorSetRGB(66, 133, 244), SkColorSetRGB(15, 157, 88) };
	paint.setShader(
		SkGradientShader::MakeLinear(points, colors, NULL, 2, SkShader::TileMode::kClamp_TileMode, 0, NULL));
	paint.setAntiAlias(true);
	canvas->clear(SK_ColorWHITE);
	SkPath path(star());
	canvas->drawPath(path, paint);
}

SkBitmap draw(int w, int h) {
	//声明
	SkBitmap bitmap;
	//设置Bitmap的配置信息
	bitmap.setInfo(SkImageInfo::Make(w, h, kBGRA_8888_SkColorType, kOpaque_SkAlphaType));
	//转换为像素填充
	bitmap.allocPixels();
	//创建画布
	SkCanvas canvas(bitmap);
	draw(&canvas);


	//返回SkBitmap对象
	return bitmap;
}

////创建SkBitmap对象并在Bitmap上绘制
//SkBitmap draw(int w, int h) {
//	//声明
//	SkBitmap bitmap;
//	//设置Bitmap的配置信息
//	bitmap.setInfo(SkImageInfo::Make(w, h, kBGRA_8888_SkColorType, kOpaque_SkAlphaType));
//	//转换为像素填充
//	bitmap.allocPixels();
//	//创建画布
//	SkCanvas canvas(bitmap);
//	//创建画笔
//	SkPaint paint;
//	//设置画布颜色
//	canvas.clear(SK_ColorWHITE);
//	//设置画笔抗锯齿
//	paint.setAntiAlias(true);
//	//设置画笔颜色(此处为红色)
//	paint.setARGB(255, 255, 0, 0);
//	//绘制圆形
//	canvas.drawCircle(80, 80, 40, paint);
//	//绘制线段
//	canvas.drawLine(0, 280, w, 280, paint);
//	//设置字体大小
//	paint.setTextSize(60);
//	//绘制字体
//	canvas.drawString("Hello Skia", 300, 150, paint);
//	//绘制点别的
//
//	//返回SkBitmap对象
//	return bitmap;
//}

//通过SDL_Surface创建SDL_Rect
SDL_Rect create_rect(SDL_Surface *surface) {
	//创建SDL_Rect对象
	SDL_Rect src = { 0, 0, surface->w, surface->h };
	//返回SDL_Rect对象
	return src;
}

//程序的入口点
int _tmain(int args, _TCHAR* argv[]) {
	//声明窗口
	SDL_Window *window;
	//声明渲染器
	SDL_Renderer *renderer;

	//声明窗口的宽高
	int width = 800;
	int height = 480;
	//声明RGBA结构体
	RGBA rgba;
	//声明Bitmap
	SkBitmap bitmap;
	//声明矩形
	SDL_Rect rect;
	//声明绘图表面
	SDL_Surface *surface;
	//声明纹理
	SDL_Texture *texture;
	
	//初始化SDL为视频显示
	SDL_Init(SDL_INIT_VIDEO);
	//创建窗口
	window = SDL_CreateWindow("Hello Skia", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN |SDL_WINDOW_RESIZABLE|SDL_WINDOW_MOUSE_CAPTURE);
	if (window == NULL) {
		return -1;
	}
	//创建渲染器
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	//获取绘制后的Bitmap
	bitmap = draw(width, height);
	//通过Bitmap的像素数据创建表面
	surface = SDL_CreateRGBSurfaceFrom(bitmap.getPixels(), width, height, 32, width * 4, rgba.rmask, rgba.gmask,
		rgba.bmask, rgba.amask);
	//清理渲染器
	SDL_RenderClear(renderer);
	//创建纹理
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	//通过SDL_Surface创建矩形
	rect = create_rect(surface);
	//辅助纹理到渲染器
	SDL_RenderCopy(renderer, texture, NULL, &rect);


	bool bQuit = false;     //程序是否退出的标志  
	SDL_Event event;
	while (!bQuit)           //如果bQuit不为true的话，循环一直进行  
	{
		//从消息循环中尝试取出一个消息，如果有的话返回ture，否则返回false  
		//SDL_PollEvent不会等待，而是立刻返回  
		while (SDL_PollEvent(&event))
		{
			switch (event.type)      //处理取到的消息，判断消息类型  
			{
			case SDL_KEYDOWN:       //如果是键盘按下事件  
				if (event.key.keysym.sym == SDLK_ESCAPE) //如果按的是ESC键  
				{
					bQuit = true;                       //退出循环  
				}
				break;
			case SDL_QUIT:          //如果是窗口关闭事件，例如点击窗口的关闭按钮  
				bQuit = true;                           //退出循环  
				break;


			default:
				break;
			}
		}

		//将内存中的内容显示到屏幕上，该函数每调用一次就会更新一次窗口的内容，即游戏中的一帧  
		//由于在这个循环中我们没有修改内存中的内容，所以屏幕窗口中显示的内容不会发生变化  
		//显示到窗口
		SDL_RenderPresent(renderer);

		////延时5秒钟
		//SDL_Delay(20000);
	}

	//释放纹理
	SDL_DestroyTexture(texture);
	//释放表面
	SDL_FreeSurface(surface);	
	//释放渲染器
	SDL_DestroyRenderer(renderer);
	//释放窗口
	SDL_DestroyWindow(window);
	//结束SDL
	SDL_Quit();
	//程序退出
	return 0;
}