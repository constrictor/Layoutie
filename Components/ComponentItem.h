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

#ifndef COMPONENT_ITEM_H
#define COMPONENT_ITEM_H

class QGraphicsItemGroup;

class ComponentItem
{
public:
	ComponentItem();

	virtual ~ComponentItem();

	void update();

	QGraphicsItemGroup*	mainItem();
	QGraphicsItemGroup*	groundPlaneItem();
protected:
	virtual void updateMainItem(QGraphicsItemGroup* inOutMainItem) = 0;
	virtual void updateGroundPlaneItem(QGraphicsItemGroup* inOutGroundPlaneItem) = 0;
private:
	QGraphicsItemGroup* mMainItem;
	QGraphicsItemGroup* mGroundPlaneItem;
};

#endif //COMPONENT_ITEM_H