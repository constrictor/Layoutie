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

#include "ComponentFactory.h"

#include <SLComponent.h>
#include <SLSMDComponent.h>
#include <SLTrackComponent.h>
#include <SLZoneComponent.h>

#include "Components/SMDItem.h"
#include "Components/TrackItem.h"
#include "Components/ZoneItem.h"

using namespace SLFormat;

ComponentItem* ComponentFactory::createItem(SLFormat::Component* inComponent)
{
	ComponentItem* res = nullptr;
	switch (inComponent->type())
	{
		case ComponentTypes::SMD:
			res = new SMDItem(static_cast<SMDComponent*>(inComponent));
			break;
		case ComponentTypes::Circle:
			break;
		case ComponentTypes::Pad:
			break;
		case ComponentTypes::Text:
			break;
		case ComponentTypes::Track:
			res = new TrackItem(static_cast<TrackComponent*>(inComponent));
			break;
		case ComponentTypes::Zone:
			res = new ZoneItem(static_cast<ZoneComponent*>(inComponent));
			break;
	}
	if (res)
		res->update();
	return res;
}
