// Chapter 4, Two segregating alleles, PM

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

constexpr int NUMGEN{10000};
// constexpr double HH{0.3};
// constexpr double KK{0.3};
constexpr double STARTPA{0.4};
constexpr double STARTPB{0.6};

enum Genotypes
{
	AABB,
	AAbb,
	AABb,
	AaBB,
	Aabb,
	AaBb,
	aaBB,
	aabb,
	aaBb
};

std::vector<double> calculate_genotype_frequencies(double pA, double pB)
{
	double pAA = pow(pA, 2);
	double paa = pow(1 - pA, 2);
	double pAa = 2 * pA * (1 - pA);
	double pBB = pow(pB, 2);
	double pbb = pow(1 - pB, 2);
	double pBb = 2 * pB * (1 - pB);

	std::vector<double> genotypeFrequencies(3 * 3);
	genotypeFrequencies[AABB] = pAA * pBB;
	genotypeFrequencies[AAbb] = pAA * pbb;
	genotypeFrequencies[AABb] = pAA * pBb;
	genotypeFrequencies[AaBB] = pAa * pBB;
	genotypeFrequencies[Aabb] = pAa * pbb;
	genotypeFrequencies[AaBb] = pAa * pBb;
	genotypeFrequencies[aaBB] = paa * pBB;
	genotypeFrequencies[aabb] = paa * pbb;
	genotypeFrequencies[aaBb] = paa * pBb;

	return genotypeFrequencies;
}

std::vector<double> calculate_allele_frequencies(std::vector<double> genotypes)
{
	double pA{genotypes[AABB] + genotypes[AAbb] + genotypes[AABb] + 0.5 * genotypes[AaBB] + 0.5 * genotypes[Aabb] + 0.5 * genotypes[AaBb]};
	double pB{genotypes[AABB] + genotypes[aaBB] + genotypes[AaBB] + 0.5 * genotypes[AABb] + 0.5 * genotypes[aaBb] + 0.5 * genotypes[AaBb]};

	return {pA, pB};
}

std::vector<double> calculate_current_fitness(double pA, double pB, double HH, double KK)
{

	double ss{HH * (pB - 0.5)};
	double tt{KK * (pA - 0.5)};
	double fitAA{1};
	double fitAa{1 - ss / 2};
	double fitaa{1 - ss};
	double fitBB{1};
	double fitBb{1 - tt / 2};
	double fitbb{1 - tt};
	std::vector<double> genotypeFitness(3 * 3);
	genotypeFitness[AABB] = fitAA * fitBB;
	genotypeFitness[AAbb] = fitAA * fitbb;
	genotypeFitness[AABb] = fitAA * fitBb;
	genotypeFitness[AaBB] = fitAa * fitBB;
	genotypeFitness[AaBb] = fitAa * fitBb;
	genotypeFitness[Aabb] = fitAa * fitbb;
	genotypeFitness[aaBB] = fitaa * fitBB;
	genotypeFitness[aabb] = fitaa * fitbb;
	genotypeFitness[aaBb] = fitaa * fitBb;

	return genotypeFitness;
}

void print_genotype_frequencies (const std::vector<double>& frequencies, std::ofstream& file){
				for (int i{0}; i < frequencies.size(); i++)
				{
					file <<"," << frequencies[i];
				}
}

void print_genotype_frequencies_to_terminal (const std::vector<double>& frequencies){
				for (int i{0}; i < frequencies.size(); i++)
				{
					std::cout <<"," << frequencies[i];
				}
}

int main()
{
	std::vector<double> genotypeFrequencies{};
	std::vector<double> frequenciesAB{}; // should equal starting
	std::vector<double> genotypeFitness{};
	
	std::vector<double> KK{ 0.3, 0.5, 0.7};
	std::vector<double> HH{ 0.3, 0.5, 0.7};
	double total{0};

	std::ofstream fileDynamics;
	fileDynamics.open("ch4_allele_frequency.res");
	fileDynamics << "k,h,A,B,AABB,AAbb,AABb,AaBB,Aabb,AaBb,aaBB,aabb,aaBb" << std::endl;

	for (int k{0}; k < KK.size(); k++)
	{
		for (int h{0}; h < HH.size(); h++)
		{
			genotypeFrequencies = calculate_genotype_frequencies(STARTPA, STARTPB); //starting population
			frequenciesAB = calculate_allele_frequencies(genotypeFrequencies); // get p(A), p(B)
			genotypeFitness = calculate_current_fitness(frequenciesAB[0], frequenciesAB[1], HH[h], KK[k]); // calculate fitness based on p(A), p(B)

			fileDynamics << KK[k] << "," << HH[h] << "," << frequenciesAB[0] << "," << frequenciesAB[1];
			print_genotype_frequencies(genotypeFrequencies, fileDynamics);
			fileDynamics << std::endl;

			for (int j{1}; j < NUMGEN; j++)
			{
				total = 0;
				for (int i{0}; i < genotypeFrequencies.size(); i++)
				{
					genotypeFrequencies[i] *= genotypeFitness[i]; //apply selection
					total += genotypeFrequencies[i];
				}
				for (int i{0}; i < genotypeFrequencies.size(); i++)
				{
					genotypeFrequencies[i] /= total; //normalize after selection
				}
				frequenciesAB = calculate_allele_frequencies(genotypeFrequencies); //calculate get p(A), p(B) after selection
				genotypeFrequencies = calculate_genotype_frequencies(frequenciesAB[0], frequenciesAB[1]); //new generation based on H-W
				fileDynamics << KK[k] << "," << HH[h] << "," << frequenciesAB[0] << "," << frequenciesAB[1];
				print_genotype_frequencies(genotypeFrequencies, fileDynamics);
			    fileDynamics << std::endl;
				genotypeFitness = calculate_current_fitness(frequenciesAB[0], frequenciesAB[1], HH[h], KK[k]); //calculate new fitness
			}
		}
	}
	fileDynamics.close();
	return 0;
}
