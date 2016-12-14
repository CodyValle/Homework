#ifndef GETBMP_H
#define GETBMP_H

// Holds bitmap data

#include <string>

using namespace std;

struct BitMapFile
{
   int sizeX;
   int sizeY;
   unsigned char *data;
};

BitMapFile *getbmp(string filename);

#endif
