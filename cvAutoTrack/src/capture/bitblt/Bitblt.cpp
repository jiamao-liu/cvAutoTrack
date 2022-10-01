#include "pch.h"
#include "Bitblt.h"

Bitblt::Bitblt()
{
	mode = Capture::Mode_Bitblt;
}

Bitblt::~Bitblt()
{
}

bool Bitblt::init()
{
	return true;
}

bool Bitblt::uninit()
{
	return true;
}

bool Bitblt::capture(cv::Mat& frame)
{
	static HBITMAP	hBmp;
	BITMAP bmp;

	DeleteObject(hBmp);

	if (giHandle == NULL)
	{
		err = 12;//���ھ��ʧЧ
		return false;
	}
	if (!IsWindow(giHandle))
	{
		err = 12;//���ھ��ʧЧ
		return false;
	}
	if (!GetWindowRect(giHandle, &giRect))
	{
		err = 12;//���ھ��ʧЧ
		return false;
	}
	if (!GetClientRect(giHandle, &giClientRect))
	{
		err = 12;//���ھ��ʧЧ
		return false;
	}

	//��ȡ��Ļ���ű���
	HWND hWnd = GetDesktopWindow();
	HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

	// ��ȡ�������߼�������߶�
	MONITORINFOEX miex;
	miex.cbSize = sizeof(miex);
	GetMonitorInfo(hMonitor, &miex);
	int cxLogical = (miex.rcMonitor.right - miex.rcMonitor.left);
	//int cyLogical = (miex.rcMonitor.bottom - miex.rcMonitor.top);

	// ��ȡ����������������߶�
	DEVMODE dm;
	dm.dmSize = sizeof(dm);
	dm.dmDriverExtra = 0;
	EnumDisplaySettings(miex.szDevice, ENUM_CURRENT_SETTINGS, &dm);
	int cxPhysical = dm.dmPelsWidth;
	//int cyPhysical = dm.dmPelsHeight;

	double horzScale = ((double)cxPhysical / (double)cxLogical);
	double screen_scale = horzScale;

	giClientSize.width = (int)(screen_scale * (giClientRect.right - giClientRect.left));
	giClientSize.height = (int)(screen_scale * (giClientRect.bottom - giClientRect.top));

	//��ȡĿ�����Ĵ��ڴ�СRECT
	GetWindowRect(giHandle, &giRect);/* ��ԭ�񴰿ڵĲ��� */

	//��ȡĿ������DC
	HDC hScreen = GetDC(giHandle);/* ��ԭ�񴰿ڵĲ��� */
	HDC hCompDC = CreateCompatibleDC(hScreen);

	//��ȡĿ�����Ŀ��Ⱥ͸߶�
	int	nWidth = (int)((screen_scale) * (giRect.right - giRect.left));
	int	nHeight = (int)((screen_scale) * (giRect.bottom - giRect.top));

	//����Bitmap����
	hBmp = CreateCompatibleBitmap(hScreen, nWidth, nHeight);//�õ�λͼ

	SelectObject(hCompDC, hBmp); //��д��ȫ��

	BitBlt(hCompDC, 0, 0, nWidth, nHeight, hScreen, 0, 0, SRCCOPY);
	
	////�ͷŶ���
	DeleteDC(hScreen);
	DeleteDC(hCompDC);

	//����ת��
	//�����ȡλͼ�Ĵ�С��Ϣ,��ʵ��Ҳ�Ǽ���DC��ͼ����ķ�Χ
	GetObject(hBmp, sizeof(BITMAP), &bmp);

	int nChannels = bmp.bmBitsPixel == 1 ? 1 : bmp.bmBitsPixel / 8;
	//int depth = bmp.bmBitsPixel == 1 ? 1 : 8;

	//mat����
	giFrame.create(cv::Size(bmp.bmWidth, bmp.bmHeight), CV_MAKETYPE(CV_8U, nChannels));

	GetBitmapBits(hBmp, bmp.bmHeight * bmp.bmWidth * nChannels, giFrame.data);

	giFrame = giFrame(cv::Rect(giClientRect.left, giClientRect.top, giClientSize.width, giClientSize.height));


	if (giFrame.empty())
	{
		err = 3;
		return false;
	}

	if (giFrame.cols < 480 || giFrame.rows < 360)
	{
		err = 13;
		return false;
	}
	frame = giFrame;
	return true;
}

bool Bitblt::setHandle(HWND handle)
{
	if (giHandle != handle)
	{
		uninit();
		giHandle = handle;
		init();
	}
	return true;
}