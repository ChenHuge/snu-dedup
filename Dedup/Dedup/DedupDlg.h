
// DedupDlg.h : ��� ����
//

#pragma once
#include "ChunkContainer.h"
#include "SparseIndex.h"
#include "afxcmn.h"
#include "Manifest.h";


// CDedupDlg ��ȭ ����
class CDedupDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CDedupDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DEDUP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.

public:
	int chunkSize;
	int segSize;
	int siEntrySize;
	int siEntryNum;
	int numZeroBit;
	int maxNumChamp;
	bool compareOnlyHash;
	bool isForceTerminated;
	ChunkContainer container;
	SparseIndex sparseIndex;

	// Result ���� ������
	long numInputFiles;
	long numInputChunks;
	long numStoredChunks;
	long totalInputSize;
	long totalStoredSize;
	DWORD dedupTime, dwStartTime, dwEndTime;
	double dedupFactor;

// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnDeltaposSpinSplrate2(NMHDR *pNMHDR, LRESULT *pResult);
	CString mv_ChunkSize;
	CString mv_SegSize;
	CString mv_SIEntrySize;
	CString mv_SIEntryNum;
	CString mv_Path;
	afx_msg void OnBnClickedBtnDir();
	afx_msg void OnBnClickedBtnStart();
	long numOfFilesInDirectory(CString dirPath);
	void StartPerDirectory(CString dirPath);
	void StartPerFile(CString filePath_, CString fileName_);
	afx_msg void OnStnClickedStaticSplrate();
	CString mv_NumZeroBit;
	CString mv_MaxNumChamp;
	CProgressCtrl mc_Progress;
	virtual BOOL DestroyWindow();
	BOOL mv_RadioHash;
	CSpinButtonCtrl mc_SpinChunkSize;
	CSpinButtonCtrl mc_SpinSegSize;
	CSpinButtonCtrl mc_SpinNumZeroBit;
	CSpinButtonCtrl mc_SpinMaxNumChamp;
	CSpinButtonCtrl mc_SpinSIEntrySize;
	CSpinButtonCtrl mc_SpinSIEntryNum;
	CString mv_NumInputFiles;
	CString mv_NumInputChunks;
	CString mv_NumStoredChunks;
	CString mv_TotalInputSize;
	CString mv_TotalStoredSize;
	CString mv_DedupTime;
	CString mv_DedupFactor;
	CListCtrl mc_List;
	void RefreshList(void);
	void addToList(CString name, int numMani);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	CString GatherChunks(CString filePath, int numMani);
	BOOL DeleteDirectory(LPCTSTR lpDirPath);
};
