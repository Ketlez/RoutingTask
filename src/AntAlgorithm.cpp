#include "AntAlgorithm.h"
#include <random>
#include <ctime>
#include <iostream>
#include <iterator>
#include "methodBranchesBoundaries.h"

static std::mt19937 gen((int)time(0));
static std::uniform_real_distribution<> urd1(0., 1.);

AntAlgorithm::Point::Point() {
	x = 0.;
	y = 0.;
	cluster = -1;
}
AntAlgorithm::Point::Point(double x, double y)
{
	this->x = x;
	this->y = y;
}

double AntAlgorithm::Point::distPoints(const Point& a, const Point& b)
{
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

AntAlgorithm::Point::~Point() {};

double AntAlgorithm::AntClustering::poW(double num, int p)
{
	double pow = 1;
	while (p)
	{
		pow *= num;
		p--;
	}
	return pow;
}

void AntAlgorithm::AntClustering::initVecFirstPoint(std::list<int>& clusterPoint, std::list<int>& NoclusterPoint)
{
	for (int i = 0; i < n; i++)
		NoclusterPoint.push_back(i);
	for (int i = 0; i < k; i++)
	{
		int index;
		bool flag = false;
		do
		{
			flag = false;
			index = rand() % n;
			
			for (auto it : clusterPoint)
			{
				if (index == it)
					flag = true;
			}
		} while (flag);
		
		clusterPoint.pop_front();
		clusterPoint.push_back(index);
		NoclusterPoint.remove(index);
		vecPoint[index].cluster = i;
	}
	
}

int AntAlgorithm::AntClustering::probability(int pointIndex, std::list<int>& clusterPoint, double alph, double bett)
{
	double zP = 0;
	
	for (auto p :clusterPoint)
	{
			double visibility = 1. / distMatrix[pointIndex][p].dist;
			zP += poW(visibility, bett) * poW(distMatrix[pointIndex][p].pher, alph);
			
	}
	
	//std::cout << zP << std::endl;
	double random = urd1(gen);
	double sum = 0;
	for (auto p : clusterPoint)
	{
			sum += (poW(1. / distMatrix[pointIndex][p].dist, bett) * poW(distMatrix[pointIndex][p].pher, alph) / zP);
			if (random <= sum)
			{
				return p;
			}
	}
	return -1;
}

double AntAlgorithm::AntClustering::targetFunc()
{
	std::vector<double> sumX(k, 0.);
	std::vector<double> sumY(k, 0.);
	std::vector<int> numPointCluster(k);
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

	double newTargetFunc = 0.;
	for (auto p = begin(vecPoint); p != end(vecPoint); ++p)
	{
		double dist = AntAlgorithm::Point::distPoints(*p, vecCentroid[p->cluster]);
		newTargetFunc += (dist * dist);

	}

	return newTargetFunc;
}

double AntAlgorithm::AntClustering::algoritm(int k, int lifeTime, int numberAnt, double alph, double bett, double Q, double p)
{	
	this->k = k;
	vecCentroid.resize(k, Point());
	double newValueTargetFunc = -1.;
	for (int i = 0; i < lifeTime; i++)
	{
		//double sumFunc = DBL_MAX;
		for (int j = 0; j < numberAnt; j++) {

			std::list<int> clusterPoint(k, -1);
			std::list<int> NOclusterPoint;
			initVecFirstPoint(clusterPoint, NOclusterPoint);
			
			std::vector<std::pair<int, int>> wayAnt;
			
			while (clusterPoint.size() != n)
			{
				int index = rand() % NOclusterPoint.size();
				int indexPointNOcluster;
				int r = 0;
				for (auto it : NOclusterPoint)
				{
					if (r == index)
						indexPointNOcluster = it;
					r++;
				}
					
				int indexPointCluster = probability(indexPointNOcluster, clusterPoint, alph, bett);
				wayAnt.push_back(std::pair<int, int>(indexPointCluster, indexPointNOcluster));
				
				
				vecPoint[indexPointNOcluster].cluster = vecPoint[indexPointCluster].cluster;
				clusterPoint.push_back(indexPointNOcluster);
				NOclusterPoint.remove(indexPointNOcluster);
			}
			
			double newValueTargetFunc = targetFunc();
			
			for (auto it:wayAnt)
			{
				
					distMatrix[it.first][it.second].delta_pher += Q / newValueTargetFunc;
					distMatrix[it.second][it.first].delta_pher += Q / newValueTargetFunc;
			}

			//if (sumFunc > newValueTargetFunc)
			//{
			//	sumFunc = newValueTargetFunc;
			//}
			
			if (valueTargetFunc > newValueTargetFunc)
			{
				bestVecPoint = vecPoint;
				bestVecCentroid = vecCentroid;
				valueTargetFunc = newValueTargetFunc;
			}
			
		}
		//std::cout << sumFunc << std::endl;
		
		changepher(p);
	}
	return valueTargetFunc;
}

Matrix AntAlgorithm::AntClustering::createMatrix(std::vector<std::vector<int>>& nearestElemCluster, AntAlgorithm::Point& base)
{
	Matrix matrix(k+1);
	
	
	for (int i = 0; i < bestVecCentroid.size(); i++)
	{
		for (int j = 0; j < bestVecCentroid.size(); j++)
		{
			double dist = DBL_MAX;
			int PointID = -1;
			for (auto p = begin(bestVecPoint); p != end(bestVecPoint); ++p)
			{
				int newPointID = p - begin(bestVecPoint);
				if (p->cluster == j && i!=j) {
					double newDist = AntAlgorithm::Point::distPoints(bestVecCentroid[i], *p);
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
	for (int i = 0; i < bestVecCentroid.size(); i++)
	{
		double dist = DBL_MAX;
		int PointID = -1;
		for (auto p = begin(bestVecPoint); p != end(bestVecPoint); ++p)
		{
			int newPointID = p - begin(bestVecPoint);
			if (p->cluster == i) {
				double newDist = AntAlgorithm::Point::distPoints(base, *p);
				if (newDist < dist)
				{
					dist = newDist;
					PointID = newPointID;
				}
			}
		}
		nearestElemCluster[k][i] = PointID;
		matrix.item(k, i) = dist;

		double distbaseClaster = AntAlgorithm::Point::distPoints(base, bestVecCentroid[i]);
		matrix.item(i, k) = dist;
	}

	return matrix;
}

void AntAlgorithm::AntClustering::writeFile(const std::string path) const
{
	std::ofstream file(path);
	file << "x,y,c" << std::endl;

	for (auto it = begin(bestVecPoint); it != end(bestVecPoint); ++it)
	{
		file << it->x << "," << it->y << "," << it->cluster << std::endl;
	}

	file.close();
}

void AntAlgorithm::AntClustering::writeFilePath(const std::string patth) const
{
	std::ofstream file(patth);
	file << "x,y" << std::endl;

	for (auto it = begin(path); it != end(path); ++it)
	{
		file << it->first << "," << it->second << std::endl;
	}

	file.close();
}

AntAlgorithm::AntClustering::AntClustering(const ModelField::ModelPoints& modelPoints)
{
	std::vector<std::pair<double, double>> vecModelPoint = modelPoints.getVecPoint();
	n = vecModelPoint.size();
	
	
	for (int i = 0; i < n; i++) {
		AntAlgorithm::Point a(vecModelPoint[i].first, vecModelPoint[i].second);
		vecPoint.push_back(a);
	}

	distMatrix.resize(n, std::vector<Matrices>(n));

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
		{
			distMatrix[i][j].dist = AntAlgorithm::Point::distPoints(vecPoint[i], vecPoint[j]);
		}
	}
}

AntAlgorithm::AntClustering::~AntClustering()
{
}

void AntAlgorithm::AntClustering::changepher(double p)
{
	for (int i = 0; i<n; ++i)
		for (int j = 0; j<n; ++j)
		{
			distMatrix[i][j].pher = (1 - p) * distMatrix[i][j].pher + distMatrix[i][j].delta_pher;
			distMatrix[i][j].delta_pher = 0;
		}
}

double AntAlgorithm::AntClustering::findPaht(std::pair<double, double> basePoint)
{
	Point base(basePoint.first, basePoint.second);
	//k+1 - база
	std::vector<std::vector<int>> nearestElemCluster(k + 1, std::vector<int>(k + 1, -1));
	
	Matrix m = createMatrix(nearestElemCluster, base);

	LittleSolver alg(m);
	alg.solve();
	std::list<int> _solution = alg.getSolution();
	std::list<std::pair<int, int>> _solutionWay;

	while (0!=_solution.size()-1)
	{
		int point1 = _solution.front();
		_solution.pop_front();
		int point2 = _solution.front();
		_solutionWay.emplace_back(point1, point2);
		
	}

	while (_solutionWay.front().first != k)
	{
		auto a = _solutionWay.front();
		_solutionWay.pop_front();
		_solutionWay.push_back(a);
	}

	double lengthPath = 0;

	for (auto claster: _solutionWay)
	{
		if (claster.first == k)
		{
			std::pair<double, double> prFerst; prFerst.first = base.x; prFerst.second = base.y;
			path.push_back(prFerst);
			int indexPoint = nearestElemCluster[claster.first][claster.second];
			std::pair<double, double> prSecond; prSecond.first = bestVecPoint[indexPoint].x; prSecond.second = bestVecPoint[indexPoint].y;
			lengthPath += AntAlgorithm::Point::distPoints(bestVecPoint[indexPoint], base);
			path.push_back(prSecond);
			std::pair<double, double> prCluster; prCluster.first = bestVecCentroid[bestVecPoint[indexPoint].cluster].x; prCluster.second = bestVecCentroid[bestVecPoint[indexPoint].cluster].y;
			path.push_back(prCluster);
			lengthPath += AntAlgorithm::Point::distPoints(bestVecCentroid[bestVecPoint[indexPoint].cluster], bestVecPoint[indexPoint]);

			for (int i = 0; i < bestVecPoint.size(); ++i)
			{
				if (i != indexPoint && bestVecPoint[i].cluster == claster.second)
				{
					std::pair<double, double> prPoint;
					prPoint.first = bestVecPoint[i].x;
					prPoint.second = bestVecPoint[i].y;
					path.push_back(prPoint);
					lengthPath += AntAlgorithm::Point::distPoints(bestVecPoint[i], bestVecCentroid[bestVecPoint[i].cluster]);
					std::pair<double, double> prCl;
					prCl.first = bestVecCentroid[bestVecPoint[i].cluster].x;
					prCl.second = bestVecCentroid[bestVecPoint[i].cluster].y;
					path.push_back(prCl);
					lengthPath += AntAlgorithm::Point::distPoints(bestVecPoint[i], bestVecCentroid[bestVecPoint[i].cluster]);
				}
			}
		}

		if (claster.second == k)
		{
			lengthPath += AntAlgorithm::Point::distPoints(bestVecCentroid[claster.first], base);
		}

		if (claster.second != k && claster.first != k)
		{
			int indexPoint = nearestElemCluster[claster.first][claster.second];
			std::pair<double, double> prPoint; prPoint.first = bestVecPoint[indexPoint].x; prPoint.second = bestVecPoint[indexPoint].y;
			path.push_back(prPoint);
			lengthPath += AntAlgorithm::Point::distPoints(bestVecPoint[indexPoint], bestVecCentroid[claster.first]);
			std::pair<double, double> prCluster; prCluster.first = bestVecCentroid[claster.second].x; prCluster.second = bestVecCentroid[claster.second].y;
			path.push_back(prCluster);
			lengthPath += AntAlgorithm::Point::distPoints(bestVecPoint[indexPoint], bestVecCentroid[claster.second]);

			for (int i = 0; i < bestVecPoint.size(); ++i)
			{
				if (i != indexPoint && bestVecPoint[i].cluster == claster.second)
				{
					std::pair<double, double> prPoint;
					prPoint.first = bestVecPoint[i].x;
					prPoint.second = bestVecPoint[i].y;
					path.push_back(prPoint);
					lengthPath += AntAlgorithm::Point::distPoints(bestVecPoint[i], bestVecCentroid[bestVecPoint[i].cluster]);
					std::pair<double, double> prCl;
					prCl.first = bestVecCentroid[bestVecPoint[i].cluster].x;
					prCl.second = bestVecCentroid[bestVecPoint[i].cluster].y;
					path.push_back(prCl);
					lengthPath += AntAlgorithm::Point::distPoints(bestVecPoint[i], bestVecCentroid[bestVecPoint[i].cluster]);
				}
			}
		}
	}

	std::pair<double, double> prLastPoint = path.front();
	path.push_back(prLastPoint);

	return lengthPath;
}
