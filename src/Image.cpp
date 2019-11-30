/*
Brady Messer (wmesser@clemson.edu)
CPSC 4040 Fall 2019
This file contains the implementation of the Image class
*/

#ifndef IMAGE
#define IMAGE

#include "Helper.h"

Pixel interpolate(Palette p, Pixel pixel) {
  int closestMatchIndex = 0;
  int temp = 0;
  int smallestDifference = 9999999;

  for (int i = 0; i < p.numColors; i++) {
    temp = p.palette[i] - pixel;
    if (temp < smallestDifference) {
      smallestDifference = temp;
      closestMatchIndex = i;
    }
  }

  return p.palette[closestMatchIndex];
}

void Image::convertTo2DPixels() {
  // Pixel pix[height][width];
  pixels2D = new Pixel*[height];
  for(int i = 0; i < height; ++i) {
      pixels2D[i] = new Pixel[width];
  }

  int k = 0;
  int l = 0;

  for (int i = 0; i < width * height * channels; i+=channels) {
    pixels2D[k][l].pix = new unsigned char (channels);
    for (int j = 0; j < channels; j++) {
      pixels2D[k][l].pix[j] = pixels[i + j];
    }
    l++;
    if (l == width) {
      l = 0;
      k++;
    }
  }
}

Image::Image() {
  //Nothing to do without a specified file, set all values to defaults
  width = 0;
  height = 0;
  channels = 0;
  pixels = NULL;
  pixels2D = NULL;
  ext = "";
  DEFAULT_OUT_OF_BOUNDS_VALUE = 0;
}

// overloaded constructor, reads in an image
Image::Image(string file) {
  ImageInput *in = ImageInput::open(file);
  if (!in) {
      cout << "Could not open file " << file << " exiting. " << endl;
      exit(1);
  }
  const ImageSpec &spec = in->spec();
  width = spec.width;
  height = spec.height;
  channels = spec.nchannels;
  ext = file.substr(file.find("."), file.length());
  pixels = new unsigned char[width*height*channels];
  in->read_image (TypeDesc::UINT8, &pixels[0]);
  in->close ();
  ImageInput::destroy (in);
  DEFAULT_OUT_OF_BOUNDS_VALUE = 0;
  convertTo2DPixels();
}

Image::Image(int w, int h, int chan) {
  width = w;
  height = h;
  channels = chan;
  ext = "undefined";
  pixels = new unsigned char[width*height*channels];
  DEFAULT_OUT_OF_BOUNDS_VALUE = 0;
}

// copy function
void Image::copy(const Image& img) {
  width = img.width;
  height = img.height;
  channels = img.channels;
  for (int i = 0; i < width * height * channels; i++) {
    pixels[i] = img.pixels[i];
  }
  ext = img.ext;
  convertTo2DPixels();
}
//writes the object to a file
void Image::writeImage(string filename) {
  const int xres = this->width, yres = this->height;
  const int channels = this->channels;  // RGB
  ImageOutput *out = ImageOutput::create (filename);
  if (!out) {
      cout << "EXITING, could not open output file" << endl;
      exit(1);
  }
  ImageSpec spec (xres, yres, channels, TypeDesc::UINT8);
  out->open (filename, spec);
  out->write_image (TypeDesc::UINT8, pixels);
  out->close ();
  // ImageOutput::destroy (out);
  ImageOutput::destroy(out);
}

// Inverts the colors of the image
void Image::invert() {
  for (int i = 0; i < width * height * channels; i++) {
    pixels[i] = 255 - pixels[i];
  }
}

// This just draws the pixels, it cannot be used as the callback function for drawing
void Image::draw() {
  switch (channels) {
    case 1:
      // set unpack alignment to 1 so that rows aren't skipped in the pixmap
      glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
      glDrawPixels(width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, pixels);
      break;
    case 2:
      // I'm not sure if 2 channels is a case but I implemented it anyways
      glDrawPixels(width, height, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, pixels);
      break;
    case 3:
      glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
      break;
    case 4:
      glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
      break;
    default:
      break;
  }
}

// Converts to a 4 channel image
void Image::convertToFourChannels() {
  if (channels == 4) {
    return;
  }
  unsigned char * pixels2 = new unsigned char[width*height*4];
  int j = 0;
  if (channels != 1) {
    for (int i = 0; i < (width * height * channels) - channels; i+=channels) {
      // RGBtoHSV(pixels[i],pixels[i+1],pixels[i+2],h,s,v);
      pixels2[j] = pixels[i];
      pixels2[j+1] = pixels[i+1];
      pixels2[j+2] = pixels[i+2];
      pixels2[j+3] = 255;
      j += 4;
    }
  } else {
    for (int i = 0; i < (width * height * channels) - channels; i++) {
      // RGBtoHSV(pixels[i],pixels[i+1],pixels[i+2],h,s,v);
      pixels2[j] = pixels[i];
      pixels2[j+1] = pixels[i];
      pixels2[j+2] = pixels[i];
      pixels2[j+3] = 255;
      j += 4;
    }
  }
  pixels = pixels2;
  channels = 4;
  convertTo2DPixels();
}

// Chromakey the image based on hsv values and the thresholds defined in thresholds.txt
void Image::chromaKey() {
  if (channels != 4) {
    printf("Can't chromakey without 4 channels, would you like the image to be converted to 4 channels for you? (y/n)\n");
    char temp;
    cin >> temp;
    if (temp != 'y' && temp != 'Y') {
      return;
    }
    // if the user agreed, convert the image to 4 channels and continue execution
    convertToFourChannels();
  }
  ifstream in;
  in.open("thresholds.txt");
  // the threshold values for h s and v
  double hV = 120.0; //The desired color, 120 hue is green
  double hT = 50.0; //threshold (hue's within the range of hT are valid)
  double sT = 0.3; // minimum desired saturation
  double vT = 0.8; // minimum desired value
  if (in != NULL) {
    // If there is a thresholds.txt file, read in the values and use them
    in >> hV >> hT >> sT >> vT;
  } else {
    cout << "Could not find thresholds.txt, using default values.\n";
  }
  double h,s,v;

  for (int i = 0; i < (width * height * channels) - channels; i+=channels) {
    RGBtoHSV(pixels[i],pixels[i+1],pixels[i+2],h,s,v);
    if (abs(h - hV) <= hT && (s >= sT || v >= vT)) {
     pixels[i+3] = 0; //TODO: Make alpha be other values than 0 and 255 (non binary)
    } else {
     pixels[i+3] = 255;
   }
  }
}

// Uses the current image object as the background (B), and overlays the parameter image (A)
// The result is an image with A composited onto B (A over B)
void Image::composite(Image A) {
  if (A.channels != 4) {
    cout << "Can't perform A over B if A isn't 4 channels.  \nWould you like the image to be converted to 4 channels and chromakeyed for you? (y/n)\n";
    char temp;
    cin >> temp;
    if (temp != 'y' && temp != 'Y') {
      return;
    }
    A.convertToFourChannels();
    A.chromaKey();
  }

  double r, g, b, a;
  double newR, newG, newB, newA;
  int j = 0;
  int currRow = 0;
  int w = A.width * A.channels;
  for (int i = 0; i < A.width * A.height * A.channels; i+=A.channels) {
    if (i > w + (w * currRow)) {
      currRow++;
      j = (width * channels) * currRow;
    }
    // first premultiply
    a = A.pixels[i+3] / 255;
    // if the pixel is transparent on A, do nothing to B
    // TODO: Once alpha is non binary (0 and 255) this will need to change
    if (a == 0) {
      j+= channels;
      continue;
    }
    r = A.pixels[i] * a;
    g = A.pixels[i+1] * a;
    b = A.pixels[i+2] * a;
    // Perform over operation
    newR = r + (1-a) * pixels[i];
    newG = g + (1-a) * pixels[i+1];
    newB = b + (1-a) * pixels[i+2];
    newA = a + (1-a) * pixels[i+3];
    pixels[j] = newR;
    pixels[j+1] = newG;
    pixels[j+2] = newB;
    pixels[j+3] = newA;
    j+= channels;
  }
}

void Image::convolve(Filter filter) {
  if (filter.array == NULL) {
    cout  << "Filter not initialized, returning\n";
    return;
  }
  if (!filter.isFlipped()) {
    filter.flip();
  }

  float pix[height][width * channels];
  float temp[filter.size][filter.size];
  float result[filter.size][filter.size];
  int k = 0;
  int l = 0;

  // Store pixels in a 2d array to make the logic easier
  for (int i = 0; i < width * height * channels; i++) {
    pix[k][l] = pixels[i];
    l++;
    if (l == width * channels) {
      l = 0;
      k++;
    }
  }

  k = 0;
  int a = 0;
  int b = 0;

  // Loop through the 2d pixels and for each pixel populate a matrice to use in multiplication
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width * channels; j+=channels) {
      for (k = j; k < j + channels; k++) {
        // Populate the array of pixels to multiply against filter
        if (i - (filter.size/2) < 0 || k - ((filter.size/2) * channels) < 0 || k + ((filter.size/2) * channels) >= (width * channels) || i + (filter.size / 2) >= height)
          continue;
        a = 0;
        b = 0;
        for (int z = i - (filter.size/2); z <= i + (filter.size / 2); z++) {
          for (int y = k - ((filter.size/2) * channels); y <= (k + ((filter.size / 2) * channels)); y+=channels) {
            if (z >= 0 && z < height && y >= 0 && y < width * channels) {
              temp[a][b] = pix[z][y];
            }
            else {
              temp[a][b] = DEFAULT_OUT_OF_BOUNDS_VALUE;
            }
            b++;  // next column
          }
          a++;  // next row
          b = 0;  // reset to first column
        }
        a = 0;
        b = 0;

        int m, n;
        // Multiply the filter * temp and store in result
        for (m = 0; m < filter.size; m++) {
            for (n = 0; n < filter.size; n++) {
                result[m][n] = 0;
                result[m][n] += (filter.array[m][n] * temp[m][n]);
            }
        }
        float sum = 0;
        // sum the result and store in pix[i][k];
        for (m = 0; m < filter.size; m++) {
          for (int n = 0; n < filter.size; n++) {
            sum += result[m][n];
          }
        }
        pixels[(i*width*channels)+k] = (unsigned char)((sum) + filter.offset);
        if (sum + filter.offset < 0) {
          pixels[(i*width*channels)+k] = 0;
        }
        if (abs(sum + filter.offset) > 255)
          pixels[(i*width*channels)+k] = 255;
        sum = 0;
      }
    }
  }
}

void Image::copyBackTo1D() {
  int k = 0;
  int l = 0;

  for (int i = 0; i < width * height * channels; i+=channels) {
    for (int j = 0; j < channels; j++) {
      pixels[i + j] = pixels2D[k][l].pix[j];
    }
    l++;
    if (l == width) {
      l = 0;
      k++;
    }
  }
}


void Image::paletize(Palette p) {
  if (channels != 4) {
    convertToFourChannels();
  }

  // Loops through pixels2D and for each pixel find the closest match from Palette p
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      pixels2D[i][j] = interpolate(p,pixels2D[i][j]);
    }
  }

  copyBackTo1D(); // the most important line of this function (image is drawn from the 1d array)
}

#endif
