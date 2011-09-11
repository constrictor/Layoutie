#include "TrackItem.h"

#include <SLTrackComponent.h>

#include "Settings.h"

TrackItem::TrackItem(SLFormat::TrackComponent* inComponent)
	:mComponent(inComponent)
{
	update();
}

TrackItem::~TrackItem()
{
}

void TrackItem::update()
{
	const QColor color = Settings::instance().layerColor(mComponent->layer());
	const QBrush br(color);
	const QPen p(color);

	const float width = mComponent->width();
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
			addToGroup(track);
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
		addToGroup(circle);
	}

}
