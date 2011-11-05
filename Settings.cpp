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

#include "MainWindow.h"
#include "BitmapExport.h"

#include <SLBoard.h>

#include <assert.h>

#include <QSettings>
#include <QDir>
#include <ui_BitmapExport.h>

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
	:mSettings(new QSettings)
{

	mGridMainPen = QPen(QColor(0x707070));
	mGridSubPen = QPen(QColor(0x404040));
	mGridOriginPen = QPen(QColor(0xA0A0A0), 0, Qt::DashLine);
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

QString Settings::layerName(int inLayer) const
{
	switch(inLayer)
	{
		case 0:
			return "Copper (side 1)";
		case 1:
			return "Silk screen (side 1)";
		case 2:
			return "Copper (side 2)";
		case 3:
			return "Silk screen (side 2)";
		case 4:
			return "Inner layer 1";
		case 5:
			return "Inner layer 2";
		case 6:
			return "Outline";
		default:
			assert(false);
			return "";
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

const QPen& Settings::gridOriginPen() const
{
	return mGridOriginPen;
}

unsigned Settings::numGridSubdivisions() const
{
	return 10;
}

template<>
void Settings::restoreSettings<MainWindow>(MainWindow* inMainWindow)
{
	inMainWindow->restoreGeometry(mSettings->value("MainWindowGeometry").toByteArray());
	inMainWindow->restoreState(mSettings->value("MainWindowState").toByteArray());

	QString path = mSettings->value("LastPath", QString()).toString();
	if (path.isEmpty())
		path = QDir::homePath();
	inMainWindow->mDir = path;
}

template<>
void Settings::saveSettings<MainWindow>(MainWindow* inMainWindow)
{
	mSettings->setValue("MainWindowGeometry", inMainWindow->saveGeometry());
	mSettings->setValue("MainWindowState", inMainWindow->saveState());

	mSettings->setValue("LastPath", inMainWindow->mDir);
}

template<>
void Settings::restoreSettings<BitmapExport>(BitmapExport* inWindow)
{
	inWindow->restoreGeometry(mSettings->value("BitmapExportWindowGeometry").toByteArray());

	bool ok;
	int resolution = mSettings->value("BitmapExportResolution", 0).toInt(&ok);
	if (!ok)
		resolution = 0;
	int resolutionUnits = mSettings->value("BitmapExportResolutionUnits", 0).toInt(&ok);
	if (!ok)
		resolutionUnits = 0;
	inWindow->setResolution(resolutionUnits, resolution);

	QString path = mSettings->value("LastBitmapExportPath", QString()).toString();
	if (path.isEmpty())
		path = QDir::homePath();
	inWindow->mDir = path;
}

template<>
void Settings::saveSettings<BitmapExport>(BitmapExport* inWindow)
{
	mSettings->setValue("BitmapExportWindowGeometry", inWindow->saveGeometry());

	int resolutionUnits;
	int resolution;
	inWindow->getResolution(resolutionUnits, resolution);
	mSettings->setValue("BitmapExportResolution", resolution);
	mSettings->setValue("BitmapExportResolutionUnits", resolutionUnits);

	mSettings->setValue("LastBitmapExportPath", inWindow->mDir);
}
