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

#include "TrackItem.h"

#include "Settings.h"

#include <SLTrackComponent.h>

#include <QBrush>
#include <QPen>
#include <QGraphicsItemGroup>

#include <cmath>

TrackItem::TrackItem(SLFormat::TrackComponent* inComponent)
	:mComponent(inComponent)
{
}

void TrackItem::createItem(QGraphicsItemGroup* inOutItem, bool inIsMainNotGround)
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
	for (auto point : mComponent->points())
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
			auto track = new QGraphicsPolygonItem;
			QPolygonF poly;
			poly << pointF - off;
			poly << pointF + off;
			poly << prev + off;
			poly << prev - off;
			poly << poly.first();
			track->setPolygon(poly);
			track->setBrush(br);
			track->setPen(p);
			inOutItem->addToGroup(track);
		}
		hasPrev = true;
		prev = pointF;

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
