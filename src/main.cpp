#include "matrix.h"
#include "Helper.h"
#include "Globals.h"

Palette pal = Palette();

int main(int argc, char *argv[]) {

   //your code to read in the input image
	 if (argc != 2) {
	 	cout << "Valid usages are: \n./main imageA.image\nWhere imageA is the image you would like to apply the limited color palette to.\n";
		exit(1);
	} else {
		image = Image(argv[1]);
		originalImage = Image(argv[1]);
	}

	int in = 0;
	string file = "";

	cout << "How would you like to create the color palette?\n1) Enter the name of a palette file\n2) Create one from another image\nPlease enter 1 or 2: ";
	cin >> in;
	while (in < 1 || in > 2) {
		cout << "Invalid input.  Please try again.\n";
		cout << "How would you like to create the color palette?\n1) Enter the name of a palette file\n2) Create one from another image\nPlease enter 1 or 2: ";
		cin >> in;
	}

	if (in == 1) {
		cout << "Enter the filename of the palette file: ";
		cin >> file;
		char * writable = new char[file.size() + 1];
		copy(file.begin(), file.end(), writable);
		writable[file.size()] = '\0';
		pal = Palette(writable);
	}
	else if (in == 2) {
		cout << "Enter the filename of the image file to generate a palette from: ";
		cin >> file;
		char * writable = new char[file.size() + 1];
		copy(file.begin(), file.end(), writable);
		writable[file.size()] = '\0';
		Image t = Image(writable);
		cout << "Generating palette from image " << file << endl;
		pal = Palette(t);
		cout << "Palette generated.  There are " << pal.numColors << " possible pixel colors in this palette.\n";
	}
	else {
		cout << "Your input was somehow still invalid which means my code sucks.  Please restart the program.\n";
		exit(1);
	}
	cout << "Applying palette to " << argv[1] << ".  Please be patient this may take a while.\n";
	image.paletize(pal);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("LIMITED COLOR PALETTE");

  glutDisplayFunc(drawImage);	  // display callback
  glutKeyboardFunc(handleKey);
  glutReshapeFunc(handleReshape); // window resize callback

  glutMainLoop();
  return 0;
}
