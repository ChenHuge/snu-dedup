
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
#include <exception>

#include "Mmsystem.h"
#pragma comment(lib, "winmm.lib")

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
	, mv_SIEntrySize(_T("1000"))
	, mv_SIEntryNum(_T("1000"))
	, mv_Path(_T(""))
	, mv_NumZeroBit(_T("1"))
	, mv_MaxNumChamp(_T("8"))
	, mv_RadioHash(FALSE)
	, mv_NumInputFiles(_T(""))
	, mv_NumInputChunks(_T(""))
	, mv_NumStoredChunks(_T(""))
	, mv_TotalInputSize(_T(""))
	, mv_TotalStoredSize(_T(""))
	, mv_DedupTime(_T(""))
	, mv_DedupFactor(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	isForceTerminated = false;
}

void CDedupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CHKSIZE, mv_ChunkSize);
	DDX_Text(pDX, IDC_EDIT_SEGSIZE, mv_SegSize);
	DDX_Text(pDX, IDC_EDIT_ENTRYSIZE, mv_SIEntrySize);
	DDX_Text(pDX, IDC_EDIT_ENTRYNUM, mv_SIEntryNum);
	DDX_Text(pDX, IDC_EDIT_DIR, mv_Path);
	DDX_Text(pDX, IDC_EDIT_NUMZEROBIT, mv_NumZeroBit);
	DDX_Text(pDX, IDC_EDIT_MAXNUMCHAMP, mv_MaxNumChamp);
	DDX_Control(pDX, IDC_PROGRESS1, mc_Progress);
	DDX_Radio(pDX, IDC_RADIO_HASH, mv_RadioHash);
	DDX_Text(pDX, IDC_EDIT_RES_NUMINPUTFILES, mv_NumInputFiles);
	DDX_Text(pDX, IDC_EDIT_RES_NUMINPUTCHUNK, mv_NumInputChunks);
	DDX_Text(pDX, IDC_EDIT_RES_NUMSTOREDCHUNK, mv_NumStoredChunks);
	DDX_Text(pDX, IDC_EDIT_RES_TOTINPUTSIZE, mv_TotalInputSize);
	DDX_Text(pDX, IDC_EDIT_RES_TOTSTOREDSIZE, mv_TotalStoredSize);
	DDX_Text(pDX, IDC_EDIT_RES_DEDUPTIME, mv_DedupTime);
	DDX_Text(pDX, IDC_EDIT_RES_DEDUPFACTOR, mv_DedupFactor);
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

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�

	//ManifestStore ������ �����ϴ��� Ȯ��. ������ ����
	CFileFind pFind;
	CString strFile = L"*.*";
    BOOL bRet = pFind.FindFile(L"ManifestStore\\" + strFile);
	if (!bRet) {
		bRet = CreateDirectory(L"ManifestStore\\", NULL);
		if (!bRet) {
			MessageBox(_T("ERROR: fail to create ManifestStore directory!"), _T("ERROR - OnInitDialog"), MB_OK);
			exit(1);
		}
	}

	//ChunkContainer ������ �����ϴ��� Ȯ��. ������ ����
    bRet = pFind.FindFile(L"ChunkContainer\\" + strFile);
	if (!bRet) {
		bRet = CreateDirectory(L"ChunkContainer\\", NULL);
		if (!bRet) {
			MessageBox(_T("ERROR: fail to create ChunkContainer directory!"), _T("ERROR - OnInitDialog"), MB_OK);
			exit(1);
		}
	}

	//SparseIndex load
	sparseIndex.load();

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

///////////////////////////////////////////////////////////////////////////////

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

	if (mv_ChunkSize == _T("") || mv_SegSize == _T("") || mv_SIEntrySize == _T("") || mv_SIEntryNum == _T("")
		|| mv_NumZeroBit == _T("") || mv_MaxNumChamp == _T("")) {
			MessageBox(_T("���ڸ� ��� �Է����ּ���."), _T("���Էµ� ���� �߰�"), MB_OK);
			return;
	}

	chunkSize = _ttoi(mv_ChunkSize) * 1024;
	segSize = _ttoi(mv_SegSize);
	siEntrySize = _ttoi(mv_SIEntrySize);
	siEntryNum = _ttoi(mv_SIEntryNum);
	numZeroBit = _ttoi(mv_NumZeroBit);
	maxNumChamp = _ttoi(mv_MaxNumChamp);

	switch (mv_RadioHash) {
	case 0:
		compareOnlyHash = true; break;
	default:
		compareOnlyHash = false; break;
	}

	sparseIndex.setEntrySize(siEntrySize);
	sparseIndex.setEntryNum(siEntryNum);


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

	//Result ���� ������ �ʱ�ȭ
	numInputFiles = 0;
	numInputChunks = 0;
	numStoredChunks = 0;
	totalInputSize = 0;
	totalStoredSize = 0;

	dwStartTime = timeGetTime();

	StartPerDirectory(mv_Path);

	dwEndTime = timeGetTime();
	dedupTime = dwEndTime - dwStartTime;

	//Dedup factor ���
	if (totalInputSize < totalStoredSize) {
		MessageBox(_T("�Է� ���ϵ��� ũ�⺸�� ����� ���ϵ��� ũ�Ⱑ �� Ů�ϴ�.\n�����ΰ� �߸��Ǿ����ϴ�."), _T("Dedup factor ��� ����"), MB_OK);
		dedupFactor = 0.0;
	}
	else {
		dedupFactor = (double)(totalInputSize - totalStoredSize) / (double)totalInputSize * 100.0;
	}

	mv_NumInputFiles = MyString::long2CString(numInputFiles);
	mv_NumInputChunks = MyString::long2CString(numInputChunks);
	mv_NumStoredChunks = MyString::long2CString(numStoredChunks);
	mv_TotalInputSize = MyString::long2CString(totalInputSize) + _T("KB");
	mv_TotalStoredSize = MyString::long2CString(totalStoredSize) + _T("KB");
	mv_DedupTime = MyString::DWORD2CString(dedupTime) + _T("ms");
	mv_DedupFactor = MyString::double2CString(dedupFactor);

	UpdateData(FALSE);

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
			numInputFiles++;
			StartPerFile(pFind.GetFilePath(), pFind.GetFileName());
		}
	}
}


void CDedupDlg::StartPerFile(CString filePath_, CString fileName_)
{
	string filePath = MyString::CString2string(filePath_);

	ChunkManager chkMng(chunkSize, segSize);
	vector<string> chunkList = chkMng.getChunkList(filePath, ADD_INFO);

	if (chunkList.size() == 0)
		return;

	int end_idx = chunkList.size() - 1;
	int LL = MyString::string2int(chunkList[end_idx]);
	chunkList.pop_back();

	vector<string> hashList = chkMng.getHashedList(&chunkList);
	
	string fileName = MyString::CString2string(fileName_);

	try {
		container.openContainer();
	}
	catch (exception &ex) {
		MessageBox((CString)ex.what(), _T("Catched at StartPerFile"), MB_OK);
		isForceTerminated = true;
		AfxGetMainWnd()->PostMessage(WM_QUIT);
	}

	ManifestStore maniStore;

	int numOfSeg = ceil((double)chunkList.size() / (double)segSize);

	for (int segNum = 0 ; segNum < numOfSeg ; segNum++) {
		vector<string> segment = chkMng.getSegment(&chunkList, segNum);
		vector<string> hashs = chkMng.getSegment(&hashList, segNum);
		vector<string> hooks = chkMng.getSample(&hashs, numZeroBit);

		vector<string> champions = sparseIndex.chooseChampions(hooks, maxNumChamp);

		hash_map<string, ManiNode> champTable;
		try {
			champTable = maniStore.loadChampions(champions);
		}
		catch (exception &ex) {
			MessageBox((CString)ex.what(), _T("Catched at StartPerFile"), MB_OK);
			isForceTerminated = true;
			AfxGetMainWnd()->PostMessage(WM_QUIT);
		}
		
		string maniPrefix = filePath;
		maniPrefix = MyString::replaceAll(maniPrefix, "\\", ";");
		maniPrefix = MyString::replaceAll(maniPrefix, ":", ";");
		Manifest manifest(maniPrefix + "__m" + MyString::int2string(segNum) + ".mani");

		for (int j = 0 ; j < segment.size() ; j++) 
		{
			numInputChunks++;
			if (segNum == numOfSeg-1 && j == segment.size() - 1) 
				totalInputSize += (long)LL;
			else
				totalInputSize += (long)chunkSize;

			// Deduplication üũ!!!!!!!!!
			hash_map<string, ManiNode>::iterator iter = champTable.find(hashs[j]);
			if (iter == champTable.end()) // �ߺ��� chunk�� ���� ���
			{
				fpos_t pos;
				try {
					pos = container.getCurPos();
				}
				catch (exception &ex) {
					MessageBox((CString)ex.what(), _T("Catched at StartPerFile"), MB_OK);
					isForceTerminated = true;
					AfxGetMainWnd()->PostMessage(WM_QUIT);
				}

				numStoredChunks++;
				size_t writeSize;
				if (segNum == numOfSeg-1 && j == segment.size() - 1)
					writeSize = container.writeChunk(segment[j], LL);
				else
					writeSize = container.writeChunk(segment[j], chunkSize);
				totalStoredSize += (long)writeSize;

				ManiNode node((string(hashs[j])), container.getName(), pos, writeSize);
				manifest.addManiNode(node);
				
			}
			else { //�ߺ��� chunk�� �ִ� ���
				ManiNode node((string(hashs[j])), iter->second.getContainer(), 
								iter->second.getPosition(), iter->second.getLength());
				manifest.addManiNode(node);
			}

			//SparseIndex�� �� manifest ���� �ֱ�
			bool isSampled = false;
			for (int z = 0 ; z < hooks.size() ; z++) {
				if (hooks[z] == hashs[j]) {
					isSampled = true;
					break;
				}
			}
			if (isSampled) { //���ø��Ǿ��� chunk ���
				sparseIndex.addIndex(hashs[j], manifest.getName());
			}

		}

		try {
			maniStore.createManifest(manifest);
		}
		catch (exception &ex) {
			MessageBox((CString)ex.what(), _T("Catched at StartPerFile"), MB_OK);
			isForceTerminated = true;
			AfxGetMainWnd()->PostMessage(WM_QUIT);
		}
	}
}


BOOL CDedupDlg::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (!isForceTerminated)
		sparseIndex.save();

	container.closeContainer();

	return CDialogEx::DestroyWindow();
}
