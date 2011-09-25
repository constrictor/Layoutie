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
	(void)inOutItem;
	(void)inIsMainNotGround;
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
			!inIsMainNotGround && mComponent->thermal() ? createPolygonThermaltem(inOutItem) : createPolygonItem(inOutItem, inIsMainNotGround);
			break;
		case SLFormat::PadType::RoundedHorizontal:
			createRoundedItem(inOutItem, inIsMainNotGround, true);
			break;
		case SLFormat::PadType::RoundedVertical:
			createRoundedItem(inOutItem, inIsMainNotGround, false);
			break;
		default:
			assert(false);
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
	//TODO: Thermal pad support
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

void PadItem::createRoundedItem(QGraphicsItemGroup* inOutItem, bool inIsMainNotGround, bool inHorizontal)
{
	//TODO: Thermal pad support
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
	
	QRectF baseRect(mComponent->center().x - radius,
		- mComponent->center().y - radius,
		2 * radius,
		2 * radius);
	QRectF rect1(mComponent->points()[0].x - radius, -mComponent->points()[0].y - radius, 2 * radius, 2 * radius);
	QRectF rect2(mComponent->points()[1].x - radius, -mComponent->points()[1].y - radius, 2 * radius, 2 * radius);
	if (inHorizontal)
	{
		baseRect.adjust(radiusDiff, 0, -radiusDiff, 0);
	}
	else
	{
		baseRect.adjust(0, radiusDiff, 0, -radiusDiff);
	}
	QPolygonF poly;
	{
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
	}
	auto r1 = new QGraphicsEllipseItem(rect1);
	r1->setBrush(br);
	r1->setPen(p);
	inOutItem->addToGroup(r1);
	auto r2 = new QGraphicsEllipseItem(rect2);
	r2->setBrush(br);
	r2->setPen(p);
	inOutItem->addToGroup(r2);
	auto r3 = new QGraphicsPolygonItem(poly);
	r3->setBrush(br);
	r3->setPen(p);
	inOutItem->addToGroup(r3);
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
	for (auto track : mComponent->thermalTracks())
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
