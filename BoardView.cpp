#include "BoardView.h"

#include "BoardScene.h"

#include <SLBoard.h>

BoardView::BoardView(QWidget* inParent, SLFormat::Board* inBoard)
	: QGraphicsView(inParent), mBoard(inBoard)
{
	mScene = new BoardScene(inBoard, this);
	this->setScene(mScene);

	setTransformationAnchor(AnchorUnderMouse);
	updateZoom();
}

BoardView::~BoardView()
{

}

void BoardView::wheelEvent(QWheelEvent* inEvent)
{
	int turns = inEvent->delta() / 8 / 15;
	double change = pow(1.4, turns);
	double newZoomLevel = mBoard->zoomLevel() * change;
	double min = 200.0 / qMax(mScene->height(), mScene->width());
	if (newZoomLevel < min)
		newZoomLevel = min;
	if (newZoomLevel > 1)
		newZoomLevel = 1;
	mBoard->setZoomLevel(newZoomLevel);
	updateZoom();
}

void BoardView::updateZoom()
{
	double zoom = mBoard->zoomLevel();
	this->resetTransform();
	scale(zoom, zoom);
}
