﻿
// DialogPandisNIDS.h: 헤더 파일
//
#include "DialogDeviceSelect.h"
#include "DialogRuleSet.h"

#pragma once


// CDialogPandisNIDS 대화 상자
class CDialogPandisNIDS : public CDialog
{
// 생성입니다.
public:
	CDialogPandisNIDS(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PANDIS_PANDISNIDS };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

// 구현입니다.
public:
	// 네트워크 인터페이스 디바이스 지정
	CDialogDeviceSelect m_DlgDevSel;
	// 패킷 탐지 규칙 지정
	CDialogRuleSet m_DlgRuleSet;
	// 메인화면 출력 컨트롤
	CEdit m_ctrlLoggingOut;
public:
	// 패킷 캡처 핸들
	pcap_t* m_hPcap;
	// 패킷 캡처 핸들러
	static void packet_handler(u_char* param, const struct pcap_pkthdr* header, const u_char* pkt_data);
	// 캡처 에러 버퍼
	char m_lpszErrbuf[PCAP_ERRBUF_SIZE];
	// 필터 구조체
	struct bpf_program m_fcode;
public:
	// 캡처 스레드 함수
	static UINT CaptureThreadFunc(LPVOID lpParam);
	// 캡처 스레드 클래스
	CWinThread* m_pThread;
	// 크리티컬 섹션
	CCriticalSection m_CS;
	// 스레드 제어 변수
	int m_ThreadStatus;
	// 스레드 상태
	enum Status {
		THREAD_RUNNING,
		THREAD_PAUSE,
		THREAD_STOP
	};
public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonPause();
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
};