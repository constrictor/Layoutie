#ifndef ZONE_ITEM_H
#define ZONE_ITEM_H

#include <QtGui>

namespace SLFormat
{
	class ZoneComponent;
}

class ZoneItem : public QGraphicsItemGroup
{
public:
	ZoneItem(SLFormat::ZoneComponent* inComponent);
	~ZoneItem();
	
	void update();
private:
	void	addLineToPolygon(QPolygonF& outPolygon1, QPolygonF& outPolygon2, QPointF inPoint1, QPointF inPoint2, qreal inRadius);

	SLFormat::ZoneComponent* mComponent;
};

#endif //ZONE_ITEM_H