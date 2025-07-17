// Stable age distribution per JMS book, individual/object approach simulation, by PM
// Each reproducing female is an idividual, independent object.
// 1) survival check at age. I.e. If you died before this age, you do not reproduce.
// 2) reproduce, with fixed fertility, if age over 0. i.e. make new objects equal to fertility with age 0.
// Note that populations can crash, even if fertility and survival is matched - they are stochastic and will die out in a long run.
// But the age distribution should remain constant, regardless of the population dynamics (Stearns,1992)

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <algorithm>
#include <unistd.h>

constexpr int POPSIZE{1000};
constexpr int NUMGEN{1000};
constexpr int NUMRUN{10};
constexpr int FERTILITY{1};
constexpr double PROBSURV{0.5};

struct Female
{
	int age{0};
	bool alive{true};
};

void check_survival(Female &FF, std::mt19937 &mt)
{
	// double survivalProb{exp(-FF.age/MEANLIFETIME)};
	// std::cout << survivalProb << std::endl;
	std::bernoulli_distribution survived{PROBSURV};
	FF.alive = survived(mt);
}

void select_population(std::vector<Female> &POP, std::mt19937 &mt)
{
	for (int i{0}; i < POP.size(); i++)
	{
		check_survival(POP[i], mt);
	}
}

void age_population(std::vector<Female> &POP)
{
	for (int i{0}; i < POP.size(); i++)
	{
		POP[i].age += 1;
	}
}

double calculate_age_fraction(std::vector<Female> &POP, int age)
{
	double count{0};

	for (int i{0}; i < POP.size(); i++)
	{
		if (POP[i].age == age)
			count += 1.0;
	}

	return count / POP.size();
}

std::vector<double> calculate_age_distribution(std::vector<Female> &POP)
{
	int maxGen{static_cast<int>(std::ceil((log(0.01) - log(NUMGEN)) / log(PROBSURV)))}; // choose 99%

	std::vector<double> ageDist(maxGen);
	double count{0};
	for (int i{0}; i < maxGen; i++)
	{
		ageDist[i] = calculate_age_fraction(POP, i);
	}

	return ageDist;
}

void print_age_distribution (std::vector<Female> &POP, std::ofstream& file){
	std::vector<double> ageDist{calculate_age_distribution(POP)};
	for (int i{0}; i < ageDist.size(); i++){
		file << ageDist[i];
		if (i<ageDist.size()-1) {
			file << ",";
		}
	}
	file << std::endl;
}

std::vector<Female> make_new_generation(std::vector<Female> &oldPOP, std::mt19937 &mt)
{
	std::poisson_distribution<int> offspring(1.0 - PROBSURV);
	int newOffspring{0};
	std::vector<int> aliveParents{};
	std::vector<Female> newGeneration{};
	for (int i{0}; i < oldPOP.size(); i++)
	{
		if (oldPOP[i].age > 0 && oldPOP[i].alive)
		{
			newOffspring += FERTILITY;
			aliveParents.push_back(i);
		}
	}
	std::vector<Female> babies(newOffspring);
	for (int i{0}; i < aliveParents.size(); i++)
	{
		newGeneration.push_back(oldPOP[aliveParents[i]]);
	}
	newGeneration.insert(newGeneration.end(), babies.begin(), babies.end());
	return newGeneration;
}

int main()
{
	std::random_device rd{};
	std::seed_seq ss{rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd()};
	std::mt19937 mt{ss};
	std::vector<Female> Pop(POPSIZE);
	std::vector<std::vector<Female>> popDynamics(NUMGEN);

	std::ofstream resultFile;
	resultFile.open("ch3_age_popsize.res");
	std::ofstream fileDynamics;
	fileDynamics.open("ch3_age_distribution.res");
	for (int i{0}; i < NUMGEN; i++)
	{
		age_population(Pop);
		select_population(Pop, mt);	
		Pop = make_new_generation(Pop, mt);
		resultFile << Pop.size() << std::endl;
		print_age_distribution(Pop, fileDynamics);
	}
	resultFile.close();
	fileDynamics.close();

	return 0;
}
