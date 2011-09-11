#ifndef SMD_ITEM_H
#define SMD_ITEM_H

#include <QtGui>

namespace SLFormat
{
	class SMDComponent;
}

class SMDItem : public QGraphicsPolygonItem
{
public:
	SMDItem(SLFormat::SMDComponent* inComponent);
	~SMDItem();

	void update();
private:
	SLFormat::SMDComponent* mComponent;
};

#endif //SMD_ITEM_H