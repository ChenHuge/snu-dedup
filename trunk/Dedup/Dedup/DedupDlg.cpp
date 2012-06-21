
// DedupDlg.cpp : 구현 파일
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

	// TODO: 여기에 추가 초기화 작업을 추가합니다

	//ManifestStore 폴더가 존재하는지 확인. 없으면 생성
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

	//ChunkContainer 폴더가 존재하는지 확인. 없으면 생성
    bRet = pFind.FindFile(L"ChunkContainer\\" + strFile);
	if (!bRet) {
		bRet = CreateDirectory(L"ChunkContainer\\", NULL);
		if (!bRet) {
			MessageBox(_T("ERROR: fail to create ChunkContainer directory!"), _T("ERROR - OnInitDialog"), MB_OK);
			exit(1);
		}
	}

	//Temp 폴더가 존재하는지 확인. 없으면 생성
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

	//리스트컨트롤 초기화
	//mc_List.ShowScrollBar(SB_HORZ);
	mc_List.InsertColumn(0, _T("Path"), LVCFMT_LEFT, 500);
	mc_List.InsertColumn(1, _T("# of Manifest"), LVCFMT_LEFT, 100);

	//리스트컨트롤에 아이템 추가
	RefreshList();

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
			MessageBox(_T("인자를 모두 입력해주세요."), _T("미입력된 인자 발견"), MB_OK);
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
		MessageBox(_T("폴더 경로를 지정해주세요."), MB_OK);
		return;
	}

	CFileFind pFind;
	BOOL res = pFind.FindFile(mv_Path);
	if (!res) {
		MessageBox(_T("존재하지 않는 폴더입니다."), MB_OK);
		return;
	}

	//Result 관련 변수들 초기화
	numInputFiles = numOfFilesInDirectory(mv_Path);
	numInputChunks = 0;
	numStoredChunks = 0;
	totalInputSize = 0;
	totalStoredSize = 0;

	//ProgressBar 초기화
	mc_Progress.SetRange(0, numInputFiles);
	mc_Progress.SetPos(0);

	dwStartTime = timeGetTime();

	StartPerDirectory(mv_Path);

	dwEndTime = timeGetTime();
	dedupTime = dwEndTime - dwStartTime;

	//ChunkContainer 닫기
	container.closeContainer();

	//Dedup factor 계산
	if (totalInputSize < totalStoredSize) {
		MessageBox(_T("입력 파일들의 크기보다 저장된 파일들의 크기가 더 큽니다.\n무엇인가 잘못되었습니다."), _T("Dedup factor 계산 오류"), MB_OK);
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

	MessageBox(_T("완료"), _T("완료"), MB_OK);
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

			// Deduplication 체크!!!!!!!!!
			hash_map<string, ManiNode>::iterator iter = champTable.find(hashs[j]);
			if (iter == champTable.end()) // 중복된 chunk가 없는 경우
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
			else { //중복된 chunk가 있는 경우
				ManiNode node((string(hashs[j])), iter->second.getContainer(), 
								iter->second.getPosition(), iter->second.getLength());
				manifest.addManiNode(node);
			}

			//SparseIndex에 새 manifest 정보 넣기
			bool isSampled = false;
			for (int z = 0 ; z < hooks.size() ; z++) {
				if (hooks[z] == hashs[j]) {
					isSampled = true;
					break;
				}
			}
			if (isSampled) { //샘플링되었던 chunk 라면
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

	//ListCtrl에 추가
	addToList(filePath_, numMani);

	// ProgressBar 1칸 진행
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
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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

	//더블클릭한 파일의 이름과 manifest 갯수를 가져온다.
	CString fileName = (mc_List.GetItemText(idx, 0));
	int numMani = MyString::CString2int(mc_List.GetItemText(idx, 1));

	CString tempName = GatherChunks(fileName, numMani);  //chunks를 모은다.

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
	BOOL res = pFind.FindFile(_T("Temp\\") + tempName);  //이미 해당 TempFile이 존재할 경우
	if (res)
		return tempName;
	pFind.Close();

	//본 파일을 저장하기 위한 임시공간(TempFile)을 만든다.
	//TempFile이름은 해당 파일의 이름과 같다. (\를 ;로 바꾼것)
	FILE* fp = fopen(("Temp\\" + fileName).c_str(), "wb");
	if (fp == NULL) {
		MessageBox(filePath_ + _T(" 파일을 위한 임시 공간을 할당하는데 실패했습니다."), _T("파일 열기 오류"), MB_OK);
		return _T("");
	}

	FILE* read_fp = NULL;

	//각 Manifest를 열어 chunks를 모은다.
	for (int i = 0 ; i < numMani ; i++) {
		string maniName = fileName + "__m" + MyString::int2string(i) + ".mani";
		ifstream fin("ManifestStore\\" + maniName, ios::in);
		if (!fin) {
			MessageBox(_T("Manifest를 여는데 실패했습니다.\n\n") + MyString::string2CString(maniName), _T("파일 열기 오류"), MB_OK);
			fclose(fp);
			return _T("");
		}

		string hash, cont, old_cont = "";
		fpos_t pos;
		size_t length;
		
		while (fin >> hash >> cont >> pos >> length) 
		{
			//이미 열린 chunk container가 아닌 경우 열어야 한다.
			if (read_fp == NULL || old_cont != cont) {  
				if (read_fp != NULL)
					fclose(read_fp);

				read_fp = fopen(("ChunkContainer\\" + cont).c_str(), "rb");
				if (read_fp == NULL) {
					MessageBox(_T("ChunkContainer를 여는데 실패했습니다.\n\n") + MyString::string2CString(cont), _T("파일 열기 오류"), MB_OK);
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
		
	// 폴더가 존재 하는 지 확인 검사	
	bRval = find.FindFile( lpDirPath );
	
	if( bRval == FALSE )		
	{		
		return bRval;		
	}
	
	while( bRval )		
	{		
		bRval = find.FindNextFile();

		// . or .. 인 경우 무시 한다.		
		if( find.IsDots() == TRUE )			
		{			
			continue;			
		}
		
		// Directory 일 경우		
		if( find.IsDirectory() )			
		{			
			szNextDirPath.Format(_T("%s\\*.*") , find.GetFilePath() );

			// Recursion function 호 출			
			DeleteDirectory( szNextDirPath );			
		}			
		// file일 경우 		
		else			
		{			
			// 파일 삭제			
			::DeleteFile( find.GetFilePath() );			
		}		
	}	
	szRoot = find.GetRoot();	
	find.Close();
	bRval = RemoveDirectory( szRoot );         	
	return bRval;
}
