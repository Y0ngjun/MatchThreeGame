#pragma once

#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsPixmapItem>
#include <QtWidgets/QGraphicsRectItem>

#include <vector>
#include <random>

#include "Item.h"

class Board : public Item::EventListener // EventListener�� �����ϱ� ���� �������̽� ���
{
private:
	QGraphicsScene* _scene;
	QGraphicsRectItem _root; // ������ ����
	std::vector<std::vector<Item*>> _items; // item �迭
	std::random_device _device; // ���� ����
	std::mt19937 _gen;

public:
	Board(QGraphicsScene* scene);
	~Board();
	void addItem(int row, int column);
	void removeItem(int row, int column);

	void moveItem(Item* item, int toRow, int toColumn); // ������ ��ġ �̵�
	void exchange(int row0, int column0, int row1, int column1); // ������ ��ȯ
	virtual void itemDragEvent(Item* item, Item::Direction dir); // �������̵�
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