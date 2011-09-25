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

#include "PadItem.h"

#include "Settings.h"

#include <SLPadComponent.h>

#include <QBrush>
#include <QPen>
#include <QGraphicsEllipseItem>

#include <assert.h>
#include <cmath>

PadItem::PadItem(SLFormat::PadComponent* inComponent)
	:mComponent(inComponent)
{
}

void PadItem::createItem(QGraphicsItemGroup* inOutItem, bool inIsMainNotGround)
{
	if (!inIsMainNotGround && mComponent->thermal())
	{
		switch (mComponent->padType())
		{
			case SLFormat::PadType::Circular:
				createCircularThermaItem(inOutItem);
				break;
			case SLFormat::PadType::Octagon:
			case SLFormat::PadType::Square:
			case SLFormat::PadType::OctagonHorizontal:
			case SLFormat::PadType::OctagonVertical:
			case SLFormat::PadType::SquareHorizontal:
			case SLFormat::PadType::SquareVertical:
				createPolygonThermaltem(inOutItem);
				break;
			case SLFormat::PadType::RoundedHorizontal:
			case SLFormat::PadType::RoundedVertical:
				createRoundedThermalItem(inOutItem);
				break;
			default:
				assert(false);
		}
	}
	else
	{
		switch (mComponent->padType())
		{
			case SLFormat::PadType::Circular:
				createCircularItem(inOutItem, inIsMainNotGround);
				break;
			case SLFormat::PadType::Octagon:
			case SLFormat::PadType::Square:
			case SLFormat::PadType::OctagonHorizontal:
			case SLFormat::PadType::OctagonVertical:
			case SLFormat::PadType::SquareHorizontal:
			case SLFormat::PadType::SquareVertical:
				createPolygonItem(inOutItem, inIsMainNotGround);
				break;
			case SLFormat::PadType::RoundedHorizontal:
			case SLFormat::PadType::RoundedVertical:
				createRoundedItem(inOutItem, inIsMainNotGround);
				break;
			default:
				assert(false);
		}
	}
}

QColor PadItem::color(bool inIsMainNotGround) const
{
	QColor col;
	if (inIsMainNotGround)
	{
		if (mComponent->through() && (mComponent->layer() == 0 || mComponent->layer() == 2))
		{
			col = gSettings().throughColor();
		}
		else
		{
			col = gSettings().layerColor(mComponent->layer());
		}
	}
	else
	{
		col = gSettings().backgroundColor();
	}
	return col;
}


void PadItem::createCircularItem(QGraphicsItemGroup* inOutItem, bool inIsMainNotGround)
{
	float radius = mComponent->externalRadius();
	if (!inIsMainNotGround)
	{
		radius += mComponent->groundPlaneDistance();
	}
	QRectF rect(mComponent->center().x - radius,
				- mComponent->center().y - radius,
				2 * radius,
				2 * radius);
	auto main = new QGraphicsEllipseItem(rect);

	QColor col(color(inIsMainNotGround));
	main->setBrush(QBrush(col));
	main->setPen(QPen(col));
	inOutItem->addToGroup(main);
}

void PadItem::createPolygonItem(QGraphicsItemGroup* inOutItem, bool inIsMainNotGround)
{
	QPolygonF poly;

	for (auto point : mComponent->points())
	{
		poly << QPointF(point.x, - point.y);
	}
	auto main = new QGraphicsPolygonItem(poly);

	QColor col(color(inIsMainNotGround));
	main->setBrush(QBrush(col));
	QPen p(col);
	if (!inIsMainNotGround)
	{
		p.setWidthF(mComponent->groundPlaneDistance() * 2);
		p.setJoinStyle(Qt::MiterJoin);
	}
	main->setPen(p);
	inOutItem->addToGroup(main);
}

void PadItem::createRoundedItem(QGraphicsItemGroup* inOutItem, bool inIsMainNotGround)
{
	const float realRadius = mComponent->externalRadius();
	float radius = realRadius;
	float radiusDiff = 0;
	if (!inIsMainNotGround)
	{
		radiusDiff = mComponent->groundPlaneDistance();
		radius += radiusDiff;
	}
	QColor col(color(inIsMainNotGround));
	QBrush br(col);
	QPen p(col);

	{
		QRectF rect1(mComponent->points()[0].x - radius, -mComponent->points()[0].y - radius, 2 * radius, 2 * radius);
		QPainterPath path;
		path.moveTo(mComponent->points()[0].x, -mComponent->points()[0].y);
		path.arcTo(rect1, -mComponent->angle() - 90, 180);
		path.closeSubpath();
		auto pathItem = new QGraphicsPathItem(path);
		pathItem->setBrush(br);
		pathItem->setPen(p);
		inOutItem->addToGroup(pathItem);
	}
	{
		QRectF rect2(mComponent->points()[1].x - radius, -mComponent->points()[1].y - radius, 2 * radius, 2 * radius);
		QPainterPath path;
		path.moveTo(mComponent->points()[1].x, -mComponent->points()[1].y);
		path.arcTo(rect2, -mComponent->angle() + 90, 180);
		path.closeSubpath();
		auto pathItem = new QGraphicsPathItem(path);
		pathItem->setBrush(br);
		pathItem->setPen(p);
		inOutItem->addToGroup(pathItem);
	}
	{
		QPolygonF poly;
		QPointF p1(mComponent->points()[0].x, -mComponent->points()[0].y);
		QPointF p2(mComponent->points()[1].x, -mComponent->points()[1].y);
		QPointF halfPoint((p1.y() - p2.y()) / 2, -(p1.x() - p2.x()) / 2);

		if (!inIsMainNotGround)
		{
			halfPoint.setX(halfPoint.x() * radius / realRadius);
			halfPoint.setY(halfPoint.y() * radius / realRadius);
		}

		poly << p1 + halfPoint;
		poly << p1 - halfPoint;
		poly << p2 - halfPoint;
		poly << p2 + halfPoint;

		auto polyItem = new QGraphicsPolygonItem(poly);
		polyItem->setBrush(br);
		polyItem->setPen(p);
		inOutItem->addToGroup(polyItem);
	}
}

void PadItem::createDrillItem(QGraphicsItemGroup* inOutItem)
{
	QPointF center(mComponent->center().x, -mComponent->center().y);
	float radius = mComponent->internalRadius();
	bool fullHole = radius == mComponent->externalRadius();

	QRectF rect(center.x() - radius,
				center.y() - radius,
				2 * radius,
			 2 * radius);
	auto circle = new QGraphicsEllipseItem(rect);
	if (fullHole)
	{
		circle->setPen(QPen(gSettings().fullDrillBorderColor()));
	}
	else if (mComponent->through())
	{
		circle->setPen(QPen(gSettings().throughDrillBorderColor()));
	}
	else
	{
		circle->setPen(QPen(gSettings().backgroundColor()));
	}
	circle->setBrush(QBrush(gSettings().backgroundColor()));
	inOutItem->addToGroup(circle);
	if (fullHole)
	{
		float lineOff = radius / 2;
		auto line1 = new QGraphicsLineItem(center.x() - lineOff, center.y() - lineOff, center.x() + lineOff, center.y() + lineOff);
		auto line2 = new QGraphicsLineItem(center.x() + lineOff, center.y() - lineOff, center.x() - lineOff, center.y() + lineOff);
		QPen p(gSettings().throughDrillCrossColor());
		line1->setPen(p);
		line2->setPen(p);
		inOutItem->addToGroup(line1);
		inOutItem->addToGroup(line2);
	}
}

void PadItem::createPolygonThermaltem(QGraphicsItemGroup* inOutItem)
{
	QColor col(color(false));
	QPen p(col);
	QBrush br(col);
	for (auto track : mComponent->thermalPolygons())
	{
		QPolygonF poly;

		for (auto point : track)
		{
			poly << QPointF(point.x, - point.y);
		}
		auto main = new QGraphicsPolygonItem(poly);

		main->setBrush(br);
		main->setPen(p);
		inOutItem->addToGroup(main);
	}
}

static float squareLength(const QPointF& inPoint)
{
	return inPoint.x() * inPoint.x() + inPoint.y() * inPoint.y();
}

void PadItem::createRoundedThermalItem(QGraphicsItemGroup* inOutItem)
{
	const float realRadius = mComponent->externalRadius();
	float radiusDiff = mComponent->groundPlaneDistance();
	float radius = realRadius + radiusDiff;
	QColor col(color(false));
	QBrush br(col);
	QPen p(col);

	const float thermalAngleOffset = std::atan2(mComponent->thermalTrackRadius(), mComponent->externalRadius() + mComponent->groundPlaneDistance()) * 180 / M_PI;
	const float thermalAngle = 90 - thermalAngleOffset;
	unsigned pointNum = 0;
	for (auto point : mComponent->points())
	{
		const QRectF rect(point.x - radius, -point.y - radius, 2 * radius, 2 * radius);

		for (int unused = 0; unused < 2; unused++)
		{
			float nextAngleShift = -mComponent->angle() + (pointNum / 2 ? 90 : -90) + (pointNum % 2 ? 90 + thermalAngleOffset : 0);

			QPainterPath path;

			// find a helper point. It's more precise than calculate it without Qt.
			path.arcMoveTo(rect, nextAngleShift);
			QPointF C = path.currentPosition();
			path.arcMoveTo(rect, nextAngleShift + thermalAngle);
			QPointF B = path.currentPosition();
			QPointF A(mComponent->points()[pointNum/2].x, -mComponent->points()[pointNum/2].y);
			QPointF pos;
			if (pointNum % 2)
			{
				float k = squareLength(B - C) / 2 / squareLength(B - A);
				pos = B * (1 - k) + A * k;
			}
			else
			{
				float k = squareLength(B - C) / 2 / squareLength(C - A);
				pos = C * (1 - k) + A * k;
			}
			path.moveTo(pos);

			path.arcTo(rect, nextAngleShift, thermalAngle);

			auto pathItem = new QGraphicsPathItem(path);
			pathItem->setBrush(br);
			pathItem->setPen(p);
			inOutItem->addToGroup(pathItem);

			pointNum++;
		}
	}
	{
		createPolygonThermaltem(inOutItem);
	}
}

void PadItem::createCircularThermaItem(QGraphicsItemGroup* inOutItem)
{
	const float realRadius = mComponent->externalRadius();
	float radiusDiff = mComponent->groundPlaneDistance();
	float radius = realRadius + radiusDiff;
	QColor col(color(false));
	QBrush br(col);
	QPen p(col);

	const float thermalAngleOffset = std::atan2(mComponent->thermalTrackRadius(), mComponent->externalRadius() + mComponent->groundPlaneDistance()) * 180 / M_PI;
	const float thermalAngle = 90 - 2 * thermalAngleOffset;
	const QRectF rect(mComponent->center().x - radius, -mComponent->center().y - radius, 2 * radius, 2 * radius);

	float startAngle = -mComponent->angle() + thermalAngleOffset - 90;
	for (int unused = 0; unused < 4; unused++)
	{
		QPainterPath path;

		// find a helper point. It's more precise than calculate it without Qt.
		path.arcMoveTo(rect, startAngle);
		auto pos1 = path.currentPosition();
		path.arcMoveTo(rect, startAngle + thermalAngle);
		auto pos2 = path.currentPosition();
		QPointF off = (pos1 - pos2) / 2;
		QPointF res = (pos1 + pos2) / 2 - QPointF(off.y(), -off.x());
		path.moveTo(res);

		path.arcTo(rect, startAngle, thermalAngle);
		auto pathItem = new QGraphicsPathItem(path);
		pathItem->setBrush(br);
		pathItem->setPen(p);
		inOutItem->addToGroup(pathItem);

		startAngle += 90;
	}
}
