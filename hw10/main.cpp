#include <iostream>
#include "BitMap.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " <source_bitmap_file> <destination_file>"<< endl;
        return 1;
    }

    BitMap bitmap(argv[1]);
    bitmap.rotate90();
    bitmap.writeToFile(argv[2]);

    return 0;

}