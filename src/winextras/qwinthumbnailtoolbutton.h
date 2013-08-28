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

#ifndef QWINTHUMBNAILTOOLBUTTON_H
#define QWINTHUMBNAILTOOLBUTTON_H

#include <QtGui/qicon.h>
#include <QtCore/qobject.h>
#include <QtCore/qscopedpointer.h>
#include <QtWinExtras/qwinextrasglobal.h>

QT_BEGIN_NAMESPACE

class QWinThumbnailToolButtonPrivate;

class Q_WINEXTRAS_EXPORT QWinThumbnailToolButton : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip)
    Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
    Q_PROPERTY(bool interactive READ isInteractive WRITE setInteractive)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible)
    Q_PROPERTY(bool dismissOnClick READ dismissOnClick WRITE setDismissOnClick)
    Q_PROPERTY(bool flat READ isFlat WRITE setFlat)

public:
    explicit QWinThumbnailToolButton(QObject *parent = 0);
    ~QWinThumbnailToolButton();

    void setToolTip(const QString &toolTip);
    QString toolTip() const;
    void setIcon(const QIcon &icon);
    QIcon icon() const;
    void setEnabled(bool enabled);
    bool isEnabled() const;
    void setInteractive(bool interactive);
    bool isInteractive() const;
    void setVisible(bool visible);
    bool isVisible() const;
    void setDismissOnClick(bool dismiss);
    bool dismissOnClick() const;
    void setFlat(bool flat);
    bool isFlat() const;

public slots:
    void click();

Q_SIGNALS:
    void clicked();
    void changed();

private:
    Q_DISABLE_COPY(QWinThumbnailToolButton)
    Q_DECLARE_PRIVATE(QWinThumbnailToolButton)
    QScopedPointer<QWinThumbnailToolButtonPrivate> d_ptr;
    friend class QWinThumbnailToolBar;
};

QT_END_NAMESPACE

#endif // QWINTHUMBNAILTOOLBUTTON_H