#pragma once

#ifdef DUI_EXPORTS
# define DUI_API __declspec(dllexport)
#else
# define DUI_API __declspec(dllimport)
#endif

/**
 * \brief The string that represents the DirectUI window class name.
 */
#ifdef UNICODE
# define WC_DIRECTUI L"DirectUI"
#else
# define WC_DIRECTUI "DirectUI"
#endif

/**
 * \brief Notification sent to the parent window when Direct2D resources need
 *        to be recreated.
 */
#define DUIN_RECREATE 0x1

/**
 * \brief Notification sent to the parent window when drawing should occur.
 */
#define DUIN_DRAW     0x2

typedef struct _DUIHDR {
    NMHDR hdr;
    ID2D1DCRenderTarget *d2dTarget;
} DUIHDR, *PDUIHDR;

/**
 * \brief Registers the DirectUI control and initializes any resources
 *        associated with the library.
 * \sa DUI_Uninitialize()
 */
EXTERN_C DUI_API void DUI_Initialize(void);

/**
 * \brief Unregisters the DirectUI control and cleans up any resources
 *        associated with the library
 * \sa DUI_Initialize()
 */
EXTERN_C DUI_API void DUI_Uninitialize(void);
