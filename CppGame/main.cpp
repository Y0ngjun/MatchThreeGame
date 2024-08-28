#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QScreen>

#include "Board.h"
#include "Score.h"
#include "Value.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	QScreen* screen = a.primaryScreen(); // ��ũ�� ���� �޾ƿ�
	QRect geometry = screen->geometry();

	QGraphicsScene scene;
	scene.setSceneRect(geometry);

	Values values; // ���� ����

	Board board(&scene, &values); // ���� ����
	Score score(&scene, &values); // ���� ����

	QGraphicsView view(&scene);
	view.showFullScreen();

	return a.exec();
}