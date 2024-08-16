#include <random>
#include "Board.h"
#include "Consts.h"

Board::Board(QGraphicsScene* scene)
	: _scene(scene)
	, _gen(_device())
{
	_scene->addItem(&_root);
	_root.setX(_scene->sceneRect().width() / 2
		- Consts::BOARD_ITEM_SIZE * Consts::BOARD_LENGTH / 2);
	_root.setY(_scene->sceneRect().height() / 2
		- Consts::BOARD_ITEM_SIZE * Consts::BOARD_LENGTH / 2);

	for (int row = 0; row < Consts::BOARD_LENGTH; ++row)
	{
		std::vector<QGraphicsPixmapItem*> rowItems(Consts::BOARD_LENGTH);
		_items.push_back(rowItems);

		for (int column = 0; column < Consts::BOARD_LENGTH; ++column)
		{
			addItem(row, column);
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

	std::uniform_int_distribution<int> dis(0, 13);

	QPixmap pixmap(Consts::paths[dis(_gen)].c_str());
	QPixmap scaled(pixmap.scaled(Consts::BOARD_ITEM_SIZE, Consts::BOARD_ITEM_SIZE));
	QGraphicsPixmapItem* item = new QGraphicsPixmapItem(scaled, &_root);
	item->setPos(column * Consts::BOARD_ITEM_SIZE, row * Consts::BOARD_ITEM_SIZE);

	_items[row][column] = item;
}

void Board::removeItem(int row, int column)
{
	auto* item = _items[row][column];

	if (item == nullptr)
		return;

	_items[row][column] = nullptr;
	item->setParentItem(nullptr);
	_scene->removeItem(item);

	delete item;
}
