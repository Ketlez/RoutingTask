#include <iostream>
#include "AlgorithmKmeans.h"
#include "AntAlgorithm.h"
#include <ctime>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <limits.h>
#include "methodBranchesBoundaries.h"

int main()
{
	srand(time(NULL));

	ShowWindow(GetConsoleWindow(), 0);


	bool flagCreat = true;
	int k, lifeTime, numberAnt, numberPoint, length = 100, width = 100;
	double alph, bett, Q, p;
	std::string pathModel = "";
	std::string pathFileEnter = "inFile/fileEnter.txt";
	std::string pathFileOut = "outFile/fileOut.txt";
	std::ifstream fileEnter(pathFileEnter);
	std::ofstream fileOut(pathFileOut);
	if (!fileEnter)
	{
		fileOut << "error" << std::endl;
		fileOut << "The input file could not be found!" << std::endl;
		return false;
	}
	try {
		std::string line;
		getline(fileEnter, line);
		k = stoi(line);
		getline(fileEnter, line);
		std::stringstream lineStream(line);
		std::string bit;
		getline(lineStream, bit, ',');
		lifeTime = stoi(bit);
		getline(lineStream, bit, ',');
		numberAnt = stoi(bit);
		getline(lineStream, bit, ',');
		alph = stof(bit);
		getline(lineStream, bit, ',');
		bett = stof(bit);
		getline(lineStream, bit, ',');
		Q = stof(bit);
		getline(lineStream, bit, '\n');
		p = stof(bit);

		getline(fileEnter, line);
		if (line == "create")
		{
			flagCreat = true;
			getline(fileEnter, line);
			std::stringstream lineStreamModel(line);
			getline(lineStreamModel, bit, ',');
			numberPoint = stoi(bit);
			
			getline(lineStreamModel, bit, ',');
			length = stoi(bit);
			
			getline(lineStreamModel, bit, '\n');
			width = stoi(bit);
		
		}
		else
		{
			flagCreat = false;
			pathModel = line;
			std::ifstream fileModel(pathModel);
			if (!fileModel)
			{
				fileOut << "error" << std::endl;
				fileOut << "File model object is not found!" << std::endl;
				return false;
			}
		}
	}
	catch(...)
	{
		fileOut << "error" << std::endl;
		fileOut << "Uncorrected data of the input file!" << std::endl;
		return false;
	}

	try {
		
		ModelField::ModelPoints modelPoints;
		if (flagCreat)
		{
			modelPoints.generatPoin(numberPoint, length, width);
			modelPoints.writeFile("outFile/outFile.csv");
		}
		else
		{
			modelPoints.creatFile(pathModel);
			modelPoints.writeFile("outFile/outFile.csv");
		}

		unsigned int start_time_algAnt = clock();
		AntAlgorithm::AntClustering algAnt(modelPoints);
		double antCluster = algAnt.algoritm(k, lifeTime, numberAnt, alph, bett, Q, p);
		unsigned int end_time_algAnt = clock();
		unsigned int search_time_algAnt = end_time_algAnt - start_time_algAnt;

		unsigned int start_time_algKmeans = clock();
		AlgorithmKmeans::Kmeans algKmeans(modelPoints);
		double kmeansCluster = algKmeans.algoritm(k);
		unsigned int end_time_algKmeans = clock();
		unsigned int search_time_Kmeans = end_time_algKmeans - start_time_algKmeans;


		double AntClPath = algAnt.findPaht(std::pair<double, double>(width / 2, length / 2));
		algAnt.writeFilePath("outFile/outFilePathAnt.csv");
		double kmeansClPath = algKmeans.findPaht(std::pair<double, double>(width / 2, length / 2));
		algKmeans.writeFilePath("outFile/outFilePathKmeans.csv");


		algAnt.writeFile("outFile/outFileNewAnt.csv");
		algKmeans.writeFile("outFile/outFileNewKmeans.csv");


		fileOut << "ok" << std::endl;
		fileOut << antCluster << std::endl;
		fileOut << kmeansCluster << std::endl;
		fileOut << AntClPath << std::endl;
		fileOut << kmeansClPath << std::endl;
		fileOut << search_time_algAnt << std::endl;
		fileOut << search_time_Kmeans << std::endl;
	}
	catch (...)
	{
		fileOut << "error" << std::endl;
		fileOut << "An error in the algorithm!" << std::endl;
		fileOut << "It may be related to:" << std::endl;
		fileOut << "- non-correct object coordinate data; " << std::endl;
		fileOut << "- incorrectly set algorithm parameters." << std::endl;
		return false;
	}







	//system("pause");
	
	return 0;
}