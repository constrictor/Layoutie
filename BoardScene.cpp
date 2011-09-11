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

#include "BoardScene.h"

#include "ComponentFactory.h"
#include "BoardView.h"
#include "BoardGrid.h"

#include <SLBoard.h>
#include <SLComponent.h>
#include "Settings.h"

#include <assert.h>

BoardScene::BoardScene(SLFormat::Board* inBoard, BoardView* inParent)
	:QGraphicsScene(inParent), mBoard(inBoard)
{
	this->setSceneRect(0, 0, mBoard->size().x, mBoard->size().y);

	QBrush brush(Qt::black);
	QPen pen(brush.color());
	mBackground = this->addRect(this->sceneRect(), pen, brush);

	mGrid = new BoardGrid(mBoard);
	addItem(mGrid);

	for (unsigned i = 0; i < SLFormat::Board::cNumberLayers; i++)
	{
		unsigned layer = gSettings().layerFromOrderNumber(i);
		mLayerGroups[layer] = new QGraphicsItemGroup;
		addItem(mLayerGroups[layer]);
	}

	for (auto comp : mBoard->components())
	{
		QGraphicsItem* item = ComponentFactory::createItem(comp);
		if (item)
		{
			unsigned layer = comp->layer();
			mLayerGroups[layer]->addToGroup(item);
		}
	}
}

BoardScene::~BoardScene()
{

}

void BoardScene::zoomed()
{
	mGrid->updateGrid();
}
