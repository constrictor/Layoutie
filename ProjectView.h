#ifndef PROJECT_VIEW_H
#define PROJECT_VIEW_H

#include <QtGui>
#include <SLProject.h>

class ProjectView : public QWidget
{
	Q_OBJECT
public:
	ProjectView(QWidget* inParent, const QString& inFileName);
	~ProjectView();

	bool	save();
	bool	saveAs();

	bool	trySaveIfModified();

	bool	isModified() const { return mModified; }

	bool	isUnused() const { return !isModified() && fileName().isEmpty(); }
	const QString& fileName() const { return mFileName; }
	QString	prettyFileName() const;

	static const QString cFileNameFilter;
signals:
	void	modifiedChanged(bool inValue);

private slots:
	void	boardModified();
private:
	void	setModified(bool inModified);

	SLFormat::Project mProject;
	QString	mFileName;
	bool	mModified;
};

#endif //PROJECT_VIEW_H
