#include <random>

#include "Board.h"
#include "Consts.h"
#include "Item.h"

Board::Board(QGraphicsScene* scene)
	: _scene(scene) // scene ����
	, _gen(_device()) // ���� ����
{
	_scene->addItem(&_root); // scene�� ������ ���� �߰�

	_root.setX(_scene->sceneRect().width() / 2 // ������ ��ġ ����
		- Consts::BOARD_ITEM_SIZE * Consts::BOARD_LENGTH / 2);
	_root.setY(_scene->sceneRect().height() / 2
		- Consts::BOARD_ITEM_SIZE * Consts::BOARD_LENGTH / 2);

	for (int row = 0; row < Consts::BOARD_LENGTH; ++row) // vector�� item �ֱ�
	{
		std::vector<Item*> rowItems(Consts::BOARD_LENGTH);
		_items.push_back(rowItems); // �� ����

		for (int column = 0; column < Consts::BOARD_LENGTH; ++column)
		{
			addItem(row, column); // �� ���� item �߰�
		}
	}
}

Board::~Board()
{
	for (int row = 0; row < _items.size(); ++row)
	{
		for (int column = 0; column < _items[row].size(); ++column)
		{
			removeItem(row, column);
		}
	}
}

void Board::addItem(int row, int column)
{

	std::uniform_int_distribution<int> dis(0, 13); // ������ 0 ~ 13 ���� �յ��ϰ� ����
	const std::string& path = Consts::paths[dis(_gen)];

	Item* item = new Item(this, path, row, column, &_root); // EventListener�� �ڽ��� ����, �������̵��� �Լ��� ȣ�Ⱑ������.

	item->setPos(column * Consts::BOARD_ITEM_SIZE, row * Consts::BOARD_ITEM_SIZE); // �θ���ġ�� �������� item ��ġ ����

	_items[row][column] = item; // �迭�� item ����
}

void Board::removeItem(int row, int column)
{
	auto* item = _items[row][column];

	if (item == nullptr) // �������� �ʴ´ٸ� ����
		return;

	_items[row][column] = nullptr; // ���� ���� ����
	item->setParentItem(nullptr); // �θ� ���� ����
	_scene->removeItem(item); // scene���� ����
	delete item; // �����Ҵ� ����
}

void Board::moveItem(Item* item, int toRow, int toColumn)
{
	item->setRow(toRow); // ��ǥ�� ����
	item->setColumn(toColumn);

	item->setPos(toColumn * Consts::BOARD_ITEM_SIZE, toRow * Consts::BOARD_ITEM_SIZE); // ���̴� ��ġ ����

	_items[toRow][toColumn] = item; // ���ͳ����� ����
}

void Board::exchange(int row0, int column0, int row1, int column1)
{
	Item* item0 = _items[row0][column0];
	Item* item1 = _items[row1][column1];

	moveItem(item0, row1, column1); // ��ȯ
	moveItem(item1, row0, column0);
}

void Board::itemDragEvent(Item* item, Item::Direction dir)
{
	int row0 = item->row(); // ���� ������
	int column0 = item->column();

	int row1 = row0; // ��ȯ�� ������
	int column1 = column0;

	switch (dir) // ����
	{
	case Item::Direction::Left:
		column1--;
		break;
	case Item::Direction::Right:
		column1++;
		break;
	case Item::Direction::Up:
		row1--;
		break;
	case Item::Direction::Down:
		row1++;
		break;
	}

	if (row1 < 0 || column1 < 0) // ������ �ٱ� ����ó��
		return;
	if (row1 >= Consts::BOARD_LENGTH || column1 >= Consts::BOARD_LENGTH)
		return;

	Item* item1 = _items[row1][column1];
	if (item1 == nullptr)
		return;

	exchange(row0, column0, row1, column1); // ��ȯ
}