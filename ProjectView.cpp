#include "ProjectView.h"

#include "BoardView.h"

#include <SLExceptions.h>
#include <SLBoard.h>

const QString ProjectView::cFileNameFilter = ProjectView::tr("Sprint Layout files (*.lay)");

ProjectView::ProjectView(QWidget* inParent, const QString& inFileName)
	: QWidget(inParent),
	mFileName(inFileName),
	mModified(false)
{
	if (!mFileName.isEmpty())
	{
		QFile file(mFileName);
		if (!file.open(QIODevice::ReadOnly))
		{
			QMessageBox::critical(this, tr("Cannot open file"), tr("Cannot open %1:\n%2") .arg(prettyFileName()) .arg(file.errorString()));
			throw std::exception();
		}
		QByteArray arr = file.readAll();
		try
		{
			mProject.loadFromStream(arr.data(), arr.size());
		}
		catch(SLFormat::Exceptions::LayoutException& e)
		{
			QMessageBox::critical(this, tr("Cannot open file"), tr("Cannot open %1:\n%2") .arg(prettyFileName()) .arg(e.what()));
			throw;
		}
	}
	auto mainLayout = new QVBoxLayout(this);
	QTabWidget* tabs = new QTabWidget();
	tabs->setDocumentMode(true);
	tabs->setTabPosition(QTabWidget::South);

	mainLayout->addWidget(tabs);
	this->setLayout(mainLayout);

	for (auto board : mProject.boards())
	{
		auto boardView = new BoardView(tabs, board);
		connect(boardView, SIGNAL(modified()), SLOT(boardModified()));
		tabs->addTab(boardView, QString::fromLocal8Bit(board->name().c_str()));
	}
}

ProjectView::~ProjectView()
{
	trySaveIfModified();
}

bool ProjectView::save()
{
	if (fileName().isEmpty())
	{
		return saveAs();
	}
	QFile file(mFileName);
	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::critical(this, tr("Cannot save file"), tr("Cannot save %1:\n%2") .arg(prettyFileName()) .arg(file.errorString()));
		return false;
	}
	try
	{
		size_t dataSize;
		char* projectData = mProject.saveToStreamVer5(&dataSize);
		if (file.write(projectData, dataSize) != (qint64)dataSize)
		{
			QMessageBox::critical(this, tr("Cannot save file"), tr("Cannot save %1:\n%2") .arg(prettyFileName()) .arg(file.errorString()));
			return false;
		}
	}
	catch(SLFormat::Exceptions::LayoutException& e)
	{
		QMessageBox::critical(this, tr("Error"), tr("Internal error: %1") .arg(e.what()));
		return false;
	}
	setModified(false);
	return true;
}

bool ProjectView::saveAs()
{
	QString saveFileName = QFileDialog::getSaveFileName(this, tr("Save project"), mFileName, ProjectView::cFileNameFilter);
	if (saveFileName.isEmpty())
	{
		return false;
	}
	QString oldFileName = mFileName;
	mFileName = saveFileName;
	if (!save())
	{
		mFileName = oldFileName;
		return false;
	}
	return true;
}

bool ProjectView::trySaveIfModified()
{
	if (!mModified)
	{
		return true;
	}
	QMessageBox::StandardButton res = QMessageBox::question(this, tr("File modified"), tr("File %1 was modified").arg(mFileName.isEmpty() ? prettyFileName() : QString()), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Cancel);
	if (res == QMessageBox::Cancel)
	{
		return false;
	}
	if (res == QMessageBox::Yes)
	{
		save();
	}
	return true;
}

QString ProjectView::prettyFileName() const
{
	if (mFileName.isEmpty())
		return tr("<Untitled>");
	return QFileInfo(mFileName).baseName();
}

void ProjectView::boardModified()
{
	setModified(true);
}

void ProjectView::setModified(bool inModified)
{
	bool changed = mModified != inModified;
	mModified = inModified;
	if (changed)
		emit modifiedChanged(inModified);
}
