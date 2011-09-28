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

#include "BoardGrid.h"

#include "Settings.h"

#include <SLBoard.h>

#include <QPen>

BoardGrid::BoardGrid(SLFormat::Board* inBoard)
	: QGraphicsItemGroup(), mBoard(inBoard)
{
	updateGrid();
}

void BoardGrid::updateGrid()
{
	for (auto mainLine : mMainLines)
	{
		this->removeFromGroup(mainLine);
		delete mainLine;
	}
	mMainLines.clear();
	unsigned gridSize = static_cast<unsigned>(mBoard->gridSize());
	unsigned numSubdivisions = gSettings().numGridSubdivisions();

	const QPen& subLinePen(gSettings().gridSubPen());
	const QPen& mainLinePen(gSettings().gridMainPen());

	const QPointF origin(mBoard->origin().x / 1000, mBoard->origin().y / 1000);

	// vertical
	{
		unsigned i = 0;
		while(true)
		{
			i++;
			float currPos = origin.x() + i * gridSize;
			if (currPos >= mBoard->size().x)
				break;
			auto line = new QGraphicsLineItem(currPos, 0, currPos, mBoard->size().y);
			line->setPen((numSubdivisions == 0 || i % numSubdivisions != 0) ? subLinePen : mainLinePen);
			this->addToGroup(line);
		}
		i = 0;
		while(true)
		{
			i++;
			float currPos = origin.x() - i * gridSize;
			if (currPos <= 0)
				break;
			auto line = new QGraphicsLineItem(currPos, 0, currPos, mBoard->size().y);
			line->setPen((numSubdivisions == 0 || i % numSubdivisions != 0) ? subLinePen : mainLinePen);
			this->addToGroup(line);
		}
	}

	// horizontal
	{
		unsigned i = 0;
		while(true)
		{
			i++;
			float currPos = origin.y() + i * gridSize;
			if (currPos >= mBoard->size().y)
				break;
			auto line = new QGraphicsLineItem(0, currPos, mBoard->size().x, currPos);
			line->setPen((numSubdivisions == 0 || i % numSubdivisions != 0) ? subLinePen : mainLinePen);
			this->addToGroup(line);
		}
		i = 0;
		while(true)
		{
			i++;
			float currPos = origin.y() - i * gridSize;
			if (currPos <= 0)
				break;
			auto line = new QGraphicsLineItem(0, currPos, mBoard->size().x, currPos);
			line->setPen((numSubdivisions == 0 || i % numSubdivisions != 0) ? subLinePen : mainLinePen);
			this->addToGroup(line);
		}
	}

	// origin point
	if (mBoard->origin().x || mBoard->origin().y)
	{
		const QPen& originPen(gSettings().gridOriginPen());
		// vertical
		{
			float currPos = mBoard->origin().x / 1000;
			if (currPos < mBoard->size().x)
			{
				auto line = new QGraphicsLineItem(currPos, 0, currPos, mBoard->size().y);
				line->setPen(originPen);
				this->addToGroup(line);
			}
		}

		// horizontal
		{
			double currPos = mBoard->origin().y / 1000;
			if (currPos < mBoard->size().y)
			{
				auto line = new QGraphicsLineItem(0, currPos, mBoard->size().x, currPos);
				line->setPen(originPen);
				this->addToGroup(line);
			}
		}
	}
}
