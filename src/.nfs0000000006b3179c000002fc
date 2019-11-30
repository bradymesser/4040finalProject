#ifndef PALETTE
#define PALETTE

#include "Helper.h"
#include <vector>

Palette::Palette() {
  palette = NULL;
  numColors = 0;
  defaultChannels = 0;
}

Palette::Palette(char * file) {
  ifstream in;
  in.open(file);
  in >> numColors;
  palette = new Pixel[numColors];
  defaultChannels = 3;
  float temp = 0.0;

  for (int i = 0; i < numColors; i++) {
    palette[i].pix = new unsigned char (defaultChannels);
    for (int j = 0; j < defaultChannels; j++) {
      in >> temp;
      // cout << (int)(temp);
      palette[i].pix[j] = temp;
    }
  }
}

Palette::Palette(Image img) {
  Pixel temp;
  Pixel temp2;
  vector<Pixel> t;
  bool found = false;
  img.convertToFourChannels();

  for (int i = 0; i < img.height; i++) {
    for (int j = 0; j < img.width; j++) {
      temp = img.pixels2D[i][j];
      for (int k = 0; k < (int)t.size(); k++) {
        temp2 = t.at(k);
        if (temp == temp2)
          found = true;
      }
      if (!found) {
        t.push_back(temp);
      }
      found = false;
    }
  }

  numColors = (int)t.size();
  defaultChannels = 3;

  palette = new Pixel[(int)t.size()];

  for (int i = 0; i < (int)t.size(); i++) {
    palette[i] = t.at(i);
  }
}

void Palette::print() {
  for (int i = 0; i < numColors; i++) {
    for (int j = 0; j < defaultChannels; j++) {
      cout << (int)palette[i].pix[j] << "|";
    }
    cout << endl;
  }
}

#endif
