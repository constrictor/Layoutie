/*
 *    This file is part of layoutie.
 *
 *    layoutie is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    layoutie is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with layoutie.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ProjectView.h"
#include "Settings.h"
#include "BitmapExport.h"

#include <QDir>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *inParent)
    : QMainWindow(inParent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->action_New, SIGNAL(triggered(bool)), SLOT(newFile()));
    connect(ui->action_Open, SIGNAL(triggered(bool)), SLOT(open()));
    connect(ui->action_Save, SIGNAL(triggered(bool)), SLOT(save()));
    connect(ui->actionSave_As, SIGNAL(triggered(bool)), SLOT(saveAs()));
    connect(ui->action_Close, SIGNAL(triggered(bool)), SLOT(closeCurrent()));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(tabChanged()));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), SLOT(closeTab(int)));
    connect(ui->action_bitmapExport, SIGNAL(triggered(bool)), SLOT(exportAsBitmap()));

    gSettings().restoreSettings(this);

    newFile();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newFile()
{
    newTab(QString());
}

void MainWindow::newTab(const QString& inFileName)
{
    ProjectView* view;
    try
    {
        view = new ProjectView(ui->tabWidget, inFileName);
    }
    catch(...)
    {
        return;
    }
    int index = ui->tabWidget->addTab(view, view->prettyFileName());
    ui->tabWidget->setCurrentIndex(index);
    connect(view, SIGNAL(modifiedChanged(bool)), SLOT(tabModified(bool)));
}

void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open project"), mDir, ProjectView::cFileNameFilter);
    if (fileName.isEmpty())
    {
        return;
    }
    mDir = QFileInfo(fileName).dir().path();

    {
        auto proj = currentProject();
        if (proj && proj->isUnused())
        {
            doCloseTab(ui->tabWidget->currentIndex(), true);
        }
    }

    newTab(fileName);
}

void MainWindow::save()
{
    auto proj = currentProject();
    if (proj)
    {
        if (proj->save())
        {
            mDir = QFileInfo(proj->fileName()).dir().path();
        }
    }
}

void MainWindow::saveAs()
{
    auto proj = currentProject();
    if (proj)
    {
        if (proj->saveAs())
        {
            mDir = QFileInfo(proj->fileName()).dir().path();
        }
    }
}

void MainWindow::closeCurrent()
{
    int index = ui->tabWidget->currentIndex();
    if (index != -1)
    {
        closeTab(index);
    }
}

void MainWindow::closeEvent(QCloseEvent* inOutEvent)
{
    while (ui->tabWidget->count())
    {
        if (!doCloseTab(0, true))
            return;
    }
    gSettings().saveSettings(this);
    QWidget::closeEvent(inOutEvent);
}

void MainWindow::tabChanged()
{
    auto proj = currentProject();
    QString title;
    if (proj)
    {
        title = tr("%1 - %2") .arg(proj->prettyFileName()) .arg(QApplication::applicationName());
    }
    else
    {
        title = QApplication::applicationName();
    }
    this->setWindowTitle(title);
}

ProjectView* MainWindow::project(int inIndex)
{
    if (inIndex == -1)
        return nullptr;
    return static_cast<ProjectView*>(ui->tabWidget->widget(inIndex));
}

ProjectView* MainWindow::currentProject()
{
    return project(ui->tabWidget->currentIndex());
}

bool MainWindow::closeTab(int inIndex)
{
    return doCloseTab(inIndex, false);
}

bool MainWindow::doCloseTab(int inIndex, bool inCloseLast)
{
    auto proj = static_cast<ProjectView*>(ui->tabWidget->widget(inIndex));
    if (!proj->trySaveIfModified())
        return false;
    const bool closingLastTab = ui->tabWidget->count() == 1;
    if (closingLastTab && !inCloseLast && proj->isUnused())
        return false;
    ui->tabWidget->removeTab(inIndex);
    if (closingLastTab && !inCloseLast)
    {
        newFile();
    }
    return true;
}

void MainWindow::tabModified(bool inValue)
{
    auto proj = qobject_cast<ProjectView*>(sender());
    if (proj)
        return;
    int index = ui->tabWidget->indexOf(proj);

    // TODO: Use icons for that
    QString name = proj->prettyFileName();
    if (inValue)
    {
        name = '*' + name;
    }
    ui->tabWidget->setTabText(index, name);
}

void MainWindow::exportAsBitmap()
{
    auto proj = currentProject();
    if (!proj)
        return;

    const SLFormat::Board* board = proj->activeBoard();
    if (!board)
        return;

    BitmapExport dlg(this, board);
    dlg.exec();
}
