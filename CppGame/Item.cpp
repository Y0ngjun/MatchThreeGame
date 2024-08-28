#include "Item.h"
#include "Consts.h"

#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QGraphicsItemAnimation>
#include <QTimeLine>
#include <QPoint>
#include <QPropertyAnimation>

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

void Item::moveTo(double toX, double toY)
{
	double diffX = toX - x();
	double diffY = toY - y();

	double time = 0;
	time += qAbs(diffX) / Consts::BOARD_ITEM_SIZE * Consts::ANIMATION_TIME;
	time += qAbs(diffY) / Consts::BOARD_ITEM_SIZE * Consts::ANIMATION_TIME;

	QTimeLine* timer = new QTimeLine(time);

	QGraphicsItemAnimation* animation = new QGraphicsItemAnimation();
	animation->setItem(this);
	animation->setTimeLine(timer);
	animation->setPosAt(0, pos());
	animation->setPosAt(1, QPointF(toX, toY));
	
	connect(timer, &QTimeLine::finished, [this, timer, animation]() {
		timer->deleteLater();
		animation->deleteLater();

		_listener->itemMoveFinished(this, nullptr, false);
		});

	timer->start();
}

void Item::moveTo(Item* other, bool canRevert)
{
	double toX = other->x();
	double toY = other->y();

	double diffX = toX - x();
	double diffY = toY - y();

	double time = 0;
	time += qAbs(diffX) / Consts::BOARD_ITEM_SIZE * Consts::ANIMATION_TIME;
	time += qAbs(diffY) / Consts::BOARD_ITEM_SIZE * Consts::ANIMATION_TIME;

	QTimeLine* timer = new QTimeLine(time);

	QGraphicsItemAnimation* animation = new QGraphicsItemAnimation();
	animation->setItem(this);
	animation->setTimeLine(timer);
	animation->setPosAt(0, pos());
	animation->setPosAt(1, QPointF(toX, toY));
	
	connect(timer, &QTimeLine::finished, [this, other,  timer, animation, canRevert]() {
		timer->deleteLater();
		animation->deleteLater();

		_listener->itemMoveFinished(this, other, canRevert);
		});

	timer->start();
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

	if (diff.x() == 0 && diff.y() == 0) // 움직이지 않았다면
	{
		return;
	}

	if (qAbs(diff.x()) > qAbs(diff.y())) // x변화량과 y변화량의 절대값 비교를 통해 이동방향 결정
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

	_listener->itemDragEvent(this, dir); // 현재 객체와 방향을 전달
}
