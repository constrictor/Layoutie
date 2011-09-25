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
#include <QPainter>

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
	QBrush solidBrush(color);
	QBrush br(solidBrush);
	if (mComponent->isHatched())
	{
		br.setTexture(createHatchedBrushTexture(mComponent->hatchedWidth()));
		br.setColor(color);
	}
	const QPen p(color);

	float width = mComponent->width();
	if (!inIsMainNotGround && !mComponent->isCutoutArea())
	{
		width += mComponent->groundPlaneDistance() * 2;
	}
	const float radius = width / 2;

	QPointF prev;
	bool hasPrev = false;

	QPolygonF poly;
	QPainterPath path;
	for (auto point : mComponent->points())
	{
		QPointF pointF(point.x, - point.y);
		if (hasPrev)
		{
			addLine(poly, path, prev, pointF, radius);
		}
		hasPrev = true;
		prev = pointF;
	}
	{
		auto point = mComponent->points()[0];
		QPointF pointF(point.x, - point.y);
		addLine(poly, path, prev, pointF, radius);
	}
	poly << poly.first();

	{
		auto track = new QGraphicsPolygonItem;
		track->setPolygon(poly);
		track->setFillRule(Qt::WindingFill);
		track->setBrush(br);
		track->setPen(p);
		inOutItem->addToGroup(track);
	}
	{
		path.setFillRule(Qt::WindingFill);
		auto pathItem = new QGraphicsPathItem(path);
		pathItem->setPen(p);
		pathItem->setBrush(solidBrush);
		inOutItem->addToGroup(pathItem);
	}

	for (auto point : mComponent->points())
	{
		QPointF pointF(point.x, - point.y);
		QRectF rect(
			pointF.x() - radius,
					pointF.y() - radius,
					width,
			  width);
		auto circle = new QGraphicsEllipseItem(rect);
		circle->setBrush(solidBrush);
		circle->setPen(p);
		inOutItem->addToGroup(circle);
	}
}

void ZoneItem::addLine(QPolygonF& outPolygon, QPainterPath& outPath, QPointF inPoint1, QPointF inPoint2, qreal inRadius)
{
	QPointF off;
	off = inPoint1 - inPoint2;
	if (off.manhattanLength() == 0)
		return;
	{
		auto tmp = -off.x();
		off.setX(off.y());
		off.setY(tmp);
		qreal koef = inRadius / std::sqrt(off.x() * off.x() + off.y() * off.y());
		off.setX(off.x() * koef);
		off.setY(off.y() * koef);
	}
	outPolygon << inPoint1 - off;
	outPolygon << inPoint2 - off;
	QPainterPath path;
	path.moveTo(inPoint1 - off);
	path.lineTo(inPoint1 + off);
	path.lineTo(inPoint2 + off);
	path.lineTo(inPoint2 - off);
	path.closeSubpath();
	outPath.addPath(path);
}

QBitmap ZoneItem::createHatchedBrushTexture(float inHatchedWidth)
{
	QImage image(inHatchedWidth, inHatchedWidth, QImage::Format_Mono);
	image.fill(1);
	QPainter p(&image);
	p.setPen(QPen(Qt::black));
	p.fillRect(0, 0, inHatchedWidth, inHatchedWidth / 2, Qt::SolidPattern);
	p.fillRect(0, inHatchedWidth / 2, inHatchedWidth / 2, inHatchedWidth / 2, Qt::SolidPattern);
	QBitmap res = QBitmap::fromImage(image, Qt::AutoColor);
	return res;
}
