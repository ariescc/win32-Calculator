/*
author: ariescc
*/
#include <windows.h>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <tchar.h>
#include <memory.h>
#include <malloc.h>
#include <stdlib.h>
#include "zdd.h"

#define MAX_RESULT     16

// 函数声明
LRESULT CALLBACK WndProc(
	HWND hWnd, // 窗口句柄
	UINT message,
	WPARAM wParam,
	LPARAM lparam
);

// 全局变量
HWND hWnd = NULL; // 窗口句柄
		   // 创建窗口
char titleName[] = "计算器"; // 窗口标题

WCHAR lpClassName[256];
WCHAR lpWindowName[256];

// 计算器相关变量
int i, j = 0;
HINSTANCE hInst;
HWND resultHwnd = NULL;
WNDPROC EditProc = NULL;
// 保存要运算的两个数字
char FirstNum[MAX_RESULT] = "";
char SecondNum[MAX_RESULT] = "";

double Num1, Num2; // 转化后实际计算的数
bool CalTag = false;							// 是否正在运算标志，用来区分输入的是第一个数还是第二个数
bool FirstNumFirstTag = true;					// 是否 第一个数第一次输入
bool SecondNumFirstTag = false;					// 是否 第二个数第一次输入
bool ConstantCalTag = false;					// 是否 正在连续运算
int CalState = NULL;								// 记录运算的类型
char result[100] = "";					// 用来显示至edit框(结果)
std::string key = "1234567890+-./%*="; // 键盘输入的合法键

BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    Edit(HWND, UINT, WPARAM, LPARAM);


void setNum(HWND resultHwnd, char* num);
void CalEqual(HWND resultHwnd);


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	// 将 char* 类型转成 LPCWSTR 类型
	memset(lpClassName, 0, sizeof(lpClassName));
	memset(lpWindowName, 0, sizeof(lpWindowName));

	MultiByteToWideChar(CP_ACP, 0, titleName, strlen(titleName) + 1, lpClassName, sizeof(lpClassName) / sizeof(lpClassName[0]));
	MultiByteToWideChar(CP_ACP, 0, titleName, strlen(titleName) + 1, lpWindowName, sizeof(lpWindowName) / sizeof(lpWindowName[0]));

	// 定义窗口类
	WNDCLASSEX wcex;

	// 设置窗口属性
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW; // 从这个窗口派生出窗口具有的风格
	wcex.lpfnWndProc = (WNDPROC)WndProc; // 消息处理函数指针
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL; // 窗口最小化图标
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = (LPCWSTR)(CHAR *)IDR_MENU1;
	wcex.lpszClassName = lpClassName; // 该窗口类的名称
	wcex.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, 0); // 窗口左上角的图标


	// 注册窗口
	::RegisterClassEx(&wcex);

	// 执行应用程序初始化
	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	// 循环获取\处理消息
	MSG msg;
	while (::GetMessage(&msg, 0, 0, 0)) {
		::TranslateMessage(&msg); // 转换键盘消息
		::DispatchMessage(&msg); // 将消息发送到相应的窗口函数
	}

	return (int)msg.wParam;

}

/*
	保存实例句柄并创建主窗口
*/
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
	hInst = hInstance;
	
	hWnd = ::CreateWindow(lpClassName, lpWindowName, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	// 窗口创建失败
	if (!hWnd) {
		return FALSE;
	}

	resultHwnd = ::CreateWindowA("edit",
		NULL,
		WS_CHILD | WS_BORDER | WS_VISIBLE | ES_READONLY | ES_RIGHT,
		10, 10,
		360, 40,
		hWnd,
		(HMENU)RESULT,
		hInst,
		NULL);
	EditProc = (WNDPROC)SetWindowLongPtr(resultHwnd, GWLP_WNDPROC, (LONG)Edit);
	::CreateWindowA("button",
		"7",
		WS_CHILD | WS_VISIBLE,
		10, 60,
		64, 64,
		hWnd,
		(HMENU)NUM7,
		hInst,
		NULL);
	::CreateWindowA("button",
		"8",
		WS_CHILD | WS_VISIBLE,
		84, 60,
		64, 64,
		hWnd,
		(HMENU)NUM8,
		hInst,
		NULL);
	::CreateWindowA("button",
		"9",
		WS_CHILD | WS_VISIBLE,
		158, 60,
		64, 64,
		hWnd,
		(HMENU)NUM9,
		hInst,
		NULL);
	::CreateWindowA("button",
		SZSQR,
		WS_CHILD | WS_VISIBLE,
		232, 60,
		64, 64,
		hWnd,
		(HMENU)SQR,
		hInst,
		NULL);
	::CreateWindowA("button",
		"C",
		WS_CHILD | WS_VISIBLE,
		306, 60,
		64, 64,
		hWnd,
		(HMENU)CLE,
		hInst,
		NULL);
	::CreateWindowA("button",
		"4",
		WS_CHILD | WS_VISIBLE,
		10, 134,
		64, 64,
		hWnd,
		(HMENU)NUM4,
		hInst,
		NULL);
	::CreateWindowA("button",
		"5",
		WS_CHILD | WS_VISIBLE,
		84, 134,
		64, 64,
		hWnd,
		(HMENU)NUM5,
		hInst,
		NULL);
	::CreateWindowA("button",
		"6",
		WS_CHILD | WS_VISIBLE,
		158, 134,
		64, 64,
		hWnd,
		(HMENU)NUM6,
		hInst,
		NULL);
	::CreateWindowA("button",
		"+",
		WS_CHILD | WS_VISIBLE,
		232, 134,
		64, 64,
		hWnd,
		(HMENU)ADD,
		hInst,
		NULL);
	::CreateWindowA("button",
		"-",
		WS_CHILD | WS_VISIBLE,
		306, 134,
		64, 64,
		hWnd,
		(HMENU)SUB,
		hInst,
		NULL);
	::CreateWindowA("button",
		"1",
		WS_CHILD | WS_VISIBLE,
		10, 208,
		64, 64,
		hWnd,
		(HMENU)NUM1,
		hInst,
		NULL);
	::CreateWindowA("button",
		"2",
		WS_CHILD | WS_VISIBLE,
		84, 208,
		64, 64,
		hWnd,
		(HMENU)NUM2,
		hInst,
		NULL);
	::CreateWindowA("button",
		"3",
		WS_CHILD | WS_VISIBLE,
		158, 208,
		64, 64,
		hWnd,
		(HMENU)NUM3,
		hInst,
		NULL);
	::CreateWindowA("button",
		"*",
		WS_CHILD | WS_VISIBLE,
		232, 208,
		64, 64,
		hWnd,
		(HMENU)MUL,
		hInst,
		NULL);
	::CreateWindowA("button",
		"/",
		WS_CHILD | WS_VISIBLE,
		306, 208,
		64, 64,
		hWnd,
		(HMENU)DIV,
		hInst,
		NULL);
	::CreateWindowA("button",
		"0",
		WS_CHILD | WS_VISIBLE,
		10, 282,
		138, 64,
		hWnd,
		(HMENU)NUM0,
		hInst,
		NULL);
	::CreateWindowA("button",
		".",
		WS_CHILD | WS_VISIBLE,
		158, 282,
		64, 64,
		hWnd,
		(HMENU)POI,
		hInst,
		NULL);
	::CreateWindowA("button",
		"=",
		WS_CHILD | WS_VISIBLE,
		232, 282,
		64, 64,
		hWnd,
		(HMENU)EQU,
		hInst,
		NULL);
	::CreateWindowA("button",
		"%",
		WS_CHILD | WS_VISIBLE,
		306, 282,
		64, 64,
		hWnd,
		(HMENU)MOD,
		hInst,
		NULL);

	// 显示窗口
	::ShowWindow(hWnd, nCmdShow);
	// 刷新窗口客户区
	::UpdateWindow(hWnd);

	return TRUE;
}

// 窗口消息处理函数
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	HDC hDC;
	RECT rect;
	PAINTSTRUCT ps;

	int wmId, wmEvent; // 菜单id
	static std::string s;

	switch (message)
	{
	case WM_PAINT:
		hDC = ::BeginPaint(hWnd, &ps);   //使无效的客户区变得有效，并取得环境设备

										 //自定义
		{
			::GetClientRect(hWnd, &rect);    //获取窗口客户区的大小
			::DrawText(hDC, TEXT(""), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);   //在指定的矩形里写入格式化的正文
		}

		::EndPaint(hWnd, &ps);   //释放环境变量句柄，和::BeginPaint 配套使用   
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);   //该函数向消息队列中插入一条 WM_QUIT 消息，由 GetMessage 函数捕获返回 0 而退出程序
		break;
		// 点击菜单退出，销毁窗口
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case NUM0:
			setNum(resultHwnd, (char*)("0"));
			//::MessageBoxA(NULL, "123", "123", MB_OK);
			break;
		case NUM1:
			setNum(resultHwnd, (char*)("1"));
			break;
		case NUM2:
			setNum(resultHwnd, (char*)("2"));
			break;
		case NUM3:
			setNum(resultHwnd, (char*)("3"));
			break;
		case NUM4:
			setNum(resultHwnd, (char*)("4"));
			break;
		case NUM5:
			setNum(resultHwnd, (char*)("5"));
			break;
		case NUM6:
			setNum(resultHwnd, (char*)("6"));
			break;
		case NUM7:
			setNum(resultHwnd, (char*)("7"));
			break;
		case NUM8:
			setNum(resultHwnd, (char*)("8"));
			break;
		case NUM9:
			setNum(resultHwnd, (char*)("9"));
			break;
		case POI:
			setNum(resultHwnd, (char*)("."));
			break;
		case ADD:						//当为某个运算时
			CalState = ADD;				//将运算类型保存
			CalTag = true;				//将运算状态置为true
			SecondNumFirstTag = true;   //因为接下来要输第二个数，所以将第二个数第一次输入置为true
			break;
		case MUL:
			CalState = MUL;
			CalTag = true;
			SecondNumFirstTag = true;
			break;
		case DIV:
			CalState = DIV;
			CalTag = true;
			SecondNumFirstTag = true;
			break;
		case SUB:
			CalState = SUB;
			CalTag = true;
			SecondNumFirstTag = true;
			break;
		case SQR:
			CalState = SQR;
			CalTag = true;
			SecondNumFirstTag = true;
			break;
		case MOD:
			CalState = MOD;
			CalTag = true;
			SecondNumFirstTag = true;
			break;
		case EQU:
			SetFocus(hWnd);
			CalEqual(resultHwnd);
			break;
		case CLE:
			memset(FirstNum, '\0', sizeof(FirstNum));		//初始化两个char数组
			memset(SecondNum, '\0', sizeof(SecondNum));
			CalTag = false;									//其他所有状态重置为初始状态
			FirstNumFirstTag = true;
			SecondNumFirstTag = false;
			CalState = NULL;
			ConstantCalTag = false;
			memset(result, '\0', sizeof(result));
			SetWindowTextA(resultHwnd, "0");
			break;
		// 菜单退出项
		case IDM_EXIT:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		SetFocus(hWnd);
		break;
	case WM_CHAR:
		//if (s.find("X=", 0) == 0)
		//{
		//	s = "";
		//}
		//s = s + char(wParam);
		////::MessageBoxA(hWnd, "123", "123", MB_OK);
		//
		////使客户区无效 进而刷新客户区
		//::InvalidateRect(hWnd, NULL, 1);
		if (key.find(wParam) == std::string::npos)
		{
			MessageBoxA(NULL, "输入不合法，请输入数字或“.”或运算符", "警告", MB_OK);
		}
		else
		{
			switch (wParam)
			{
			case '0':
				setNum(resultHwnd, (char*)("0"));
				break;
			case '1':
				setNum(resultHwnd, (char*)("1"));
				break;
			case '2':
				setNum(resultHwnd, (char*)("2"));
				break;
			case '3':
				setNum(resultHwnd, (char*)("3"));
				break;
			case '4':
				setNum(resultHwnd, (char*)("4"));
				break;
			case '5':
				setNum(resultHwnd, (char*)("5"));
				break;
			case '6':
				setNum(resultHwnd, (char*)("6"));
				break;
			case '7':
				setNum(resultHwnd, (char*)("7"));
				break;
			case '8':
				setNum(resultHwnd, (char*)("8"));
				break;
			case '9':
				setNum(resultHwnd, (char*)("9"));
				break;
			case '.':
				setNum(resultHwnd, (char*)("."));
				break;
			case '+':
				CalState = ADD;
				CalTag = true;
				SecondNumFirstTag = true;
				break;
			case '*':
				CalState = MUL;
				CalTag = true;
				SecondNumFirstTag = true;
				break;
			case '/':
				CalState = DIV;
				CalTag = true;
				SecondNumFirstTag = true;
				break;
			case '-':
				CalState = SUB;
				CalTag = true;
				SecondNumFirstTag = true;
				break;
			case '%':
				CalState = MOD;
				CalTag = true;
				SecondNumFirstTag = true;
				break;
			case '=':
				CalEqual(resultHwnd);
				break;
			case VK_BACK:
				memset(FirstNum, '\0', sizeof(FirstNum));
				memset(SecondNum, '\0', sizeof(SecondNum));
				CalTag = false;
				FirstNumFirstTag = true;
				SecondNumFirstTag = false;
				CalState = NULL;
				ConstantCalTag = false;
				memset(result, '\0', sizeof(result));
				SetWindowTextA(resultHwnd, "0");
				break;
			}
		}
		break;
	}
	return ::DefWindowProc(hWnd, message, wParam, lParam);

}

void setNum(HWND resultHwnd, char* num)
{
	if (CalTag == false)						//false表示 输入的是第一个数
	{
		if (FirstNumFirstTag == true)
		{
			if (*num == '.')
			{
				if (FirstNum[0] == '0')
				{
					strcat_s(FirstNum, num);
					FirstNumFirstTag = false;
					i = j = 0;
					return;
				}
				else
				{
					MessageBoxA(resultHwnd, "请先输入数字", "警告", MB_OK);
					return;
				}
			}
			i = j = 0;
			FirstNum[0] = *num;
			FirstNumFirstTag = false;
		}
		else
		{
			if (FirstNum[0] == '0'&& *num == '0'&&FirstNum[j] != '.')
			{
				return;
			}
			if ((FirstNum[i] == '.' || FirstNum[j] == '.') && *num == '.')
			{
				return;
			}
			if (GetWindowTextLength(resultHwnd) >= MAX_RESULT)
			{
				MessageBoxA(resultHwnd, "超过最大长度", "警告", MB_OK);
				return;
			}
			if (*num == '.') j = i + 1;
			i++;
			strcat_s(FirstNum, num);
		}
		ConstantCalTag = false;				//输入第一个数则表示 没有连续运算
		::SetWindowTextA(resultHwnd, FirstNum);
	}
	else
	{										//true表示输入的是第二个数
		if (SecondNumFirstTag == true)
		{
			if (*num == '.')
			{
				MessageBoxA(resultHwnd, "请先输入数字", "警告", MB_OK);
				return;
			}
			else
			{
				SecondNum[0] = *num;
				SecondNumFirstTag = false;
			}
			i = j = 0;
		}
		else
		{
			if (SecondNum[0] == '0'&& *num == '0'&&SecondNum[i] != '.')
			{
				return;
			}
			if ((SecondNum[j] == '.' || SecondNum[i] == '.') && *num == '.')
			{
				return;
			}
			if (GetWindowTextLength(resultHwnd) >= MAX_RESULT)
			{
				MessageBoxA(resultHwnd, "超过最大长度", "警告", MB_OK);
				return;
			}
			if (*num == '.') i = j + 1;
			j++;
			strcat_s(SecondNum, num);
		}

		//strcat_s(SecondNum, num);
		::SetWindowTextA(resultHwnd, SecondNum);
	}

	//::MessageBoxA(NULL, "123", "123", MB_OK);
}

void CalEqual(HWND resultHwnd)
{
	if (SecondNumFirstTag == true)					//为true，说明还未输入第二个数就按了=号
	{
		MessageBoxA(resultHwnd, "请输入第二个数字", "警告", MB_OK);
		return;
	}
	if (ConstantCalTag == false)					//为false,说明没有连续运算，则需要转化第一个数
	{									//为true,则直接用上一次运算的结果充当num1参与本次运算
		Num1 = strtod(FirstNum, NULL);
	}
	Num2 = strtod(SecondNum, NULL);
	switch (CalState)
	{
	case ADD:
		Num1 = Num1 + Num2;
		break;
	case MUL:
		Num1 = Num1 * Num2;
		break;
	case DIV:
		if (Num2 == 0.0)
		{
			MessageBoxA(resultHwnd, "被除数不能是0", "警告", MB_OK);
			SecondNumFirstTag = true;
			return;
		}
		Num1 = Num1 / Num2;
		break;
	case SUB:
		Num1 = Num1 - Num2;
		break;
	case MOD:
		if (Num2 == 0.0)
		{
			MessageBoxA(resultHwnd, "被除数不能是0", "警告", MB_OK);
			SecondNumFirstTag = true;
			return;
		}
		Num1 = fmod(Num1, Num2);
		break;
	case SQR:
		if (Num2 == 0.0)
		{
			MessageBoxA(resultHwnd, "被除数不能是0", "警告", MB_OK);
			SecondNumFirstTag = true;
			return;
		}
		Num1 = pow(Num1, 1.0 / Num2);
		break;
	default:
		return;
	}
	char str[100];
	_gcvt(Num1, MAX_RESULT, str);
	//MessageBoxA(NULL, result, "", MB_OK);//将结果转化为char*
	//SetFocus(resultHwnd);
	SetWindowTextA(resultHwnd, str);
	ConstantCalTag = true;						//运算完后 连续运算置为true，运算标志位置为true,
	CalTag = false;								//第一个数也要第一次输入了，将FirstNumFirstTag也置为true
	FirstNumFirstTag = true;
	CalState = NULL;							//运算状态置为空

	memset(result, '\0', sizeof(result));
	memset(FirstNum, '\0', sizeof(FirstNum));
	memset(SecondNum, '\0', sizeof(SecondNum));
}

LRESULT CALLBACK Edit(HWND hLdg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_SETFOCUS)
	{
		SetFocus(hWnd);
		return 0;
	}
	else
	{
		CallWindowProc(EditProc, hLdg, msg, wParam, lParam);
	}
}