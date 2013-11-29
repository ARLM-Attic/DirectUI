#pragma once

#ifdef DUI_EXPORTS
# define DUI_API __declspec(dllexport)
#else
# define DUI_API __declspec(dllimport)
#endif

#ifdef UNICODE
# define WC_DIRECTUI L"DirectUI"
#else
# define WC_DIRECTUI "DirectUI"
#endif

#define DUIN_RECREATE 0x1
#define DUIN_DRAW     0x2

typedef struct _DUIHDR {
    NMHDR hdr;
    ID2D1DCRenderTarget *d2dTarget;
} DUIHDR, *PDUIHDR;

EXTERN_C DUI_API void DUI_Initialize(void);
EXTERN_C DUI_API void DUI_Uninitialize(void);
