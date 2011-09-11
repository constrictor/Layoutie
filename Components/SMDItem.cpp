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

#include "SMDItem.h"

#include "Settings.h"

#include <SLSMDComponent.h>

#include <QBrush>
#include <QPen>

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
	QColor color = gSettings().layerColor(mComponent->layer());
	QBrush br(color);
	setBrush(br);
	QPen p(color);
	setPen(p);
}
