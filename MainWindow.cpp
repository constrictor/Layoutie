#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "ProjectView.h"

const QString cAppName = MainWindow::tr("Layoutie");

MainWindow::MainWindow(QWidget *inParent)
	: QMainWindow(inParent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	connect(ui->action_New, SIGNAL(triggered(bool)), SLOT(newFile()));
	connect(ui->action_Open, SIGNAL(triggered(bool)), SLOT(open()));
	connect(ui->action_Save, SIGNAL(triggered(bool)), SLOT(save()));
	connect(ui->actionSave_As, SIGNAL(triggered(bool)), SLOT(saveAs()));
	connect(ui->action_Close, SIGNAL(triggered(bool)), SLOT(close()));
	connect(ui->tabWidget, SIGNAL(currentChanged(int)), SLOT(tabChanged()));
	connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), SLOT(closeTab(int)));
	mDir = QDir::homePath(); //TODO: Save current dir

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
	newTab(fileName);
	if (ui->tabWidget->count() == 2 && project(0)->isUnused())
	{
		closeTab(0);
	}
}

void MainWindow::save()
{
	auto proj = currentProject();
	if (proj)
	{
		proj->save();
	}
}

void MainWindow::saveAs()
{
	auto proj = currentProject();
	if (proj)
	{
		proj->saveAs();
	}
}

void MainWindow::close()
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
		if (!closeTab(0))
			return;
	}
	QWidget::closeEvent(inOutEvent);
}

void MainWindow::tabChanged()
{
	auto proj = currentProject();
	QString title;
	if (proj)
	{
		title = tr("%1 - %2") .arg(cAppName) .arg(proj->prettyFileName());
	}
	else
	{
		title = cAppName;
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
	auto proj = static_cast<ProjectView*>(ui->tabWidget->widget(inIndex));
	if (!proj->trySaveIfModified())
		return false;
	const bool closingLastTab = ui->tabWidget->count() == 1;
	if (closingLastTab && proj->isUnused())
		return false;
	ui->tabWidget->removeTab(inIndex);
	if (closingLastTab)
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
