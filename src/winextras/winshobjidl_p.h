/****************************************************************************
 **
 ** Copyright (C) 2013 Ivan Vizir <define-true-false@yandex.com>
 ** Contact: http://www.qt-project.org/legal
 **
 ** This file is part of the QtWinExtras module of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:LGPL$
 ** Commercial License Usage
 ** Licensees holding valid commercial Qt licenses may use this file in
 ** accordance with the commercial license agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and Digia.  For licensing terms and
 ** conditions see http://qt.digia.com/licensing.  For further information
 ** use the contact form at http://qt.digia.com/contact-us.
 **
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.LGPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU Lesser General Public License version 2.1 requirements
 ** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Digia gives you certain additional
 ** rights.  These rights are described in the Digia Qt LGPL Exception
 ** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
 **
 ** GNU General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU
 ** General Public License version 3.0 as published by the Free Software
 ** Foundation and appearing in the file LICENSE.GPL included in the
 ** packaging of this file.  Please review the following information to
 ** ensure the GNU General Public License version 3.0 requirements will be
 ** met: http://www.gnu.org/copyleft/gpl.html.
 **
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#ifndef WINSHOBJIDL_P_H
#define WINSHOBJIDL_P_H

#include <shobjidl.h>

#ifndef __ITaskbarList_INTERFACE_DEFINED__
#define __ITaskbarList_INTERFACE_DEFINED__

extern const GUID IID_ITaskbarList;

struct ITaskbarList : IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE HrInit() = 0;
    virtual HRESULT STDMETHODCALLTYPE AddTab(HWND hwnd) = 0;
    virtual HRESULT STDMETHODCALLTYPE DeleteTab(HWND hwnd) = 0;
    virtual HRESULT STDMETHODCALLTYPE ActivateTab(HWND hwnd) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetActiveAlt(HWND hwnd) = 0;
};

#endif

#ifndef __ITaskbarList2_INTERFACE_DEFINED__
#define __ITaskbarList2_INTERFACE_DEFINED__

extern const GUID IID_ITaskbarList2;

struct ITaskbarList2 : ITaskbarList
{
    virtual HRESULT STDMETHODCALLTYPE MarkFullscreenWindow(HWND hwnd, BOOL fullscreen) = 0;
};

#endif

#ifndef __ITaskbarList3_INTERFACE_DEFINED__
#define __ITaskbarList3_INTERFACE_DEFINED__

extern const GUID IID_ITaskbarList3;

enum THUMBBUTTONMASK {
    THB_BITMAP    = 0x00000001,
    THB_ICON      = 0x00000002,
    THB_TOOLTIP   = 0x00000004,
    THB_FLAGS     = 0x00000008
};

enum THUMBBUTTONFLAGS {
    THBF_ENABLED          = 0x00000000,
    THBF_DISABLED         = 0x00000001,
    THBF_DISMISSONCLICK   = 0x00000002,
    THBF_NOBACKGROUND     = 0x00000004,
    THBF_HIDDEN           = 0x00000008,
    THBF_NONINTERACTIVE   = 0x00000010
};

enum TBPFLAG {
    TBPF_NOPROGRESS      = 0x00000000,
    TBPF_INDETERMINATE   = 0x00000001,
    TBPF_NORMAL          = 0x00000002,
    TBPF_ERROR           = 0x00000004,
    TBPF_PAUSED          = 0x00000008
};

enum STPFLAG {
    STPF_NONE                        = 0x00000000,
    STPF_USEAPPTHUMBNAILALWAYS       = 0x00000001,
    STPF_USEAPPTHUMBNAILWHENACTIVE   = 0x00000002,
    STPF_USEAPPPEEKALWAYS            = 0x00000004,
    STPF_USEAPPPEEKWHENACTIVE        = 0x00000008
};

struct THUMBBUTTON
{
    THUMBBUTTONMASK  dwMask;
    UINT             iId;
    UINT             iBitmap;
    HICON            hIcon;
    WCHAR            szTip[260];
    THUMBBUTTONFLAGS dwFlags;
};

struct ITaskbarList3 : ITaskbarList2
{
    virtual HRESULT STDMETHODCALLTYPE SetProgressValue(HWND hwnd, ULONGLONG completed, ULONGLONG total) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetProgressState(HWND hwnd, TBPFLAG tbpFlags) = 0;
    virtual HRESULT STDMETHODCALLTYPE RegisterTab(HWND tab, HWND window) = 0;
    virtual HRESULT STDMETHODCALLTYPE UnregisterTab(HWND tab) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetTabOrder(HWND tab, HWND insertBefore) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetTabActive(HWND tab, HWND window, DWORD reserved = 0) = 0;
    virtual HRESULT STDMETHODCALLTYPE ThumbBarAddButtons(HWND hwnd, UINT count, THUMBBUTTON buttons) = 0;
    virtual HRESULT STDMETHODCALLTYPE ThumbBarUpdateButtons(HWND hwnd, UINT count, THUMBBUTTON *buttons) = 0;
    virtual HRESULT STDMETHODCALLTYPE ThumbBarSetImageList(HWND hwnd, HIMAGELIST imglist) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetOverlayIcon(HWND hwnd, HICON icon, LPCWSTR description) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetThumbnailTooltip(HWND hwnd, LPCWSTR tooltip) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetThumbnailClip(HWND hwnd, RECT *clip) = 0;
};

#endif

#ifndef __ITaskbarList4_INTERFACE_DEFINED__
#define __ITaskbarList4_INTERFACE_DEFINED__

extern const GUID IID_ITaskbarList4;

struct ITaskbarList4 : ITaskbarList3
{
    virtual HRESULT STDMETHODCALLTYPE SetTabProperties(HWND tab, STPFLAG flags) = 0;
};

#endif

#ifndef __IObjectArray_INTERFACE_DEFINED__
#define __IObjectArray_INTERFACE_DEFINED__

extern const GUID CLSID_EnumerableObjectCollection;
extern const GUID IID_IObjectArray;

struct IObjectArray : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetCount(UINT *count) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetAt(UINT index, REFIID iid, void **ppv) = 0;
};

#endif

#ifndef __IObjectCollection_INTERFACE_DEFINED__
#define __IObjectCollection_INTERFACE_DEFINED__

extern const GUID IID_IObjectCollection;

struct IObjectCollection : IObjectArray
{
public:
    virtual HRESULT STDMETHODCALLTYPE AddObject(IUnknown *punk) = 0;
    virtual HRESULT STDMETHODCALLTYPE AddFromArray(IObjectArray *poaSource) = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveObjectAt(UINT uiIndex) = 0;
    virtual HRESULT STDMETHODCALLTYPE Clear() = 0;
};

#endif

#ifndef __ICustomDestinationList_INTERFACE_DEFINED__
#define __ICustomDestinationList_INTERFACE_DEFINED__

enum KNOWNDESTCATEGORY {
    KDC_FREQUENT = 1,
    KDC_RECENT
};

extern const GUID IID_ICustomDestinationList;
extern const GUID CLSID_DestinationList;

struct ICustomDestinationList : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetAppID(LPCWSTR pszAppID) = 0;
    virtual HRESULT STDMETHODCALLTYPE BeginList(UINT *pcMinSlots, REFIID riid, void **ppv) = 0;
    virtual HRESULT STDMETHODCALLTYPE AppendCategory(LPCWSTR pszCategory, IObjectArray *poa) = 0;
    virtual HRESULT STDMETHODCALLTYPE AppendKnownCategory(KNOWNDESTCATEGORY category) = 0;
    virtual HRESULT STDMETHODCALLTYPE AddUserTasks(IObjectArray *poa) = 0;
    virtual HRESULT STDMETHODCALLTYPE CommitList() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetRemovedDestinations(REFIID riid, void **ppv) = 0;
    virtual HRESULT STDMETHODCALLTYPE DeleteList(LPCWSTR pszAppID) = 0;
    virtual HRESULT STDMETHODCALLTYPE AbortList() = 0;
};

#endif

#ifndef __IApplicationDocumentLists_INTERFACE_DEFINED__
#define __IApplicationDocumentLists_INTERFACE_DEFINED__

enum APPDOCLISTTYPE {
    ADLT_RECENT = 0,
    ADLT_FREQUENT
};

extern const GUID IID_IApplicationDocumentLists;
extern const GUID CLSID_ApplicationDocumentLists;

struct IApplicationDocumentLists : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetAppID(LPCWSTR pszAppID) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetList(APPDOCLISTTYPE listtype, UINT cItemsDesired, REFIID riid, void **ppv) = 0;
};
#endif

#ifndef __IApplicationDestinations_INTERFACE_DEFINED__
#define __IApplicationDestinations_INTERFACE_DEFINED__

extern const GUID IID_IApplicationDestinations;
extern const GUID CLSID_ApplicationDestinations;

struct IApplicationDestinations : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetAppID(LPCWSTR pszAppID) = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveDestination(IUnknown *punk) = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveAllDestinations( void) = 0;
};

#endif

#if (defined _MSC_VER && _MSC_VER < 1600) || defined(Q_CC_MINGW)

#   if !defined(__MINGW64_VERSION_MAJOR) || !defined(__MINGW64_VERSION_MINOR) || __MINGW64_VERSION_MAJOR * 100 + __MINGW64_VERSION_MINOR < 301

typedef struct SHARDAPPIDINFOLINK
{
    IShellLink *psl;        // An IShellLink instance that when launched opens a recently used item in the specified
                            // application. This link is not added to the recent docs folder, but will be added to the
                            // specified application's destination list.
    PCWSTR pszAppID;        // The id of the application that should be associated with this recent doc.
} SHARDAPPIDINFOLINK;

#   endif // !defined(__MINGW64_VERSION_MAJOR) || !defined(__MINGW64_VERSION_MINOR) || __MINGW64_VERSION_MAJOR * 100 + __MINGW64_VERSION_MINOR < 301

#endif

#endif // WINSHOBJIDL_P_H
