// CParams.cpp: 实现文件
//

#include "pch.h"
#include "cpphwfinal.h"
#include "CParams.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>


using namespace cv;
using namespace std;
// CParams

IMPLEMENT_DYNCREATE(CParams, CFormView)

CParams::CParams()
	: CFormView(IDD_FORMVIEW)
{

}

CParams::~CParams()
{
}

void CParams::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CParams, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CParams::OnBnClickedButton1)
    ON_BN_CLICKED(IDC_BUTTON2, &CParams::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CParams::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CParams::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CParams::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CParams::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CParams::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &CParams::OnBnClickedButton8)
END_MESSAGE_MAP()


// CParams 诊断

#ifdef _DEBUG
void CParams::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CParams::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CParams 消息处理程序

CString strFilePath;
string cvname;
Mat img;
Mat grayimg;
Mat hist;
Mat dftresult;
Mat Gauss;
Mat cannyedge;
Mat sobeledge;
Mat lapedge;
bool canny_flag = false;
bool sobel_flag = false;
bool laplacian_flag = false;
void CImage2Mat(CImage& cimage, Mat& mat);
void Mat2CImage(Mat& mat, CImage& cimage);



void CParams::OnBnClickedButton1()		//读取并打开图片
{
	// TODO: 在此添加控件通知处理程序代码
    //选择图片
    CFileDialog fileDlg(TRUE, _T("png"), NULL, 0, _T("image Files(*.bmp; *.jpg;*.png)|*.JPG;*.PNG;*.BMP|All Files (*.*) |*.*||"), this);
    //打开文件选择窗体
    if (fileDlg.DoModal() == IDCANCEL) return; //如果点击“取消”按钮，直接退出
    //获取图片路径（包含名称）
    strFilePath = fileDlg.GetPathName();//绝对路径
    cvname = CStringA(strFilePath);

    //判断路径不为空
    if (strFilePath == _T(""))
    {
        return;
    }
    //使用CImage的Load、Draw函数显示图像
    CImage image;
    image.Load(strFilePath);
	img = imread(cvname);

}


void CParams::OnBnClickedButton2()
{
    // TODO: 在此添加控件通知处理程序代码

    cvtColor(img, grayimg, CV_RGB2GRAY);

}


void CImage2Mat(CImage &cimage, Mat &mat)	//CImage转换为opencvMat
{
    // TODO: 在此处添加实现代码.
	if (true == cimage.IsNull())
	{
		return;
	}


	int nChannels = cimage.GetBPP() / 8;
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth = cimage.GetWidth();
	int nHeight = cimage.GetHeight();


	//重建mat
	if (1 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC1);
	}
	else if (3 == nChannels)
	{
		mat.create(nHeight, nWidth, CV_8UC3);
	}


	//拷贝数据


	uchar* pucRow;									//指向数据区的行指针
	uchar* pucImage = (uchar*)cimage.GetBits();		//指向数据区的指针
	int nStep = cimage.GetPitch();					//每行的字节数,注意这个返回值有正有负


	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				pucRow[nCol] = *(pucImage + nRow * nStep + nCol);
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0; nCha < 3; nCha++)
				{
					pucRow[nCol * 3 + nCha] = *(pucImage + nRow * nStep + nCol * 3 + nCha);
				}
			}
		}
	}

}


void Mat2CImage(Mat& mat, CImage& cimage)	//opencvmat转换为CImage输出
{
	// TODO: 在此处添加实现代码.
	if (0 == mat.total())
	{
		return;
	}


	int nChannels = mat.channels();
	if ((1 != nChannels) && (3 != nChannels))
	{
		return;
	}
	int nWidth = mat.cols;
	int nHeight = mat.rows;


	//重建cimage
	cimage.Destroy();
	cimage.Create(nWidth, nHeight, 8 * nChannels);


	//拷贝数据


	uchar* pucRow;									//指向数据区的行指针
	uchar* pucImage = (uchar*)cimage.GetBits();		//指向数据区的指针
	int nStep = cimage.GetPitch();					//每行的字节数,注意这个返回值有正有负


	if (1 == nChannels)								//对于单通道的图像需要初始化调色板
	{
		RGBQUAD* rgbquadColorTable;
		int nMaxColors = 256;
		rgbquadColorTable = new RGBQUAD[nMaxColors];
		cimage.GetColorTable(0, nMaxColors, rgbquadColorTable);
		for (int nColor = 0; nColor < nMaxColors; nColor++)
		{
			rgbquadColorTable[nColor].rgbBlue = (uchar)nColor;
			rgbquadColorTable[nColor].rgbGreen = (uchar)nColor;
			rgbquadColorTable[nColor].rgbRed = (uchar)nColor;
		}
		cimage.SetColorTable(0, nMaxColors, rgbquadColorTable);
		delete[]rgbquadColorTable;
	}


	for (int nRow = 0; nRow < nHeight; nRow++)
	{
		pucRow = (mat.ptr<uchar>(nRow));
		for (int nCol = 0; nCol < nWidth; nCol++)
		{
			if (1 == nChannels)
			{
				*(pucImage + nRow * nStep + nCol) = pucRow[nCol];
			}
			else if (3 == nChannels)
			{
				for (int nCha = 0; nCha < 3; nCha++)
				{
					*(pucImage + nRow * nStep + nCol * 3 + nCha) = pucRow[nCol * 3 + nCha];
				}
			}
		}
	}
}


void CParams::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	Mat matRGB[3];
	split(img, matRGB);

	int Channels[] = { 0 };
	int nHistSize[] = { 256 };
	float range[] = { 0, 255 };
	const float* fHistRanges[] = { range };
	
	// 计算直方图
	Mat histR, histG, histB;
	calcHist(&matRGB[0], 1, Channels, Mat(), histB, 1, nHistSize, fHistRanges, true, false);
	calcHist(&matRGB[1], 1, Channels, Mat(), histG, 1, nHistSize, fHistRanges, true, false);
	calcHist(&matRGB[2], 1, Channels, Mat(), histR, 1, nHistSize, fHistRanges, true, false);

	// 创建直方图画布
	int nHistWidth = 400;
	int nHistHeight = 300;
	int nBinWidth = cvRound((double)nHistWidth / nHistSize[0]);
	Mat matHistImage(nHistHeight, nHistWidth, CV_8UC3, Scalar(255, 255, 255));
	// 直方图归一化
	normalize(histB, histB, 0.0, matHistImage.rows, NORM_MINMAX, -1, Mat());
	normalize(histG, histG, 0.0, matHistImage.rows, NORM_MINMAX, -1, Mat());
	normalize(histR, histR, 0.0, matHistImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 1; i < nHistSize[0]; i++)
	{
		line(matHistImage,
			Point(nBinWidth * (i - 1), nHistHeight - cvRound(histB.at<float>(i - 1))),
			Point(nBinWidth * (i), nHistHeight - cvRound(histB.at<float>(i))),
			Scalar(255, 0, 0),
			2,
			8,
			0);
		line(matHistImage,
			Point(nBinWidth * (i - 1), nHistHeight - cvRound(histG.at<float>(i - 1))),
			Point(nBinWidth * (i), nHistHeight - cvRound(histG.at<float>(i))),
			Scalar(0, 255, 0),
			2,
			8,
			0);
		line(matHistImage,
			Point(nBinWidth * (i - 1), nHistHeight - cvRound(histR.at<float>(i - 1))),
			Point(nBinWidth * (i), nHistHeight - cvRound(histR.at<float>(i))),
			Scalar(0, 0, 255),
			2,
			8,
			0);
	}

	hist = matHistImage;
}


void CParams::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码

	Mat padded;                 //以0填充输入图像矩阵
	int m = getOptimalDFTSize(grayimg.rows);
	int n = getOptimalDFTSize(grayimg.cols);

	//填充输入图像I，输入矩阵为padded，上方和左方不做填充处理
	copyMakeBorder(grayimg, padded, 0, m - grayimg.rows, 0, n - grayimg.cols, BORDER_CONSTANT, Scalar::all(0));

	Mat planes[] = { Mat_<float>(padded), Mat::zeros(padded.size(),CV_32F) };
	Mat complexI;
	merge(planes, 2, complexI);     //将planes融合合并成一个多通道数组complexI

	dft(complexI, complexI);        //进行傅里叶变换

	//计算幅值，转换到对数尺度(logarithmic scale)
	//=> log(1 + sqrt(Re(DFT(I))^2 + Im(DFT(I))^2))
	split(complexI, planes);        //planes[0] = Re(DFT(I),planes[1] = Im(DFT(I))
									//即planes[0]为实部,planes[1]为虚部
	magnitude(planes[0], planes[1], planes[0]);     //planes[0] = magnitude
	Mat magI = planes[0];
	magI += Scalar::all(1);
	log(magI, magI);                //转换到对数尺度(logarithmic scale)

	//如果有奇数行或列，则对频谱进行裁剪
	magI = magI(Rect(0, 0, magI.cols & -2, magI.rows & -2));

	//重新排列傅里叶图像中的象限，使得原点位于图像中心
	int cx = magI.cols / 2;
	int cy = magI.rows / 2;

	Mat q0(magI, Rect(0, 0, cx, cy));       //左上角图像划定ROI区域
	Mat q1(magI, Rect(cx, 0, cx, cy));      //右上角图像
	Mat q2(magI, Rect(0, cy, cx, cy));      //左下角图像
	Mat q3(magI, Rect(cx, cy, cx, cy));     //右下角图像

	//变换左上角和右下角象限
	Mat tmp;
	q0.copyTo(tmp);
	q3.copyTo(q0);
	tmp.copyTo(q3);

	//变换右上角和左下角象限
	q1.copyTo(tmp);
	q2.copyTo(q1);
	tmp.copyTo(q2);

	//归一化处理，用0-1之间的浮点数将矩阵变换为可视的图像格式
	normalize(magI, magI, 0, 1, CV_MINMAX);

	imwrite("fft.jpg", magI * 256);

	dftresult = magI;

}


void CParams::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码

	GaussianBlur(img, Gauss, Size(5, 5), 3, 3);
	GaussianBlur(Gauss, Gauss, Size(5, 5), 3, 3);

	/*image.Destroy();
	pWnd->ReleaseDC(pDc);*/
}


void CParams::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码

	Canny(grayimg, cannyedge, 30, 80);

	canny_flag = true;
	sobel_flag = false;
	laplacian_flag = false;

}


void CParams::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	Mat  x_edgeImg, y_edgeImg, abs_x_edgeImg, abs_y_edgeImg;

	/*****先对x方向进行边缘检测********/
	//因为Sobel求出来的结果有正负，8位无符号表示不全，故用16位有符号表示
	Sobel(grayimg, x_edgeImg, CV_16S, 1, 0, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(x_edgeImg, abs_x_edgeImg);//将16位有符号转化为8位无符号

	/*****再对y方向进行边缘检测********/
	Sobel(grayimg, y_edgeImg, CV_16S, 0, 1, 3, 1, 1, BORDER_DEFAULT);
	convertScaleAbs(y_edgeImg, abs_y_edgeImg);

	addWeighted(abs_x_edgeImg, 0.5, abs_y_edgeImg, 0.5, 0, sobeledge);

	canny_flag = false;
	sobel_flag = true;
	laplacian_flag = false;

}


void CParams::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	Laplacian(grayimg, lapedge, CV_16S, 5, 1, 0, BORDER_DEFAULT);
	convertScaleAbs(lapedge, lapedge);

	canny_flag = false;
	sobel_flag = false;
	laplacian_flag =true;

}





void CParams::OnDraw(CDC* /*pDC*/)
{
	// TODO: 在此添加专用代码和/或调用基类
	CImage image;
	if (!img.empty())
	{

		image.Load(strFilePath);

		//获取控件的矩形
		CRect rectControl;  //控件矩形对象
		CWnd* pWnd = GetDlgItem(IDC_IMG); //Picture Control的ID为IDC_IMAGE
		pWnd->GetClientRect(&rectControl);

		//以控件为画布，在其上画图
		CDC* pDc = GetDlgItem(IDC_IMG)->GetDC();
		SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//绘图前必须调用此函数（设置缩放模式），否则失真严重

		//画图（以下两种方法都可）
		//image.StretchBlt(pDc->m_hDC, rectPicture, SRCCOPY); //将图片绘制到Picture控件表示的矩形区域
		image.Draw(pDc->m_hDC, rectControl);                //将图片绘制到Picture控件表示的矩形区域
	}
	if (!grayimg.empty())
	{
		Mat2CImage(grayimg, image);

		//获取控件的矩形
		CRect rectControl;  //控件矩形对象
		CWnd* pWnd = GetDlgItem(IDC_IMG2); //Picture Control的ID为IDC_IMAGE
		pWnd->GetClientRect(&rectControl);

		//以控件为画布，在其上画图
		CDC* pDc = GetDlgItem(IDC_IMG2)->GetDC();
		SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//绘图前必须调用此函数（设置缩放模式），否则失真严重

		//画图（以下两种方法都可）
		//image.StretchBlt(pDc->m_hDC, rectPicture, SRCCOPY); //将图片绘制到Picture控件表示的矩形区域
		image.Draw(pDc->m_hDC, rectControl);                //将图片绘制到Picture控件表示的矩形区域
	}
	if (!hist.empty())
	{
		Mat2CImage(hist, image);

		CRect rectControl;  //控件矩形对象
		CWnd* pWnd = GetDlgItem(IDC_IMG3); //Picture Control的ID为IDC_IMAGE
		pWnd->GetClientRect(&rectControl);

		//以控件为画布，在其上画图
		CDC* pDc = GetDlgItem(IDC_IMG3)->GetDC();
		SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//绘图前必须调用此函数（设置缩放模式），否则失真严重

		//画图（以下两种方法都可）
		//image.StretchBlt(pDc->m_hDC, rectPicture, SRCCOPY); //将图片绘制到Picture控件表示的矩形区域
		image.Draw(pDc->m_hDC, rectControl);                //将图片绘制到Picture控件表示的矩形区域
	}
	if (!dftresult.empty())
	{
		Mat dftres = imread("fft.jpg");

		Mat2CImage(dftres, image);

		CRect rectControl;  //控件矩形对象
		CWnd* pWnd = GetDlgItem(IDC_IMG4); //Picture Control的ID为IDC_IMAGE
		pWnd->GetClientRect(&rectControl);

		//以控件为画布，在其上画图
		CDC* pDc = GetDlgItem(IDC_IMG4)->GetDC();
		SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//绘图前必须调用此函数（设置缩放模式），否则失真严重

		//画图（以下两种方法都可）
		//image.StretchBlt(pDc->m_hDC, rectPicture, SRCCOPY); //将图片绘制到Picture控件表示的矩形区域
		image.Draw(pDc->m_hDC, rectControl);                //将图片绘制到Picture控件表示的矩形区域
	}
	if (!Gauss.empty())
	{
		Mat2CImage(Gauss, image);

		CRect rectControl;  //控件矩形对象
		CWnd* pWnd = GetDlgItem(IDC_IMG5); //Picture Control的ID为IDC_IMAGE
		pWnd->GetClientRect(&rectControl);

		//以控件为画布，在其上画图
		CDC* pDc = GetDlgItem(IDC_IMG5)->GetDC();
		SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//绘图前必须调用此函数（设置缩放模式），否则失真严重

		//画图（以下两种方法都可）
		//image.StretchBlt(pDc->m_hDC, rectPicture, SRCCOPY); //将图片绘制到Picture控件表示的矩形区域
		image.Draw(pDc->m_hDC, rectControl);                //将图片绘制到Picture控件表示的矩形区域
	}
	if ((!cannyedge.empty()) && canny_flag)
	{
		Mat2CImage(cannyedge, image);

		CRect rectControl;  //控件矩形对象
		CWnd* pWnd = GetDlgItem(IDC_IMG6); //Picture Control的ID为IDC_IMAGE
		pWnd->GetClientRect(&rectControl);

		//以控件为画布，在其上画图
		CDC* pDc = GetDlgItem(IDC_IMG6)->GetDC();
		SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//绘图前必须调用此函数（设置缩放模式），否则失真严重

		//画图（以下两种方法都可）
		//image.StretchBlt(pDc->m_hDC, rectPicture, SRCCOPY); //将图片绘制到Picture控件表示的矩形区域
		image.Draw(pDc->m_hDC, rectControl);                //将图片绘制到Picture控件表示的矩形区域
	}
	if ((!sobeledge.empty()) && sobel_flag)
	{
		Mat2CImage(sobeledge, image);

		CRect rectControl;  //控件矩形对象
		CWnd* pWnd = GetDlgItem(IDC_IMG6); //Picture Control的ID为IDC_IMAGE
		pWnd->GetClientRect(&rectControl);

		//以控件为画布，在其上画图
		CDC* pDc = GetDlgItem(IDC_IMG6)->GetDC();
		SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//绘图前必须调用此函数（设置缩放模式），否则失真严重

		//画图（以下两种方法都可）
		//image.StretchBlt(pDc->m_hDC, rectPicture, SRCCOPY); //将图片绘制到Picture控件表示的矩形区域
		image.Draw(pDc->m_hDC, rectControl);                //将图片绘制到Picture控件表示的矩形区域
	}
	if ((!lapedge.empty()) && laplacian_flag)
	{
		Mat2CImage(lapedge, image);

		CRect rectControl;  //控件矩形对象
		CWnd* pWnd = GetDlgItem(IDC_IMG6); //Picture Control的ID为IDC_IMAGE
		pWnd->GetClientRect(&rectControl);

		//以控件为画布，在其上画图
		CDC* pDc = GetDlgItem(IDC_IMG6)->GetDC();
		SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//绘图前必须调用此函数（设置缩放模式），否则失真严重

		//画图（以下两种方法都可）
		//image.StretchBlt(pDc->m_hDC, rectPicture, SRCCOPY); //将图片绘制到Picture控件表示的矩形区域
		image.Draw(pDc->m_hDC, rectControl);                //将图片绘制到Picture控件表示的矩形区域

	}

}
