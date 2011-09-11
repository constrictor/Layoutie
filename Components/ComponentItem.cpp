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

#include "ComponentItem.h"
#include <QGraphicsItemGroup>

ComponentItem::ComponentItem()
	:mMainItem(new QGraphicsItemGroup),
	mGroundPlaneItem(nullptr)
{
}

ComponentItem::~ComponentItem()
{
}

void ComponentItem::update()
{
	updateMainItem(mMainItem);
	if (mGroundPlaneItem)
		updateGroundPlaneItem(mGroundPlaneItem);
}

QGraphicsItemGroup* ComponentItem::mainItem()
{
	return mMainItem;
}

QGraphicsItemGroup* ComponentItem::groundPlaneItem()
{
	if (!mGroundPlaneItem)
	{
		mGroundPlaneItem = new QGraphicsItemGroup;
		updateGroundPlaneItem(mGroundPlaneItem);
	}
	return mGroundPlaneItem;
}
