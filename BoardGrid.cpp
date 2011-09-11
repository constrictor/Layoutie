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
	double gridSize = mBoard->gridSize();
	unsigned numSubdivisions = gSettings().numGridSubdivisions();

	SLFormat::UIntPoint startPoint = mBoard->origin(); //TODO: origin support

	const QPen& subLinePen(gSettings().gridSubPen());

	// vertical
	{
		double currPos = startPoint.x;
		
		unsigned i = 0;
		while(true)
		{
			i++;
			currPos = startPoint.x + i * gridSize;
			if (currPos >= mBoard->size().x)
				break;
			if (numSubdivisions == 0 || i % numSubdivisions != 0)
			{
				auto line = new QGraphicsLineItem(currPos, 0, currPos, mBoard->size().y);
				line->setPen(subLinePen);
				this->addToGroup(line);
			}
		}
	}

	// horizontal
	{
		double currPos = startPoint.y;

		unsigned i = 0;
		while(true)
		{
			i++;
			currPos = startPoint.y + i * gridSize;
			if (currPos >= mBoard->size().y)
				break;
			if (numSubdivisions == 0 || i % numSubdivisions != 0)
			{
				auto line = new QGraphicsLineItem(0, currPos, mBoard->size().x, currPos);
				line->setPen(subLinePen);
				this->addToGroup(line);
			}
		}
	}

	// draw bold lines
	if (numSubdivisions)
	{
		QPen mainLinePen(gSettings().gridMainPen());
		const double width = mainLinePen.widthF()  / mBoard->zoomLevel();
		mainLinePen.setWidthF(width);
		
		// vertical
		{
			double currPos = startPoint.x;
			
			unsigned i = 0;
			while(true)
			{
				i += numSubdivisions;
				currPos = startPoint.x + i * gridSize;
				if (currPos >= mBoard->size().x)
					break;
				auto line = new QGraphicsLineItem(currPos, width / 2, currPos, mBoard->size().y - width / 2);
				line->setPen(mainLinePen);
				this->addToGroup(line);
				mMainLines.push_back(line);
			}
		}

		// horizontal
		{
			double currPos = startPoint.y;

			unsigned i = 0;
			while(true)
			{
				i += numSubdivisions;
				currPos = startPoint.y + i * gridSize;
				if (currPos >= mBoard->size().y)
					break;
				auto line = new QGraphicsLineItem(width / 2, currPos, mBoard->size().x - width / 2, currPos);
				line->setPen(mainLinePen);
				this->addToGroup(line);
				mMainLines.push_back(line);
			}
		}
	}
}
