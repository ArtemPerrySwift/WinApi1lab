// WinApi1lab.cpp : Определяет точку входа для приложения.
//
#define _USE_MATH_DEFINES
#include "framework.h"
#include "WinApi1lab.h"
#include <map>
#include <string>
#include <cmath>



#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

struct hwndInfo
{
    bool color;
    POINT pos;
    UINT_PTR timer;
};

std::map<HWND, hwndInfo> mapHwnd;
//std::map<HWND, POINT> mapHwndPos;
const int windCount = 2;
bool showPosChoice = false;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.
    // kod

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_WINAPI1LAB, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    for(int i = 0; i < windCount; i++)
        if (!InitInstance (hInstance, nCmdShow))
            return FALSE;
        


    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WINAPI1LAB));

    MSG msg;
    std::map <HWND, hwndInfo> ::iterator it;
    for (it = mapHwnd.begin(); it != mapHwnd.end(); it++)
        it->second.timer = SetTimer(it->first, 0, 500, NULL);
    
    
    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPI1LAB));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_WINAPI1LAB);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   mapHwnd[hWnd].color = false;
   POINT p = { 0, 0 };
   mapHwnd[hWnd].pos = p;
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

int xCloclArrow(int r, double phi)
{
    return r * sin(phi);
}

int yCloclArrow(int r, double phi)
{
    return -r * cos(phi);
}

int writePos(HWND hWnd, HDC hdc, LONG w, LONG h)
{
    HPEN hpen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
    SelectObject(hdc, hpen);
    POINT point = mapHwnd[hWnd].pos;

    std::wstring strPos = L"X = ";
    strPos += std::to_wstring(point.x);
    strPos += L"\nY = ";
    strPos += std::to_wstring(point.y);
    //strPos += std::to_wstring(clockTime.wMinute);
    RECT textRect;
    SetRect(&textRect, 0, h - 30, w, h);
    DrawText(hdc, strPos.c_str(), strPos.size(), &textRect, 0);
    //TextOut(hdc, 0, h / 2, strPos.c_str(), strPos.size());
    DeleteObject(hpen);

    return 0;
}

int paintCross(HWND hWnd, HDC hdc, LONG w, LONG h, COLORREF color)
{
    HPEN hpen = CreatePen(PS_SOLID, 5, color);
    SelectObject(hdc, hpen);
    MoveToEx(hdc, 0, 0, 0);
    LineTo(hdc, w, h);

    MoveToEx(hdc, w, 0, 0);
    LineTo(hdc, 0, h);
    DeleteObject(hpen);

    return 0;
}

int paintClock(HWND hWnd, HDC hdc, LONG w, LONG h, COLORREF color)
{
    // TODO: Добавьте сюда любой код прорисовки, использующий HDC...

    SYSTEMTIME clockTime;
    GetSystemTime(&clockTime);

    //colorType++;

    

    HPEN hpen = CreatePen(PS_SOLID, 5, color);
    SelectObject(hdc, hpen);
    //MoveToEx(hdc, 0, 0, 0);
    //LineTo(hdc, w, h);

    //MoveToEx(hdc, w, 0, 0);
    //LineTo(hdc, 0, h);

    //MoveToEx(hdc, w/2, h/2, 0);
    LONG r;
    LONG xClockCenter = w / 2, yClockCenter = h / 2;
    if (h > w)
    {
        Ellipse(hdc, 0, (h - w) / 2, w, (h + w) / 2);
        r = w / 2;
    }
    else
    {
        Ellipse(hdc, (w - h) / 2, 0, (w + h) / 2, h);
        r = h / 2;
    }
    DeleteObject(hpen);
    
    /*Отрисовываем секундную стрелку*/
    hpen = CreatePen(PS_SOLID, 2, color);
    SelectObject(hdc, hpen);
    MoveToEx(hdc, xClockCenter, yClockCenter, 0);
    double phiSec = clockTime.wSecond / 30.0 * M_PI;
    LineTo(hdc, xClockCenter + xCloclArrow(r * 19/20, phiSec), yClockCenter + yCloclArrow(r * 19 / 20, phiSec));
    DeleteObject(hpen);

    /*Отрисовываем минутную стрелку*/
    hpen = CreatePen(PS_SOLID, 5, color);
    SelectObject(hdc, hpen);
    MoveToEx(hdc, xClockCenter, yClockCenter, 0);
    double phiMin = clockTime.wMinute / 30.0 * M_PI;
    LineTo(hdc, xClockCenter + xCloclArrow(r * 4 / 5, phiMin), yClockCenter + yCloclArrow(r * 9 / 10, phiMin));
    DeleteObject(hpen);

    hpen = CreatePen(PS_SOLID, 10, color);
    SelectObject(hdc, hpen);
    /*Отрисовываем стрелку, отвечающую за час дня*/
    MoveToEx(hdc, xClockCenter, yClockCenter, 0);
    double phiHour = (clockTime.wHour + 7) / 6.0 * M_PI;
    LineTo(hdc, xClockCenter + xCloclArrow(r * 3 / 5, phiHour), yClockCenter + yCloclArrow(r * 3 / 5, phiHour));
    //Ellipse(hdc, 0, 0, w, h);
    DeleteObject(hpen);

    return 0;
}

int paint(HWND hWnd)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    // TODO: Добавьте сюда любой код прорисовки, использующий HDC...

    RECT rect;
    GetClientRect(hWnd, &rect);

   

    SYSTEMTIME clockTime;
    GetSystemTime(&clockTime);
    
    LONG w = rect.right - rect.left;
    LONG h = rect.bottom - rect.top;
    //colorType++;
    COLORREF color = (mapHwnd[hWnd].color) ? RGB(0, 0, 0) : RGB(255, 0, 0);

    
    paintClock(hWnd, hdc, w, h, color);
    //paintCross(hWnd, hdc, w, h, color);
    writePos(hWnd, hdc, w, h);
    /*
    POINT point = mapHwndPos[hWnd];
    //GetCursorPos(&point);

        //char* strPosX;
        //char* strPosY;
        //itoa(pPoint->x, strPosX, 5);
        //itoa(pPoint->y, strPosY, 5);

        //char strPos[40];

    std::wstring strPos = L"X = ";
    strPos += std::to_wstring(point.x);
    strPos += L", Y = ";
    strPos += std::to_wstring(point.y);
        //strPos += std::to_wstring(clockTime.wMinute);
    TextOut(hdc, 0, h / 2, strPos.c_str(), strPos.size());
 
    
    HPEN hpen = CreatePen(PS_SOLID, 1, color);
    SelectObject(hdc, hpen);
    //MoveToEx(hdc, 0, 0, 0);
    //LineTo(hdc, w, h);

    //MoveToEx(hdc, w, 0, 0);
    //LineTo(hdc, 0, h);

    //MoveToEx(hdc, w/2, h/2, 0);
    LONG r;
    LONG xClockCenter = w / 2, yClockCenter = h / 2;
    if (h > w)
    {
        Ellipse(hdc, 0, (h - w) / 2, w, (h + w) / 2);
        r = w/2;
    }
    else
    {
        Ellipse(hdc, (w - h) / 2, 0, (w + h) / 2, h);
        r = h/2; 
    }

    MoveToEx(hdc, xClockCenter, yClockCenter, 0);
    double phiMin = clockTime.wMinute / 30.0 * M_PI;
    LineTo(hdc, xClockCenter + xCloclArrow(r * 9 / 10, phiMin), yClockCenter + yCloclArrow(r * 9 / 10, phiMin));

    MoveToEx(hdc, xClockCenter, yClockCenter, 0);
    double phiSec = clockTime.wSecond / 30.0 * M_PI;
    LineTo(hdc, xClockCenter + xCloclArrow(r, phiSec), yClockCenter + yCloclArrow(r, phiSec));

    MoveToEx(hdc, xClockCenter, yClockCenter, 0);
    double phiHour = (clockTime.wHour + 7) / 6.0 * M_PI;
    LineTo(hdc, xClockCenter + xCloclArrow(r * 4 / 5, phiHour), yClockCenter + yCloclArrow(r * 4 / 5, phiHour));
    //Ellipse(hdc, 0, 0, w, h);


    DeleteObject(hpen);

    */

    EndPaint(hWnd, &ps);

    return 0;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    RECT rect;    

    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDM_POS:
            {
                showPosChoice = !showPosChoice;
                std::map <HWND, hwndInfo> ::iterator it = mapHwnd.begin();
                for (it = mapHwnd.begin(); it != mapHwnd.end(); it++)
                {
                    HMENU hmenu = GetMenu(it->first);

                    int state = GetMenuState(hmenu, wmId, MF_BYCOMMAND);

                    state == MF_CHECKED ? CheckMenuItem(hmenu, wmId, MF_UNCHECKED) : CheckMenuItem(hmenu, wmId, MF_CHECKED);
                    //if (state == MF_CHECKED) 
                    //    CheckMenuItem(hmenu, wmId, MF_UNCHECKED);
                  
                    //else 
                    //    CheckMenuItem(hmenu, wmId, MF_CHECKED);
                    

                    GetClientRect(it->first, &rect);
                    InvalidateRect(it->first, &rect, 1);
                }
            }               
                break;

            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

    case  WM_MOUSEMOVE:
        {
        POINT curPos;
        GetCursorPos(&curPos);
            if (showPosChoice)
            {
                std::map <HWND, hwndInfo> ::iterator it;
                for (it = mapHwnd.begin(); it != mapHwnd.end(); it++)
                {
                    mapHwnd[it->first].pos = curPos;
                    GetClientRect(it->first, &rect);
                    InvalidateRect(it->first, &rect, 1);
                }                    
            }
            else
            {
                mapHwnd[hWnd].pos = curPos;
                GetClientRect(hWnd, &rect);
                InvalidateRect(hWnd, &rect, 1);
            }                
        }
        break;

    case WM_LBUTTONDOWN:
        {
            mapHwnd[hWnd].color = !mapHwnd[hWnd].color;
            GetClientRect(hWnd, &rect);

            int a = InvalidateRect(hWnd, &rect, 1);
        }    
        break;

    case WM_PAINT:
        {
            paint(hWnd);
        }
        break;   

    case WM_TIMER:
        {
            GetClientRect(hWnd, &rect);
            InvalidateRect(hWnd, &rect, 1);
        }
        break;
    case WM_DESTROY:
    {
        std::map <HWND, hwndInfo> ::iterator it;
        for (it = mapHwnd.begin(); it != mapHwnd.end(); it++)
            KillTimer(it->first, it->second.timer);

        PostQuitMessage(0);
    }       
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
