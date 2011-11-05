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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
	class MainWindow;
}

class ProjectView;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	friend class Settings;
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
private slots:
	void	newFile();
	void	open();
	void	save();
	void	saveAs();
	void	closeCurrent();
	void	exportAsBitmap();
	void	tabChanged();
	bool	closeTab(int inIndex);
	void	tabModified(bool inValue);
protected:
    virtual void closeEvent(QCloseEvent* inOutEvent);
private:
	Ui::MainWindow *ui;

	bool	doCloseTab(int inIndex, bool inCloseLast);
	ProjectView* project(int inIndex);
	ProjectView* currentProject();
	void	newTab(const QString& inFileName);

	QString mDir;
};

#endif // MAINWINDOW_H
