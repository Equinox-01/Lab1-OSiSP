#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include "main.h"

#define SPRITE_START_POSITION 200
#define STEP 10
#define SPRITE_SIZE 128
#define WINDOW_HEADER_SIZE 26
#define FORCE_STEP 75
#define MASK RGB(255, 0, 255)

HINSTANCE hInst;
HBITMAP hBitmap;

TCHAR szClassName[ ] = _T("Lab1");

int x = SPRITE_START_POSITION;
int y = SPRITE_START_POSITION;

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);
void CheckBorder(HWND hwnd);

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_ACTIVEBORDER;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Лабораторная работа №1 Вересковский С.В. гр. 551006"),       /* Title Text */
               WS_OVERLAPPEDWINDOW | WS_VISIBLE, /* default window */
               CW_USEDEFAULT,       /* Windows decides the position */
               CW_USEDEFAULT,       /* where the window ends up on the screen */
               640,                 /* The programs width */
               480,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    static char direction;

    switch (message)
    {
    case WM_ACTIVATE:
    {
        hBitmap = (HBITMAP)LoadImage(hInst, "C:\\Sprite.bmp", IMAGE_BITMAP, 512, 128, LR_LOADFROMFILE);
        direction = RIGHT_CODE;
    }
    case WM_PAINT:
    {
        hdc = BeginPaint(hwnd, &ps);
        HDC hdcMem = CreateCompatibleDC(hdc);

        HGDIOBJ hbmOld = SelectObject(hdcMem, hBitmap);
        BITMAP bm;
        GetObject(hBitmap, sizeof(bm), &bm);

        switch (direction)
        {
        case UP_CODE:
            TransparentBlt(hdc, x, y, SPRITE_SIZE, SPRITE_SIZE, hdcMem, UP_CODE_DIRECTION,0,SPRITE_SIZE,SPRITE_SIZE, MASK);
            break;
        case DOWN_CODE:
            TransparentBlt(hdc, x, y, SPRITE_SIZE, SPRITE_SIZE, hdcMem, DOWN_CODE_DIRECTION,0,SPRITE_SIZE,SPRITE_SIZE, MASK);
            break;
        case LEFT_CODE:
            TransparentBlt(hdc, x, y, SPRITE_SIZE, SPRITE_SIZE, hdcMem, LEFT_CODE_DIRECTION,0,SPRITE_SIZE,SPRITE_SIZE, MASK);
            break;
        case RIGHT_CODE:
            TransparentBlt(hdc, x, y, SPRITE_SIZE, SPRITE_SIZE, hdcMem,  RIGHT_CODE_DIRECTION,0,SPRITE_SIZE,SPRITE_SIZE, MASK);
            break;
        }
        SelectObject(hdcMem, hbmOld);
        DeleteDC(hdcMem);
        EndPaint(hwnd, &ps);
    }
    break;

    case WM_MOUSEWHEEL:
    {
        switch((int)wParam)
        {
        case UP_CODE_WPARAM:
        {
            y-=STEP;
            direction = UP_CODE;
        }
        break;
        case DOWN_CODE_WPARAM:
        {
            y+=STEP;
            direction = DOWN_CODE;
        }
        break;
        case LEFT_CODE_WPARAM:
        {
            x-=STEP;
            direction = LEFT_CODE;
        }
        break;
        case RIGHT_CODE_WPARAM:
        {
            x+=STEP;
            direction = RIGHT_CODE;
        }
        break;
        }
        CheckBorder(hwnd);
        InvalidateRect(hwnd,NULL,TRUE);
        UpdateWindow(hwnd);
        return 0;
    }
    case WM_DESTROY:
        DeleteDC(hdc);
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return (LRESULT) NULL;
}

inline void CheckBorder(HWND hwnd)
{
    RECT rect;
    GetWindowRect(hwnd, &rect);
    if (x < 0)
        x+=FORCE_STEP;
    if (y < 0)
        y+=FORCE_STEP;

    if (x > rect.right - rect.left - SPRITE_SIZE)
        x-=FORCE_STEP;
    if (y > rect.bottom - rect.top - WINDOW_HEADER_SIZE - SPRITE_SIZE)
        y-=FORCE_STEP;
}
