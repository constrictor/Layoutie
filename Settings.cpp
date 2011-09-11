#include "Settings.h"

#include <assert.h>

#include <SLBoard.h>

Settings* Settings::mInstance = nullptr;

Settings& Settings::instance()
{
	if (!mInstance)
	{
		mInstance = new Settings();
	}
	return *mInstance;
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

QColor Settings::backgroundColor() const
{
	return Qt::black;
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
/*	switch(inOrderNum)
	{
		case 0:
			return 2;
		case 1:
			return 4;
		case 2:
			return 3;
		case 3:
			return 5;
		case 4:
			return 0;
		case 5:
			return 1;
		case 6:
			return 6;
		default:
			assert(false);
			return 0;
	}*/
}
