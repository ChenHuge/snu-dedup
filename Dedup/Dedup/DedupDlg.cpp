
// DedupDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "Dedup.h"
#include "DedupDlg.h"
#include "afxdialogex.h"

#include <iostream>
#include "ChunkContainer.h"
#include "ManifestStore.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDedupDlg 대화 상자




CDedupDlg::CDedupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDedupDlg::IDD, pParent)
	, mv_ChunkSize(_T("4"))
	, mv_SegSize(_T("8"))
	, mv_SmpRate(_T("50"))
	, mv_SIEntrySize(_T("10"))
	, mv_SIEntryNum(_T("100"))
	, mv_Path(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	/*mv_ChunkSize = _T("4");
	mv_SegSize = _T("8");
	mv_SIEntrySize = _T("10");
	mv_SIEntryNum = _T("100");*/
}

void CDedupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CHKSIZE, mv_ChunkSize);
	DDX_Text(pDX, IDC_EDIT_SEGSIZE, mv_SegSize);
	DDX_Text(pDX, IDC_EDIT_SPLRATE, mv_SmpRate);
	DDX_Text(pDX, IDC_EDIT_ENTRYSIZE, mv_SIEntrySize);
	DDX_Text(pDX, IDC_EDIT_ENTRYNUM, mv_SIEntryNum);
	DDX_Text(pDX, IDC_EDIT_DIR, mv_Path);
}

BEGIN_MESSAGE_MAP(CDedupDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_DIR, &CDedupDlg::OnBnClickedBtnDir)
	ON_BN_CLICKED(IDC_BTN_START, &CDedupDlg::OnBnClickedBtnStart)
END_MESSAGE_MAP()


// CDedupDlg 메시지 처리기

BOOL CDedupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	//ManifestStore 폴더가 존재하는지 확인. 없으면 생성
	CFileFind pFind;
	CString strFile = L"*.*";
    BOOL bRet = pFind.FindFile(L"ManifestStore\\" + strFile);
	if (!bRet) {
		bRet = CreateDirectory(L"ManifestStore\\", NULL);
		if (!bRet) {
			cerr << "ERROR: fail to create a folder 'ManifestStore'" << endl;
			exit(1);
		}
	}

	

	/*ManifestStore ms;
	Manifest mf("a.mp3__m4.txt");
	mf.addManiNode(ManiNode("ccc", "aa", 1234, 23525));
	mf.addManiNode(ManiNode("bbbbbbbb", "aa", 1234, 23525));
	ms.createManifest(mf);

	vector<string> cn;
	cn.push_back("a.mp3__m4.txt");
	list<Manifest> champ = ms.loadChampions(cn);*/



	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDedupDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CDedupDlg::OnPaint()
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
HCURSOR CDedupDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDedupDlg::OnBnClickedBtnDir()
{
	CString strFolder = _T("");
	ITEMIDLIST* pidlBrowse;
    TCHAR initPath[MAX_PATH];
	TCHAR str[MAX_PATH];
	SHGetSpecialFolderPath(NULL, initPath, CSIDL_DESKTOP, FALSE);
    BROWSEINFO brInfo;

	memset(&brInfo, 0x00, sizeof(BROWSEINFO));
    wcscpy(str, _T("Select a directory to be deduplicated"));
	
	brInfo.hwndOwner = NULL;
    brInfo.pidlRoot = NULL;
    brInfo.pszDisplayName = initPath;
    brInfo.lpszTitle  = str;
    brInfo.ulFlags = BIF_RETURNONLYFSDIRS;
    brInfo.lpfn = NULL;
    pidlBrowse = ::SHBrowseForFolder(&brInfo);

    if(pidlBrowse != NULL) {
        SHGetPathFromIDList(pidlBrowse, initPath);        
        strFolder = initPath;
		mv_Path = strFolder;
		UpdateData(FALSE);
    }
}


void CDedupDlg::OnBnClickedBtnStart()
{
	UpdateData();
	chunkSize = _ttoi(mv_ChunkSize);
	segSize = _ttoi(mv_SegSize);
	smpRate = _ttoi(mv_SmpRate);
	siEntrySize = _ttoi(mv_SIEntrySize);
	siEntryNum = _ttoi(mv_SIEntryNum);


}
