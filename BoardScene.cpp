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
#include "Components/ComponentItem.h"

#include <SLBoard.h>
#include <SLComponent.h>
#include "Settings.h"

#include <assert.h>

BoardScene::BoardScene(const SLFormat::Board* inBoard, QWidget* inParent)
    :QGraphicsScene(inParent), mBoard(inBoard)
{
    this->setSceneRect(0, 0, mBoard->size().x, mBoard->size().y);

    // background
    const unsigned activeLayer = mBoard->activeLayer();
    QBrush brush(gSettings().backgroundColor());
    QPen pen(brush.color());
    mBackground = this->addRect(this->sceneRect(), pen, brush);

    // ground planes (initially invisible)
    for (unsigned i = 0; i < SLFormat::Board::cNumberLayers; i++)
    {
        unsigned layer = gSettings().layerFromOrderNumber(i);
        if (mBoard->isGroundPlaneSupported(layer))
        {
            QGraphicsItemGroup* plane = new QGraphicsItemGroup;
            mGroundPlanes[layer] = plane;

            addItem(plane);
            plane->setVisible(false);

            auto rect = new QGraphicsRectItem(this->sceneRect());
            const QColor color = gSettings().groundPlaneLayerColor(layer);
            rect->setBrush(QBrush(color));
            rect->setPen(QPen(color));
            plane->addToGroup(rect);
        }
        else
        {
            mGroundPlanes[layer] = nullptr;
        }
    }

    // grid
    mGrid = new BoardGrid(mBoard);
    addItem(mGrid);

    // layers
    for (unsigned i = 0; i < SLFormat::Board::cNumberLayers; i++)
    {
        unsigned layer = gSettings().layerFromOrderNumber(i);
        mLayerGroups[layer] = new QGraphicsItemGroup;
        addItem(mLayerGroups[layer]);
    }

    mDrillHoles = new QGraphicsItemGroup;
    addItem(mDrillHoles);

    // components on layers
    for (auto comp : mBoard->components())
    {
        ComponentItem* item = ComponentFactory::createItem(comp);
        if (item)
        {
            unsigned layer = comp->layer();
            mLayerGroups[layer]->addToGroup(item->mainItem());
            if (mBoard->isGroundPlaneSupported(layer))
            {
                mGroundPlanes[layer]->addToGroup(item->groundPlaneItem());
            }
            mDrillHoles->addToGroup(item->drillItem());
        }
    }

    // show ground plane for active layer
    if (mBoard->isGroundPlaneEnabled(activeLayer))
    {
        assert(mGroundPlanes[activeLayer]);
        mGroundPlanes[activeLayer]->setVisible(true);
    }
}

BoardScene::~BoardScene()
{

}

void BoardScene::zoomed()
{
}

bool BoardScene::isLayerShown(unsigned inLayerNumber) const
{
    assert(inLayerNumber < SLFormat::Board::cNumberLayers);
    return mLayerGroups[inLayerNumber]->isVisible();
}


void BoardScene::showLayer(unsigned inLayerNumber, bool inShow)
{
    assert(inLayerNumber < SLFormat::Board::cNumberLayers);
    mLayerGroups[inLayerNumber]->setVisible(inShow);
    if (mGroundPlanes[inLayerNumber])
    {
        mGroundPlanes[inLayerNumber]->setVisible(inShow && mBoard->isGroundPlaneEnabled(inLayerNumber));
    }
}

bool BoardScene::isGridShown() const
{
    return mGrid->isVisible();
}

void BoardScene::showGrid(bool inShow)
{
    mGrid->setVisible(inShow);
}

bool BoardScene::isDrillHolesShown() const
{
    return mDrillHoles->isVisible();
}

void BoardScene::showDrillHoles(bool inShow)
{
    mDrillHoles->setVisible(inShow);
}
