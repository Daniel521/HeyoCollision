#include "LoadPNG\lodepng.h"
#include <HeyoEngine.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

class Collision
{
public:
	unsigned w;
	unsigned h;
	unsigned char * out;
	std::string address;
	std::vector<Heyo::Rect> rect_list;
	std::vector<std::vector<bool>> grid;
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
	Heyo::Rect makeRect(int x, int y);
	bool isListed(int x, int y);

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
	for (int i = 0; i < h; i++)
	{
		grid[i].resize(w);
		for (int j = 0; j < w; j++)
		{
			grid[i][j] = false;
		}
	}

	int green_pix;
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			green_pix = y * w * 3 + (x * 3) + 1;
			//std::cout << green_pix << ": " << static_cast<int>(out[green_pix]) << std::endl;
			if (out[green_pix] == 255)
			{

				//std::cout << "Green:\t( " << x << " , " << y << " )   " << green_pix << std::endl;
				grid[y][x] = true;
			}
			else
			{
				//std::cout << "\t( " << x << " , " << y << " )   " << green_pix << std::endl;
			}
		}
	}

}

void Collision::display()
{
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
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

	for (int i = 0; i < rect_list.size(); i++)
	{
		write << rect_list[i].x << ' ' <<
			rect_list[i].y << ' ' <<
			rect_list[i].w << ' ' <<
			rect_list[i].h << '\n';
	}
	write.close();
}

void Collision::makeRectList()
{
	//std::cout << "Making Rect_List" << std::endl;

	for (int y = 0; y < grid.size(); y++)
	{
		for (int x = 0; x < grid[y].size(); x++)
		{
			if (grid[y][x] == true && isListed(x, y) == false)
			{
				rect_list.push_back(makeRect(x, y));
			}
		}
	}

}

Heyo::Rect Collision::makeRect(int x, int y)
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
			if (grid[i][j] != true && j == x)
			{
				max_y = i;
				break;
			}
			if (grid[i][j] != true)
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

	//std::cout << "Rect: " << rect.x << " , " << rect.y << " , " << rect.w << " , " << rect.h << "." << std::endl;

	return rect;
}

bool Collision::isListed(int x, int y)
{
	SDL_Point dot = { x,y };
	for (int i = 0; i < rect_list.size(); i++)
	{
		if (SDL_PointInRect(&dot, &rect_list[i]))
			return true;
	}
	return false;
}
