#pragma once
#include <vector>
#include <list>
#include <fstream>
#include "ModelPoints.h"
#include "matrix.h"

namespace AntAlgorithm {


	class Point
	{
	public:
		double x = 0., y = 0.;
		int cluster = -1;

		Point();
		Point(double x, double y);
		static double distPoints(const Point& a, const Point& b);
		~Point();
	};
	
	class AntClustering
	{
		struct Matrices
		{
			double dist = 0;
			double pher = 0.2;
			double delta_pher = 0;
		};
		double valueTargetFunc = DBL_MAX;
		int k, n;
		
		std::vector <std::vector <Matrices>> distMatrix;
		std::vector <AntAlgorithm::Point> vecPoint;
		std::vector <AntAlgorithm::Point> vecCentroid;
		std::vector <AntAlgorithm::Point> bestVecPoint;
		std::vector <AntAlgorithm::Point> bestVecCentroid;
		std::list<std::pair<double, double>> path;

		double poW(double num, int p);
		void initVecFirstPoint(std::list<int>& clusterPoint, std::list<int>& NoclusterPoint);
		int probability(int pointIndex, std::list<int>& clusterPoint, double alph, double bett);
		double targetFunc();
		void changepher(double p);
		Matrix createMatrix(std::vector<std::vector<int>>& nearestElemCluster, AntAlgorithm::Point& base);

	public:
		double findPaht(std::pair<double, double> base = std::pair<double, double>(50., 50.));
		double algoritm(int k = 3, int lifeTime = 10, int numberAnt = 10, double alph = 1, double bett = 5, double Q = 10000, double p = 0.38);
		void writeFile(const std::string path) const;
		void writeFilePath(const std::string path) const;

		AntClustering(const ModelField::ModelPoints& createModel);

		~AntClustering();
	};
}