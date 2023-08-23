#include "ModelPoints.h"
#include <math.h>
#include <fstream>
#include <string>
#include <sstream>

ModelField::ModelPoints::ModelPoints(){}

int ModelField::ModelPoints::getSize() const
{
	return vecPoint.size();
}

void ModelField::ModelPoints::generatPoin(const size_t size, const unsigned int length, const unsigned int width)
{
	for (size_t i = 0; i < size; i++)
	{
		double x = (double)rand() / RAND_MAX * (width);
		double y = (double)rand() / RAND_MAX * (length);
		vecPoint.push_back(std::pair<double, double>(x, y));
	}
}

void ModelField::ModelPoints::creatFile(std::string path)
{
	std::string line;
	std::ifstream file(path);

	while (getline(file, line)) {
		std::stringstream lineStream(line);
		std::string bit;
		double x, y;
		getline(lineStream, bit, ',');
		x = stof(bit);
		getline(lineStream, bit, '\n');
		y = stof(bit);

		vecPoint.push_back(std::pair<double, double>(x,y));
	}
}

const std::vector<std::pair<double, double>>& ModelField::ModelPoints::getVecPoint() const
{
	return vecPoint;
}

void ModelField::ModelPoints::writeFile(const std::string path) const
{
	std::ofstream file(path);
	file << "x,y" << std::endl;

	for (auto it = begin(vecPoint); it != end(vecPoint); ++it)
	{
		file << it->first << "," << it->second << std::endl;
	}

	file.close();
}

double ModelField::ModelPoints::distancePoints(const std::pair<double, double>& a, const std::pair<double, double>& b) const
{
	return sqrt((a.first-b.first) * (a.first - b.first) + (a.second - b.second) * (a.second - b.second));
}