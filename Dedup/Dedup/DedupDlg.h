
// DedupDlg.h : 헤더 파일
//

#pragma once
#include "ChunkContainer.h"
#include "SparseIndex.h"
#include "afxcmn.h"

// CDedupDlg 대화 상자
class CDedupDlg : public CDialogEx
{
// 생성입니다.
public:
	CDedupDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DEDUP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

public:
	int chunkSize;
	int segSize;
	int siEntrySize;
	int siEntryNum;
	int numZeroBit;
	int maxNumChamp;
//	int smpRate;
	ChunkContainer container;
	SparseIndex sparseIndex;

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
	afx_msg void OnDeltaposSpinSplrate2(NMHDR *pNMHDR, LRESULT *pResult);
	CString mv_ChunkSize;
	CString mv_SegSize;
	CString mv_SmpRate;
	CString mv_SIEntrySize;
	CString mv_SIEntryNum;
	CString mv_Path;
	afx_msg void OnBnClickedBtnDir();
	afx_msg void OnBnClickedBtnStart();
	void StartPerDirectory(CString dirPath);
	void StartPerFile(CString filePath_, CString fileName_);
	afx_msg void OnStnClickedStaticSplrate();
	CString mv_NumZeroBit;
	CString mv_MaxNumChamp;
	CProgressCtrl mc_Progress;
	virtual BOOL DestroyWindow();
};
