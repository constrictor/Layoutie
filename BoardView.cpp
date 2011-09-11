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

#include "BoardView.h"

#include "BoardScene.h"

#include <QWheelEvent>

#include <SLBoard.h>

#include <cmath>

BoardView::BoardView(QWidget* inParent, SLFormat::Board* inBoard)
	: QGraphicsView(inParent), mBoard(inBoard)
{
	mScene = new BoardScene(inBoard, this);
	this->setScene(mScene);

	setTransformationAnchor(AnchorUnderMouse);
	setRenderHint(QPainter::Antialiasing);
	updateZoom();
}

BoardView::~BoardView()
{

}

void BoardView::wheelEvent(QWheelEvent* inEvent)
{
	int turns = inEvent->delta() / 8 / 15;
	double change = std::pow(1.4, turns);
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
	mScene->zoomed();
}
