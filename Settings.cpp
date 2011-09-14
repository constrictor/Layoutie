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

#include "Settings.h"

#include <assert.h>

#include <SLBoard.h>

Settings* Settings::mInstance = nullptr;

Settings& gSettings()
{
	if (!Settings::mInstance)
	{
		Settings::mInstance = new Settings();
	}
	return *Settings::mInstance;
}

Settings::Settings()
{
	mGridMainPen = QPen(QColor(0x404040), 2.0);
	mGridSubPen = QPen(QColor(0x404040));
}


QColor Settings::layerColor(int inLayer) const
{
	switch(inLayer)
	{
		case 0:
			return Qt::blue;
		case 1:
			return Qt::red;
		case 2:
			return Qt::green;
		case 3:
			return Qt::yellow;
		case 4:
			return 0xD76600;
		case 5:
			return 0xEAA844;
		case 6:
			return Qt::white;
		default:
			assert(false);
			return Qt::black;
	}
}

QColor Settings::groundPlaneLayerColor(int inLayer) const
{
	return layerColor(inLayer).darker(300);
}

QColor Settings::backgroundColor() const
{
	return Qt::black;
}

QColor Settings::throughColor()
{
	return 0x127F96;
}

QColor Settings::fullDrillBorderColor()
{
	return Qt::white;
}

QColor Settings::throughDrillBorderColor()
{
	return Qt::red;
}

QColor Settings::throughDrillCrossColor()
{
	return Qt::green;
}

unsigned int Settings::layerFromOrderNumber(int inOrderNum) const
{
	switch(inOrderNum)
	{
		case 0:
			return 4;
		case 1:
			return 5;
		case 2:
			return 0;
		case 3:
			return 2;
		case 4:
			return 1;
		case 5:
			return 3;
		case 6:
			return 6;
		default:
			assert(false);
			return 0;
	}
}

const QPen& Settings::gridMainPen() const
{
	return mGridMainPen;
}

const QPen& Settings::gridSubPen() const
{
	return mGridSubPen;
}

unsigned Settings::numGridSubdivisions() const
{
	return 10;
}
