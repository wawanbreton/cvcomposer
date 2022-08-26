// Copyright 2016 Erwan MATHIEU <wawanbreton@gmail.com>
//
// This file is part of CvComposer.
//
// CvComposer is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// CvComposer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with CvComposer.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <QMainWindow>

#include <QSettings>

class QUndoStack;

namespace Ui { class MainWidget; }

class MainWidget : public QMainWindow
{
    Q_OBJECT

    public:
        explicit MainWidget(QWidget *parent = nullptr);
        ~MainWidget();

    protected:
        virtual void closeEvent(QCloseEvent *event) override;

    private:
        void onNew();

        void onSave();

        void onLoad();

        void onLoadRecent();

        void onDisplaySettings();

        void onSettingsAccepted();

        void onDisplayPlugTypesHelp();

        void onDisplayLicence();

        void onAboutQt();

        void onAboutOpenCV();

        void onDisplayCredits();

    private:
        void updateTitle();

        void addRecent(const QString &file);

        void updateRecents(const QSettings &settings = QSettings());

        void loadFile(const QString &filePath);

    private:
        Ui::MainWidget *_ui;
        QString _currentFilePath;
        QUndoStack *const _commandsStack;
};
