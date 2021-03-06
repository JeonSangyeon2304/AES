
// AESDlg.h: 헤더 파일
//

#pragma once

#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>

// CAESDlg 대화 상자
class CAESDlg : public CDialogEx
{
// 생성입니다.
public:
	CAESDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AES_DIALOG };
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
	CString Cpasswd;
	CString CHpasswd;
	afx_msg void OnPathClicked();
	
	CString in_file;
	CString out_file;
	CEdit pre;
	afx_msg void OnEncrypt();
	afx_msg void OnDecrypt();
};
