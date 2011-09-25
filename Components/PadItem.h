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

#ifndef PAD_ITEM_H
#define PAD_ITEM_H

#include "ComponentItem.h"
#include <QColor>

namespace SLFormat
{
	class PadComponent;
}

class PadItem : public ComponentItem
{
public:
	PadItem(SLFormat::PadComponent* inComponent);

protected:
	virtual void createItem(QGraphicsItemGroup* inOutItem, bool inIsMainNotGround);
    virtual void createDrillItem(QGraphicsItemGroup* inOutItem);
private:
	void createCircularItem(QGraphicsItemGroup* inOutItem, bool inIsMainNotGround);
	void createPolygonItem(QGraphicsItemGroup* inOutItem, bool inIsMainNotGround);
	void createRoundedItem(QGraphicsItemGroup* inOutItem, bool inIsMainNotGround);

	void createCircularThermaItem(QGraphicsItemGroup* inOutItem);
	void createPolygonThermaltem(QGraphicsItemGroup* inOutItem);
	void createRoundedThermalItem(QGraphicsItemGroup* inOutItem);

	QColor	color(bool inIsMainNotGround) const;

	SLFormat::PadComponent* mComponent;
};

#endif //PAD_ITEM_H