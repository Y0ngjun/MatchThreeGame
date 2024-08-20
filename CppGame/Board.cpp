#include <random>

#include "Board.h"
#include "Consts.h"
#include "Item.h"

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
		std::vector<Item*> rowItems(Consts::BOARD_LENGTH);
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
	const std::string& path = Consts::paths[dis(_gen)];

	Item* item = new Item(this, path, row, column, &_root); // EventListener로 자신을 전달, 오버라이딩한 함수가 호출가능해짐.

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

void Board::moveItem(Item* item, int toRow, int toColumn)
{
	item->setRow(toRow); // 좌표값 변경
	item->setColumn(toColumn);

	item->setPos(toColumn * Consts::BOARD_ITEM_SIZE, toRow * Consts::BOARD_ITEM_SIZE); // 보이는 위치 변경

	_items[toRow][toColumn] = item; // 벡터내에서 변경
}

void Board::exchange(int row0, int column0, int row1, int column1)
{
	Item* item0 = _items[row0][column0];
	Item* item1 = _items[row1][column1];

	moveItem(item0, row1, column1); // 교환
	moveItem(item1, row0, column0);
}

void Board::itemDragEvent(Item* item, Item::Direction dir)
{
	int row0 = item->row(); // 기존 아이템
	int column0 = item->column();

	int row1 = row0; // 교환할 아이템
	int column1 = column0;

	switch (dir) // 방향
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

	if (row1 < 0 || column1 < 0) // 보드판 바깥 예외처리
		return;
	if (row1 >= Consts::BOARD_LENGTH || column1 >= Consts::BOARD_LENGTH)
		return;

	Item* item1 = _items[row1][column1];
	if (item1 == nullptr)
		return;

	exchange(row0, column0, row1, column1); // 교환
}