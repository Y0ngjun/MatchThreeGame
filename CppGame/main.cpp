#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QScreen>

#include "Board.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	QScreen* screen = a.primaryScreen(); // 스크린 정보 받아옴
	QRect geometry = screen->geometry();
	QGraphicsScene scene;
	scene.setSceneRect(geometry);

	Board board(&scene); // 보드 생성

	QGraphicsView view(&scene);
	view.showFullScreen();

	return a.exec();
}