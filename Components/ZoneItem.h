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

#ifndef ZONE_ITEM_H
#define ZONE_ITEM_H

#include "ComponentItem.h"

#include <QPointF>
#include <QBitmap>

namespace SLFormat
{
    class ZoneComponent;
}

class QPolygonF;

class ZoneItem : public ComponentItem
{
public:
    ZoneItem(SLFormat::ZoneComponent* inComponent);

protected:
    virtual void createItem(QGraphicsItemGroup* inOutItem, bool inIsMainNotGround);

private:
    void    addLine(QPolygonF& outPolygon, QPainterPath& outPath, QPointF inPoint1, QPointF inPoint2, qreal inRadius);

    QBitmap createHatchedBrushTexture(float inHatchedWidth);

    SLFormat::ZoneComponent* mComponent;
};

#endif //ZONE_ITEM_H