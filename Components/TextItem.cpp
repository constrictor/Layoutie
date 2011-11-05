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

#include "TextItem.h"

#include "Settings.h"

#include <SLTextComponent.h>

#include <QBrush>
#include <QPen>
#include <QGraphicsItemGroup>

#include <cmath>

TextItem::TextItem(SLFormat::TextComponent* inComponent)
	:mComponent(inComponent)
{
}

void TextItem::createItem(QGraphicsItemGroup* inOutItem, bool inIsMainNotGround)
{
	if (inIsMainNotGround && mComponent->isCutoutArea())
		return;
	const QColor color = inIsMainNotGround ?
		gSettings().layerColor(mComponent->layer())
		: gSettings().backgroundColor();
	const QBrush br(color);
	const QPen p(color);

	for (auto track : mComponent->tracks())
	{
		QPainterPath path;
		path.setFillRule(Qt::WindingFill);

		QPainterPath path2;
		path2.setFillRule(Qt::WindingFill);

		float width = track.width();
		if (!inIsMainNotGround && !mComponent->isCutoutArea())
		{
			width += mComponent->groundPlaneDistance() * 2;
		}
		const float radius = width / 2;
		QPointF prev;
		bool hasPrev = false;
		for (auto point : track.points())
		{
			QPointF pointF(point.x, - point.y);
			if (hasPrev)
			{
				QPointF off;
				off = pointF - prev;
				{
					auto tmp = -off.x();
					off.setX(off.y());
					off.setY(tmp);
					qreal koef = radius / sqrt(off.x() * off.x() + off.y() * off.y());
					off.setX(off.x() * koef);
					off.setY(off.y() * koef);
				}
				QPolygonF poly;
				poly << pointF - off;
				poly << pointF + off;
				poly << prev + off;
				poly << prev - off;
				poly << poly.first();
				path.addPolygon(poly);
			}
			hasPrev = true;
			prev = pointF;

			QRectF rect(
				pointF.x() - radius,
				pointF.y() - radius,
				width,
				width);
			path2.addEllipse(rect);
		}

		auto pathItem = new QGraphicsPathItem(path);
		pathItem->setBrush(br);
		pathItem->setPen(p);
		inOutItem->addToGroup(pathItem);

		auto pathItem2 = new QGraphicsPathItem(path2);
		pathItem2->setBrush(br);
		pathItem2->setPen(p);
		inOutItem->addToGroup(pathItem2);
	}
}
