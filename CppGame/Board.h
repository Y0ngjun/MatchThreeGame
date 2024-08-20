#pragma once

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsRectItem>

#include <vector>
#include <random>

#include "Item.h"

class Board : public Item::EventListener // EventListener로 제어하기 위해 인터페이스 상속
{
private:
	QGraphicsScene* _scene;
	QGraphicsRectItem _root; // 보드판 묶음
	std::vector<std::vector<Item*>> _items; // item 배열
	std::random_device _device; // 난수 생성
	std::mt19937 _gen;

public:
	Board(QGraphicsScene* scene);
	~Board();
	void addItem(int row, int column);
	void removeItem(int row, int column);

	void moveItem(Item* item, int toRow, int toColumn); // 아이템 위치 이동
	void exchange(int row0, int column0, int row1, int column1); // 아이템 교환
	virtual void itemDragEvent(Item* item, Item::Direction dir); // 오버라이드
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