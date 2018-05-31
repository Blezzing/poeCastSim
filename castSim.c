#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void concludeInf(){
	printf("Result:\nYou can cast forever!\n");
	exit(0);
}

void concludeOom(int casts, int elapsed){
	printf("Result:\nYou can cast %i times over %i ms!\n", casts, elapsed);
	exit(0);
}

void concludeMaxIter(){
	printf("Result:\nYou can probably cast forever, stopped simulating!\n");
	exit(0);
}

bool isOverCapped(float curr, float max){
	return curr > max;
}

bool isAbleToCast(float curr, float cost){
	return curr > cost;
}

void cast(int* casts, float* mana, float manaCost){
	*casts += 1;
	*mana -= manaCost;
}

void regen(float* mana, int* elapsedMillis, float manaRegen, int castInterval){
	*elapsedMillis += castInterval;
	*mana += (manaRegen / 1000) * castInterval;
}

void simulate(float maxMana, float manaRegen, float manaCost, float castsPerSec, int maxIterations){
	int castInterval  = (int)(1000.0 / castsPerSec);
	float mana        = maxMana;
	int elapsedMillis = 0;
	int casts         = 0;

	while(maxIterations-- > 0){
		if(isOverCapped(mana, maxMana)){
			concludeInf();
		}
		else if(!isAbleToCast(mana, manaCost)){
			concludeOom(casts, elapsedMillis);
		}
		else{
			cast(&casts, &mana, manaCost);
			regen(&mana, &elapsedMillis, manaRegen, castInterval);
		}
	}

	concludeMaxIter();
}

typedef struct arguments{
	float maxMana;
	float manaRegen;
	float manaCost;
	float castsPerSec;
	int maxIterations;
} arguments;

void usage(){
	printf("Usage:\n<Program> maxMana manaRegenPerSec manaCost castsPerSec\n");
}

void argsInit(arguments* args, int argc, char* argv[]){
	//check
	if(argc < 5){
		usage();
		exit(1);
	}

	args->maxMana = atof(argv[1]);
	args->manaRegen = atof(argv[2]);
	args->manaCost = atof(argv[3]);
	args->castsPerSec = atof(argv[4]);
	args->maxIterations = (argc > 5)?(atoi(argv[5])):(1000);
}

void argsPresent(arguments* args){
	printf("Parameters:\nMax mana:\t%8.2f\nMana regen:\t%8.2f\nMana cost:\t%8.2f\nCasts/s\t\t%8.2f\n\n",
	args->maxMana, args->manaRegen, args->manaCost, args->castsPerSec);
}

void main(int argc, char* argv[]){
	//prepare args with defaults
	arguments args;
	argsInit(&args, argc, argv);
	argsPresent(&args);
	
	simulate(args.maxMana, args.manaRegen, args.manaCost, args.castsPerSec, args.maxIterations);
}
