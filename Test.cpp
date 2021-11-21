#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <list_img.h>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
	Hashmap mylist;

	// Paths
	string path = "Resources/sol.jpg";
	string path_2 = "Resources/smily.jpg";
	string path_3 = "Resources/colombia.jpg";
	
	// Add images
	mylist.add_image("Sol", path);
	mylist.add_image("Colombia", path_3);
	mylist.add_image("Chip", path_2);

	// Display
	mylist.display_letter("C");
	
	// Resize
	mylist.change_size_letter("Colombia",0.5);
	mylist.display_letter("Colombia");

	// Crop image
	mylist.crop_image_letter("Sol", 50,600,50,600);
	// Rotate image
	mylist.rotate_image_all(20);
	mylist.display_list();

	// Annotate image
	Point start_point(300, 115);
	Point end_point(475, 225);

	mylist.draw_rectangle_all(start_point, end_point);
	mylist.draw_circle_letter("Sol", start_point, 100);
	mylist.draw_line_letter("C", start_point, end_point);
	mylist.draw_text_all("Proyecto algoritmos", Point(600, 600));
	
	mylist.display_list();

	// Print list of titles on the images
	mylist.img_titles();

	// Un archivo .xml es generado en la carpeta Output 
	mylist.serialization_letter("Chip");
	// La serialización no afecta el input dentro de la estructura
	mylist.display_letter("Chip");

	// Download all the modified images
	mylist.download_all();
	return 0;

}