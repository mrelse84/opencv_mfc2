
// opencv_mfc2Dlg.h: 헤더 파일
//

#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/objdetect/objdetect.hpp> 
#include <opencv2/features2d/features2d.hpp> 
//#include <opencv2/nonfree/features2d.hpp> 
#include <opencv2/xfeatures2d/nonfree.hpp> 
#include <opencv2/flann/flann.hpp> 


using namespace std;
using namespace cv;
using namespace xfeatures2d;

// Copencvmfc2Dlg 대화 상자
class Copencvmfc2Dlg : public CDialogEx
{
// 생성입니다.
public:
	Copencvmfc2Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENCV_MFC2_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpen();
	afx_msg void OnBnClickedBtnCapture();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	// Images
	//=========================================================================================
	// OpenCV Images
	Mat m_matImg;				// Mat
	IplImage  m_InImage;		// IplImage
	IplImage* m_pInImage;		// IplImage Pointer
	IplImage* m_pOutImage;		// IplImage Pointer

								// MFC Images
	CImage   m_cImage;			// MS Official
	//CvvImage m_cvvImage;		// Non-Official

								// Windows Images
	CStatic m_picImg1;
	CStatic m_picImg2;

	// Display Image(s)
	//=========================================================================================
	//void ShowCvvImage();
	void ShowCImage();


	// Video
	VideoCapture* m_pCapture;


	// Detect Face and Eye
	//=========================================================================================
	String face_cascade = "d:/sglee/dev_sgl/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
	String eye_cascade = "d:/sglee/dev_sgl/opencv/sources/data/haarcascades/haarcascade_eye.xml";
	CascadeClassifier face;
	CascadeClassifier eye;
	void FaceAndEyeDetect(Mat img);

	afx_msg void OnBnClickedBtnDetect();
	afx_msg void OnBnClickedBtnObject();
};
