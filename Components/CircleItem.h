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

#ifndef CIRCLE_ITEM_H
#define CIRCLE_ITEM_H

#include "ComponentItem.h"

class QGraphicsPolygonItem;

namespace SLFormat
{
    class CircleComponent;
}

class CircleItem : public ComponentItem
{
public:
    CircleItem(SLFormat::CircleComponent* inComponent);

protected:
    virtual void createItem(QGraphicsItemGroup* inOutItem, bool inIsMainNotGround);

private:
    SLFormat::CircleComponent* mComponent;
};

#endif //CIRCLE_ITEM_H