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

#ifndef BITMAPEXPORT_H
#define BITMAPEXPORT_H

#include <QDialog>

#include <SLBoard.h>

namespace Ui {
	class BitmapExport;
}

class BoardScene;

class QCheckBox;

class BitmapExport : public QDialog
{
	Q_OBJECT
	
public:
	explicit BitmapExport(QWidget *inParent, const SLFormat::Board* inBoard);
	~BitmapExport();

protected:
	virtual void resizeEvent(QResizeEvent* );
private slots:
	void	updateView();
	void	resolutionUnitsChanged();

	void	resolutionSliderChanged(int inValue);
	void	resolutionEditChanged(const QString& inValue);

	void	accepted();
	void	rejected();

private:
	Ui::BitmapExport *ui;

	QCheckBox*	mLayerCheckBoxes[SLFormat::Board::cNumberLayers];

	const SLFormat::Board*	mBoard;
	BoardScene*	mScene;
};

#endif // BITMAPEXPORT_H
