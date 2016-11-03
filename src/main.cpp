#include "MainView.h"


int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Wrong arguments. Expected one .obj file";
        exit(1);
    }

    MainView mainView(argv[1]);
    mainView.show();

    return 0;
}

