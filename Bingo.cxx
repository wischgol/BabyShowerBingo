#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <list>
#include <set>

// background image for bingo card
#define BACKGROUND "background.jpg"
// number of tiles per bingo card
#define N 25
// number of images to choose from (numbered 0 - Max-1)
#define Max 100
// number of tiles per row
#define Width 5
// number of tiles per column (should be the same as Width)
#define Height 5
// x-offset where first tile needs to be put
#define Xoffset 180
// y-offset where first tile needs to be put
#define Yoffset 688
// delta offset between tiles in x direction
#define Xdelta 240
// delta offset between tiles in y direction
#define Ydelta 240

std::list<int*> arraylist;

int* randomArray ()
{
  // Source: https://lemire.me/blog/2013/08/16/picking-n-distinct-numbers-at-random-how-to-do-it-fast/
#if 0
  HashSet<Integer> s = new HashSet<Integer>();
  while (s.size() < N)
  s.add(rand.nextInt(Max));
#else
  int *ans = new int[N];
  for (int k = 0; k < N; ++k)
    ans[k]=k;
  for(int k = N ; k < Max; ++k) {
    int v = std::rand() % (k + 1); // * (k+1) / RAND_MAX;
    if (v < N) {
      ans[v] = k;
    }
  }
  ans[Width*(Height/2) + Width/2] = Max;
  return ans;
#endif
}

int checkArray (std::set<int> entry, int *array)
{
  std::set<int> comp, diff;
    int i, j;

    // check first diagonal
    for (i=0; i<Width; i++) {
      comp.insert (array[Width*i+i]);
    }
    if (entry == comp)
      return 0;

    // check second diagonal
    comp.clear ();
    for (i=0; i<Width; i++) {
      comp.insert (array[Width*i + Width -i - 1]);
    }
    if (entry == comp)
      return 0;

    // check each row
    comp.clear ();
    for (j=0; j<Height; j++) {
      for (i=0; i<Width; i++) {
        comp.insert (array[Width*j + i]);
      }
      if (entry == comp)
	return 0;
      else {
	diff.clear ();
	std::set_difference(entry.begin(), entry.end(), 
	                    comp.begin(), comp.end(),
	                    std::inserter(diff, diff.begin()));
	if (diff.size () == 1 && *(diff.begin ()) == Max)
	  return 0;
	diff.clear ();
	std::set_difference(comp.begin(), comp.end(), 
	                    entry.begin(), entry.end(),
	                    std::inserter(diff, diff.begin()));
	if (diff.size () == 1 && *(diff.begin ()) == Max)
	  return 0;
      }
    }

    // check each column
    comp.clear ();
    for (j=0; j<Width; j++) {
      for (i=0; i<Height; i++) {
        comp.insert (array[Width*i + j]);
      }
      if (entry == comp)
	return 0;
      else {
	diff.clear ();
	std::set_difference(entry.begin(), entry.end(), 
	                    comp.begin(), comp.end(),
	                    std::inserter(diff, diff.begin()));
	if (diff.size () == 1 && *(diff.begin ()) == Max)
	  return 0;
	diff.clear ();
	std::set_difference(comp.begin(), comp.end(), 
	                    entry.begin(), entry.end(),
	                    std::inserter(diff, diff.begin()));
	if (diff.size () == 1 && *(diff.begin ()) == Max)
	  return 0;
      }
    }

    return 1;
}

int checkArray (int *array)
{
  std::list<int*>::iterator iter = arraylist.begin ();
  std::set<int> comp;
  int i, j;

  while (iter != arraylist.end ()) {
    // check first diagonal
    comp.clear ();
    for (i=0; i<Width; i++) {
      comp.insert (array[Width*i + 1]);
    }
    if (checkArray (comp, *iter) == 0)
      return 0;

    // check second diagonal
    comp.clear ();
    for (i=0; i<Width; i++) {
      comp.insert (array[Width*i + Width - i - 1]);
    }
    if (checkArray (comp, *iter) == 0)
      return 0;

    // check each row
    for (j=0; j<Height; j++) {
      comp.clear ();
      for (i=0; i<Width; i++) {
        comp.insert (array[Width*j + i]);
      }
      if (checkArray (comp, *iter) == 0)
	return 0;
    }

    // check each column
    for (j=0; j<Width; j++) {
      comp.clear ();
      for (i=0; i<Height; i++) {
        comp.insert (array[Width*i + j]);
      }
      if (checkArray (comp, *iter) == 0)
	return 0;
    }

    // check corners
    if (array[0] == (*iter)[0] &&
	array[Width-1] == (*iter)[Width-1] &&
	array[Width*Height-Width] == (*iter)[Width*Height-Width] &&
	array[Width*Height-1] == (*iter)[Width*Height-1])
      return 0;

    ++iter;
  }

  return 1;
}

void addText (cv::Mat image, int x, int y, const cv::String &text)
{
  auto font = cv::FONT_HERSHEY_SCRIPT_SIMPLEX;
  cv::putText(image,text,cv::Point (x, y), font, 1,
	      cv::Scalar (148,24,248), 2, cv::LINE_AA);
}

void insert (cv::Mat small_image, cv::Mat big_image, int x, int y)
{
  //printf ("%d %d %d %d %d %d\n", x, y, small_image.cols, small_image.rows, big_image.cols, big_image.rows);
  small_image.copyTo(big_image(cv::Rect(x,y,small_image.cols,
					small_image.rows)));
}

cv::Mat loadImage (int number)
{
  cv::Mat result;
  char filename[64], line[64];
  sprintf (filename, "../images/image-%d.jpg", number);
  result = cv::imread (filename, cv::IMREAD_COLOR);

  sprintf (filename, "../images/image-%d.txt", number);
  std::ifstream file (filename, std::ifstream::in);
  if (file.is_open ()) {
    file.getline (line, 63);

    if (file.good ())
      addText (result, 5, 180, cv::String (line));

    file.close ();
  }

  return result;
}

int main( int argc, char *argv[] )
{
  int i, j, k, result;
  char filename[64];
  int *array = 0;

  std::vector<int> compression_params;
  compression_params.push_back(cv::IMWRITE_PNG_COMPRESSION);
  compression_params.push_back(9);

  /* initialize random seed: */
  std::srand (time(NULL));

#if 0
  array = randomArray ();
  for (i=0; i<N; i++)
    std::cout << array[i] << std::endl;
  exit (0);
#endif

  cv::Mat bingotile, icon;

  for (i=0; i<40; i++) {
    sprintf (filename, "bingocard-%d.jpg", i);

    printf ("Generating card %s\n", filename);

    bingotile = cv::imread (BACKGROUND, cv::IMREAD_COLOR);

    // randomly insert icon images
    do {
      if (array)
	delete [] array;
      array = randomArray ();
    } while (checkArray (array) == 0);

#if 0
    for (j=0; j<25; j++)
      printf ("%d ", array[j]);
    printf ("\n");
#endif

    for (j=0; j<Width; j++)
      for (k=0; k<Height; k++) {
	// skip the center image
	if (j != Width/2 ||
	    k != Height/2) {
	  icon = loadImage (array[k*Width + j]);

	  insert (icon,
		  bingotile,
		  Xoffset + j * Xdelta,
		  Yoffset + k * Ydelta);
	}
      }

    arraylist.push_back (array);
    array = 0;

    try {
      result = cv::imwrite (filename, bingotile);
    }
    catch (const cv::Exception& ex)
    {
        fprintf(stderr,
		"Exception converting image to PNG format: %s\n",
		ex.what());
    }

    if (!result)
      printf ("error writing file %s\n", filename);
  }
}
