/****************************************************************************
 **
 ** Copyright (C) 2013 Ivan Vizir <define-true-false@yandex.com>
 ** Contact: http://www.qt-project.org/legal
 **
 ** This file is part of the test suite of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:LGPL21$
 ** Commercial License Usage
 ** Licensees holding valid commercial Qt licenses may use this file in
 ** accordance with the commercial license agreement provided with the
 ** Software or, alternatively, in accordance with the terms contained in
 ** a written agreement between you and Digia. For licensing terms and
 ** conditions see http://qt.digia.com/licensing. For further information
 ** use the contact form at http://qt.digia.com/contact-us.
 **
 ** GNU Lesser General Public License Usage
 ** Alternatively, this file may be used under the terms of the GNU Lesser
 ** General Public License version 2.1 or version 3 as published by the Free
 ** Software Foundation and appearing in the file LICENSE.LGPLv21 and
 ** LICENSE.LGPLv3 included in the packaging of this file. Please review the
 ** following information to ensure the GNU Lesser General Public License
 ** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
 ** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 **
 ** In addition, as a special exception, Digia gives you certain additional
 ** rights. These rights are described in the Digia Qt LGPL Exception
 ** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#include "testwidget.h"
#include "ui_testwidget.h"

#include <QtWin>
#include <QWinEvent>
#include <QDebug>
#include <qt_windows.h>

TestWidget::TestWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TestWidget)
{
    ui->setupUi(this);

    connect(ui->btnPeekDisallow,  &QAbstractButton::clicked, this, &TestWidget::onDisallowPeekClicked);
    connect(ui->btnPeekExclude,   &QAbstractButton::clicked, this, &TestWidget::onExcludeFromPeekClicked);
    connect(ui->radioFlipDefault, &QAbstractButton::clicked, this, &TestWidget::onFlip3DPolicyChanged);
    connect(ui->radioFlipAbove,   &QAbstractButton::clicked, this, &TestWidget::onFlip3DPolicyChanged);
    connect(ui->radioFlipBelow,   &QAbstractButton::clicked, this, &TestWidget::onFlip3DPolicyChanged);
    connect(ui->btnFrameReset,    &QAbstractButton::clicked, this, &TestWidget::onResetGlassFrameClicked);
    typedef void(QSpinBox::*IntSignal)(int);
    connect(ui->frameTop, static_cast<IntSignal>(&QSpinBox::valueChanged),
            this, &TestWidget::onGlassMarginsChanged);
    connect(ui->frameRight, static_cast<IntSignal>(&QSpinBox::valueChanged),
            this, &TestWidget::onGlassMarginsChanged);
    connect(ui->frameBottom, static_cast<IntSignal>(&QSpinBox::valueChanged),
            this, &TestWidget::onGlassMarginsChanged);
    connect(ui->frameLeft, static_cast<IntSignal>(&QSpinBox::valueChanged),
            this, &TestWidget::onGlassMarginsChanged);
}

TestWidget::~TestWidget()
{
    delete ui;
}

void TestWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

bool TestWidget::event(QEvent *e)
{
    if (e->type() == QWinEvent::CompositionChange) {
        QWinCompositionChangeEvent *event = static_cast<QWinCompositionChangeEvent *>(e);
        qDebug() << "Composition state change: " << event->isCompositionEnabled();
    } else if (e->type() == QWinEvent::ThemeChange) {
        qDebug() << "Theme change.";
    }
    return QWidget::event(e);
}

void TestWidget::onDisallowPeekClicked()
{
    QtWin::setWindowDisallowPeek(this, ui->btnPeekDisallow->isChecked());
}

void TestWidget::onExcludeFromPeekClicked()
{
    QtWin::setWindowExcludedFromPeek(this, ui->btnPeekExclude->isChecked());
}

void TestWidget::onFlip3DPolicyChanged()
{
    QtWin::WindowFlip3DPolicy policy;
    if (ui->radioFlipAbove->isChecked())
        policy = QtWin::FlipExcludeAbove;
    else if (ui->radioFlipBelow->isChecked())
        policy = QtWin::FlipExcludeBelow;
    else
        policy = QtWin::FlipDefault;
    QtWin::setWindowFlip3DPolicy(this, policy);
}

void TestWidget::onGlassMarginsChanged()
{
    if (!QtWin::isCompositionEnabled())
        return;

    // what you see here is the only way to force widget to redraw itself
    // so it actually redraws itself without caching and without any glitch
    // but causes flickering :(
    // and yes, update() and redraw() do nothing

    if (!testAttribute(Qt::WA_NoSystemBackground)) {

        QSize original = size();
        QSize modified = original;
        modified.setWidth(original.height() + 1);
        resize(modified);

        setAttribute(Qt::WA_NoSystemBackground);
        QtWin::extendFrameIntoClientArea(this,
                                         ui->frameTop->value(),
                                         ui->frameRight->value(),
                                         ui->frameBottom->value(),
                                         ui->frameLeft->value());

        resize(original);

        ui->groupBox_2->setAutoFillBackground(true);
        ui->groupBox_3->setAutoFillBackground(true);
        ui->groupBox_4->setAutoFillBackground(true);
    } else {
        QtWin::extendFrameIntoClientArea(this,
                                         ui->frameLeft->value(),
                                         ui->frameTop->value(),
                                         ui->frameRight->value(),
                                         ui->frameBottom->value());
    }
}

void TestWidget::onResetGlassFrameClicked()
{
    if (!testAttribute(Qt::WA_NoSystemBackground))
        return;

    ui->frameTop->setValue(0);
    ui->frameRight->setValue(0);
    ui->frameBottom->setValue(0);
    ui->frameLeft->setValue(0);

    QtWin::resetExtendedFrame(this);
    setAttribute(Qt::WA_NoSystemBackground, false);

    QSize original = size();
    QSize modified = original;
    modified.setHeight(original.height() + 1);
    resize(modified);

    ui->groupBox_2->setAutoFillBackground(false);
    ui->groupBox_3->setAutoFillBackground(false);
    ui->groupBox_4->setAutoFillBackground(false);

    resize(original);
}
