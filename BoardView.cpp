#include "BoardView.h"

#include <SLBoard.h>

BoardView::BoardView(QWidget* inParent, SLFormat::Board* inBoard)
	: QWidget(inParent), mBoard(inBoard)
{
	//TODO: Create a scene
}

BoardView::~BoardView()
{

}
