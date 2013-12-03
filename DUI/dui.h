/**
 * @file dui.h
 *
 * @brief The main DirectUI library header.
 *
 * @author Chris Lamberson
 */

#ifndef __DUI_H__
#define __DUI_H__

#ifdef DUI_EXPORTS
# define DUI_API __declspec(dllexport)
#else
# define DUI_API __declspec(dllimport)
#endif

/**
 * @brief The DirectUI window class.
 *
 * @details Use this value for any calls to CreateWindow and friends.
 */
#ifdef UNICODE
# define WC_DIRECTUI L"DirectUI"
#else
# define WC_DIRECTUI "DirectUI"
#endif

/**
 * @brief Notification to recreate Direct2D resources.
 *
 * @details This notification is sent to the parent window when Direct2D
 *          resources have become dirty for any reason the API deems necessary.
 *          When received, recreate any device-dependent resources needed by
 *          your program.
 */
#define DUIN_RECREATE 0x1

/**
 * @brief Notification to perform any drawing.
 *
 * @details This notification is sent to the parent window whenever the
 *          DirectUI is asked to paint itself.
 */
#define DUIN_DRAW     0x2

/**
 * @brief Custom \c NMHDR sent by DirectUI.
 *
 * @details All notification messages sent from a DirectUI control to its
 *          parent window send this header as the \c lParam value.
 */
typedef struct _DUIHDR DUIHDR, *PDUIHDR;

/**
 * @brief Initializes the DirectUI library.
 *
 * @details Registers the DirectUI control and allocates any resources
 *          associated with the library.
 *
 * @return An \c HRESULT, containing \c S_OK if successful or an error code if
 *         not.
 *
 * @see DUI_Uninitialize()
 */
EXTERN_C DUI_API HRESULT DUI_Initialize(void);

/**
 * @brief Tear down the DirectUI library.
 *
 * @details Unregisters the DirectUI control and cleans up any resources
 *          associated with the library.
 *
 * @return An \c HRESULT, containing \c S_OK if successful or an error code if
 *         not.
 *
 * @see DUI_Initialize()
 */
EXTERN_C DUI_API HRESULT DUI_Uninitialize(void);

/**
 * @brief Get the \c ID2D1RenderTarget associated with the DirectUI control.
 *
 * @details This function uses the \c DUIHDR sent by DirectUI notifications to
 *          retrieve a pointer to an \c ID2D1RenderTarget interface which allows
 *          for drawing into the control.
 *
 * @param[in]  phdrDUI  A pointer to a \c DUIHDR object.
 * @param[out] ppRT     An address of a pointer to the Direct2D render target.
 *
 * @return An \c HRESULT, containing \c S_OK if successful or an error code if
 *         not.
 *
 * @warning Do not hold onto this pointer for longer than the lifetime of the
 *          notification. At any point, the device-dependent resources can be
 *          recreated, rendering any leftover pointers invalid.
 */
EXTERN_C DUI_API HRESULT DUI_GetRenderTarget(PDUIHDR phdrDUI,
                                             ID2D1RenderTarget **ppRT);

#endif
