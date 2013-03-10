/****************************************************************************
 **
 ** Copyright (C) 2013 Ivan Vizir <define-true-false@yandex.com>
 ** Contact: http://www.qt-project.org/legal
 **
 ** This file is part of QtWinExtras in the Qt Toolkit.
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

#include "qwincompositionstatechangeevent.h"
#include "qwincompositionstatechangeevent_p.h"

QT_BEGIN_NAMESPACE

QWinCompositionStateChangeEvent::QWinCompositionStateChangeEvent(bool enabled) :
    QEvent(QWinCompositionStateChangeEventPrivate::eventType()), d_ptr(new QWinCompositionStateChangeEventPrivate)
{
    Q_D(QWinCompositionStateChangeEvent);
    d->enabled = enabled;
}

QWinCompositionStateChangeEvent::~QWinCompositionStateChangeEvent()
{
}

bool QWinCompositionStateChangeEvent::isCompositionEnabled() const
{
    Q_D(const QWinCompositionStateChangeEvent);
    return d->enabled;
}

QEvent::Type QWinCompositionStateChangeEvent::eventType()
{
    return QWinCompositionStateChangeEventPrivate::eventType();
}



int QWinCompositionStateChangeEventPrivate::eventTypeId = 0;

QEvent::Type QWinCompositionStateChangeEventPrivate::eventType()
{
    if (!eventTypeId)
        eventTypeId = QEvent::registerEventType();
    return static_cast<QEvent::Type>(eventTypeId);
}

QT_END_NAMESPACE
