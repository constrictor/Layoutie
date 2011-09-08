#ifndef BOARD_H
#define BOARD_H

#include <QtGui>

namespace SLFormat
{
	class Board;
}

class BoardView : public QWidget
{
	Q_OBJECT
public:
	BoardView(QWidget* inParent, SLFormat::Board* inBoard);
	~BoardView();

signals:
	void	modified();

private:
	SLFormat::Board* mBoard;
};

#endif //BOARD_H