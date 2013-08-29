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

#ifndef QWINTASKBARPROGRESS_H
#define QWINTASKBARPROGRESS_H

#include <QtCore/qobject.h>
#include <QtCore/qscopedpointer.h>
#include <QtWinExtras/qwinextrasglobal.h>

QT_BEGIN_NAMESPACE

class QWinTaskbarProgressPrivate;

class Q_WINEXTRAS_EXPORT QWinTaskbarProgress : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(int minimum READ minimum WRITE setMinimum NOTIFY minimumChanged)
    Q_PROPERTY(int maximum READ maximum WRITE setMaximum NOTIFY maximumChanged)
    Q_PROPERTY(ProgressState state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibilityChanged)
    Q_PROPERTY(bool paused READ isPaused WRITE setPaused NOTIFY pausedChanged)
    Q_ENUMS(ProgressState)

public:
    enum ProgressState {
        NormalState,
        ErrorState
    };

    explicit QWinTaskbarProgress(QObject *parent = 0);
    ~QWinTaskbarProgress();

    ProgressState state() const;
    int value() const;
    int minimum() const;
    int maximum() const;
    bool isVisible() const;
    bool isPaused() const;

public Q_SLOTS:
    void setState(ProgressState state);
    void setValue(int value);
    void setMinimum(int minimum);
    void setMaximum(int maximum);
    void setRange(int minimum, int maximum);
    void reset();
    void show();
    void hide();
    void setVisible(bool visible);
    void pause();
    void resume();
    void setPaused(bool paused);

Q_SIGNALS:
    void valueChanged(int value);
    void minimumChanged(int minimum);
    void maximumChanged(int maximum);
    void stateChanged(QWinTaskbarProgress::ProgressState state);
    void visibilityChanged(bool visible);
    void pausedChanged(bool paused);

private:
    Q_DISABLE_COPY(QWinTaskbarProgress)
    Q_DECLARE_PRIVATE(QWinTaskbarProgress)
    QScopedPointer<QWinTaskbarProgressPrivate> d_ptr;
};

QT_END_NAMESPACE

#endif // QWINTASKBARPROGRESS_H
