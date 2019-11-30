/*
Brady Messer (wmesser@clemson.edu)
CPSC 4040 Fall 2019
This file contains the necessary globals for GLUT
The name of the keyboard callback function is: handleKey
The name of the drawImage callback function is: drawImage
The name of the reshape callback function is: handleReshape
*/

#ifndef GLOBALS
#define GLOBALS

#include "Helper.h"

int WIDTH = 500;
int HEIGHT = 500;

Image image = Image();
Image originalImage = Image();
Filter filt = Filter();
string out = "";
Matrix3D M;
void read_input(Matrix3D &M);
void warp();

void handleKey(unsigned char key, int x, int y) {
  switch(key){
    case 'q':		// q - quit
    case 'Q':
    case 27:		// esc - quit
      exit(0);
    case 'w':
    case 'W': {
      if (out == "") {
        string temp;
        cout << "Enter the name of the output file: ";
        cin >> temp;
        image.writeImage(temp);
      } else {
        image.writeImage(out);
      }
      break;
    }
    case 'i':
    case 'I':
      image.invert();
      break;
    case 'c':
    case 'C':
      image.convolve(filt);
      break;
    case 'r':
    case 'R':
      image.copy(originalImage);
      break;
    case 'f':
    case 'F': {
      string temp;
      cout << "Enter the file name of the new filter: ";
      cin >> temp;
      char * writable = new char[temp.size() + 1];
      copy(temp.begin(), temp.end(), writable);
      writable[temp.size()] = '\0'; // don't forget the terminating 0
      filt = Filter(writable);
      delete[] writable;
      break;
    }
    case 'p':
    case 'P':
      read_input(M);
      warp();
      break;
    case 't':
      image.convertToFourChannels();
      break;
    default:		// not a valid key -- just ignore it
      return;
  }
}
// This should display the image on screen but I have not tested it yet
void drawImage() {
  // specify window clear (background) color to be opaque white
  glClearColor(1,1,1,1);

  // clear window to background color
  glClear(GL_COLOR_BUFFER_BIT);

	// Set window to be the same size as the image
  glutReshapeWindow(image.width, image.height);
  glutPostRedisplay();

	image.draw();
  // flush the OpenGL pipeline to the viewport
  glFlush();
}

void handleReshape(int w, int h) {
	// set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// define the drawing coordinate system on the viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
  glOrtho( 0, w, 0, h, 0.1, 1 );
  // flip the image to the correct orientation
  glPixelZoom( 1, -1 );
  glRasterPos3f(0, h - 1, -0.3);
}

void Rotate(Matrix3D &M) {
	float theta;
	cout << "Enter theta: ";
	cin >> theta;
	if (cin) {
		cout << "calling rotate\n";
	}
	else {
		cerr << "invalid rotation angle\n";
		cin.clear();
		return;
	}

	Matrix3D R;
	double rad, c, s;
	rad = PI * theta / 180.0;
	c = cos(rad);
	s = sin(rad);
	R[0][0] = c;
	R[0][1] = s;
	R[1][0] = -s;
	R[1][1] = c;

	M = R * M;

}

void Scale(Matrix3D &M) {
	float sx;
	float sy;
	cout << "Enter scaleX then scaleY: ";
	cin >> sx >> sy;
	if (sx == 0 || sy == 0) {
		cout << "Nice try, you think I wouldn't catch that scale by 0? Returning.\n";
		return;
	}
	if (cin) {
		cout << "calling scale\n";
	}
	else {
		cerr << "invalid scale\n";
		cin.clear();
		return;
	}

	Matrix3D R;

	R[0][0] = sx;
	R[1][1] = sy;

	M = R * M;

}

void Translate(Matrix3D &M) {
	float v02;
	float v12;
	cout << "Enter translation value dx then dy: ";
	cin >> v02 >> v12;
	if (cin) {
		cout << "calling translate\n";
	}
	else {
		cerr << "invalid translation\n";
		cin.clear();
		return;
	}

	Matrix3D R;

	R[0][2] = v02;
	R[1][2] = v12;;

	M = R * M;

}

void Shear(Matrix3D &M) {
	float v01;
	float v10;
	cout << "Enter shear value hx then hy: ";
	cin >> v01 >> v10;
	if (cin) {
		cout << "calling shear\n";
	}
	else {
		cerr << "invalid shear\n";
		cin.clear();
		return;
	}

	Matrix3D R;

	R[0][0] = 1;
	R[1][1] = 1;
	R[2][2] = 1;
	R[0][1] = v01;
	R[1][0] = v10;

	M = R * M;
}


void Flip(Matrix3D &M) {
	bool h = false;
	bool v = false;
	// cout << "horizontal or vertical? (h/v): \n";
	// char in;
	// cin >> in;
	// float val = 0.0;
	// cout << "floating point value: \n";
	// cin >> val;
	int fx,fy;
	cout << "Enter fx then fy: ";
	cin >> fx >> fy;
	if (cin) {
		if (fx == 1)
			h = true;
		if (fy == 1)
			v = true;
	}
	else {
		cerr << "Invalid input. To flip horizontally or vertically enter 1 for fx and/or fy respectively.\n";
		cin.clear();
		return;
	}

	Matrix3D R;

	if (h && v) {
		cout << "Can only flip one direction at a time, doing horizontal.\n";
	}

	if (h) {
		cout << "Flipping horizontally\n";
		R[0][0] = -1;
		R[1][1] = 1;
		R[0][2] = fx;
	}
	else if (v) {
		cout << "Flipping vertically\n";
		R[0][0] = 1;
		R[1][1] = -1;
		R[1][2] = fy;
	}


	M = R * M;
}

void Perspective(Matrix3D &M) {
	int s = 8;
	float vals[s];
	cout << "Enter the 8 perspective warp values: \n";
	for (int i = 0; i < s; i++) {
		cout << "Factor: " << i << endl;
		cin >> vals[i];
	}
	if (cin) {
		cout << "calling perspective\n";
	}
	else {
		cerr << "invalid perspective\n";
		cin.clear();
		return;
	}

	Matrix3D R;

	for (int i = 0; i < sqrt(s+1); i++) {
		for (int j = 0; j < sqrt(s+1); j++) {
			// cout << (i*(sqrt(s+1)))+j << endl;
			if (i == 2 && j == 2)
				R[i][j] = 1;
			else
				R[i][j] = vals[(int)(i*(sqrt(s+1))+j)];
		}
	}
	M = R * M;
}

/*
Build a transformation matrix from input text
*/
void read_input(Matrix3D &M) {
	string cmd;

	/* prompt for user input */
	do
	{
		cout << "> Please input one of the following to choose a perspective warp: \nr, s, t, h, f, p, d\n";
		cin >> cmd;
		if (cmd.length() != 1){
			cout << "invalid command, enter r, s, t, h, f, p, d\n";
		}
		else {
			switch (cmd[0]) {
				case 'r':		/* Rotation, accept angle in degrees */
					Rotate(M);
					break;
				case 's':		/* scale, accept scale factors */
					Scale(M);
					break;
				case 't':		/* Translation, accept translations */
					Translate(M);
					break;
				case 'h':		/* Shear, accept shear factors */
					Shear(M);
					break;
				case 'f':		/* Flip, accept flip factors */
					Flip(M);
					break;
				case 'p':		/* Perspective, accept perspective factors */
					Perspective(M);
					break;
				case 'd':		/* Done, that's all for now */
					break;
				default:
					cout << "invalid command, enter r, s, t, h, f, p, d\n";
			}
		}
	} while (cmd.compare("d")!=0);

}

void warp() {
  cout << "Accumulated Matrix: " << endl;
  M.print();

 // your code to perform inverse mapping (4 steps)
 // 1)
 Vector2D corners[4];
 corners[0] = Vector2D(0,0);
 corners[1] = Vector2D(image.width,0);
 corners[2] = Vector2D(0,image.height);
 corners[3]= Vector2D(image.width,image.height);
 Vector3D temp = M * corners[0];
 if (temp.z != 1) {
   temp.x /= temp.z;
   temp.y /= temp.z;
   temp.z /= temp.z;
 }
 float minX = temp.x;
 float minY = temp.y;
 float maxX = temp.x;
 float maxY = temp.y;

 for (int i = 0; i < 4; i++) {
   temp = M * corners[i];
   if (temp.z != 1) {
     temp.x /= temp.z;
     temp.y /= temp.z;
     temp.z /= temp.z;
   }
   if (temp.x < minX)
    minX = temp.x;
   else if (temp.x > maxX)
    maxX = temp.x;
   if (temp.y < minY)
    minY = temp.y;
   else if (temp.y > maxY)
    maxY = temp.y;
 }

 //2)
 int w = maxX - minX;
 int h = maxY - minY;
 cout << "Input image size : " << image.width << "x" << image.height << endl;
 cout << "Output image size: " << w << "x" << h << endl;
 if (w < 1 || h < 1) {
   cerr << "Invalid warp matrix caused the output image to not exist.  Exiting.\n";
   exit(1);
 }
 Image img = Image(w,h,image.channels);

 //3)
 Matrix3D inv = M.inverse();
 //4)
 Vector3D origin = Vector3D(minX, minY, 1);
 Vector3D t;

 int k = 0;
 int l = 0;

 // TODO: Make the inverse mapping a function of the image class
 for (int i = 0; i < img.width * img.height * img.channels; i+=img.channels) {
   Vector3D pixel_out(l,k,1);
   pixel_out.x += origin.x;
   pixel_out.y += origin.y;
   Vector3D pixel_in = inv * pixel_out;

   float u = pixel_in.x / pixel_in.z;
   float v = pixel_in.y / pixel_in.z;
   if (v >= 0 && v < image.height && u >= 0 && u < image.width) {
     for (int j = 0; j < image.channels; j++) {
        img.pixels[i+j] = image.pixels2D[(int)v][(int)u].pix[j];
    }
  }
   l++;
   if (l == img.width) {
     l = 0;
     k++;
   }
 }

 image = img;	// the most important line
 image.convertTo2DPixels();
}
#endif
