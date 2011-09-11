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

#include "CircleItem.h"

#include "Settings.h"

#include <SLCircleComponent.h>

#include <QBrush>
#include <QPen>
#include <QGraphicsItemGroup>

#include <cmath>

CircleItem::CircleItem(SLFormat::CircleComponent* inComponent)
	:mComponent(inComponent)
{
}

void CircleItem::createItem(QGraphicsItemGroup* inOutItem, bool inIsMainNotGround)
{
	if (inIsMainNotGround && mComponent->isCutoutArea())
		return;
	const QColor color = inIsMainNotGround ?
		gSettings().layerColor(mComponent->layer())
		: gSettings().backgroundColor();

	float width = mComponent->width();
	if (!inIsMainNotGround && !mComponent->isCutoutArea())
	{
		width += mComponent->groundPlaneDistance() * 2;
	}
	QPen p(color, width);
	p.setCapStyle(Qt::RoundCap);
	p.setJoinStyle(Qt::RoundJoin);

	const float radius = mComponent->diameter() / 2;

	QRectF rect(mComponent->center().x - radius, -mComponent->center().y - radius,
		mComponent->diameter(), mComponent->diameter());
	
	QPainterPath path;
	path.arcMoveTo(rect, mComponent->angle());
	unsigned span = mComponent->spanAngle();
	if (span == 0)
		span = 360;
	path.arcTo(rect, mComponent->angle(), span);
	if (mComponent->isFilled())
	{
		path.closeSubpath();
	}
	auto item = new QGraphicsPathItem(path);
	item->setPen(p);
	if (mComponent->isFilled())
	{
		const QBrush br(color);
		item->setBrush(br);
	}
	inOutItem->addToGroup(item);
}
