#include <random>

#include "Board.h"
#include "Consts.h"

Board::Board(QGraphicsScene* scene)
	: _scene(scene) // scene 설정
	, _gen(_device()) // 난수 생성
{
	_scene->addItem(&_root); // scene에 보드판 묶음 추가

	_root.setX(_scene->sceneRect().width() / 2 // 보드판 위치 설정
		- Consts::BOARD_ITEM_SIZE * Consts::BOARD_LENGTH / 2);
	_root.setY(_scene->sceneRect().height() / 2
		- Consts::BOARD_ITEM_SIZE * Consts::BOARD_LENGTH / 2);

	for (int row = 0; row < Consts::BOARD_LENGTH; ++row) // vector에 item 넣기
	{
		std::vector<QGraphicsPixmapItem*> rowItems(Consts::BOARD_LENGTH);
		_items.push_back(rowItems); // 행 삽입

		for (int column = 0; column < Consts::BOARD_LENGTH; ++column)
		{
			addItem(row, column); // 각 열에 item 추가
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

	std::uniform_int_distribution<int> dis(0, 13); // 난수를 0 ~ 13 까지 균등하게 생성

	QPixmap pixmap(Consts::paths[dis(_gen)].c_str()); // item 선택
	QPixmap scaled(pixmap.scaled(Consts::BOARD_ITEM_SIZE, Consts::BOARD_ITEM_SIZE)); // 크기 조정
	QGraphicsPixmapItem* item = new QGraphicsPixmapItem(scaled, &_root); // 동적 할당, _root를 부모로 설정

	item->setPos(column * Consts::BOARD_ITEM_SIZE, row * Consts::BOARD_ITEM_SIZE); // 부모위치를 기준으로 item 위치 설정
	_items[row][column] = item; // 배열에 item 삽입
}

void Board::removeItem(int row, int column)
{
	auto* item = _items[row][column];

	if (item == nullptr) // 존재하지 않는다면 종료
		return;

	_items[row][column] = nullptr; // 벡터 원소 해제
	item->setParentItem(nullptr); // 부모 정보 해제
	_scene->removeItem(item); // scene에서 해제
	delete item; // 동적할당 해제
}
