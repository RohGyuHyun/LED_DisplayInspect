#pragma once


#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

//20161017
#include <conio.h>

#include <pylonc/PylonC.h>

//20161019 ht ī�޶� ����üũ ���� define
#define SUCCESS_CAMERA				0
#define ERROR_CAMERA_NOT_OPEN		1
#define ERROR_CAMERA_AVAILABLE		2



//20161018 ht ī�޶� C������� ����
/*		
InitCamera ->ī�޶� �ʱ�ȭ �� heartbeat �ð� ����
grab -> grab�Լ�
setHeartbeatTimeout -> heartbeat �ð� ����
Release -> ī�޶� ������ ����
*/



class CBaslerManage
{
public:
	CBaslerManage();
	CBaslerManage(int _w, int _h);
	~CBaslerManage();
	
	BYTE *GetGrabImgBuf(){return m_pImageBuf;}
	PylonGrabResult_t ReturnGrabResult(){return m_struct_GrabResult;}


private:
	PYLON_DEVICE_HANDLE m_hDevice;
	int32_t m_nPayloadSize;
	PylonGrabResult_t m_struct_GrabResult;
	BYTE *m_pImageBuf;

public:
	int InitCamera(int heartbeat_ms = 500);
	BOOL Grab(BOOL disp = FALSE);
	GENAPIC_RESULT setHeartbeatTimeout( int timeout_ms );
	void Release();
	
};

