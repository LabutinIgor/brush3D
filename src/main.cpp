#include "MainView.h"


int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Wrong arguments. Expected .obj file and texture file";
        exit(1);
    }

    MainView mainView(argv[1], argv[2]);
    mainView.show();

    return 0;
}

