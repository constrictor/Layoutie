#include "BoardScene.h"

#include "ComponentFactory.h"

#include <SLBoard.h>
#include <SLComponent.h>
#include "Settings.h"

#include <assert.h>

BoardScene::BoardScene(SLFormat::Board* inBoard, BoardView* inParent)
	:QGraphicsScene(inParent)
{
	this->setSceneRect(0, 0, inBoard->size().x, inBoard->size().y);
	updateGrid();

	for (unsigned i = 0; i < SLFormat::Board::cNumberLayers; i++)
	{
		unsigned layer = Settings::instance().layerFromOrderNumber(i);
		mLayerGroups[layer] = new QGraphicsItemGroup;
		addItem(mLayerGroups[layer]);
	}

	for (auto comp : inBoard->components())
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

void BoardScene::updateGrid()
{
	QBrush brush(Qt::black);
	QPen pen(brush.color());
	this->addRect(this->sceneRect(), pen, brush);
	//TODO: Grid
}