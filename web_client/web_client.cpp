// web_client.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include "framework.h"
#include "web_client.h"

#include "Function.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;


using namespace std;

int main(int argc, char* argv[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(nullptr);
	if (hModule != nullptr)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, nullptr, ::GetCommandLine(), 0))
		{
			// TODO: code your application's behavior here.
			wprintf(L"Fatal Error: MFC initialization failed\n");
			nRetCode = 1;
		}
		else
		{
			// TODO: code your application's behavior here.

			// Khoi tao thu vien
			if (AfxSocketInit() == FALSE)
			{
				cout << "Khong the khoi tao thu vien Socket";
				return FALSE;
			}

			if (argc < 2)
			{
				cout << "\nKhong tim thay URL!\n";
				return 0;
			}

			vector<thread> threads;
			this_thread::sleep_for(chrono::milliseconds(200));
			for (int i = 0; i < argc - 1; i++)
			{
				threads.push_back(thread(Download, argv[i + 1]));
				this_thread::sleep_for(chrono::milliseconds(200));
			}
			this_thread::sleep_for(chrono::milliseconds(500));
			for (int i = 0; i < threads.size(); i++)
			{
				threads[i].join();
			}
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		wprintf(L"Fatal Error: GetModuleHandle failed\n");
		nRetCode = 1;
	}
	return nRetCode;
}
