/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
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

#include <QtWin>

#include <QGuiApplication>
#include <QScopedArrayPointer>
#include <QStringList>
#include <QPixmap>
#include <QImage>
#include <QFileInfo>
#include <QDir>
#include <iostream>

/* This example demonstrates the Windows-specific image conversion
 * functions. */

int main(int argc, char *argv[])
{
    QGuiApplication a(argc, argv);

    QStringList arguments = QCoreApplication::arguments();
    arguments.pop_front();
    const bool large = !arguments.isEmpty() && arguments.front() == "-l";
    if (large)
        arguments.pop_front();
    if (arguments.size() < 1) {
        std::cout << "Usage: iconextractor [OPTIONS] FILE [IMAGE_FILE_FOLDER]\n\n"
                     "Extracts Windows icons from executables, DLL or icon files\n"
                     "and writes them out as numbered .png-files.\n\n"
                     "Options: -l    Extract large icons.\n\n"
                     "Based on Qt " << QT_VERSION_STR << "\n";
        return 1;
    }
    const QString sourceFile = arguments.at(0);
    QString imageFileRoot = arguments.size() > 1 ? arguments.at(1) : QDir::currentPath();
    const QFileInfo imageFileRootInfo(imageFileRoot);
    if (!imageFileRootInfo.isDir()) {
        std::cerr << imageFileRoot.toStdString() << " is not a directory.\n";
        return 1;
    }

    const UINT iconCount = ExtractIconEx((wchar_t *)sourceFile.utf16(), -1, 0, 0, 0);
    if (!iconCount) {
        std::cerr << sourceFile.toStdString() << " does not appear to contain icons.\n";
        return 1;
    }

    QScopedArrayPointer<HICON> icons(new HICON[iconCount]);
    const UINT extractedIconCount = large ?
        ExtractIconEx((wchar_t *)sourceFile.utf16(), 0, icons.data(), 0, iconCount) :
        ExtractIconEx((wchar_t *)sourceFile.utf16(), 0, 0, icons.data(), iconCount);
    if (!extractedIconCount) {
        qErrnoWarning("Failed to extract icons from %s", qPrintable(sourceFile));
        return 1;
    }

    std::cout << sourceFile.toStdString() << " contains " << extractedIconCount << " icon(s).\n";

    imageFileRoot = imageFileRootInfo.absoluteFilePath() + QLatin1Char('/') + QFileInfo(sourceFile).baseName();
    for (UINT i = 0; i < extractedIconCount; ++i) {
        const QPixmap pixmap = QtWin::fromHICON(icons[i]);
        if (pixmap.isNull()) {
            std::cerr << "Error converting icons.\n";
            return 1;
        }
        const QString fileName = QString::fromLatin1("%1%2.png").arg(imageFileRoot)
            .arg(i, 3, 10, QLatin1Char('0'));
        if (!pixmap.save(fileName)) {
            std::cerr << "Error writing image file " << fileName.toStdString() << ".\n";
            return 1;
        }
        std::cout << "Wrote image file "
                  << QDir::toNativeSeparators(fileName).toStdString() << ".\n";
    }
    return 0;
}
