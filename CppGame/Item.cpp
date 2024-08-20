#include "Item.h"
#include "Consts.h"

#include <QtWidgets/QGraphicsSceneMouseEvent>

Item::Item(EventListener* listener, const std::string& path, int row, int column, QGraphicsItem* parent)
	: QGraphicsPixmapItem(
		QPixmap(path.c_str()).scaled(Consts::BOARD_ITEM_SIZE, Consts::BOARD_ITEM_SIZE)
		, parent)
	, _listener(listener)
	, _path(path)
	, _row(row)
	, _column(column)
{

}

std::string Item::path() const
{
	return _path;
}

int Item::row() const
{
	return _row;
}

int Item::column() const
{
	return _column;
}

void Item::setRow(int row)
{
	_row = row;
}

void Item::setColumn(int column)
{
	_column = column;
}

void Item::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
	_pressPos = event->pos();
}

void Item::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
	QPointF releasePos = event->pos();
	QPointF diff = releasePos - _pressPos;
	Direction dir;

	if (diff.x() == 0 && diff.y() == 0) // �������� �ʾҴٸ�
	{
		return;
	}

	if (qAbs(diff.x()) > qAbs(diff.y())) // x��ȭ���� y��ȭ���� ���밪 �񱳸� ���� �̵����� ����
	{
		if (diff.x() > 0)
		{
			dir = Direction::Right;
		}
		else
		{
			dir = Direction::Left;
		}
	}
	else
	{
		if (diff.y() > 0)
		{
			dir = Direction::Down;
		}
		else
		{
			dir = Direction::Up;
		}
	}

	_listener->itemDragEvent(this, dir); // ���� ��ü�� ������ ����
}
