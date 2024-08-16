#include <random>

#include "Board.h"
#include "Consts.h"

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
		std::vector<QGraphicsPixmapItem*> rowItems(Consts::BOARD_LENGTH);
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

	QPixmap pixmap(Consts::paths[dis(_gen)].c_str()); // item ����
	QPixmap scaled(pixmap.scaled(Consts::BOARD_ITEM_SIZE, Consts::BOARD_ITEM_SIZE)); // ũ�� ����
	QGraphicsPixmapItem* item = new QGraphicsPixmapItem(scaled, &_root); // ���� �Ҵ�, _root�� �θ�� ����

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
