#pragma once
#ifndef _IMG_LIST_HPP
#define _IMG_LIST_HPP

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <string>
#include <string.h>
#include <cctype>
#include <vector>
#include <fstream>

using namespace std;
using namespace cv;

const unsigned TABLE_SIZE = 26;

struct NodeImg {
	string title;
	Mat img;
	NodeImg* next = nullptr;
};

// Nuestra estructura facilita la manipulación de muchas imagenes
// Permitiendo realizar procedimientos sobre todas ellas con facilidad.

class Hashmap {
private:	
	NodeImg** table;
	unsigned tablesize;
	unsigned sizes;

	unsigned hash(string key);
	void destroy_recursive(NodeImg* node);

	void change_size_node(NodeImg* node, double size_change);
	void crop_image_node(NodeImg* node, int range_1, int range_2, int range_3, int range_4);
	void rotate_image_node(NodeImg* node, double angle);
	void draw_line_node(NodeImg* node, Point start, Point end, Scalar color, int tickness);
	void draw_circle_node(NodeImg* node, Point center, int radius, Scalar color, int tickness); // -1 to be filled
	void draw_rectangle_node(NodeImg* node, Point superior_izq, Point inferior_der, Scalar color, int tickness);
	void text_node(NodeImg* node, string texto, Point centro, double font_size, Scalar color);
	void serialization_node(NodeImg* node);
	void download_node(NodeImg* node);

	// Serialization using the method Base 128 Varints
	// It does not work we explain in the last lines
	//string decToBinary(int n);
	//int binToDecimal(string s);
	//string cleanBin(string bin);
	//string encode(int num);
	//int count_delimiter(string s);
	//int decode(string code);
public:
	Hashmap();
	~Hashmap();

	/// Add images or change existing ones (same title)
	void add_image(string title, string path);
	// Delete image
	void remove(string title);
	// Display images
	void display_node(NodeImg* tmp);
	void display_letter(string a);
	void display_list();
	// Find by id
	NodeImg* find(string title);
	// Image resizing
	void change_size_letter(string title_or_letter, double size_change);
	void change_size_all(double size_change);
	// Cropping an image
	void crop_image_letter(string title_or_letter, int range_1, int range_2, int range_3, int range_4);
	void crop_image_all(int range_1, int range_2, int range_3, int range_4);
	// Image rotation and translation
	void rotate_image_letter(string title_or_letter, double angle);
	void rotate_image_all(double angle);
	// Annotating
	void draw_line_letter(string title_or_letter, Point start, Point end, Scalar color, int tickness);
	void draw_line_all(Point start, Point end, Scalar color, int tickness);
	void draw_circle_letter(string title_or_letter, Point center, int radius, Scalar color, int tickness); // -1 to be filled
	void draw_circle_all(string title_or_letter, Point center, int radius, Scalar color, int tickness); // -1 to be filled
	void draw_rectangle_letter(string title_or_letter, Point superior_izq, Point inferior_der, Scalar color, int tickness);
	void draw_rectangle_all(Point superior_izq, Point inferior_der, Scalar color, int tickness);
	void draw_text_letter(string title_or_letter, string texto, Point centro, double font_size, Scalar color);
	void draw_text_all(string texto, Point centro, double font_size, Scalar color);
	// cout titles of images
	void img_titles();
	// Download all images
	void download_all();
	// serialize

	void serialization_letter(string texto);
	void serialization_all();


};

// La función hash solo organiza las imagenes en el array a través de la
// primera letra se su titulo.
unsigned Hashmap::hash(string key) {
	int pos = 0;
	for (char& c : key) {
		int index = toupper(c) - 64;
		return index;
	}
	
}

// Constructor
Hashmap::Hashmap() {
	tablesize = TABLE_SIZE;
	table = new NodeImg * [tablesize];
	for (unsigned i = 0; i < tablesize; i++)
		table[i] = nullptr;
	sizes = 0;
}

// Destructor
Hashmap::~Hashmap() {
	for (unsigned i = 0; i < tablesize; i++) {
		NodeImg* tmp = table[i];
		destroy_recursive(tmp);
	}
	delete[] table;
}

// Función para el destructor
void Hashmap::destroy_recursive(NodeImg* node) {
	if (node != nullptr) {
		destroy_recursive(node->next);
		delete node;
	}
}

// Encuentra el apuntador a un NodeImg a través del titulo de la imagen
NodeImg* Hashmap::find(string title) {
	unsigned bucket = hash(title);
	NodeImg* tmp = table[bucket];
	while (tmp != nullptr) {
		if (tmp->title == title)
			return tmp;
		tmp = tmp->next;
	}
	return tmp;
}

// Se añade una imagen, si el titulo ya existe se intercambia la imagen
void Hashmap::add_image(string title, string path) {
	NodeImg* tmp = find(title);
	Mat r_img = imread(path);
	if (!tmp) {
		unsigned bucket = hash(title);
		NodeImg* newnode = new NodeImg;
		newnode->title = title;
		newnode->img = r_img;
		newnode->next = table[hash(title)];
		table[hash(title)] = newnode;
	} else {
		tmp->img = r_img;
	}
	sizes++;
}

// Se elimina una imagen a través del título
void Hashmap::remove(string title) {
	NodeImg* tmp = find(title);

	if (!tmp)
		throw std::runtime_error("remove: Key not found in table.");

	// Se obtienen las coordenadas
	unsigned index = hash(title);
	NodeImg* apuntador = table[index];

	if (apuntador == tmp) {
		table[index] = tmp->next;
	}
	else {
		while (apuntador->next != tmp)
			apuntador = apuntador->next;
		apuntador->next = tmp->next;
	}
	delete tmp;
	sizes--;
}

/* Display funcions
*/

// Show the img that is in an struct NodeImg. The image is showed until a keyboard is press
void Hashmap::display_node(NodeImg* tmp) {
	imshow(tmp->title, tmp->img);
	waitKey(0);
}

// Search the pointer which correspond to a title and display it
void Hashmap::display_letter(string title_or_letter) {
	if (title_or_letter.length() == 1) {
		unsigned index = hash(title_or_letter);
		NodeImg* tmp = table[index];
		while (tmp != nullptr) {
			display_node(tmp);
			tmp = tmp->next;
		}
	} else {
		NodeImg* tmp = find(title_or_letter);
		if (!tmp)
			display_node(tmp);
		else
			cout << "title does not exists" << endl;
	}
}

// Display all the images included in the list.
void Hashmap::display_list() {
	for (unsigned i = 0; i < tablesize; i++) {
		NodeImg* tmp = table[i];
		while (tmp != nullptr) {
			display_node(tmp);
			tmp = tmp->next;
		}
	}
}

/* Resizing - Change the resolution of the image multiplying by a double
*/

void Hashmap::change_size_node(NodeImg* node, double size_change) {
	int width = node->img.size().width;
	int height = node->img.size().height;
	width = width * size_change;
	height = height * size_change;
	resize(node->img, node->img, Size(width, height), INTER_LINEAR);
}

void Hashmap::change_size_letter(string title_or_letter, double size_change) {
	if (title_or_letter.length() == 1) {
		unsigned index = hash(title_or_letter);
		NodeImg* tmp = table[index];
		while (tmp != nullptr) {
			change_size_node(tmp,size_change);
			tmp = tmp->next;
		}
	} else {
		NodeImg* tmp = find(title_or_letter);
		if (tmp)
			change_size_node(tmp, size_change);
		else
			cout << "title does not exists" << endl;
	}
}

void Hashmap::change_size_all(double size_change) {
	for (unsigned i = 0; i < tablesize; i++) {
		NodeImg* tmp = table[i];
		while (tmp != nullptr) {
			change_size_node(tmp, size_change);
			tmp = tmp->next;
		}
	}
}

/* Crop images - ranges are the limits of the rectangle
*/
void Hashmap::crop_image_node(NodeImg* node, int range_1, int range_2, int range_3, int range_4) {
	cout << "crop_node" << endl;
	node->img = node->img(Range(range_1, range_2), Range(range_3, range_4));
}

void Hashmap::crop_image_letter(string title_or_letter, int range_1, int range_2, int range_3, int range_4) {
	if (title_or_letter.length() == 1) {
		unsigned index = hash(title_or_letter);
		NodeImg* tmp = table[index];
		while (tmp != nullptr) {
			crop_image_node(tmp, range_1, range_2, range_3, range_4);
			tmp = tmp->next;
		}
	}
	else {
		NodeImg* tmp = find(title_or_letter);
		if (tmp) {
			crop_image_node(tmp, range_1, range_2, range_3, range_4);
		}
		else
			cout << "title does not exists" << endl;
	}
}

void Hashmap::crop_image_all(int range_1, int range_2, int range_3, int range_4) {
	for (unsigned i = 0; i < tablesize; i++) {
		NodeImg* tmp = table[i];
		while (tmp != nullptr) {
			crop_image_node(tmp, range_1, range_2, range_3, range_4);
			tmp = tmp->next;
		}
	}
}

/* Rotate images - angle in degrees
*/
void Hashmap::rotate_image_node(NodeImg* node, double angle) {
	Point2f center((node->img.cols - 1) / 2.0, (node->img.rows - 1) / 2.0);
	Mat rotation_matrix = getRotationMatrix2D(center, angle, 1.0);
	warpAffine(node->img, node->img, rotation_matrix, node->img.size());
}

void Hashmap::rotate_image_letter(string title_or_letter, double angle) {
	if (title_or_letter.length() == 1) {
		unsigned index = hash(title_or_letter);
		NodeImg* tmp = table[index];
		while (tmp != nullptr) {
			rotate_image_node(tmp,angle);
			tmp = tmp->next;
		}
	}
	else {
		NodeImg* tmp = find(title_or_letter);
		if (tmp)
			rotate_image_node(tmp,angle);
		else
			cout << "title does not exists" << endl;
	}
}

void Hashmap::rotate_image_all(double angle) {
	for (unsigned i = 0; i < tablesize; i++) {
		NodeImg* tmp = table[i];
		while (tmp != nullptr) {
			rotate_image_node(tmp, angle);
			tmp = tmp->next;
		}
	}
}

/* Annotate images
*/

// Lines - Punto de inicio y final
void Hashmap::draw_line_node(NodeImg* node, Point start, Point end, Scalar color =(0,0,255), int tickness = 3) {
	line(node->img, start, end, color, tickness, 8, 0);
}

void Hashmap::draw_line_letter(string title_or_letter, Point start, Point end, Scalar color = (0, 0, 255), int tickness = 3) {
	if (title_or_letter.length() == 1) {
		unsigned index = hash(title_or_letter);
		NodeImg* tmp = table[index];
		while (tmp != nullptr) {
			draw_line_node(tmp, start, end, color, tickness);
			tmp = tmp->next;
		}
	}
	else {
		NodeImg* tmp = find(title_or_letter);
		if (tmp)
			draw_line_node(tmp, start, end, color, tickness);
		else
			cout << "title does not exists" << endl;
	}
}

void Hashmap::draw_line_all(Point start, Point end, Scalar color = (0, 0, 255), int tickness = 3) {
	for (unsigned i = 0; i < tablesize; i++) {
		NodeImg* tmp = table[i];
		while (tmp != nullptr) {
			draw_line_node(tmp, start, end, color, tickness);
			tmp = tmp->next;
		}
	}
}

// Circle - Punto central y radio
void Hashmap::draw_circle_node(NodeImg* node, Point center, int radius, Scalar color = (0, 0, 255), int tickness = 3) {
	circle(node->img, center, radius, color, tickness, 8, 0);
}

void Hashmap::draw_circle_letter(string title_or_letter, Point center, int radius, Scalar color = (0, 0, 255), int tickness = 3) {
	if (title_or_letter.length() == 1) {
		unsigned index = hash(title_or_letter);
		NodeImg* tmp = table[index];
		while (tmp != nullptr) {
			draw_circle_node(tmp, center, radius, color, tickness);
			tmp = tmp->next;
		}
	}
	else {
		NodeImg* tmp = find(title_or_letter);
		if (tmp)
			draw_circle_node(tmp, center, radius, color, tickness);
		else
			cout << "title does not exists" << endl;
	}
}

void Hashmap::draw_circle_all(string title_or_letter, Point center, int radius, Scalar color = (0, 0, 255), int tickness = 3) {
	for (unsigned i = 0; i < tablesize; i++) {
		NodeImg* tmp = table[i];
		while (tmp != nullptr) {
			draw_circle_node(tmp, center, radius, color, tickness);
			tmp = tmp->next;
		}
	}
}

// Rectangle
void Hashmap::draw_rectangle_node(NodeImg* node, Point superior_izq, Point inferior_der, Scalar color = (0, 0, 255), int tickness = 3) {
	rectangle(node->img, superior_izq, inferior_der, color, tickness, 8, 0);
}

void Hashmap::draw_rectangle_letter(string title_or_letter, Point superior_izq, Point inferior_der, Scalar color = (0, 0, 255), int tickness = 3) {
	if (title_or_letter.length() == 1) {
		unsigned index = hash(title_or_letter);
		NodeImg* tmp = table[index];
		while (tmp != nullptr) {
			draw_rectangle_node(tmp, superior_izq, inferior_der, color, tickness);
			tmp = tmp->next;
		}
	}
	else {
		NodeImg* tmp = find(title_or_letter);
		if (tmp)
			draw_rectangle_node(tmp, superior_izq, inferior_der, color, tickness);
		else
			cout << "title does not exists" << endl;
	}
}

void Hashmap::draw_rectangle_all(Point superior_izq, Point inferior_der, Scalar color = (0, 0, 255), int tickness = 3) {
	for (unsigned i = 0; i < tablesize; i++) {
		NodeImg* tmp = table[i];
		while (tmp != nullptr) {
			draw_rectangle_node(tmp, superior_izq, inferior_der, color, tickness);
			tmp = tmp->next;
		}
	}
}


/* Text
*/ 
void Hashmap::text_node(NodeImg* node, string texto, Point origen, double font_size = 1.5, Scalar color = (0, 0, 255)) {
	putText(node->img, texto, origen, FONT_HERSHEY_COMPLEX, font_size, color);
}

void Hashmap::draw_text_letter(string title_or_letter, string texto, Point origen, double font_size = 1.5, Scalar color = (0, 0, 255)) {
	if (title_or_letter.length() == 1) {
		unsigned index = hash(title_or_letter);
		NodeImg* tmp = table[index];
		while (tmp != nullptr) {
			text_node(tmp, texto, origen, font_size, color);
			tmp = tmp->next;
		}
	}
	else {
		NodeImg* tmp = find(title_or_letter);
		if (tmp)
			text_node(tmp, texto, origen, font_size, color);
		else
			cout << "title does not exists" << endl;
	}
}

void Hashmap::draw_text_all(string texto, Point origen, double font_size = 1.5, Scalar color = (0, 0, 255)) {
	for (unsigned i = 0; i < tablesize; i++) {
		NodeImg* tmp = table[i];
		while (tmp != nullptr) {
			text_node(tmp, texto, origen, font_size, color);
			tmp = tmp->next;
		}
	}
}

/* Display titles
*/
void Hashmap::img_titles() {
	for (unsigned i = 0; i < tablesize; i++) {
		NodeImg* tmp = table[i];
		while (tmp != nullptr) {
			cout << tmp->title << endl;
			tmp = tmp->next;
		}
	}
}

/* Download images
*/

void Hashmap::download_node(NodeImg* node) {
	imwrite("Output/" + node->title + ".jpg", node->img);
}

void Hashmap::download_all() {
	for (unsigned i = 0; i < tablesize; i++) {
		NodeImg* tmp = table[i];
		while (tmp != nullptr) {
			download_node(tmp);
			tmp = tmp->next;
		}
	}
}

/* Serialization to xml
*/

void Hashmap::serialization_node(NodeImg* node) {
	FileStorage fs("Output/" + node->title+".xml", FileStorage::WRITE);
	Mat serialized_matrix = node->img;
	fs << "serialized_matrix" << serialized_matrix;
	fs.release();
}
 
void Hashmap::serialization_letter(string texto) {
	if (texto.length() == 1) {
		unsigned index = hash(texto);
		NodeImg* tmp = table[index];
		while (tmp != nullptr) {
			serialization_node(tmp);
			tmp = tmp->next;
		}
	}
	else {
		NodeImg* tmp = find(texto);
		if (tmp)
			serialization_node(tmp);
		else
			cout << "title does not exists" << endl;
	}
}

void Hashmap::serialization_all() {
	for (unsigned i = 0; i < tablesize; i++) {
		NodeImg* tmp = table[i];
		while (tmp != nullptr) {
			serialization_node(tmp);
			tmp = tmp->next;
		}
	}
}

// Serialization where we use Base 128 Varints method
// This section is commented because generate an error at the moment of push_back
// tmp_serialize into img_serialize.
/* 
#include <iostream>
#include <string>
#include <string.h>
using namespace std;

string decToBinary(int n)
{
    // array que guarda el num binario

    string binary = "";

    // loop para agregar valores a binary
    while (n > 0) {

        binary = binary + to_string(n % 2);
        n = n / 2;
    }

    // array para guardar el num binario en reversa
    string binaryNum = "";
    int a = 0;
    for (int i = binary.size()-1; i >= 0; i--){
        binaryNum = binaryNum + binary[i];

        a++;
      }

    return binaryNum;
}

int binToDecimal(string s){
  int sum = 0;
  int b = 1;
  for(int i = s.size()-1; i >= 0; i--){
    char val = s[i];
    if(val == '1'){
      sum = sum + b;
    }
    b = b*2;
  }
  return sum;
}

string cleanBin(string bin){
  while (bin.size()%8 != 0){
    bin = "0" + bin;
  }
  return bin;
}

string encode(int num){

  string b = decToBinary(num);
  string bin = cleanBin(b);

  int nBytes = bin.size()/8;
  string arr0[nBytes];
  int arr1[nBytes];

  for(int i = 0; i < nBytes; i++){
    arr0[i] = bin.substr((bin.size()-i*8)-8,8);
  }

  for(int j = 0; j < nBytes; j++){
    arr1[j] = binToDecimal(arr0[j]);
  }

  string st = "";
  for(int k = 0; k < nBytes; k++){
    st = st + to_string(arr1[k]) + " " ;
  }

  string code = st.substr(0,st.size()-1);

  return code;
}


int count_delimiter(string s){
  int count = 0;

  for (int i = 0; i < s.size(); i++){
    if (s[i] == ' ')
      count++;
  }
  return count;
}


int decode(string code){
  int n = count_delimiter(code)+1;
  string arr0[n];
  string temp = code;
  for(int i = 0; i < n-1; i++){
    string s = temp.substr(0, temp.find(" "));
    arr0[i] = s;
    temp = temp.substr(temp.find(" ")+1,temp.size());
  }
  arr0[n-1] = temp;

  string arr1[n];
  for(int j = 0; j < n; j++){
    int v = stoi(arr0[j]);
    string bin = decToBinary(v);
    arr1[j] = cleanBin(bin);
  }

  string val = "";
  for(int k = n-1; k >= 0; k--){
    val = val + arr1[k];
  }

  int numDec = binToDecimal(val);

  return numDec;

}

vector<vector<string>> img_serialize;

uint8_t *mydata = node->img.data;
int width = node->img.cols;
int height = node->img.rows;
int _stride = node->img.step;//in case cols != strides
for (int i = 0; i < height; i++)
{
	vector<string> tmp_serialize;
	for (int j = 0; j < width; j++)
	{
		uint8_t val = mydata[i * _stride + j];
		int a = val;
		tmp_serialize.push_back(encode(a));
		//do whatever you want with your value
	}
	
	cout << "height" << i << endl;
	//for (unsigned i = 0; i < tmp_serialize.size(); i++)
	//	cout << "Value " << encode(tmp_serialize[i]) << endl;
	img_serialize.push_back(tmp_serialize);
}



for (int i = 0; i < height; i++) {
	for (int j = 0; j < width; j++) {
		int v = img_serialize[0][0];
		string a = encode(v);
		cout << a << endl;
	}
}
*/

#endif