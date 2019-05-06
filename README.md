# Baby Shower Bingo

This software automatically generates bingo cards for a Baby Shower
Bingo game. It is based on OpenCV and STL to place images of
individual gifts from the baby shower participants and include them in
the bingo cards.

These individual images of the gifts need to be downloaded or
generated before running this software. One way of obtaining the
images is to simply download the registry website using, for example,
Firefox. A shell script is provided to grab and rescale the images as
the software assumes that the images are already scaled to fit into
the tiles of the bingocard. Alternatively, an image tool, such as
IrfanView can be used for this step. Optionally, a text file can be
generated for an image which will then be placed as text over the
bottom of the image. This text file needs to have the same name as the
corresponding image but with the extension ".txt".

There are a number of configuration settings at the top of the source
file that one can use to tweak for optimal placement of the individual
images onto the bingo card. This includes the actual background image
to be used as well as spacing and size parameters. Also, the number of
individual images available and bingo cards to be generated can be set
here.

The software will then generate random sequences to place the
individual images onto the bingo cards. It will keep track of the
cards generated so far to avoid multiple winners. Currently, it checks
for all rows and colums as well as diagonals and four corners.

## Build

The source code uses cmake to generate the build environment. I only
tried this on Linux but it should work on other platforms as well
provided OpenCV is available. On Linux, a simple "cmake ." followed by
"make" will build it. On Windows, the cmake GUI and compiler of choice
is needed.

## Requirements

This software uses OpenCV to load images and place them at their
appropriate locations. In addition, cmake and a compile is needed.
