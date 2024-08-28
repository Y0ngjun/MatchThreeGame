#pragma once

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsRectItem>

#include <vector>
#include <random>
#include <set>

#include "Item.h"

using MatchPair = std::pair<int, int>; // 앨리어스
using MatchSet = std::set<MatchPair>;

class Board : public Item::EventListener // EventListener로 제어하기 위해 인터페이스 상속
{
private:
	QGraphicsScene* _scene;
	QGraphicsRectItem _root; // 보드판 묶음
	std::vector<std::vector<Item*>> _items; // item 배열
	std::random_device _device; // 난수 생성
	std::mt19937 _gen;

	int _moveCount;

public:
	Board(QGraphicsScene* scene);
	~Board();
	void addItem(int row, int column);
	void removeItem(int row, int column);

	void moveItem(int fromRow, int fromColumn, int toRow, int toColumn);
	void moveItem(Item* item, int toRow, int toColumn); // 아이템 위치 이동

	void exchangeItems(int row0, int column0, int row1, int column1, bool canRevert); // 아이템 교환
	bool refresh();
	MatchSet matchedItems() const;
	MatchSet matchedItems(int row, int column) const;
	MatchSet matchedItemsHorizontal(int row, int column) const;
	MatchSet matchedItemsVertical(int row, int column) const;


	virtual void itemDragEvent(Item* item, Item::Direction dir) override; // 오버라이딩
	virtual void itemMoveFinished(Item* item0, Item* item1, bool canRevert) override; // 오버라이딩
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