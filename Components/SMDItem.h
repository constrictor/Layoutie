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

#ifndef SMD_ITEM_H
#define SMD_ITEM_H

#include "ComponentItem.h"

namespace SLFormat
{
	class SMDComponent;
}

class SMDItem : public ComponentItem
{
public:
	SMDItem(SLFormat::SMDComponent* inComponent);

protected:
	virtual void updateMainItem(QGraphicsItemGroup* inOutMainItem);
	virtual void updateGroundPlaneItem(QGraphicsItemGroup* inOutGroundPlaneItem);
private:

	SLFormat::SMDComponent* mComponent;
};

#endif //SMD_ITEM_H