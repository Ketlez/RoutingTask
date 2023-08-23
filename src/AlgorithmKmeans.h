#pragma once
#include <vector>
#include <fstream>
#include "ModelPoints.h"
#include "matrix.h"
#include <list>
#include "methodBranchesBoundaries.h"

namespace AlgorithmKmeans {
	
	class Point
	{
	public:
		double x = 0., y = 0.;
		int cluster = -1;
		
		Point(double x, double y);
		static double distPoints(const Point& a, const Point& b);
		Point();
		~Point();
	};

	class Kmeans
	{
		double valueTargetFunc = DBL_MAX;
		int k, n;
		std::vector <AlgorithmKmeans::Point> vecPoint;
		std::vector <AlgorithmKmeans::Point> vecCentroid;
		std::list<std::pair<double, double>> path;

		void generCentroids();
		void clustering();
		void updateCentroids();
		double targetFunc() const;
		Matrix createMatrix(std::vector<std::vector<int>>& nearestElemCluster, AlgorithmKmeans::Point& base) const;

	public:
		double algoritm(int numberCluster = 3);
		double findPaht(std::pair<double, double> base = std::pair<double, double>(50., 50.));
		void writeFile(const std::string path) const;
		void writeFilePath(const std::string path) const;

		Kmeans(const ModelField::ModelPoints& createModel);
		~Kmeans();
	};
}