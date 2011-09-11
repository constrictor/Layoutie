#ifndef COMPONENT_FACTORY_H
#define COMPONENT_FACTORY_H

namespace SLFormat
{
	class Component;
}

class QGraphicsItem;

namespace ComponentFactory
{
	QGraphicsItem* createItem(SLFormat::Component* inComponent);
}

#endif //COMPONENT_FACTORY_H