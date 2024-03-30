// ProcuraEngine.cpp : Defines the entry point for the console application.
//

#include <stdio.h>

#include "OitoDamas.h"
#include "Puzzle8.h"
#include "Particao.h"
#include "JogoDoGalo.h"

int main(int argc, char* argv[])
{
	COitoDamas oitoDamas;
	CPuzzle8 puzzle8;
	CParticao particao;
	CJogoDoGalo jogoDoGalo;

	TRand::srand(20);
	puzzle8.TesteManual("Puzzle8", 20);
	particao.TesteManual("Particao",20);
	oitoDamas.TesteManual("Oito Damas",20);
	jogoDoGalo.TesteManual("Jogo do Galo",20);
}
