#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <QScreen>

#include "Board.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	QScreen* screen = a.primaryScreen();
	QRect geometry = screen->geometry();

	QGraphicsScene scene;
	scene.setSceneRect(geometry);

	Board board(&scene);

	QGraphicsView view(&scene);
	view.showFullScreen();

	return a.exec();
}