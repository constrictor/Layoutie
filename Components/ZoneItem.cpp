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

#include "ZoneItem.h"

#include "Settings.h"

#include <SLZoneComponent.h>

#include <QBrush>
#include <QPen>
#include <QGraphicsItemGroup>

#include <cmath>

ZoneItem::ZoneItem(SLFormat::ZoneComponent* inComponent)
	:mComponent(inComponent)
{
}

void ZoneItem::createItem(QGraphicsItemGroup* inOutItem, bool inIsMainNotGround)
{
	if (inIsMainNotGround && mComponent->isCutoutArea())
		return;
	const QColor color = inIsMainNotGround ?
		gSettings().layerColor(mComponent->layer())
		: gSettings().backgroundColor();
	const QBrush br(color);
	const QPen p(color);

	float width = mComponent->width();
	if (!inIsMainNotGround && !mComponent->isCutoutArea())
	{
		width += mComponent->groundPlaneDistance() * 2;
	}
	const float radius = width / 2;

	QPointF prev;
	bool hasPrev = false;

	QPolygonF poly1;
	QPolygonF poly2;
	for (auto point : mComponent->points())
	{
		QPointF pointF(point.x, - point.y);
		if (hasPrev)
		{
			addLineToPolygon(poly1, poly2, prev, pointF, radius);
		}
		hasPrev = true;
		prev = pointF;
	}
	{
		auto point = mComponent->points()[0];
		QPointF pointF(point.x, - point.y);
		if (pointF != prev)
		{
			addLineToPolygon(poly1, poly2, prev, pointF, radius);
		}
	}
	poly1 << poly1.first();
	poly2 << poly2.first();

	auto track = new QGraphicsPolygonItem;
	track->setPolygon(poly1);
	track->setFillRule(Qt::WindingFill);
	track->setBrush(br);
	track->setPen(p);
	inOutItem->addToGroup(track);
	track = new QGraphicsPolygonItem;
	track->setPolygon(poly2);
	track->setFillRule(Qt::WindingFill);
	track->setBrush(br);
	track->setPen(p);
	inOutItem->addToGroup(track);

	for (auto point : mComponent->points())
	{
		QPointF pointF(point.x, - point.y);
		QRectF rect(
			pointF.x() - radius,
					pointF.y() - radius,
					width,
			  width);
		auto circle = new QGraphicsEllipseItem(rect);
		circle->setBrush(br);
		circle->setPen(p);
		inOutItem->addToGroup(circle);
	}
}

void ZoneItem::addLineToPolygon(QPolygonF& outPolygon1, QPolygonF& outPolygon2, QPointF inPoint1, QPointF inPoint2, qreal inRadius)
{
	QPointF off;
	off = inPoint1 - inPoint2;
	{
		auto tmp = -off.x();
		off.setX(off.y());
		off.setY(tmp);
		qreal koef = inRadius / std::sqrt(off.x() * off.x() + off.y() * off.y());
		off.setX(off.x() * koef);
		off.setY(off.y() * koef);
	}
	outPolygon1 << inPoint1 - off;
	outPolygon2 << inPoint1 + off;
	outPolygon1 << inPoint2 - off;
	outPolygon2 << inPoint2 + off;
}
