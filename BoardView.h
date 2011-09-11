#ifndef BOARD_VIEW_H
#define BOARD_VIEW_H

#include <QtGui>

namespace SLFormat
{
	class Board;
}

class BoardScene;

class BoardView : public QGraphicsView
{
	Q_OBJECT
public:
	BoardView(QWidget* inParent, SLFormat::Board* inBoard);
	~BoardView();

signals:
	void	modified();

protected:
	virtual void wheelEvent(QWheelEvent* inEvent);

private:
	void	updateZoom();

	SLFormat::Board* mBoard;
	BoardScene*	mScene;
};

#endif //BOARD_VIEW_H
