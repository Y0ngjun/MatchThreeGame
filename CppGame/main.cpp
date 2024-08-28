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
	QScreen* screen = a.primaryScreen(); // 스크린 정보 받아옴
	QRect geometry = screen->geometry();

	QGraphicsScene scene;
	scene.setSceneRect(geometry);

	Values values; // 점수 저장

	Board board(&scene, &values); // 보드 생성
	Score score(&scene, &values); // 점수 생성

	QGraphicsView view(&scene);
	view.showFullScreen();

	return a.exec();
}