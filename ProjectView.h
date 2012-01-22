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

#ifndef PROJECT_VIEW_H
#define PROJECT_VIEW_H

#include <QWidget>
#include <SLProject.h>

class QTabWidget;

class ProjectView : public QWidget
{
    Q_OBJECT
public:
    ProjectView(QWidget* inParent, const QString& inFileName);
    ~ProjectView();

    bool    save();
    bool    saveAs();

    bool    trySaveIfModified();

    bool    isModified() const { return mModified; }

    bool    isUnused() const { return !isModified() && fileName().isEmpty(); }
    const QString& fileName() const { return mFileName; }
    QString    prettyFileName() const;

    const SLFormat::Board*    activeBoard() const;

    static const QString cFileNameFilter;
signals:
    void    modifiedChanged(bool inValue);

private slots:
    void    boardModified();
private:
    void    setModified(bool inModified);

    QTabWidget*     mTabWidget;

    SLFormat::Project mProject;
    QString    mFileName;
    bool    mModified;
};

#endif //PROJECT_VIEW_H
