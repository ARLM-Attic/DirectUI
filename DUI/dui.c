#include "stdafx.h"
#include "resource.h"
#include "dui.h"

#define RECTWIDTH(rc)  ((rc).right - (rc).left)
#define RECTHEIGHT(rc) ((rc).bottom - (rc).top)

typedef struct _DATA {
    ID2D1Factory *d2dFactory;
    ID2D1DCRenderTarget *d2dTarget;
} DATA, *PDATA;

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static HRESULT CreateIndependent(PDATA);
static HRESULT CreateDependent(PDATA);
static VOID DestroyIndependent(PDATA);
static VOID DestroyDependent(PDATA);
static VOID PrimeNotification(HWND, UINT, LPNMHDR);
static LRESULT SendRecreate(HWND);
static LRESULT SendDraw(HWND);

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hInstDLL);
        break;
    }

    return TRUE;
}

void DUI_Initialize(void) {
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS;
    wc.lpfnWndProc = &WndProc;
    wc.cbWndExtra = sizeof(PDATA);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = WC_DIRECTUI;

    RegisterClassEx(&wc);
}

void DUI_Uninitialize(void) {
    UnregisterClass(WC_DIRECTUI, NULL);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    LRESULT lRet;
    PDATA pd;
    RECT rc;
    PAINTSTRUCT ps;
    HDC hDC;
    HRESULT hr;

    lRet = 0;
    pd = (PDATA)GetWindowLongPtr(hWnd, 0);

    switch (msg) {
    case WM_NCCREATE:
        pd = (PDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(DATA));
        SetWindowLongPtr(hWnd, 0, (LONG_PTR)pd);
        lRet = DefWindowProc(hWnd, msg, wParam, lParam);
        break;

    case WM_CREATE:
        CreateIndependent(pd);
        break;

    case WM_PAINT:
        GetClientRect(hWnd, &rc);
        if (!pd->d2dTarget) {
            CreateDependent(pd);
            SendRecreate(hWnd);
        }
        hDC = BeginPaint(hWnd, &ps);
        ID2D1DCRenderTarget_BindDC(pd->d2dTarget, hDC, &rc);
        ID2D1DCRenderTarget_BeginDraw(pd->d2dTarget);
        SendDraw(hWnd);
        hr = ID2D1DCRenderTarget_EndDraw(pd->d2dTarget, NULL, NULL);
        EndPaint(hWnd, &ps);
        if (hr == D2DERR_RECREATE_TARGET)
            DestroyDependent(pd);
        break;

    case WM_NCDESTROY:
        SetWindowLongPtr(hWnd, 0, (LONG_PTR)NULL);
        DestroyDependent(pd);
        DestroyIndependent(pd);
        HeapFree(GetProcessHeap(), 0, pd);
        break;

    default:
        lRet = DefWindowProc(hWnd, msg, wParam, lParam);
    }

    return lRet;
}

HRESULT CreateIndependent(PDATA pd) {
    return D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &IID_ID2D1Factory, NULL, (LPVOID *)&pd->d2dFactory);
}

HRESULT CreateDependent(PDATA pd) {
    D2D1_PIXEL_FORMAT pf;
    D2D1_RENDER_TARGET_PROPERTIES rtp;

    pf.format = DXGI_FORMAT_B8G8R8A8_UNORM;
    pf.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;

    ZeroMemory(&rtp, sizeof(D2D1_RENDER_TARGET_PROPERTIES));
    rtp.pixelFormat = pf;
    rtp.dpiX = 96.0f;
    rtp.dpiY = 96.0f;

    return ID2D1Factory_CreateDCRenderTarget(pd->d2dFactory, &rtp, &pd->d2dTarget);
}

VOID DestroyIndependent(PDATA pd) {
    if (pd->d2dFactory) {
        ID2D1Factory_Release(pd->d2dFactory);
        pd->d2dFactory = NULL;
    }
}

VOID DestroyDependent(PDATA pd) {
    if (pd->d2dTarget) {
        ID2D1DCRenderTarget_Release(pd->d2dTarget);
        pd->d2dTarget = NULL;
    }
}

VOID PrimeNotification(HWND hWnd, UINT msg, LPNMHDR hdr) {
    hdr->hwndFrom = hWnd;
    hdr->idFrom = (UINT_PTR)GetWindowLongPtr(hWnd, GWLP_ID);
    hdr->code = msg;
}

LRESULT SendRecreate(HWND hWnd) {
    DUIHDR hdr;

    PrimeNotification(hWnd, DUIN_RECREATE, (LPNMHDR)&hdr);
    hdr.d2dTarget = ((PDATA)GetWindowLongPtr(hWnd, 0))->d2dTarget;

    return SendMessage(GetParent(hWnd), WM_NOTIFY, hdr.hdr.idFrom, (LPARAM)&hdr);
}

LRESULT SendDraw(HWND hWnd) {
    DUIHDR hdr;

    PrimeNotification(hWnd, DUIN_DRAW, (LPNMHDR)&hdr);
    hdr.d2dTarget = ((PDATA)GetWindowLongPtr(hWnd, 0))->d2dTarget;

    return SendMessage(GetParent(hWnd), WM_NOTIFY, hdr.hdr.idFrom, (LPARAM)&hdr);
}
