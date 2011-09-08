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
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
private slots:
	void	newFile();
	void	open();
	void	save();
	void	saveAs();
	void	close();
	void	tabChanged();
	bool	closeTab(int inIndex);
	void	tabModified(bool inValue);
protected:
    virtual void closeEvent(QCloseEvent* inOutEvent);
private:
	Ui::MainWindow *ui;

	ProjectView* project(int inIndex);
	ProjectView* currentProject();
	void	newTab(const QString& inFileName);

	QString mDir;
};

#endif // MAINWINDOW_H
