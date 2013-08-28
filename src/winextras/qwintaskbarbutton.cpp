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

#include "qwintaskbarbutton.h"
#include "qwintaskbarbutton_p.h"
#include "qwintaskbarprogress.h"
#include "qwinfunctions.h"
#include "qwineventfilter_p.h"
#include "qwinevent.h"
#include "winshobjidl_p.h"

#include <QWindow>
#include <QIcon>
#include <QPair>
#include <dwmapi.h>
#include <shobjidl.h>

QT_BEGIN_NAMESPACE

/*!
    \class QWinTaskbarButton
    \inmodule QtWinExtras
    \brief The QWinTaskbarButton class represents the Windows taskbar button for
    a window.

    \since 5.2

    The QWinTaskbarButton class enables you to set overlay icons on a taskbar
    button, to display a progress indicator, and to add a small toolbar to the
    window thumbnail popup.
 */

static TBPFLAG nativeProgressState(QWinTaskbarProgress::ProgressState state)
{
    TBPFLAG flag;
    switch (state) {
    default :
    case QWinTaskbarProgress::NoProgressState:    flag = TBPF_NOPROGRESS; break;
    case QWinTaskbarProgress::IndeterminateState: flag = TBPF_INDETERMINATE; break;
    case QWinTaskbarProgress::NormalState:        flag = TBPF_NORMAL; break;
    case QWinTaskbarProgress::PausedState:        flag = TBPF_PAUSED; break;
    case QWinTaskbarProgress::ErrorState:         flag = TBPF_ERROR; break;
    }
    return flag;
}

QWinTaskbarButtonPrivate::QWinTaskbarButtonPrivate() : progressBar(0), pTbList(0), window(0)
{
    HRESULT hresult = CoCreateInstance(CLSID_TaskbarList, 0, CLSCTX_INPROC_SERVER, IID_ITaskbarList4, reinterpret_cast<void **>(&pTbList));
    if (FAILED(hresult)) {
        pTbList = 0;
        const QString err = QtWinExtras::errorStringFromHresult(hresult);
        qWarning("QWinTaskbarButton: IID_ITaskbarList4 was not created: %#010x, %s.", (unsigned)hresult, qPrintable(err));
    } else if (FAILED(pTbList->HrInit())) {
        pTbList->Release();
        pTbList = 0;
        const QString err = QtWinExtras::errorStringFromHresult(hresult);
        qWarning("QWinTaskbarButton: IID_ITaskbarList4 was not initialized: %#010x, %s.", (unsigned)hresult, qPrintable(err));
    }
}

QWinTaskbarButtonPrivate::~QWinTaskbarButtonPrivate()
{
    pTbList->Release();
}

HWND QWinTaskbarButtonPrivate::handle()
{
    return reinterpret_cast<HWND>(window->winId());
}

int QWinTaskbarButtonPrivate::iconSize() const
{
    return GetSystemMetrics(SM_CXSMICON);
}

void QWinTaskbarButtonPrivate::updateOverlayIcon()
{
    if (!pTbList || !window)
        return;

    wchar_t *descrPtr = 0;
    HICON hicon = 0;
    if (!overlayIconDescription.isEmpty()) {
        descrPtr = new wchar_t[overlayIconDescription.length() + 1];
        descrPtr[overlayIconDescription.toWCharArray(descrPtr)] = 0;
    }
    if (!overlayIcon.isNull())
        hicon = QtWinExtras::toHICON(overlayIcon.pixmap(iconSize()));

    if (hicon)
        pTbList->SetOverlayIcon(handle(), hicon, descrPtr);
    else if (!hicon && !overlayIcon.isNull())
        pTbList->SetOverlayIcon(handle(), (HICON)LoadImage(0, IDI_APPLICATION, IMAGE_ICON, SM_CXSMICON, SM_CYSMICON, LR_SHARED), descrPtr);

    if (hicon)
        DeleteObject(hicon);
    if (descrPtr)
        delete[] descrPtr;
}

void QWinTaskbarButtonPrivate::_q_updateProgressValue()
{
    const QWinTaskbarProgress::ProgressState state = progressBar ? progressBar->state() : QWinTaskbarProgress::NoProgressState;
    if (!pTbList || !window || state == QWinTaskbarProgress::NoProgressState || state == QWinTaskbarProgress::IndeterminateState)
        return;

    const int min = progressBar->minimum();
    const int max = progressBar->maximum();
    const int range = max - min;
    if (range > 0) {
        const int value = 100.0 * (progressBar->value() - min) / range;
        pTbList->SetProgressValue(handle(), value, 100);

        if (state == QWinTaskbarProgress::IndeterminateState)
            progressBar->setState(QWinTaskbarProgress::NormalState);
    }
}

void QWinTaskbarButtonPrivate::_q_updateProgressState()
{
    if (!pTbList || !window || !progressBar)
        return;

    pTbList->SetProgressState(handle(), nativeProgressState(progressBar->state()));
}

/*!
    Constructs a QWinTaskbarButton with the parent object \a parent.
 */
QWinTaskbarButton::QWinTaskbarButton(QObject *parent) :
    QObject(parent), d_ptr(new QWinTaskbarButtonPrivate)
{
    QWinEventFilter::setup();
    setWindow(qobject_cast<QWindow *>(parent));
}

/*!
    Destroys the QWinTaskbarButton.
 */
QWinTaskbarButton::~QWinTaskbarButton()
{
}

/*!
    \property QWinTaskbarButton::window
    \brief the window whose taskbar button is manipulated
 */
void QWinTaskbarButton::setWindow(QWindow *window)
{
    Q_D(QWinTaskbarButton);
    if (d->window)
        d->window->removeEventFilter(this);
    d->window = window;
    if (d->window) {
        d->window->installEventFilter(this);
        if (d->window->isVisible()) {
            d->_q_updateProgressValue();
            d->_q_updateProgressState();
            d->updateOverlayIcon();
        }
    }
}

QWindow *QWinTaskbarButton::window() const
{
    Q_D(const QWinTaskbarButton);
    return d->window;
}

/*!
    \property QWinTaskbarButton::overlayIcon
    \brief the overlay icon of the button
 */
QIcon QWinTaskbarButton::overlayIcon() const
{
    Q_D(const QWinTaskbarButton);
    return d->overlayIcon;
}

void QWinTaskbarButton::setOverlayIcon(const QIcon &icon)
{
    Q_D(QWinTaskbarButton);

    d->overlayIcon = icon;
    d->updateOverlayIcon();
}

/*!
    Clears the previously set overlay icon.
    \sa QWinTaskbarButton::setOverlayIcon()
 */
void QWinTaskbarButton::clearOverlayIcon()
{
    setOverlayIconAccessibilityDescription(QString());
    setOverlayIcon(QIcon());
}

/*!
    \property QWinTaskbarButton::overlayIconAccessibilityDescription
    \brief the overlay icon accessibility description text
 */
QString QWinTaskbarButton::overlayIconAccessibilityDescription() const
{
    Q_D(const QWinTaskbarButton);
    return d->overlayIconDescription;
}

void QWinTaskbarButton::setOverlayIconAccessibilityDescription(const QString &description)
{
    Q_D(QWinTaskbarButton);

    d->overlayIconDescription = description;
    d->updateOverlayIcon();
}

/*!
    \property QWinTaskbarButton::progressBar
    \brief the progress indicator of the taskbar button
 */
QWinTaskbarProgress *QWinTaskbarButton::progressBar() const
{
    Q_D(const QWinTaskbarButton);
    if (!d->progressBar) {
        QWinTaskbarButton *that = const_cast<QWinTaskbarButton *>(this);
        QWinTaskbarProgress *pbar = new QWinTaskbarProgress(that);
        connect(pbar, SIGNAL(valueChanged(int)), this, SLOT(_q_updateProgressValue()));
        connect(pbar, SIGNAL(stateChanged(QWinTaskbarProgress::ProgressState)), this, SLOT(_q_updateProgressState()));
        that->d_func()->progressBar = pbar;
        that->d_func()->_q_updateProgressValue();
        that->d_func()->_q_updateProgressState();
    }
    return d->progressBar;
}

/*!
    \internal
    Intercepts TaskbarButtonCreated messages.
 */
bool QWinTaskbarButton::eventFilter(QObject *object, QEvent *event)
{
    Q_D(QWinTaskbarButton);
    if (object == d->window && event->type() == QWinEvent::TaskbarButtonCreated) {
        d->_q_updateProgressState();
        d->_q_updateProgressValue();
        d->updateOverlayIcon();
    }
    return false;
}

QT_END_NAMESPACE

#include "moc_qwintaskbarbutton.cpp"
