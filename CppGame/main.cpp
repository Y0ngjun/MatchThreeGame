#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsRectItem>
#include <QtWidgets/QGraphicsView>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QGraphicsScene scene;
    QGraphicsRectItem rect(0, 0, 200, 100);
    scene.addItem(&rect);

    QGraphicsView view(&scene);
    view.showFullScreen();

    return a.exec();
}