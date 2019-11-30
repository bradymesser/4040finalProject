Brady Messer (wmesser)
CPSC 4040 Fall 2019
Final project
Color palettes + image manipulation

How to compile the program:
  - type 'make' on the command line, the compiled program will be called main
How to run the program:
  - ./main inputImage.image
    - example: ./main images/lenna.png

  - Once the program is running you will be presented with two options
    1) Option 1 is to specify a color palette in a file, an example palette file
        is provided which is called 'test.pal'.  The format of a color palette file
        is the following:
        number of pixels
        r g b
        r g b
        r g b
        ...

        A more concrete example is the following:
        3
        255 255 255
        0 0 0
        128 128 128

      Please refer to test.pal if this is not clear enough.
    2) Option 2 is to specify an image that you would like to generate a color
        palette from.  This is done by opening the specified image and generating
        a collection of unique pixel values, meaning that every rgb combination that
        appears in the specified image will be added to the palette only once.  Then
        this palette is applied to the image specified on the command line by
        interpolating each pixel in the image to find the closest value in the color
        palette and assigning the pixel in the input image to be that closest palette
        value.

  - Once the palettized image is complete, it will be displayed on the screen.
    From here you can use the following keys to perform different operations:
    - 'q' - quit the program
    - 'w' - write the displayed image to a specified output file
    - 'i' - invert the colors of the image
    - 'r' - reset the image to its original form
    - 'f' - load a filter to be used with convolution, a collection of sample filters are
          provided in the sub directory labeled filters
    - 'c' - apply the convolution filter (can be done multiple times) (note that you must
          specify a filter with the keyboard function 'f' first)
    - 'p' - apply a perspective or projective warp to the image, the following are the possible warps:
        - 'r' - rotate the image
        - 's' - scale the image
        - 't' - translate the image
        - 'h' - shear the image
        - 'f' - flip the image
        - 'p' - perspective, apply a perspective warp
        - 'd' - done applying warps, shows the final result

** Please note that since most of the aforementioned files are in sub directories, that you
will have to prepend the directory to the file name when specifying on the command line.

Some example runs will be listed below:
--------------------------------------------------------------------------------
./main images/lenna.png
1
test.pal
q
--------------------------------------------------------------------------------
./main images/waves.png
2
images/lenna.png
q
--------------------------------------------------------------------------------
./main images/checkers.png
1
test.pal
p
s
2.0 2.0
d
q
--------------------------------------------------------------------------------
./main images/waves.png
2
images/checkers.png
f
filters/cross.filt
c
w
outputFileName.image
q
--------------------------------------------------------------------------------
