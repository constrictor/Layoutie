#include "SMDItem.h"

#include <SLSMDComponent.h>

#include "Settings.h"

SMDItem::SMDItem(SLFormat::SMDComponent* inComponent)
	:mComponent(inComponent)
{
	update();
}

SMDItem::~SMDItem()
{
}

void SMDItem::update()
{
	QPolygonF poly;
	for (auto point : mComponent->rect().points)
	{
		poly << QPointF(point.x, - point.y);
	}
	poly << poly.first();
	setPolygon(poly);
	QColor color = Settings::instance().layerColor(mComponent->layer());
	QBrush br(color);
	setBrush(br);
	QPen p(color);
	setPen(p);
}
