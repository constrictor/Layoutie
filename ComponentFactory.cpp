#include "ComponentFactory.h"

#include <SLComponent.h>
#include <SLSMDComponent.h>
#include <SLTrackComponent.h>
#include <SLZoneComponent.h>

#include "Components/SMDItem.h"
#include "Components/TrackItem.h"
#include "Components/ZoneItem.h"

using namespace SLFormat;

QGraphicsItem* ComponentFactory::createItem(SLFormat::Component* inComponent)
{
	switch (inComponent->type())
	{
		case ComponentTypes::SMD:
			return new SMDItem(static_cast<SMDComponent*>(inComponent));
			break;
		case ComponentTypes::Circle:
			return nullptr;
			break;
		case ComponentTypes::Pad:
			return nullptr;
			break;
		case ComponentTypes::Text:
			return nullptr;
			break;
		case ComponentTypes::Track:
			return new TrackItem(static_cast<TrackComponent*>(inComponent));
			break;
		case ComponentTypes::Zone:
			return new ZoneItem(static_cast<ZoneComponent*>(inComponent));
			break;
		default:
			return nullptr;
	}
}
