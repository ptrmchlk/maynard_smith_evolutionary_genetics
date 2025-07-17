// Dawkins' Weasel as per JMS, by PM

#include <cstdio>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <random>
#include <algorithm>

const std::string LETTERS{"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
const std::string PEAK{"METHINKSITISAWEASEL"};
constexpr int POPSIZE{100};
constexpr double MUTRATE{0.01};
constexpr int NUMGEN{1000};
constexpr int NUMRUN{10};

int calculate_fitness(const std::string &current, const std::string &target)
{
	int fitness{0};
	for (int i{0}; i < target.length(); i++)
	{
		if (current[i] == target[i])
			fitness++;
	}
	return fitness;
}

std::string make_sequence(int seqLength, std::mt19937 &mt, std::uniform_int_distribution<int> &pull_letter)
{
	std::string sequence(seqLength, '0');

	for (int i{0}; i < seqLength; i++)
	{
		sequence[i] = LETTERS[pull_letter(mt)];
	}
	return sequence;
}

std::pair<int, int> get_max_fitness(const std::vector<std::string> &population, std::mt19937 &mt)
{
	std::vector<int> fitnesses(population.size());
	int fitness{0};
	std::vector<int> indices{};

	for (int i{0}; i < population.size(); i++)
	{
		fitnesses[i] = calculate_fitness(population[i], PEAK);
	}

	for (int i{0}; i < fitnesses.size(); i++)
	{
		if (fitnesses[i] > fitness)
			fitness = fitnesses[i];
	}

	for (int i{0}; i < fitnesses.size(); i++)
	{
		if (fitnesses[i] == fitness)
			indices.push_back(i);
	}

	std::uniform_int_distribution pull_index{0, static_cast<int>(indices.size() - 1)};
	int bestSequence{indices[pull_index(mt)]};
	return {fitness, bestSequence};
}

std::string mutate_sequence(const std::string &oldSequence, std::mt19937 &mt, std::bernoulli_distribution &mutate, std::uniform_int_distribution<int> &pull_letter)
{
	std::string newSequence{oldSequence};
	char letter = '0';

	for (int i{0}; i < oldSequence.size(); i++)
	{
		if (mutate(mt) == true)
		{
			letter = LETTERS[pull_letter(mt)];
			while (LETTERS.size() > 1 && letter == newSequence[i])
			{
				letter = LETTERS[pull_letter(mt)];
			}
			newSequence[i] = letter;
		}
	}
	return newSequence;
}

int main()
{
	std::mt19937 mt{45};
	std::pair<int, int> currentBestFit{};
	std::string currentBestSeq(PEAK.size(), '0');
	int maxFitness{calculate_fitness(PEAK, PEAK)};

	std::bernoulli_distribution mutate{MUTRATE};
	std::uniform_int_distribution pull_letter{0, static_cast<int>(LETTERS.length() - 1)};

	std::string startSeq{make_sequence(PEAK.size(), mt, pull_letter)};

	std::ofstream resultFile;
	resultFile.open("weasel_results.res");

	if (!resultFile.is_open())
	{
		std::cerr << "File opening failed. For some reason." << std::endl;
		return 1;
	}

	resultFile << "nrun,ngen,fitness,sequence\n";

	std::vector<std::string> newPopulation(POPSIZE, std::string(PEAK.size(), '0'));

	for (int k{0}; k < NUMRUN; k++)
	{
		currentBestFit.first = 0;
		startSeq = make_sequence(PEAK.size(), mt, pull_letter);
		resultFile << k + 1 << "," << 0 << "," << calculate_fitness(startSeq, PEAK) << "," << startSeq << "\n";

		for (int i{0}; i < newPopulation.size(); i++)
		{
			newPopulation[i] = startSeq;
		}

		for (int j{0}; j < NUMGEN; j++)
		{
			if (currentBestFit.first < maxFitness)
			{
				for (int i{0}; i < newPopulation.size(); i++)
				{
					newPopulation[i] = mutate_sequence(newPopulation[i], mt, mutate, pull_letter);
				}

				currentBestFit = get_max_fitness(newPopulation, mt);
				currentBestSeq = newPopulation[currentBestFit.second];

				for (int i{0}; i < newPopulation.size(); i++)
				{
					newPopulation[i] = currentBestSeq;
				}
				resultFile << k + 1 << "," << j + 1 << "," << currentBestFit.first << "," << currentBestSeq << "\n";
			}
		}
	}

	resultFile.close();
	return 0;
}
