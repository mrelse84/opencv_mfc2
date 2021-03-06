
// opencv_mfc2Dlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "opencv_mfc2.h"
#include "opencv_mfc2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define RED Scalar(0,0,255)

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Copencvmfc2Dlg 대화 상자



Copencvmfc2Dlg::Copencvmfc2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPENCV_MFC2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Copencvmfc2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_IMAGE1, m_picImg1);
}

BEGIN_MESSAGE_MAP(Copencvmfc2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_OPEN, &Copencvmfc2Dlg::OnBnClickedBtnOpen)
	ON_BN_CLICKED(IDC_BTN_CAPTURE, &Copencvmfc2Dlg::OnBnClickedBtnCapture)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_DETECT, &Copencvmfc2Dlg::OnBnClickedBtnDetect)
	ON_BN_CLICKED(IDC_BTN_OBJECT, &Copencvmfc2Dlg::OnBnClickedBtnObject)
END_MESSAGE_MAP()


// Copencvmfc2Dlg 메시지 처리기

BOOL Copencvmfc2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_matImg = NULL;
	m_pInImage = NULL;
	m_pOutImage = NULL;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void Copencvmfc2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void Copencvmfc2Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR Copencvmfc2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Copencvmfc2Dlg::ShowCImage()
{
	if (m_pInImage != NULL)
	{
		RECT r;
		m_picImg1.GetClientRect(&r);
		cv::Size winSize(r.right, r.bottom);
		m_cImage.Create(winSize.width, winSize.height, 24);

		int bpp = 8 * m_matImg.elemSize();
		int border = 0;
		//32 bit image is always DWORD aligned because each pixel requires 4 bytes
		if (bpp < 32)
		{
			border = 4 - (m_matImg.cols % 4);
		}

		BITMAPINFO bitInfo;
		bitInfo.bmiHeader.biBitCount = bpp;
		bitInfo.bmiHeader.biWidth = m_matImg.cols;
		bitInfo.bmiHeader.biHeight = -m_matImg.rows;
		bitInfo.bmiHeader.biPlanes = 1;
		bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitInfo.bmiHeader.biCompression = BI_RGB;
		bitInfo.bmiHeader.biClrImportant = 0;
		bitInfo.bmiHeader.biClrUsed = 0;
		bitInfo.bmiHeader.biSizeImage = 0;
		bitInfo.bmiHeader.biXPelsPerMeter = 0;
		bitInfo.bmiHeader.biYPelsPerMeter = 0;

		if (m_matImg.cols == winSize.width  && m_matImg.rows == winSize.height)
		{
			// source and destination have same size
			// transfer memory block
			// NOTE: the padding border will be shown here. Anyway it will be max 3px width

			SetDIBitsToDevice(m_cImage.GetDC(),
				//destination rectangle
				0, 0, winSize.width, winSize.height,
				0, 0, 0, m_matImg.rows,
				m_matImg.data, &bitInfo, DIB_RGB_COLORS);
		}
		else
		{
			// destination rectangle
			int destx = 0, desty = 0;
			int destw = winSize.width;
			int desth = winSize.height;

			// rectangle defined on source bitmap
			// using imgWidth instead of mat_temp.cols will ignore the padding border
			int imgx = 0, imgy = 0;
			int imgWidth = m_matImg.cols - border;
			int imgHeight = m_matImg.rows;

			StretchDIBits(m_cImage.GetDC(),
				destx, desty, destw, desth,
				imgx, imgy, imgWidth, imgHeight,
				m_matImg.data, &bitInfo, DIB_RGB_COLORS, SRCCOPY);
		}


		HDC dc = ::GetDC(m_picImg1.m_hWnd);
		m_cImage.BitBlt(dc, 0, 0);

		::ReleaseDC(m_picImg1.m_hWnd, dc);
		m_cImage.ReleaseDC();
		m_cImage.Destroy();
	}
}


void Copencvmfc2Dlg::OnBnClickedBtnOpen()
{
	char szFilter[] = "Image Files (*.bmp;*.jpg;*.png)|*.bmp;*.jpg;*.png|All Files (*.*)|*.*||";

	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (dlg.DoModal() == IDOK)
	{
		CString strFile = dlg.GetPathName();
		char szFile[1000];
		strcpy(szFile, strFile);

		// OpenCV의 IplImage 사용
		if (m_pInImage != NULL && m_pInImage->imageData)
			cvReleaseImage(&m_pInImage);

		m_pInImage = cvLoadImage(strFile, -1);	// IplImage*로 로드
		m_matImg = cvarrToMat(m_pInImage);		// Mat로 변환

												//ShowCvvImage(); // 직접 표시 함수 호출
		ShowCImage(); // 직접 표시 함수 호출

					  //m_matImg = imread(szFile, -1);	// Mat로 로드
					  //m_InImage = IplImage(m_matImg);	// IplImage로 변환
					  //m_pInImage = &m_InImage;

					  //this->Invalidate(FALSE); // OnPaint로 표시
	}
}

void Copencvmfc2Dlg::OnBnClickedBtnCapture()
{
	m_pCapture = new VideoCapture(0);
	RECT r;
	m_picImg1.GetClientRect(&r);
	cv::Size winSize(r.right, r.bottom);
	m_pCapture->set(CV_CAP_PROP_FRAME_WIDTH, winSize.width);
	m_pCapture->set(CV_CAP_PROP_FRAME_HEIGHT, winSize.height);

	SetTimer(1000, 30, NULL);
}


void Copencvmfc2Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	KillTimer(1000);

	m_pCapture->read(m_matImg);
	m_InImage = IplImage(m_matImg);	// IplImage로 변환
	m_pInImage = &m_InImage;
	ShowCImage();

	SetTimer(1000, 30, NULL);

	CDialogEx::OnTimer(nIDEvent);
}

void Copencvmfc2Dlg::FaceAndEyeDetect(Mat img)
{
	Mat gray;
	cvtColor(img, gray, CV_BGR2GRAY);

	vector<Rect> face_pos;	// 얼굴 검출 결과를 표시하는 직사각형
	face.detectMultiScale(gray, face_pos, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(10, 10));

	for (int i = 0; i<face_pos.size(); i++)
		rectangle(img, face_pos[i], Scalar(255, 0, 0), 2);

	//namedWindow("얼굴 검출"); imshow("얼굴 검출", img);
}

void Copencvmfc2Dlg::OnBnClickedBtnDetect()
{
	//m_matImg = imread("D:/sglee/images/test_images/model3.jpg");
	m_matImg = imread("D:/sglee/images/test_images/girlsgeneration.jpg");
	//m_matImg = imread("D:/sglee/images/test_images/unitedstate.jpg");
	assert(m_matImg.data);

	bool b1 = face.load(face_cascade);
	bool b2 = eye.load(eye_cascade);
	assert(b1 && b2);

	// OpenCV의 IplImage 사용
	if (m_pInImage != NULL && m_pInImage->imageData)
		cvReleaseImage(&m_pInImage);

	m_pInImage = &IplImage(m_matImg);	// IplImage*로 변환
	ShowCImage(); // 직접 표시 함수 호출

	FaceAndEyeDetect(m_matImg);
	ShowCImage(); // 직접 표시 함수 호출
}


void Copencvmfc2Dlg::OnBnClickedBtnObject()
{
	Mat img1 = imread("D:/sglee/images/test_images/model3.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat img2 = imread("D:/sglee/images/test_images/scene.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	assert(img1.data && img2.data);


	//-- Step 1: Detect the keypoints using SURF Detector
	int minHessian = 400;

	Ptr<SURF> detector = SURF::create(minHessian);

	std::vector<KeyPoint> keypoints_1, keypoints_2;

	detector->detect(img1, keypoints_1);
	detector->detect(img2, keypoints_2);

	//-- Draw keypoints
	Mat img_keypoints_1; Mat img_keypoints_2;

	drawKeypoints(img1, keypoints_1, img_keypoints_1, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
	drawKeypoints(img2, keypoints_2, img_keypoints_2, Scalar::all(-1), DrawMatchesFlags::DEFAULT);

	//-- Show detected (drawn) keypoints
	imshow("Keypoints 1", img_keypoints_1);
	imshow("Keypoints 2", img_keypoints_2);


	//SiftFeatureDetector detector;// (0.3);
	//std::vector<KeyPoint> keypoint1, keypoint2;

	//detector.detect(img1, keypoint1);
	//detector.detect(img2, keypoint2);

	//Mat disp;
	//drawKeypoints(img1, keypoint1, disp, RED, DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
	//namedWindow("키포인트"); imshow("키포인트", disp);

	//SiftDescriptorExtractor extractor;
	//Mat descriptor1, descriptor2;
	//extractor.compute(img1, keypoint1, descriptor1);
	//extractor.compute(img2, keypoint2, descriptor2);
}
