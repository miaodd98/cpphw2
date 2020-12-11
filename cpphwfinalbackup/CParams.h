#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>

// CParams 窗体视图

class CParams : public CFormView
{
	DECLARE_DYNCREATE(CParams)

protected:
	CParams();           // 动态创建所使用的受保护的构造函数
	virtual ~CParams();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORMVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
//	virtual void OnDraw(CDC* /*pDC*/);
	virtual void OnDraw(CDC* /*pDC*/);
};


