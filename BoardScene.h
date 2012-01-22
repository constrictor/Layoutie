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

#ifndef BOARD_SCENE_H
#define BOARD_SCENE_H

#include <QGraphicsScene>

#include <SLBoard.h>

class BoardView;

class BoardGrid;

class BoardScene : public QGraphicsScene
{
    Q_OBJECT
public:
    BoardScene(const SLFormat::Board* inBoard, QWidget* inParent);
    ~BoardScene();

    void zoomed();

    virtual void    showGrid(bool inShow = true);
    bool    isGridShown() const;

    virtual void    showLayer(unsigned inLayerNumber, bool inShow = true);
    bool    isLayerShown(unsigned inLayerNumber) const;

    virtual void    showDrillHoles(bool inShow = true);
    bool    isDrillHolesShown() const;
private:

    const SLFormat::Board* mBoard;
    QGraphicsItemGroup* mLayerGroups[SLFormat::Board::cNumberLayers];
    QGraphicsItemGroup* mGroundPlanes[SLFormat::Board::cNumberLayers];
    QGraphicsItemGroup* mDrillHoles;
    QGraphicsRectItem* mBackground;
    BoardGrid* mGrid;
};

#endif //BOARD_SCENE_H