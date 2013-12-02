/**
 * @file dui.h
 * @brief The main DirectUI library header.
 */

#ifndef __DUI_H__
#define __DUI_H__

#ifdef DUI_EXPORTS
# define DUI_API __declspec(dllexport)
#else
# define DUI_API __declspec(dllimport)
#endif

/**
 * The DirectUI window class. Use this value for any calls to CreateWindow and
 * friends.
 */
#ifdef UNICODE
# define WC_DIRECTUI L"DirectUI"
#else
# define WC_DIRECTUI "DirectUI"
#endif

/**
 * Notification to recreate Direct2D resources. This notification is sent to
 * the parent window when Direct2D resources have become dirty for any reason
 * the API deems necessary. When received, recreate any device-dependent
 * resources needed by your program.
 */
#define DUIN_RECREATE 0x1

/**
 * Notification to perform any drawing. This notification is sent to the parent
 * window whenever the DirectUI is asked to paint itself.
 */
#define DUIN_DRAW     0x2

/**
 * Custom NMHDR sent by DirectUI. All notification messages sent from a
 * DirectUI control to its parent window send this header as the lParam value.
 */
typedef struct _DUIHDR DUIHDR, *PDUIHDR;

/**
 * Initializes the DirectUI library. Registers the DirectUI control and
 * allocates any resources associated with the library.
 * @return An HRESULT containing S_OK if successful, or an error code if not
 * @see DUI_Uninitialize()
 */
EXTERN_C DUI_API HRESULT DUI_Initialize(void);

/**
 * Tear down the DirectUI library. Unregisters the DirectUI control and cleans
 * up any resources associated with the library.
 * @return An HRESULT containing S_OK if successful, or an error code if not
 * @see DUI_Initialize()
 */
EXTERN_C DUI_API HRESULT DUI_Uninitialize(void);

/**
 * Get the ID2D1RenderTarget associated with the DirectUI control.
 * @param phdrSelf [in] A pointer to a DUIHDR object.
 * @param ppRenderTarget [out] An address of a pointer to the Direct2D render target.
 * @return An HRESULT containing S_OK if successful, or an error code if not
 */
EXTERN_C DUI_API HRESULT DUI_GetRenderTarget(PDUIHDR phdrSelf, ID2D1RenderTarget **ppRenderTarget);

#endif
