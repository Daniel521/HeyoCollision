#include "LoadPNG\lodepng.h"
#include <HeyoEngine.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class Collision
{
public:
	enum Pixel
	{
		black = 0,
		red = 1,
		green = 2,
		blue = 3
	};
public:
	class Rect_list
	{
	public:
		Heyo::Rect rect;
		Pixel color;
	};
public:
	unsigned w;
	unsigned h;
	unsigned char * out;
	std::string address;
	std::vector<Rect_list> rect_list;
	std::vector<std::vector<Pixel>> grid;
	string write_address;
	std::ofstream write;
public:
	Collision()
	{
		out = NULL;
	}
	~Collision()
	{
		free(out);
	}

	void calculate();
	void display();
	void showInfo();
	void output();

private:
	void makeGrid();
	void makeRectList();
	Heyo::Rect makeRect(int x, int y, Pixel color);
	bool isListed(int x, int y, Pixel color);

};

void welcome();

int HeyoMain()
{
	Collision coll;

	welcome();
	std::cin >> coll.address;
	coll.address = "Data\\" + coll.address;
	std::cout << std::endl;

	if (lodepng_decode24_file(&coll.out, &coll.w, &coll.h, coll.address.c_str()) != 0)
	{
		// it broke
		std::cout << "Error" << std::endl;
		std::cin.get();
		std::cin.get();
		return 0;
	}

	std::cout << coll.address << " was found!" << std::endl;
	std::cout << "Loading up information..." << std::endl << std::endl;

	coll.calculate();

	coll.showInfo();
	//coll.display();
	coll.output();

	std::cout << std::endl;
	std::cout << "All done. Good bye!" << std::endl;
	std::cin.get();

	return 0;
}

void welcome()
{
	std::cout << "    Heyo Collision    " << std::endl;
	std::cout << "----------------------" << std::endl << std::endl;

	std::cout << "Make sure to insert the png file in the same Data folder." << std::endl;
	std::cout << "Enter the name of the png file (including the .png):" << std::endl;
	std::cout << "> ";
}

void Collision::calculate()
{
	makeGrid();
	makeRectList();
}

void Collision::makeGrid()
{
	grid.resize(h);
	for (unsigned int i = 0; i < h; i++)
	{
		grid[i].resize(w);
		for (unsigned int j = 0; j < w; j++)
		{
			grid[i][j] = Pixel::black;
		}
	}

	int pix;
	for (unsigned int y = 0; y < h; y++)
	{
		for (unsigned int x = 0; x < w; x++)
		{
			pix = y * w * 3 + (x * 3); // Pixel at red
			//std::cout << green_pix << ": " << static_cast<int>(out[green_pix]) << std::endl;
			if (out[pix] == 255)
			{

				//std::cout << "Green:\t( " << x << " , " << y << " )   " << green_pix << std::endl;
				grid[y][x] = Pixel::red;
			}
			else if (out[pix+1] == 255)
			{

				//std::cout << "Green:\t( " << x << " , " << y << " )   " << green_pix << std::endl;
				grid[y][x] = Pixel::green;
			}
			else if (out[pix+2] == 255)
			{

				//std::cout << "Green:\t( " << x << " , " << y << " )   " << green_pix << std::endl;
				grid[y][x] = Pixel::blue;
			}
		}
	}

}

void Collision::display()
{
	for (unsigned int i = 0; i < h; i++)
	{
		for (unsigned int j = 0; j < w; j++)
		{
			std::cout << static_cast<int>(grid[i][j]) << ' ';
		}
		std::cout << std::endl;
	}
}

void Collision::showInfo()
{
	std::cout << address << " Info:" << std::endl;
	std::cout << "-------------------------------" << std::endl;
	std::cout << "width: " << w << '\t' << "height: " << h << std::endl;
	std::cout << "Number of Rects: " << rect_list.size() << std::endl;
}

void Collision::output()
{
	std::cout << "Type the name of the file you would like to output," << std::endl << "it will be stored in the Data folder." << std::endl;
	std::cout << "Ex: collision.txt (make sure to type the .txt part as well)" << std::endl;
	std::cout << "> ";
	std::cin >> write_address;
	write_address = "Data\\" + write_address;

	write.open(write_address);
	write << "Coll:\n";
	for (int i = 0; i < rect_list.size(); i++)
	{
		if (rect_list[i].color == Pixel::green)
		{
		write << rect_list[i].rect.x << ' ' <<
			rect_list[i].rect.y << ' ' <<
			rect_list[i].rect.w << ' ' <<
			rect_list[i].rect.h << '\n';
		}
	}
	write << "Spawn:\n";
	for (int i = 0; i < rect_list.size(); i++)
	{
		if (rect_list[i].color == Pixel::red)
		{
			write << rect_list[i].rect.x << ' ' <<
				rect_list[i].rect.y << ' ' <<
				rect_list[i].rect.w << ' ' <<
				rect_list[i].rect.h << '\n';
		}
	}
	write << "Enc:\n";
	for (int i = 0; i < rect_list.size(); i++)
	{
		if (rect_list[i].color == Pixel::blue)
		{
			write << rect_list[i].rect.x << ' ' <<
				rect_list[i].rect.y << ' ' <<
				rect_list[i].rect.w << ' ' <<
				rect_list[i].rect.h << '\n';
		}
	}

	write.close();
}

void Collision::makeRectList()
{
	for (int y = 0; y < grid.size(); y++)
	{
		for (int x = 0; x < grid[y].size(); x++)
		{
			if (grid[y][x] == Pixel::red && isListed(x, y,Pixel::red) == false)
			{
				rect_list.push_back({ makeRect(x, y, Pixel::red),red });
			}
			if (grid[y][x] == Pixel::green && isListed(x, y, Pixel::green) == false)
			{
				rect_list.push_back({ makeRect(x, y, Pixel::green), Pixel::green });
			}
			if (grid[y][x] == Pixel::blue && isListed(x, y, Pixel::blue) == false)
			{
				rect_list.push_back({ makeRect(x, y, Pixel::blue),Pixel::blue });
			}
		}
	}
}

Heyo::Rect Collision::makeRect(int x, int y, Pixel color)
{
	Heyo::Rect rect;
	rect.x = x;
	rect.y = y;

	int max_x = w;
	int max_y = h;

	for (int i = y; i < max_y; i++)
	{
		for (int j = x; j < max_x; j++)
		{
			if (grid[i][j] != color && j == x)
			{
				max_y = i;
				break;
			}
			if (grid[i][j] != color)
			{
				if (j < max_x)
				{
					max_x = j;
				}
				break;
			}

		}
	}
	rect.h = max_y - y;
	rect.w = max_x - x;
	return rect;
}

bool Collision::isListed(int x, int y, Pixel color)
{
	SDL_Point dot = { x,y };
	for (int i = 0; i < rect_list.size(); i++)
	{
		if (SDL_PointInRect(&dot, &rect_list[i].rect) && rect_list[i].color == color)
			return true;
	}
	return false;
}
