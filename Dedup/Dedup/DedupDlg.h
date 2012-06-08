
// DedupDlg.h : ��� ����
//

#pragma once
#include "ChunkContainer.h"
#include "SparseIndex.h"
#include "afxcmn.h"

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
//	int smpRate;
	ChunkContainer container;
	SparseIndex sparseIndex;

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
