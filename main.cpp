#include<iostream>
#include<stdio.h>
#include<string.h>
#include <Windows.h>
#include<time.h>
#include <graphics.h>
#include <conio.h>
#include<algorithm>
#include<stdlib.h>
#include <random>
#include <string>
#include <fstream>
#include <ddraw.h>
#include<iomanip>
#include <Psapi.h>
#include<vector>
#include<io.h>
#include <sapi.h>                                             // 语音头文件
#include <sphelper.h>                                     // 语音识别头文件
#include <thread>                                           //多线程
                                                                        //vs2019自定义控制台代码模板 by mao
                                                                        //作者QQ1296193245
using namespace std;
#pragma warning(disable : 4996)
#pragma comment (lib,"ddraw.lib")
#pragma comment(lib,"sapi.lib")
#pragma comment(lib,"ole32.lib")
#pragma intrinsic(__rdtsc)
unsigned __int64 t1, t2, t3, t4;
double run_time;                                                   //微秒级算法时间计时器
LARGE_INTEGER time_start;	                                //开始时间
LARGE_INTEGER time_over;	                                //结束时间
double dqFreq;		                                               //计时器频率
LARGE_INTEGER f;	                                               //计时器频率
double tt;
long double totaltime;

inline void runstart()
{
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	t1 = __rdtsc();
	QueryPerformanceCounter(&time_start);	       //计时开始
}
inline void runend()
{
	QueryPerformanceCounter(&time_over);	        //计时结束
	run_time = 1000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
	t2 = __rdtsc();
	t3 = t2 - t1;
}
void ShowProcessMemoryUsageInfo()
{
	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
	float memoryUsage_M = pmc.WorkingSetSize / (1024.0 * 1024.0);
	float memoryUsage_K = pmc.WorkingSetSize / 1024.0;
	std::cout << std::fixed << setprecision(0) << "程序内存使用率：" << memoryUsage_K << " KB = " << setprecision(2) << memoryUsage_M << " MB" << std::endl;
}
__int64 CompareFileTime(FILETIME time1, FILETIME time2)//转换为整数时间
{
	__int64 a = static_cast<long long>(time1.dwHighDateTime) << 32 | time1.dwLowDateTime;
	__int64 b = static_cast<long long>(time2.dwHighDateTime) << 32 | time2.dwLowDateTime;
	return   (b - a);
}
void rundisplay()
{
	cout << endl;
	cout << "-------------------------------------------------------" << endl;
	if (t3 < 10000)
	{
		cout << "CPU脉冲数：" << (t3) << endl;
	}
	else if (t3 < 100000000 && t3 >= 10000)
	{
		tt = t3;
		cout << "CPU脉冲数：" << (tt) / 10000 << "万" << endl;
	}
	else if (t3 > 100000000000)
	{
		t4 = t3;
		t4 = t3 / 10000000;
		tt = t4;
		cout << "CPU脉冲数：" << (tt) / 10 << "亿" << endl;
	}
	else
	{
		t4 = t3;
		t4 = t3 / 10000;
		tt = t4;
		cout << "CPU脉冲数：" << (tt) / 10000 << "亿" << endl;
	}
	if (run_time > 10000)
	{
		cout << "算法运行时间：" << run_time / 1000 << "秒" << endl;
	}
	else if (run_time < 1)
	{
		cout << "算法运行时间：" << run_time * 1000 << "微秒" << endl;
	}
	else
	{
		cout << "算法运行时间：" << run_time << "毫秒" << endl;
	}
	totaltime = clock();
	cout << "程序运行时间：" << totaltime / 1000 << "秒" << endl;
	if (run_time > 0.4)
	{
		double frequency = t3 / run_time / 1000;
		cout << "CPU频率：" << frequency << "MHZ" << endl;
	}
	ShowProcessMemoryUsageInfo();
	long lRet;
	HKEY hKey;
	TCHAR tchData[64];
	DWORD dwSize;
	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Hardware\\Description\\System\\CentralProcessor\\0", 0, KEY_QUERY_VALUE, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		dwSize = sizeof(tchData);
		lRet = RegQueryValueEx(hKey, L"ProcessorNameString", NULL, NULL, (LPBYTE)tchData, &dwSize);
		if (lRet == ERROR_SUCCESS)
		{
			cout << "CPU 名字：";
			wcout << tchData << endl;
		}
		else
		{
			cout << "CPU 名字：";
			wcout << L"未知" << endl;
		}
	}
	else
	{
		cout << "CPU 名字：";
		wcout << L"未知" << endl;
	}
	RegCloseKey(hKey);
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	double totalphy = statex.ullTotalPhys / 1024 / 1024;
	totalphy = totalphy / 1024;
	cout << "总物理内存：    " << setprecision(4) << left << setw(7) << totalphy << " GB" << endl;
	double totalvir = statex.ullTotalPageFile / 1024 / 1024;
	totalvir = totalvir / 1024;
	cout << "总虚拟内存：    " << setw(7) << totalvir - totalphy << " GB" << endl;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordScreen = { 0, 0 }; //光标位置
	CONSOLE_SCREEN_BUFFER_INFO csbi{};
	if (GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		//printf("光标坐标:(%d,%d)\n", csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y);
	}
	HANDLE   hStdout;
	//   光标位置   
	COORD     cursorPos;
	//   标准输出句柄   
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hEvent;
	BOOL res;
	FILETIME preidleTime;
	FILETIME prekernelTime;
	FILETIME preuserTime;
	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;
	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
	preidleTime = idleTime;
	prekernelTime = kernelTime;
	preuserTime = userTime;
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);// 初始值为 nonsignaled ，并且每次触发后自动设置为nonsignaled
	double freephy = statex.ullAvailPhys / 1024 / 1024;
	freephy = freephy / 1024;
	double usephy = (totalphy - freephy) * 100.0 / totalphy;
	cout << "内存使用率：    " << setprecision(3) << usephy << "%" << endl;
	double freetotal = statex.ullAvailPageFile / 1024 / 1024;
	freetotal = freetotal / 1024;
	cout << "总空闲内存：    " << setprecision(3) << freetotal << " GB" << endl;
	double freevir = (totalvir - freetotal) - (totalphy * statex.dwMemoryLoad / 100.0);
	cout << "虚拟内存使用量：" << setprecision(3) << freevir << " GB" << endl;
	double usevir = freevir * 100 / (totalvir - totalphy);
	cout << "虚拟内存使用率：";
	printf("%.2f%%", usevir);
	cout << endl;
	cout << "CPU利用率：    " << right << setprecision(2) << setw(6) << 0.00 << "%" << endl;
	cout << "CPU空闲率：    " << setw(6) << 100.0<< "%" << endl;
	cout << "-------------------------------------------------------" << endl;
	cout << "按任意键退出程序" << endl;
	while (!_kbhit())
	{
		GlobalMemoryStatusEx(&statex);
		WaitForSingleObject(hEvent, 1000);
		res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
		int idle = CompareFileTime(preidleTime, idleTime);
		int kernel = CompareFileTime(prekernelTime, kernelTime);
		int user = CompareFileTime(preuserTime, userTime);
		float cpu = (kernel + user - idle) * 100.0 / (kernel + user);
		float cpuidle = (idle) * 100.0 / (kernel + user);
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 0;
		SetConsoleCursorPosition(hStdout, cursorPos);
		//cout << "内存使用率：" << statex.dwMemoryLoad << "%" << endl;
		double freephy = statex.ullAvailPhys / 1024 / 1024;
		freephy = freephy / 1024;
		double usephy = (totalphy - freephy) * 100.0 / totalphy;
		cout << "内存使用率：    " << setprecision(3) << usephy << "%" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 1;
		SetConsoleCursorPosition(hStdout, cursorPos);
		double freetotal = statex.ullAvailPageFile / 1024 / 1024;
		freetotal = freetotal / 1024;
		cout << "总空闲内存：    " << setprecision(3) << freetotal <<" GB" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 2;
		SetConsoleCursorPosition(hStdout, cursorPos);
		double freevir = (totalvir - freetotal) - (totalphy * statex.dwMemoryLoad / 100.0);
		cout << "虚拟内存使用量：" << setprecision(3) << freevir <<" GB" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 3;
		SetConsoleCursorPosition(hStdout, cursorPos);
		double usevir = freevir * 100 / (totalvir - totalphy);
		cout << "虚拟内存使用率：";
		printf("%.2f%%", usevir);
		cout << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 4;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "CPU利用率：    " << right << setprecision(2) << setw(6) << cpu << "%" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 5;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "CPU空闲率：    " << setw(6) << cpuidle << "%"<<endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 6;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "-------------------------------------------------------" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 7;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "按任意键退出程序" << endl;
		preidleTime = idleTime;
		prekernelTime = kernelTime;
		preuserTime = userTime;
	}
	cout << endl;
	
}


//**************************************************************//************************非主函数

#define MAX_VERtEX_NUM 20                   //顶点的最大个数
#define VRType int                          //表示顶点之间的关系的变量类型
#define InfoType char                       //存储弧或者边额外信息的指针变量类型
#define VertexType int                      //图中顶点的数据类型
typedef enum { DG, DN, UDG, UDN }GraphKind;       //枚举图的 4 种类型
typedef struct
{
	VRType adj;                             //对于无权图，用 1 或 0 表示是否相邻；对于带权图，直接为权值。
	InfoType* info;                        //弧或边额外含有的信息指针
}ArcCell, AdjMatrix[MAX_VERtEX_NUM][MAX_VERtEX_NUM];
typedef struct 
{
	VertexType vexs[MAX_VERtEX_NUM];        //存储图中顶点数据
	AdjMatrix arcs;                         //二维数组，记录顶点之间的关系
	int vexnum, arcnum;                      //记录图的顶点数和弧（边）数
	GraphKind kind;                         //记录图的种类
}MGraph;
//根据顶点本身数据，判断出顶点在二维数组中的位置
int LocateVex(MGraph* G, VertexType v) 
{
	int i = 0;
	//遍历一维数组，找到变量v
	for (; i < G->vexnum; i++) 
	{
		if (G->vexs[i] == v) 
		{
			break;
		}
	}
	//如果找不到，输出提示语句，返回-1
	if (i > G->vexnum) 
	{
		printf("no such vertex.\n");
		return -1;
	}
	return i;
}

//构造有向图
void CreateDG(MGraph* G)
{
	//输入图含有的顶点数和弧的个数
	scanf("%d,%d", &(G->vexnum), &(G->arcnum));
	//依次输入顶点本身的数据
	for (int i = 0; i < G->vexnum; i++) 
	{
		scanf("%d", &(G->vexs[i]));
	}
	//初始化二维矩阵，全部归0，指针指向NULL
	for (int i = 0; i < G->vexnum; i++) 
	{
		for (int j = 0; j < G->vexnum; j++)
		{
			G->arcs[i][j].adj = 0;
			G->arcs[i][j].info = NULL;
		}
	}
	//在二维数组中添加弧的数据
	for (int i = 0; i < G->arcnum; i++)
	{
		int v1, v2;
		//输入弧头和弧尾
		scanf("%d,%d", &v1, &v2);
		//确定顶点位置
		int n = LocateVex(G, v1);
		int m = LocateVex(G, v2);
		//排除错误数据
		if (m == -1 || n == -1)
		{
			printf("no this vertex\n");
			return;
		}
		//将正确的弧的数据加入二维数组
		G->arcs[n][m].adj = 1;
	}
}
//构造无向图
void CreateDN(MGraph* G)
{
	scanf("%d,%d", &(G->vexnum), &(G->arcnum));
	for (int i = 0; i < G->vexnum; i++)
	{
		scanf("%d", &(G->vexs[i]));
	}
	for (int i = 0; i < G->vexnum; i++)
	{
		for (int j = 0; j < G->vexnum; j++) 
		{
			G->arcs[i][j].adj = 0;
			G->arcs[i][j].info = NULL;
		}
	}
	for (int i = 0; i < G->arcnum; i++)
	{
		int v1, v2;
		scanf("%d,%d", &v1, &v2);
		int n = LocateVex(G, v1);
		int m = LocateVex(G, v2);
		if (m == -1 || n == -1)
		{
			printf("no this vertex\n");
			return;
		}
		G->arcs[n][m].adj = 1;
		G->arcs[m][n].adj = 1;//无向图的二阶矩阵沿主对角线对称
	}
}
//构造有向网，和有向图不同的是二阶矩阵中存储的是权值。
void CreateUDG(MGraph* G) 
{
	scanf("%d,%d", &(G->vexnum), &(G->arcnum));
	for (int i = 0; i < G->vexnum; i++) 
	{
		scanf("%d", &(G->vexs[i]));
	}
	for (int i = 0; i < G->vexnum; i++)
	{
		for (int j = 0; j < G->vexnum; j++)
		{
			G->arcs[i][j].adj = 0;
			G->arcs[i][j].info = NULL;
		}
	}
	for (int i = 0; i < G->arcnum; i++) 
	{
		int v1, v2, w;
		scanf("%d,%d,%d", &v1, &v2, &w);
		int n = LocateVex(G, v1);
		int m = LocateVex(G, v2);
		if (m == -1 || n == -1) 
		{
			printf("no this vertex\n");
			return;
		}
		G->arcs[n][m].adj = w;
	}
}
//构造无向网。和无向图唯一的区别就是二阶矩阵中存储的是权值
void CreateUDN(MGraph* G) 
{
	scanf("%d,%d", &(G->vexnum), &(G->arcnum));
	for (int i = 0; i < G->vexnum; i++) 
	{
		scanf("%d", &(G->vexs[i]));
	}
	for (int i = 0; i < G->vexnum; i++) 
	{
		for (int j = 0; j < G->vexnum; j++)
		{
			G->arcs[i][j].adj = 0;
			G->arcs[i][j].info = NULL;
		}
	}
	for (int i = 0; i < G->arcnum; i++) 
	{
		int v1, v2, w;
		scanf("%d,%d,%d", &v1, &v2, &w);
		int m = LocateVex(G, v1);
		int n = LocateVex(G, v2);
		if (m == -1 || n == -1) 
		{
			printf("no this vertex\n");
			return;
		}
		G->arcs[n][m].adj = w;
		G->arcs[m][n].adj = w;//矩阵对称
	}
}
void CreateGraph(MGraph* G)
{
	//选择图的类型
	scanf("%d", &(G->kind));
	//根据所选类型，调用不同的函数实现构造图的功能
	switch (G->kind) {
	case DG:
		return CreateDG(G);
		break;
	case DN:
		return CreateDN(G);
		break;
	case UDG:
		return CreateUDG(G);
		break;
	case UDN:
		return CreateUDN(G);
		break;
	default:
		break;
	}
}
//输出函数
void PrintGrapth(MGraph G)
{
	for (int i = 0; i < G.vexnum; i++)
	{
		for (int j = 0; j < G.vexnum; j++)
		{
			printf("%d ", G.arcs[i][j].adj);
		}
		printf("\n");
	}
}

//**************************************************************//************************

int main()
{

	//*************************************************************************



	//*************************************************************************
	runstart();
	//*************************计时开始***************************************
	
	MGraph G;//建立一个图的变量
	CreateGraph(&G);//调用创建函数，传入地址参数
	PrintGrapth(G);//输出图的二阶矩阵

	//*************************计时结束***************************************
	runend();



	



	rundisplay();                                                                   //计时结果显示
	_getch();
	//closegraph();
	// system("pause");
	//Sleep(10000);
	return 0;
}
