
// AESDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "AES.h"
#include "AESDlg.h"
#include "afxdialogex.h"

#include <stdio.h>
#include <string.h>
#include <openssl/aes.h>
#include <openssl/md5.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


char md5Hash[32] = { 0, };
#define BLOCK_SIZE 16
#define FREAD_COUNT 4096
#define KEY_BIT 256
#define IV_SIZE 16
#define RW_SIZE 1
#define SUCC 0
#define FAIL -1

AES_KEY aes_ks3;
unsigned char iv[IV_SIZE];

int fs_encrypt_aes(char *in_file, char *out_file);
int fs_decrypt_aes(char *in_file, char *out_file);
int fs_passwd_aes(char *plain);

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


// CAESDlg 대화 상자



CAESDlg::CAESDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AES_DIALOG, pParent)
	, Cpasswd(_T(""))
	, CHpasswd(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAESDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, CHpasswd);
	DDX_Control(pDX, IDC_EDIT2, pre);
	DDX_Text(pDX, IDC_EDIT1, Cpasswd);
}

BEGIN_MESSAGE_MAP(CAESDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CAESDlg::OnPathClicked)
	ON_BN_CLICKED(IDC_BUTTON2, &CAESDlg::OnEncrypt)
	ON_BN_CLICKED(IDC_BUTTON3, &CAESDlg::OnDecrypt)
END_MESSAGE_MAP()


// CAESDlg 메시지 처리기

BOOL CAESDlg::OnInitDialog()
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

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CAESDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAESDlg::OnPaint()
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
HCURSOR CAESDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CAESDlg::OnPathClicked() {
	CString szFilter = _T("All Files(*.*)|*.*||");
	CString strPathName;
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if (IDOK == dlg.DoModal()) {
		strPathName = _T(dlg.GetPathName());
		CFile fp;
		CFileException e;
		if (!fp.Open(strPathName, CFile::modeRead, &e)) {
			e.ReportError();
			return;
		}
		else {
			in_file = strPathName;
			pre.SetWindowTextA(strPathName);
			fp.Close();
		}
	}
}




void CAESDlg::OnEncrypt() {
	UpdateData(TRUE);
	char *passwd = (char *)malloc(Cpasswd.GetLength());
	strcpy(passwd, (LPTSTR)(LPCTSTR)Cpasswd);
	fs_passwd_aes(passwd);

	char *infile = (char *)malloc(in_file.GetLength());
	strcpy(infile, (LPTSTR)(LPCTSTR)in_file);

	char *out_file = (char *)malloc(in_file.GetLength() + 4);
	strcpy(out_file, (LPTSTR)(LPCTSTR)in_file);
	strcat(out_file, ".aes");

	if (!(fs_encrypt_aes(infile, out_file)))
		AfxMessageBox("암호화에 성공하였습니다.");
	UpdateData(FALSE);
}


void CAESDlg::OnDecrypt() {
	UpdateData(TRUE);
	char *passwd = (char *)malloc(Cpasswd.GetLength());
	strcpy(passwd, (LPTSTR)(LPCTSTR)Cpasswd);
	fs_passwd_aes(passwd);

	char *infile = (char *)malloc(in_file.GetLength());
	strcpy(infile, (LPTSTR)(LPCTSTR)in_file);


	char *out_file = (char *)malloc(in_file.GetLength() - 4);
	strcpy(out_file, (LPTSTR)(LPCTSTR)in_file);
	for (int i = 0; i < in_file.GetLength(); i++) {
		if(out_file[i] == '.')
			if (out_file[i+1] == 'a')
				if (out_file[i+2] == 'e')
					if (out_file[i+3] == 's') {
						out_file[i] = '\0';
					}

	}

	if (!(fs_decrypt_aes(infile, out_file)))
		AfxMessageBox("복호화에 성공하였습니다.");
	UpdateData(FALSE);
}

int fs_encrypt_aes(char *in_file, char *out_file) {
	int i = 0;
	int len = 0;
	int padding_len = 0;
	int size;
	
	//암호화 할 파일 구조체 선언
	FILE *fp = fopen(in_file, "rb");
	if (fp == NULL) {
		CString err = "[ERROR] can not fopen in_file";
		AfxMessageBox(err);
		return FAIL;
	}

	//파일의 길이를 알아낸 뒤에 파일의 길이반큼의 buffer를 생성
	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char *buf = (char *)malloc(size + BLOCK_SIZE);

	//암호화 된 파일 구조체 선언
	FILE *wfp = fopen(out_file, "wb");
	if (wfp == NULL) {
		CString err = "[ERROR] can not fopen out_file";
		AfxMessageBox(err);
		return FAIL;
	}

	//iv 초기화
	memset(iv, 0, sizeof(iv));

	//암호화에 사용할 키를 md5방식으로 aes_ks3라는 aes_key 자료형에 256bit으로 저장
	AES_set_encrypt_key((const unsigned char *)md5Hash, KEY_BIT, &aes_ks3);

	//읽은 파일의 내용을 buf에 쓴뒤 암호화해서 암호화한 내용이 저장된 파일에 쓰기
	while (len = fread(buf, RW_SIZE, size, fp)) {
		if (size != len) {
			break;
		}
		AES_cbc_encrypt((const unsigned char*)buf, (unsigned char*)buf, len, &aes_ks3, iv, AES_ENCRYPT);
		fwrite(buf, RW_SIZE, len, wfp);

		fclose(wfp);
		fclose(fp);

		return SUCC;
	}
}

int fs_decrypt_aes(char *in_file, char *out_file) {
	{
		int len = 0;
		int total_size = 0;
		int save_len = 0;
		int w_len = 0;
		int size;

		//복호화 할 파일 구조체 선언
		FILE *fp = fopen(in_file, "rb");
		if (fp == NULL) {
			CString err = "[ERROR] can not fopen in_file";
			AfxMessageBox(err);
			return FAIL;
		}

		//파일의 길이를 알아낸 뒤에 파일의 길이반큼의 buffer를 생성
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		char *buf = (char *)malloc(size + BLOCK_SIZE);

		//복호화 된 파일 구조체 선언
		FILE *wfp = fopen(out_file, "wb");
		if (wfp == NULL) {
			CString err = "[ERROR] can not fopen out_file";
			AfxMessageBox(err);
			return FAIL;
		}

		//iv 초기화
		memset(iv, 0, sizeof(iv));

		//복호화에 사용할 키를 md5방식으로 aes_ks3라는 aes_key 자료형에 256bit으로 저장
		AES_set_decrypt_key((const unsigned char *)md5Hash, KEY_BIT, &aes_ks3);

		//파일의 길이를 알아낸 뒤에 파일의 길이반큼의 buffer를 생성
		fseek(fp, 0, SEEK_END);
		total_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		//읽은 파일의 내용을 buf에 쓴뒤 암호화해서 복호화한 내용이 저장된 파일에 쓰기
		while (len = fread(buf, RW_SIZE, size, fp)) {
			if (size == 0) {
				break;
			}
			save_len += len;
			w_len = len;

			AES_cbc_encrypt((const unsigned char*)buf, (unsigned char*)buf, len, &aes_ks3, iv, AES_DECRYPT);
			if (save_len == total_size) { // check last block
				buf[len] = EOF;
			}

			fwrite(buf, RW_SIZE, w_len, wfp);
		}

		fclose(wfp);
		fclose(fp);

		return SUCC;
	}
}

int fs_passwd_aes(char *plain) {
	unsigned char digest[MD5_DIGEST_LENGTH];

	MD5_CTX context;
	MD5_Init(&context);
	MD5_Update(&context, plain, strlen(plain));
	MD5_Final(digest, &context);

	for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
		sprintf(md5Hash + (i * 2), "%02x", digest[i]);
	}

	return SUCC;
}
