#pragma once

#include <string>
#include <QtWidgets/QGraphicsPixmapItem>

class Item : public QGraphicsPixmapItem, public QObject
{
	class EventListener; // 전방선언

public:
	Item(EventListener* listener, const std::string& path, int row, int column, QGraphicsItem* parent); // 생성자

	std::string path() const; // Getter
	int row() const;
	int column() const;

	void setRow(int row); // Setter
	void setColumn(int column);

	void moveTo(double toX, double toY);
	void moveTo(Item* other, bool canRevert);

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override; // 마우스 누를 때 호출
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override; // 마우스 뗄 때 호출

private:
	const std::string _path; // 이미지 종류
	int _row; // 좌표
	int _column;
	EventListener* _listener;
	QPointF _pressPos;

public:
	enum class Direction
	{
		Left,
		Right,
		Up,
		Down
	};

	class EventListener // 인터페이스
	{
	public:
		virtual void itemDragEvent(Item* item, Item::Direction dir) = 0;
		virtual void itemMoveFinished(Item* item0, Item* item1, bool canRevert) = 0;
	};
};