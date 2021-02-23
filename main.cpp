#include <QApplication>
#include "ruler.h"

int main(int argc, char * argv[])
{
    QApplication app(argc,argv);

    Ruler r;
    r.show();
    return app.exec();
}
