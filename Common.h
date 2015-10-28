#pragma once

#include <string>
#include <windows.h>

namespace Common
{
	std::wstring ansi2unicode(const std::string& ansi);
	std::string unicode2ansi(const std::wstring& unicode);
	std::wstring utf8ToUnicode(const std::string& str);
	std::string toString(const char *fmt, ...);
	std::string getErrString();
}

#pragma warning(disable : 4200)					// 使用了非标准扩展 : 结构/联合中的零大小数组

#define MAX_NAME_LEN 256
#pragma pack(push)								//保存对齐状态
#pragma pack(4)									//设定为4字节对齐
struct IM_MSG
{
	int   nIMType;								//聊天类型，好友聊天=0x1，群聊天=0x2，文件传输=0x3
	int   nDirection;							//消息方向，消息或文件的接收= 0，消息或文件的发送= 1
	INT64 uTime;								//消息的产生时间，使用个字节
	wchar_t szLocalAccount[MAX_NAME_LEN];		//本地登陆账号名，字符串使用unicode编码，数组大小共*2=512字节
	wchar_t szTargetAccount[MAX_NAME_LEN];		//对方账号名
	int nMessageLen;							//聊天消息长度
	wchar_t szMsg[0];

	IM_MSG()
	{
		nIMType = 0;
		nDirection = 0;
		uTime = 0;
		wmemset(szLocalAccount, 0, _countof(szLocalAccount));
		wmemset(szTargetAccount, 0, _countof(szTargetAccount));
		nMessageLen = 0;
	}
};
#pragma pack(pop)//恢复对齐状态
