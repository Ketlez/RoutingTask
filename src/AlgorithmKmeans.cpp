#include "algorithmKmeans.h"
#include <iostream>

void AlgorithmKmeans::Kmeans::generCentroids()
{
	std::vector<int> indexV(k, -1);
	for (int i = 0; i < k; ++i)
	{
		int index;
		bool flag = true;	
		while (flag)
		{
			flag = false;
			index = rand() % n;
			if(std::find(indexV.begin(), indexV.end(), index) != indexV.end())
				flag = true;
		}
		indexV[i] = index;
		vecCentroid.push_back(vecPoint[index]);
	}
}

void AlgorithmKmeans::Kmeans::clustering()
{
	for (auto p = begin(vecPoint); p != end(vecPoint); ++p)
	{
		double minDist = DBL_MAX;
		for (auto c = begin(vecCentroid); c != end(vecCentroid); ++c)
		{
			int clasterID = c - begin(vecCentroid);
			double dist = AlgorithmKmeans::Point::distPoints(*c, *p);
			if (dist < minDist)
			{
				minDist = dist;
				p->cluster = clasterID;
			}
		}
	}
}

void AlgorithmKmeans::Kmeans::updateCentroids()
{
	std::vector<double> sumX(k, 0.);
	std::vector<double> sumY(k, 0.);
	std::vector<int> numPointCluster(k, 0);
	for (auto p = begin(vecPoint); p != end(vecPoint); ++p)
	{
		int clusterID = p->cluster;
		sumX[clusterID] += p->x;
		sumY[clusterID] += p->y;
		numPointCluster[clusterID]++;
	}
	for (auto c = begin(vecCentroid); c != end(vecCentroid); ++c)
	{
		int clusterID = c - begin(vecCentroid);
		c->x = sumX[clusterID] / numPointCluster[clusterID];
		c->y = sumY[clusterID] / numPointCluster[clusterID];
	}
}

double AlgorithmKmeans::Kmeans::targetFunc() const
{
	double newTargetFunc = 0.;
	for (auto p = begin(vecPoint); p != end(vecPoint); ++p)
	{
		double dist = AlgorithmKmeans::Point::distPoints(*p, vecCentroid[p->cluster]);
		newTargetFunc += (dist * dist);
	}
	return newTargetFunc;
}

Matrix AlgorithmKmeans::Kmeans::createMatrix(std::vector<std::vector<int>>& nearestElemCluster, AlgorithmKmeans::Point& base) const
{
	Matrix matrix(k + 1);


	for (int i = 0; i < vecCentroid.size(); i++)
	{
		for (int j = 0; j < vecCentroid.size(); j++)
		{
			double dist = DBL_MAX;
			int PointID = -1;
			for (auto p = begin(vecPoint); p != end(vecPoint); ++p)
			{
				int newPointID = p - begin(vecPoint);
				if (p->cluster == j && i != j) {
					double newDist = AlgorithmKmeans::Point::distPoints(vecCentroid[i], *p);
					if (newDist < dist)
					{
						dist = newDist;
						PointID = newPointID;
					}
				}
			}
			nearestElemCluster[i][j] = PointID;
			if (i == j) {
				matrix.item(i, j) = 0;
			}
			else
				matrix.item(i, j) = dist;
		}
	}
	for (int i = 0; i < vecCentroid.size(); i++)
	{
		double dist = DBL_MAX;
		int PointID = -1;
		for (auto p = begin(vecPoint); p != end(vecPoint); ++p)
		{
			int newPointID = p - begin(vecPoint);
			if (p->cluster == i) {
				double newDist = AlgorithmKmeans::Point::distPoints(base, *p);
				if (newDist < dist)
				{
					dist = newDist;
					PointID = newPointID;
				}
			}
		}
		nearestElemCluster[k][i] = PointID;
		matrix.item(k, i) = dist;

		double distbaseClaster = AlgorithmKmeans::Point::distPoints(base, vecCentroid[i]);
		matrix.item(i, k) = dist;
	}

	return matrix;
}

double AlgorithmKmeans::Kmeans::findPaht(std::pair<double, double> basePoint)
{
	Point base(basePoint.first, basePoint.second);
	//k+1 - база
	std::vector<std::vector<int>> nearestElemCluster(k + 1, std::vector<int>(k + 1, -1));

	Matrix matrix = createMatrix(nearestElemCluster, base);

	LittleSolver alg(matrix);
	alg.solve();
	std::list<int> solution = alg.getSolution();
	std::list<std::pair<int, int>> _solutionWay;

	while (0 != solution.size() - 1)
	{
		int point1 = solution.front();
		solution.pop_front();
		int point2 = solution.front();
		_solutionWay.emplace_back(point1, point2);
	}

	while (_solutionWay.front().first != k)
	{
		auto a = _solutionWay.front();
		_solutionWay.pop_front();
		_solutionWay.push_back(a);
	}

	double lengthPath = 0;

	for (auto claster : _solutionWay)
	{
		if (claster.first == k)
		{
			std::pair<double, double> prFerst; prFerst.first = base.x; prFerst.second = base.y;
			path.push_back(prFerst);
			int indexPoint = nearestElemCluster[claster.first][claster.second];
			std::pair<double, double> prSecond; prSecond.first = vecPoint[indexPoint].x; prSecond.second = vecPoint[indexPoint].y;
			lengthPath += AlgorithmKmeans::Point::distPoints(vecPoint[indexPoint], base);
			path.push_back(prSecond);
			std::pair<double, double> prCluster; prCluster.first = vecCentroid[vecPoint[indexPoint].cluster].x; prCluster.second = vecCentroid[vecPoint[indexPoint].cluster].y;
			path.push_back(prCluster);
			lengthPath += AlgorithmKmeans::Point::distPoints(vecCentroid[vecPoint[indexPoint].cluster], vecPoint[indexPoint]);

			for (int i = 0; i < vecPoint.size(); ++i)
			{
				if (i != indexPoint && vecPoint[i].cluster == claster.second)
				{
					std::pair<double, double> prPoint;
					prPoint.first = vecPoint[i].x;
					prPoint.second = vecPoint[i].y;
					path.push_back(prPoint);
					lengthPath += AlgorithmKmeans::Point::distPoints(vecPoint[i], vecCentroid[vecPoint[i].cluster]);
					std::pair<double, double> prCl;
					prCl.first = vecCentroid[vecPoint[i].cluster].x;
					prCl.second = vecCentroid[vecPoint[i].cluster].y;
					path.push_back(prCl);
					lengthPath += AlgorithmKmeans::Point::distPoints(vecPoint[i], vecCentroid[vecPoint[i].cluster]);
				}
			}
		}

		if (claster.second == k)
		{
			lengthPath += AlgorithmKmeans::Point::distPoints(vecCentroid[claster.first], base);
		}

		if (claster.second != k && claster.first != k)
		{
			int indexPoint = nearestElemCluster[claster.first][claster.second];
			std::pair<double, double> prPoint; prPoint.first = vecPoint[indexPoint].x; prPoint.second = vecPoint[indexPoint].y;
			path.push_back(prPoint);
			lengthPath += AlgorithmKmeans::Point::distPoints(vecPoint[indexPoint], vecCentroid[claster.first]);
			std::pair<double, double> prCluster; prCluster.first = vecCentroid[claster.second].x; prCluster.second = vecCentroid[claster.second].y;
			path.push_back(prCluster);
			lengthPath += AlgorithmKmeans::Point::distPoints(vecPoint[indexPoint], vecCentroid[claster.second]);

			for (int i = 0; i < vecPoint.size(); ++i)
			{
				if (i != indexPoint && vecPoint[i].cluster == claster.second)
				{
					std::pair<double, double> prPoint;
					prPoint.first = vecPoint[i].x;
					prPoint.second = vecPoint[i].y;
					path.push_back(prPoint);
					lengthPath += AlgorithmKmeans::Point::distPoints(vecPoint[i], vecCentroid[vecPoint[i].cluster]);
					std::pair<double, double> prCl;
					prCl.first = vecCentroid[vecPoint[i].cluster].x;
					prCl.second = vecCentroid[vecPoint[i].cluster].y;
					path.push_back(prCl);
					lengthPath += AlgorithmKmeans::Point::distPoints(vecPoint[i], vecCentroid[vecPoint[i].cluster]);
				}
			}
		}
	}

	std::pair<double, double> prLastPoint = path.front();
	path.push_back(prLastPoint);

	return lengthPath;
}

double AlgorithmKmeans::Kmeans::algoritm(int numberCluster)
{
	this->k = numberCluster;
	generCentroids();
	double newValueTargetFunc;
	while (true)
	{
		clustering();
		updateCentroids();
		newValueTargetFunc = targetFunc();
		
		if (newValueTargetFunc == valueTargetFunc)
			return valueTargetFunc;

		valueTargetFunc = newValueTargetFunc;
	}
}

void AlgorithmKmeans::Kmeans::writeFile(const std::string path) const
{
	std::ofstream file(path);
	file << "x,y,c" << std::endl;

	for (auto it = begin(vecPoint); it != end(vecPoint); ++it)
	{
		file << it->x << "," << it->y << "," << it->cluster << std::endl;
	}

	file.close();
}

void AlgorithmKmeans::Kmeans::writeFilePath(const std::string patth) const
{
	std::ofstream file(patth);
	file << "x,y" << std::endl;

	for (auto it = begin(path); it != end(path); ++it)
	{
		file << it->first << "," << it->second << std::endl;
	}

	file.close();
}

AlgorithmKmeans::Kmeans::Kmeans(const ModelField::ModelPoints& modelPoints)
{
	std::vector<std::pair<double, double>> vecModelPoint = modelPoints.getVecPoint();
	n = vecModelPoint.size();
	for (auto it :vecModelPoint)
	{
		AlgorithmKmeans::Point a(it.first, it.second);
		vecPoint.push_back(a);
	}
}

AlgorithmKmeans::Kmeans::~Kmeans() {};

AlgorithmKmeans::Point::Point(){}

AlgorithmKmeans::Point::Point(double x, double y)
{
	this->x = x;
	this->y = y;
}

double AlgorithmKmeans::Point::distPoints(const Point& a, const Point& b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

AlgorithmKmeans::Point::~Point() {};
