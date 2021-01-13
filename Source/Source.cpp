#include "FTPClient.hpp"

//int main() 
//{
//	FTPClient *client = new FTPClient();
//	bool connected = false;
//
//	int x; //for decisionmaking
//
//	if (client->Connect(IP, PORT))
//	{
//		if (client->Login(username, password))
//		{
//			connected = true;
//		}
//	}
//	
//	//if all goes well, this is where we perform everything. The "main loop".
//	do 
//	{
//		std::cout << "Commands:\n1. List Everything.\n2. Disconnect." << std::endl;
//		std::cin >> x;
//		switch (x)
//		{
//		case 1: 
//			client->ListEverything();
//			break;
//		case 2:
//			client->Disconnect();
//			connected = false;
//			break;
//		default: std::cout << "Not a command bruh." << std::endl;
//		}
//	} while (connected);
//
//	return 0;
//}

#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <sstream>   // for std::stringstream
#include <algorithm> // for std::copy
#include <iterator>  // for std::ostream_iterator
#include <fstream>

HWND Connect, ContentWindow, ListDirectory, Disconnect, Username, Password, Adress, Port;
static HINSTANCE hInstance;
static bool connected = false;
static FTPClient* client = new FTPClient();

void AppendWindowTextA(HWND hwnd, const char* text) {
   size_t size_window = GetWindowTextLengthA(hwnd);
   size_t size_text = strlen(text);
   size_t bufsize = size_window + size_text;
  
   //the array to put merge both arrays into
   char* buffer = new char[bufsize];

   //array to get text from window
   char* windowText = new char[size_window];

   std::memset(buffer, 0, sizeof(char));
   std::memset(windowText, 0, sizeof(char));
   //get window text
   GetWindowTextA(hwnd, windowText, sizeof(char));

   memcpy(buffer, windowText, strlen(windowText) * sizeof(char));
   memcpy(buffer + strlen(windowText), text, strlen(text) * sizeof(char));

   SetWindowTextA(hwnd, buffer);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"FTPClient",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{    
    switch (uMsg)
    {
    case WM_CREATE:
        Connect = CreateWindowExA(WS_EX_WINDOWEDGE, "Button", "Connect", WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 10, 130, 20, hwnd, NULL, hInstance, (LPVOID)lParam);
        Disconnect = CreateWindowExA(WS_EX_WINDOWEDGE, "Button", "Disconnect", WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 40, 130, 20, hwnd, NULL, hInstance, (LPVOID)lParam);
        ListDirectory = CreateWindowExA(WS_EX_WINDOWEDGE, "Button", "List everything", WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 70, 130, 20, hwnd, NULL, hInstance, (LPVOID)lParam);
        
        Adress = CreateWindowExA(WS_EX_WINDOWEDGE, "Edit", "127.0.0.1", WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 100, 130, 20, hwnd, NULL, hInstance, (LPVOID)lParam);
        Port = CreateWindowExA(WS_EX_WINDOWEDGE, "Edit", "21", WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 130, 130, 20, hwnd, NULL, hInstance, (LPVOID)lParam);


        Username = CreateWindowExA(WS_EX_WINDOWEDGE, "Edit", "admin", WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 160, 130, 20, hwnd, NULL, hInstance, (LPVOID)lParam);
        Password = CreateWindowExA(WS_EX_WINDOWEDGE, "Edit", "lol", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD, 10, 190, 130, 20, hwnd, NULL, hInstance, (LPVOID)lParam);

        //window with content
        ContentWindow = CreateWindowExA(WS_EX_WINDOWEDGE, "Edit", "test", WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | ES_MULTILINE, 10, 250, 1000, 300, hwnd, NULL, hInstance, (LPVOID)lParam);

        break;
    case WM_COMMAND:
        if (uMsg == WM_COMMAND && (HWND)lParam == Connect)
        {
            char username[100];
            char password[100];
            char adress[100];
            char port[100];

            memset(username, 0, sizeof(username));
            memset(password, 0, sizeof(password));
            memset(adress, 0, sizeof(adress));
            memset(port, 0, sizeof(port));

            GetWindowTextA(Adress, (LPSTR)adress, _countof(adress));
            GetWindowTextA(Port, (LPSTR)port, _countof(adress));
            GetWindowTextA(Username, (LPSTR)username, _countof(adress));
            GetWindowTextA(Password, (LPSTR)password, _countof(adress));
            
            int p = atoi(port);
            if (client->Connect(adress, p))
            {
                if (client->Login(username, password))
                {
                    SetWindowTextA(ContentWindow, "connected");
                    connected = true;
                }
            }
            else
            {
                SetWindowTextA(ContentWindow, "Failed to connect");
            }
        }

        if (uMsg == WM_COMMAND && (HWND)lParam == Disconnect)
        {
            if (connected)
            {
                client->Disconnect();
                SetWindowTextA(ContentWindow, "Disconnected.");
            }
            else
            {
                SetWindowTextA(ContentWindow, "calm down adhd kid, you haven't even connected yet");
            }
        }

        if (uMsg == WM_COMMAND && (HWND)lParam == ListDirectory)
        {
            client->ListEverything();
            if (connected) 
            {
                std::string tmp = client->ReturnDirectory();
                int size = tmp.size();
                std::string content = tmp + "\n\nsize of string was: " + std::to_string(size);
                AppendWindowTextA(ContentWindow, content.c_str());
            }
            else
            {
                SetWindowTextA(ContentWindow, "calm down adhd kid, you haven't even connected yet");
            }
        }

        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);



        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        EndPaint(hwnd, &ps);
    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
