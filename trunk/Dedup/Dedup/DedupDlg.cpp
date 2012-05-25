
// DedupDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "Dedup.h"
#include "DedupDlg.h"
#include "afxdialogex.h"

#include <iostream>
#include <sstream>
#include <math.h>
#include "ChunkContainer.h"
#include "ChunkManager.h"
#include "ManifestStore.h"
#include "MyString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CDedupDlg ��ȭ ����




CDedupDlg::CDedupDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDedupDlg::IDD, pParent)
	, mv_ChunkSize(_T("4"))
	, mv_SegSize(_T("8"))
	, mv_SmpRate(_T("50"))
	, mv_SIEntrySize(_T("10"))
	, mv_SIEntryNum(_T("500"))
	, mv_Path(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
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


// CDedupDlg �޽��� ó����

BOOL CDedupDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	//ManifestStore ������ �����ϴ��� Ȯ��. ������ ����
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

	//ChunkContainer ������ �����ϴ��� Ȯ��. ������ ����
    bRet = pFind.FindFile(L"ChunkContainer\\" + strFile);
	if (!bRet) {
		bRet = CreateDirectory(L"ChunkContainer\\", NULL);
		if (!bRet) {
			cerr << "ERROR: fail to create a folder 'ChunkContainer'" << endl;
			exit(1);
		}
	}

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CDedupDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
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
	chunkSize = _ttoi(mv_ChunkSize) * 1024;
	segSize = _ttoi(mv_SegSize);
	smpRate = _ttoi(mv_SmpRate);
	siEntrySize = _ttoi(mv_SIEntrySize);
	siEntryNum = _ttoi(mv_SIEntryNum);

	if (mv_Path == _T("")) {
		MessageBox(_T("���� ��θ� �������ּ���."), MB_OK);
		return;
	}

	CFileFind pFind;
	BOOL res = pFind.FindFile(mv_Path);
	if (!res) {
		MessageBox(_T("�������� �ʴ� �����Դϴ�."), MB_OK);
		return;
	}

	StartPerDirectory(mv_Path);

	MessageBox(_T("�Ϸ�"), _T("�Ϸ�"), MB_OK);
}


void CDedupDlg::StartPerDirectory(CString dirPath)
{
	CFileFind pFind;
	BOOL bWorking = pFind.FindFile(dirPath + "\\*.*");

	while (bWorking) 
	{
		bWorking = pFind.FindNextFileW();

		if (pFind.IsDirectory()) {
			if (pFind.GetFileName() == _T(".") || pFind.GetFileName() == _T(".."))
				continue;

			StartPerDirectory(pFind.GetFilePath());
		} else {
			StartPerFile(pFind.GetFilePath(), pFind.GetFileName());
		}
	}
}


void CDedupDlg::StartPerFile(CString filePath_, CString fileName_)
{
	string filePath = MyString::CString2string(filePath_);

	ChunkManager chkMng(chunkSize, segSize);
	vector<char*> chunkList = chkMng.getChunkList(filePath.c_str(), ADD_INFO);
	vector<char*>::iterator iter = chunkList.end();
	iter--;
	int LL = atoi(*iter);
	chunkList.pop_back();

	vector<char*> hashList = chkMng.getHashedList(&chunkList);
	
	ChunkContainer container;
	string fileName = MyString::CString2string(fileName_);
	container.openContainer(fileName);

	ManifestStore maniStore;

	int numOfSeg = ceil((double)chunkList.size() / (double)segSize);

	for (int segNum = 0 ; segNum < numOfSeg ; segNum++) {
		vector<char*> segment = chkMng.getSegment(&chunkList, segNum);
		vector<char*> hooks = chkMng.getSegment(&hashList, segNum);

		
		string maniPrefix = filePath;
		maniPrefix = MyString::replaceAll(maniPrefix, "\\", ";");
		maniPrefix = MyString::replaceAll(maniPrefix, ":", ";");
		
		//string maniPrefix = MyString::CString2string(fileName_);
		Manifest manifest(maniPrefix + "__m" + MyString::int2string(segNum) + ".mani");

		for (int j = 0 ; j < segment.size() ; j++) {
			fpos_t pos = container.getCurPos();

			size_t writeSize;
			if (segNum == numOfSeg-1 && j == segment.size() - 1) 
				writeSize = container.writeChunk(segment[j], LL);
			else
				writeSize = container.writeChunk(segment[j], chunkSize);

			ManiNode node((string(hooks[j])), fileName, pos, writeSize);
			manifest.addManiNode(node);
		}

		maniStore.createManifest(manifest);
	}

	container.closeContainer();
}
