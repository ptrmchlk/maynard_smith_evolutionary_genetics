// gentic drift, by ptrmchlk

#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <algorithm>

constexpr int POPSIZE{100};
constexpr int NUMGEN{1000};
constexpr int NUMRUN{10};

int print_population(std::ofstream &resultFile, int runInd, const std::vector<std::vector<int>> &pop)
{
	if (!resultFile.is_open())
	{
		std::cerr << "File opening failed. For some reason." << std::endl;
		return 1;
	}
	for (int j{0}; j < pop.size(); j++)
	{
		resultFile << runInd << "," << j;
		for (int i{0}; i < pop[j].size(); i++)
		{
			resultFile << "," << pop[j][i];
		}
		resultFile << "\n";
	}
	return 0;
}

int print_header(std::ofstream &resultFile)
{
	if (!resultFile.is_open())
	{
		std::cerr << "File opening failed. For some reason." << std::endl;
		return 1;
	}
	resultFile << "nrun,ngen";
	for (int i{0}; i < POPSIZE; i++)
	{
		resultFile << ",ind" << i;
	}
	resultFile << "\n";
	return 0;
}

bool check_fixed(std::vector<int> &vv, int val)
{
	return std::all_of(
		vv.begin(), vv.end(),
		[val](int x)
		{ return x == val; });
}

int main()
{
	// alternative seeding as suggested by https://www.learncpp.com
	std::random_device rd{};
	std::seed_seq ss{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
	std::mt19937 mt{ss};
	std::uniform_int_distribution pull_parent{0, POPSIZE - 1};
	std::vector<int> initPopulation(POPSIZE);

	for (int i{0}; i < POPSIZE; i++)
	{
		initPopulation[i] = i + 1;
	}
	std::ofstream resultFile;
	resultFile.open("drift_results.res");
	print_header(resultFile);

	for (int k{0}; k < NUMRUN; k++)
	{
		bool typeFixed{false};

		std::vector<std::vector<int>> currentPopulation(NUMGEN + 1, std::vector<int>(POPSIZE));
		currentPopulation[0] = initPopulation;

		for (int j{0}; j < NUMGEN && !typeFixed; j++)
		{
			for (int i{0}; i < POPSIZE; i++)
			{
				currentPopulation[j + 1][i] = currentPopulation[j][pull_parent(mt)];
			}

			if ((j + 1) % 50 == 0)
			{
				int firstValue{currentPopulation[j + 1][0]};
				typeFixed = check_fixed(currentPopulation[j + 1], firstValue);
				if (typeFixed)
					// modify this, it's tricky to figure out
					currentPopulation.erase(currentPopulation.begin() + j + 2, currentPopulation.end());
			}
		}

		int currentIndex{static_cast<int>(currentPopulation.size() - 1)};
		int firstValue{currentPopulation[currentIndex][0]};

		while (typeFixed)
		{
			typeFixed = check_fixed(currentPopulation[currentIndex], firstValue);
			if (!typeFixed)
				currentPopulation.erase(currentPopulation.begin() + currentIndex + 2, currentPopulation.end());
			currentIndex--;
		}

		print_population(resultFile, k + 1, currentPopulation);
	}
	resultFile.close();

	return 0;
}
