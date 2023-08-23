#pragma once
#include <vector>
#include <string>

namespace ModelField
{

	class ModelPoints
	{
		size_t size;
		std::vector <std::pair<double, double>> vecPoint;
	public:
		ModelPoints();
		int getSize() const;
		double distancePoints(const std::pair<double, double>& a, const std::pair<double, double>& b) const;
		
		void generatPoin(const size_t size = 100, const unsigned int length = 100, const unsigned int width = 100);
		void creatFile(std::string path);
		const std::vector <std::pair<double, double>>& getVecPoint() const;
		void writeFile(const std::string path) const;
	};
	
}