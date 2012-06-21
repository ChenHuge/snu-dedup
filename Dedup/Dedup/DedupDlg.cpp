
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
	DDX_Control(pDX, IDC_LIST1, mc_List);
}

BEGIN_MESSAGE_MAP(CDedupDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_DIR, &CDedupDlg::OnBnClickedBtnDir)
	ON_BN_CLICKED(IDC_BTN_START, &CDedupDlg::OnBnClickedBtnStart)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDedupDlg::OnNMDblclkList1)
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

	//Temp ������ �����ϴ��� Ȯ��. ������ ����
    bRet = pFind.FindFile(L"Temp\\" + strFile);
	if (!bRet) {
		bRet = CreateDirectory(L"Temp\\", NULL);
		if (!bRet) {
			MessageBox(_T("ERROR: fail to create Temp directory!"), _T("ERROR - OnInitDialog"), MB_OK);
			exit(1);
		}
	}

	//SparseIndex load
	sparseIndex.load();

	//����Ʈ��Ʈ�� �ʱ�ȭ
	//mc_List.ShowScrollBar(SB_HORZ);
	mc_List.InsertColumn(0, _T("Path"), LVCFMT_LEFT, 500);
	mc_List.InsertColumn(1, _T("# of Manifest"), LVCFMT_LEFT, 100);

	//����Ʈ��Ʈ�ѿ� ������ �߰�
	RefreshList();

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
	numInputFiles = numOfFilesInDirectory(mv_Path);
	numInputChunks = 0;
	numStoredChunks = 0;
	totalInputSize = 0;
	totalStoredSize = 0;

	//ProgressBar �ʱ�ȭ
	mc_Progress.SetRange(0, numInputFiles);
	mc_Progress.SetPos(0);

	dwStartTime = timeGetTime();

	StartPerDirectory(mv_Path);

	dwEndTime = timeGetTime();
	dedupTime = dwEndTime - dwStartTime;

	//ChunkContainer �ݱ�
	container.closeContainer();

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

long CDedupDlg::numOfFilesInDirectory(CString dirPath)
{
	long cnt = 0;
	CFileFind pFind;
	BOOL bWorking = pFind.FindFile(dirPath + "\\*.*");
	
	while (bWorking) 
	{
		bWorking = pFind.FindNextFileW();

		if (pFind.IsDirectory()) {
			if (pFind.GetFileName() == _T(".") || pFind.GetFileName() == _T(".."))
				continue;

			cnt += numOfFilesInDirectory(pFind.GetFilePath());
		} else {
			cnt++;
		}
	}

	return cnt;
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
	int numMani = 0;

	ChunkManager chkMng(chunkSize, segSize);
	vector<char*>* p_chunkList = chkMng.getChunkList(filePath.c_str(), ADD_INFO);

	if (p_chunkList->size() == 0)
		return;

	int end_idx = p_chunkList->size() - 1;
	int LL = MyString::string2int(p_chunkList->at(end_idx));
	p_chunkList->pop_back();

	vector<string> hashList = chkMng.getHashedList(p_chunkList);
	
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

	int numOfSeg = ceil((double)p_chunkList->size() / (double)segSize);

	for (int segNum = 0 ; segNum < numOfSeg ; segNum++) {
		vector<char*> segment = chkMng.getSegment(p_chunkList, segNum);
		vector<string> hashs = chkMng.getSegmentForHash(&hashList, segNum);
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
			numMani++;
		}
		catch (exception &ex) {
			MessageBox((CString)ex.what(), _T("Catched at StartPerFile"), MB_OK);
			isForceTerminated = true;
			AfxGetMainWnd()->PostMessage(WM_QUIT);
		}
	}

	//ListCtrl�� �߰�
	addToList(filePath_, numMani);

	// ProgressBar 1ĭ ����
	int nPos = mc_Progress.GetPos();
	mc_Progress.SetPos(nPos + 1);
}


void CDedupDlg::RefreshList(void)
{
	CFileFind pFind;
	BOOL bWorking = pFind.FindFile(_T("ManifestStore\\*.*"));
	string filename = "";
	int nCount = 0;

	mc_List.DeleteAllItems();
	
	while (bWorking) 
	{
		bWorking = pFind.FindNextFileW();

		if (pFind.IsDirectory()) {
			if (pFind.GetFileName() == _T(".") || pFind.GetFileName() == _T(".."))
				continue;
		} else {
			string name = MyString::CString2string(pFind.GetFileName());

			string::size_type pos = name.find_last_of("__");
			name = name.substr(0, pos-1);

			pos = name.find(";", 0);
			if (pos == 1)
				name.replace(name.begin() + pos, name.begin() + pos + 1, ":");

			name = MyString::replaceAll(name, ";", "\\");

			if (filename == name) {
				int num = MyString::CString2int(mc_List.GetItemText(nCount-1, 1));
				mc_List.SetItemText(nCount-1, 1, MyString::int2CString(num+1));
				continue;
			}
			else
				filename = name;

			mc_List.InsertItem(nCount, MyString::string2CString(name));
			mc_List.SetItemText(nCount, 1, _T("1"));
			nCount++;
		}
	}
}


BOOL CDedupDlg::DestroyWindow()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	if (!isForceTerminated)
		sparseIndex.save();

	container.closeContainer();

	DeleteDirectory(_T("Temp"));

	return CDialogEx::DestroyWindow();
}


void CDedupDlg::addToList(CString name, int numMani)
{
	int nCount = mc_List.GetItemCount();
	mc_List.InsertItem(nCount, name);
	mc_List.SetItemText(nCount, 1, MyString::int2CString(numMani));
}


void CDedupDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int idx = pNMListView->iItem;

	if (idx < 0)
		return;

	//����Ŭ���� ������ �̸��� manifest ������ �����´�.
	CString fileName = (mc_List.GetItemText(idx, 0));
	int numMani = MyString::CString2int(mc_List.GetItemText(idx, 1));

	CString tempName = GatherChunks(fileName, numMani);  //chunks�� ������.

	ShellExecute(NULL, _T("open"), _T("Temp\\") + tempName, NULL, NULL, SW_SHOWNORMAL);

	*pResult = 0;
}


CString CDedupDlg::GatherChunks(CString filePath_, int numMani)
{
	string fileName = MyString::CString2string(filePath_);
	fileName = MyString::replaceAll(fileName, "\\", ";");
	fileName = MyString::replaceAll(fileName, ":", ";");

	CFileFind pFind;
	CString tempName = MyString::string2CString(fileName);
	BOOL res = pFind.FindFile(_T("Temp\\") + tempName);  //�̹� �ش� TempFile�� ������ ���
	if (res)
		return tempName;
	pFind.Close();

	//�� ������ �����ϱ� ���� �ӽð���(TempFile)�� �����.
	//TempFile�̸��� �ش� ������ �̸��� ����. (\�� ;�� �ٲ۰�)
	FILE* fp = fopen(("Temp\\" + fileName).c_str(), "wb");
	if (fp == NULL) {
		MessageBox(filePath_ + _T(" ������ ���� �ӽ� ������ �Ҵ��ϴµ� �����߽��ϴ�."), _T("���� ���� ����"), MB_OK);
		return _T("");
	}

	FILE* read_fp = NULL;

	//�� Manifest�� ���� chunks�� ������.
	for (int i = 0 ; i < numMani ; i++) {
		string maniName = fileName + "__m" + MyString::int2string(i) + ".mani";
		ifstream fin("ManifestStore\\" + maniName, ios::in);
		if (!fin) {
			MessageBox(_T("Manifest�� ���µ� �����߽��ϴ�.\n\n") + MyString::string2CString(maniName), _T("���� ���� ����"), MB_OK);
			fclose(fp);
			return _T("");
		}

		string hash, cont, old_cont = "";
		fpos_t pos;
		size_t length;
		
		while (fin >> hash >> cont >> pos >> length) 
		{
			//�̹� ���� chunk container�� �ƴ� ��� ����� �Ѵ�.
			if (read_fp == NULL || old_cont != cont) {  
				if (read_fp != NULL)
					fclose(read_fp);

				read_fp = fopen(("ChunkContainer\\" + cont).c_str(), "rb");
				if (read_fp == NULL) {
					MessageBox(_T("ChunkContainer�� ���µ� �����߽��ϴ�.\n\n") + MyString::string2CString(cont), _T("���� ���� ����"), MB_OK);
					fclose(fp);
					fin.close();
					return _T("");
				}

				old_cont = cont;
			}

			char* chunk = new char[length + 1];
			fseek(read_fp, pos, SEEK_SET);
			size_t read_len = fread(chunk, sizeof(char), length, read_fp);
			fwrite(chunk, sizeof(char), read_len, fp);
			delete[] chunk;
		}

		fin.close();
	}
	
	fclose(read_fp);
	fclose(fp);

	return tempName;
}


BOOL CDedupDlg::DeleteDirectory(LPCTSTR lpDirPath)
{
	if( lpDirPath == NULL )		
	{		
		return FALSE;		
	}		
	
	BOOL bRval = FALSE;	
	int nRval = 0;	
	CString szNextDirPath   = _T("");	
	CString szRoot = _T("");	
	CFileFind find;	
		
	// ������ ���� �ϴ� �� Ȯ�� �˻�	
	bRval = find.FindFile( lpDirPath );
	
	if( bRval == FALSE )		
	{		
		return bRval;		
	}
	
	while( bRval )		
	{		
		bRval = find.FindNextFile();

		// . or .. �� ��� ���� �Ѵ�.		
		if( find.IsDots() == TRUE )			
		{			
			continue;			
		}
		
		// Directory �� ���		
		if( find.IsDirectory() )			
		{			
			szNextDirPath.Format(_T("%s\\*.*") , find.GetFilePath() );

			// Recursion function ȣ ��			
			DeleteDirectory( szNextDirPath );			
		}			
		// file�� ��� 		
		else			
		{			
			// ���� ����			
			::DeleteFile( find.GetFilePath() );			
		}		
	}	
	szRoot = find.GetRoot();	
	find.Close();
	bRval = RemoveDirectory( szRoot );         	
	return bRval;
}
