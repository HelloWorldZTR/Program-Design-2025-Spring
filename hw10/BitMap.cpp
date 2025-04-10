#include "BitMap.h"
#include <iostream>
#include <fstream>

using namespace std;

BitMap::BitMap() : filename(nullptr), pBits(nullptr) {
}
// Copy constructor
BitMap::BitMap(const BitMap& bitmap) {
    this->filename = new char[strlen(bitmap.filename) + 1];
    strcpy(this->filename, bitmap.filename);
    this->bfh = bitmap.bfh;
    this->bih = bitmap.bih;
    this->pBits = new BYTE[totalBytes];
    memcpy(this->pBits, bitmap.pBits, totalBytes);
}

BitMap::BitMap(const char *filename) {
    this->filename = new char[strlen(filename) + 1];
    strcpy(this->filename, filename);

    readFromFile(filename);
}

BitMap::~BitMap() {
    delete[] filename;
    delete[] pBits;
}

void BitMap::readFromFile(const char *filename) {
    fstream inputFile(filename, ios::binary | ios::in);
    if (!inputFile) {
        cerr << "File not found: " << filename << endl;
        return;
    }
    inputFile.read((char*)(&bfh), sizeof(bfh));
    inputFile.read((char*)(&bih), sizeof(bih));

    clog << "File Size: " << hex << bfh.bfSize << endl;
    clog << "Width: " << dec << bih.biWidth << endl;
    clog << "Height: " << bih.biHeight << endl;
    clog << "Bit Count: " << bih.biBitCount << endl;

    //Stupid alignment
    int rowSize = ((bih.biWidth * bih.biBitCount + 31) / 32) * 4;
    totalBytes = rowSize * abs(bih.biHeight);

    // Check if the file is a valid BMP file
    if (bfh.bfType != 0x4D42) {
        cerr << "Not a valid BMP file: " << filename << endl;
        inputFile.close();
        return;
    }
    pBits = new BYTE[totalBytes];
    inputFile.seekg(bfh.bfOffBits, ios::beg);// The start of the pixel data
    inputFile.read((char*)pBits, totalBytes);

    cout << "First pixel" << int(pBits[0]) << " " << int(pBits[1]) << " " << int(pBits[2]) << endl;

    // If reads illegal data
    if (!inputFile) {
        cerr << "Error reading file: " << filename << endl;
        delete[] pBits;
        pBits = nullptr;
    }
    inputFile.close(); // Gracefully close 
}

void BitMap::rotate90(){
    if (pBits == nullptr) {
        cerr << "No bitmap data to rotate." << endl;
        return;
    }

    int rowSize = ((bih.biWidth * bih.biBitCount + 31) / 32) * 4;
    int newRowSize = ((bih.biHeight * bih.biBitCount + 31) / 32) * 4;
    totalBytes = newRowSize * abs(bih.biWidth);
    BYTE *rotatedBits = new BYTE[totalBytes];

    for (LONG y = 0; y < bih.biHeight; y++) {
        for (LONG x = 0; x < bih.biWidth; x++) {
            LONG oldPixelIdx = y * rowSize + x * (bih.biBitCount / 8);
            LONG newPixelIdx = ((bih.biWidth - 1 - x) * newRowSize + y * (bih.biBitCount / 8));
            for (int i = 0; i < (bih.biBitCount / 8); i++) {
                rotatedBits[newPixelIdx + i] = pBits[oldPixelIdx + i];
            }
        }
    }

    delete[] pBits;
    pBits = rotatedBits;
    swap(bih.biWidth, bih.biHeight);

    bfh.bfSize = sizeof(bfh) + sizeof(bih) + rowSize * abs(bih.biHeight);
    
}

void BitMap::writeToFile(const char *filename) {
    fstream outputFile(filename, ios::binary | ios::out);
    if (!outputFile) {
        cerr << "Error opening file for writing: " << filename << endl;
        return;
    }
    outputFile.write((char*)&bfh, sizeof(bfh));
    outputFile.write((char*)&bih, sizeof(bih));
    outputFile.seekp(bfh.bfOffBits, ios::beg);// Skip a few bits
    outputFile.write((char*)pBits, totalBytes);
    // Fill the file with 0 to bfSize
    // outputFile.seekp(bfh.bfSize-1, ios::beg);
    // outputFile.write("\0", 1);
    outputFile.close();


}