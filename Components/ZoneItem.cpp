#include "ZoneItem.h"

#include <SLZoneComponent.h>

#include "Settings.h"

ZoneItem::ZoneItem(SLFormat::ZoneComponent* inComponent)
	:mComponent(inComponent)
{
	update();
}

ZoneItem::~ZoneItem()
{
}

void ZoneItem::update()
{
	const QColor color = Settings::instance().layerColor(mComponent->layer());
	const QBrush br(color);
	const QPen p(color);

	const float width = mComponent->width();
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
	addToGroup(track);
	track = new QGraphicsPolygonItem;
	track->setPolygon(poly2);
	track->setFillRule(Qt::WindingFill);
	track->setBrush(br);
	track->setPen(p);
	addToGroup(track);

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
		addToGroup(circle);
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
		qreal koef = inRadius / sqrt(off.x() * off.x() + off.y() * off.y());
		off.setX(off.x() * koef);
		off.setY(off.y() * koef);
	}
	outPolygon1 << inPoint1 - off;
	outPolygon2 << inPoint1 + off;
	outPolygon1 << inPoint2 - off;
	outPolygon2 << inPoint2 + off;
}
