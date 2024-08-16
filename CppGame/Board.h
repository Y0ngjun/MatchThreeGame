#pragma once

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsRectItem>

#include <vector>
#include <random>

class Board
{
private:
	QGraphicsScene* _scene;
	QGraphicsRectItem _root; // 보드판 묶음
	std::vector<std::vector<QGraphicsPixmapItem*>> _items; // item 배열
	std::random_device _device; // 난수 생성
	std::mt19937 _gen;

public:
	Board(QGraphicsScene* scene);
	~Board();
	void addItem(int row, int column);
	void removeItem(int row, int column);
};

/*
	items
	1 2 3 -> _items[0], row0
	4 5 6 -> _items[1], row1
	7 8 9 -> _items[2], row2

	_items[0][0] -> 1
	_items[0][1] -> 2
	_items[0][2] -> 3
*/