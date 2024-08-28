#include <random>

#include "Board.h"
#include "Consts.h"
#include "Item.h"

Board::Board(QGraphicsScene* scene)
	: _scene(scene) // scene 설정
	, _gen(_device()) // 난수 생성
	, _moveCount(0)
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

	refresh(); // 처음에 생기는 3match 제거
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

	std::uniform_int_distribution<int> dis(0, Consts::MAX_ITEM_TYPES); // 난수를 균등하게 생성
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

void Board::moveItem(int fromRow, int fromColumn, int toRow, int toColumn)
{
	Item* item = _items[fromRow][fromColumn];
	if (item == nullptr)
		return;
	moveItem(item, toRow, toColumn);
}

void Board::moveItem(Item* item, int toRow, int toColumn)
{
	item->setRow(toRow); // 좌표값 변경
	item->setColumn(toColumn);
	_moveCount++;
	item->moveTo(toColumn * Consts::BOARD_ITEM_SIZE, toRow * Consts::BOARD_ITEM_SIZE); // 애니메이션
	_items[toRow][toColumn] = item; // 벡터내에서 변경

}

void Board::exchangeItems(int row0, int column0, int row1, int column1, bool canRevert)
{
	Item* item0 = _items[row0][column0];
	Item* item1 = _items[row1][column1];

	item0->setRow(row1);
	item1->setRow(row0);

	item0->setColumn(column1);
	item1->setColumn(column0);

	_items[row0][column0] = item1;
	_items[row1][column1] = item0;

	item0->moveTo(item1, canRevert);
	item1->moveTo(item0, canRevert);

	_moveCount += 2;
}

bool Board::refresh()
{
	MatchSet matched = matchedItems(); // 3개 이상 연속된 아이템 모음
	if (matched.size() < 3) // 3개 이하라면 종료
		return false;

	for (const auto& pair : matched) // 3개 이상이라면 아이템 제거
	{
		removeItem(pair.first, pair.second);
	}

	// 빈곳에 아이템 내리기
	for (int column = 0; column < _items[0].size(); ++column)
	{
		for (int row = _items.size() - 1; row >= 0; --row)
		{
			if (_items[row][column] != nullptr)
			{
				continue;
			}
			for (int i = row - 1; i >= 0; --i)
			{
				if (_items[i][column] != nullptr)
				{
					moveItem(i, column, row, column);
					_items[i][column] = nullptr;
					break;
				}
			}
		}
	}

	// 순회 하며 열마다 빈곳의 개수를 저장한다.
	std::vector<int> emptyCounts;
	for (int column = 0; column < _items[0].size(); ++column)
	{
		int emptyCount = 0;
		for (int row = 0; row < _items.size(); ++row)
		{
			if (_items[row][column] == nullptr)
			{
				emptyCount++;
			}
			else
			{
				break;
			}
		}
		emptyCounts.push_back(emptyCount);
	}

	// 새 아이템 채우기
	for (int column = 0; column < _items[0].size(); ++column)
	{
		for (int row = 0; row < _items.size(); ++row)
		{
			if (_items[row][column] == nullptr)
			{
				addItem(row, column);

				Item* item = _items[row][column];
				item->setY(item->y() - emptyCounts[column] * Consts::BOARD_ITEM_SIZE); // 빈곳의 개수만큼 위에서 떨어진다.
				moveItem(row, column, row, column);
			}
			else
			{
				break;
			}
		}
	}
	return true;
}

// 3개이상 연속된 아이템을 찾아서 리턴하는 함수
MatchSet Board::matchedItems() const
{
	MatchSet matched;
	for (int row = 0; row < _items.size(); ++row)
	{
		for (int column = 0; column < _items[row].size(); ++column)
		{
			MatchSet m = matchedItems(row, column);
			if (m.size() >= 3)
			{
				matched.insert(m.begin(), m.end());
			}
		}
	}

	return matched;
}

// 한개의 아이템을 기준으로 가로세로 검사한다.
MatchSet Board::matchedItems(int row, int column) const
{
	MatchSet horizontalMatched = matchedItemsHorizontal(row, column);
	MatchSet verticalMatched = matchedItemsVertical(row, column);
	MatchSet matched;
	if (horizontalMatched.size() >= 3)
		matched.insert(horizontalMatched.begin(), horizontalMatched.end());
	if (verticalMatched.size() >= 3)
		matched.insert(verticalMatched.begin(), verticalMatched.end());
	return matched;
}

// 가로 검사
MatchSet Board::matchedItemsHorizontal(int row, int column) const
{
	Item* item = _items[row][column];
	if (item == nullptr)
		return {};

	MatchSet leftMatched;
	for (int i = column - 1; i >= 0; --i)
	{
		if (_items[row][i] && _items[row][i]->path() == item->path())
		{
			leftMatched.insert({ row,i });
		}
		else
		{
			break;
		}
	}

	MatchSet rightMatched;
	for (int i = column + 1; i < _items[row].size(); ++i)
	{
		if (_items[row][i] && _items[row][i]->path() == item->path())
		{
			rightMatched.insert({ row,i });
		}
		else
		{
			break;
		}

	}

	if (leftMatched.size() + rightMatched.size() + 1 >= 3)
	{
		leftMatched.insert(rightMatched.begin(), rightMatched.end());
		leftMatched.insert({ row,column });
		return leftMatched;
	}
	else
	{
		return {};
	}
}

// 세로 검사
MatchSet Board::matchedItemsVertical(int row, int column) const
{
	Item* item = _items[row][column];
	if (item == nullptr)
		return {};

	MatchSet upMatched;
	for (int i = row - 1; i >= 0; --i)
	{
		if (_items[i][column] && _items[i][column]->path() == item->path())
		{
			upMatched.insert({ i,column });
		}
		else
		{
			break;
		}
	}

	MatchSet downMatched;
	for (int i = row + 1; i < _items.size(); ++i)
	{
		if (_items[i][column] && _items[i][column]->path() == item->path())
		{
			downMatched.insert({ i,column });
		}
		else
		{
			break;
		}

	}

	if (upMatched.size() + downMatched.size() + 1 >= 3)
	{
		upMatched.insert(downMatched.begin(), downMatched.end());
		upMatched.insert({ row,column });
		return upMatched;
	}
	else
	{
		return {};
	}
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

	exchangeItems(row0, column0, row1, column1, true); // 교환
}

void Board::itemMoveFinished(Item* item0, Item* item1, bool canRevert)
{
	if (--_moveCount > 0)
	{
		return;
	}

	if (refresh())
		return;

	if(!canRevert)
		return;

	if (item0 == nullptr || item1 == nullptr)
		return;

	// revert exchange
	exchangeItems(item0->row(), item0->column(), item1->row(), item1->column(), false);
}
