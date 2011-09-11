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
	BoardScene(SLFormat::Board* inBoard, BoardView* inParent);
	~BoardScene();

	void zoomed();
	
signals:
	void	modified();

private:

	SLFormat::Board* mBoard;
	QGraphicsItemGroup* mLayerGroups[SLFormat::Board::cNumberLayers];
	QGraphicsRectItem* mBackground;
	BoardGrid* mGrid;
};

#endif //BOARD_SCENE_H