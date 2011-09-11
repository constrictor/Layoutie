#ifndef BOARD_SCENE_H
#define BOARD_SCENE_H

#include <QtGui>

#include <SLBoard.h>

#include "BoardView.h"

class BoardScene : public QGraphicsScene
{
	Q_OBJECT
public:
	BoardScene(SLFormat::Board* inBoard, BoardView* inParent);
	~BoardScene();
	
signals:
	void	modified();

private:
	void	updateGrid();

	SLFormat::Board* mBoard;
	QGraphicsItemGroup* mLayerGroups[SLFormat::Board::cNumberLayers];
};

#endif //BOARD_SCENE_H