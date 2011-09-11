#ifndef TRACK_ITEM_H
#define TRACK_ITEM_H

#include <QtGui>

namespace SLFormat
{
	class TrackComponent;
}

class TrackItem : public QGraphicsItemGroup
{
public:
	TrackItem(SLFormat::TrackComponent* inComponent);
	~TrackItem();
	
	void update();
private:
	SLFormat::TrackComponent* mComponent;
};

#endif //TRACK_ITEM_H