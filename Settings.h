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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QColor>
#include <QPen>

class Settings;

Settings& gSettings();

class Settings
{
public:
	QColor layerColor(int inLayer) const;
	QColor groundPlaneLayerColor(int inLayer) const;
	QColor backgroundColor() const;
	//QColor gridColor() const;
	const QPen& gridMainPen() const;
	const QPen& gridSubPen() const;
	unsigned numGridSubdivisions() const;

	unsigned layerFromOrderNumber(int inOrderNum) const;

	friend Settings& gSettings();
private:
	Settings();

	static Settings* mInstance;

	QPen mGridMainPen;
	QPen mGridSubPen;
};

#endif //SETTINGS_H