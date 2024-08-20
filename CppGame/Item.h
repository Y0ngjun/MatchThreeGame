#pragma once

#include <string>
#include <QtWidgets/QGraphicsPixmapItem>

class Item : public QGraphicsPixmapItem
{
	class EventListener; // ���漱��

public:
	Item(EventListener* listener, const std::string& path, int row, int column, QGraphicsItem* parent); // ������

	std::string path() const; // Getter
	int row() const;
	int column() const;

	void setRow(int row); // Setter
	void setColumn(int column);

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override; // ���콺 ���� �� ȣ��
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override; // ���콺 �� �� ȣ��

private:
	const std::string _path; // �̹��� ����
	int _row; // ��ǥ
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

	class EventListener // �������̽�
	{
	public:
		virtual void itemDragEvent(Item* item, Item::Direction dir) = 0;
	};
};