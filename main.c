#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <time.h>
#include <locale.h>

#define Ilosc_wspol 3

//tworzenie funkcji 
void tablica(double wspol[Ilosc_wspol])
{
	printf("podaj par A: ");
	scanf_s("%lf", &wspol[0]);
	printf("podaj par B: ");
	scanf_s("%lf", &wspol[1]);
	do
	{
			printf("Ten współczynnik nie może być zerem!\n");
			Sleep(1000);
			printf("podaj par B jeszcze raz: ");
			scanf_s("%lf", &wspol[1]);
	} while (wspol[1] == 0);
	printf("podaj par C: ");
	scanf_s("%lf", &wspol[2]);
}

//ustalenie dziedziny funkcji
void dziedzina(double* dMin, double* dMax)
{
	double* dF;

	printf("podaj dolną wart. dziedziny: ");
	scanf_s("%lf", dMin);
	printf("podaj górną wart. dziedziny: ");
	scanf_s("%lf", dMax);
	if (dMin > dMax)
	{
		dF = dMin;
		dMin = dMax;
		dMax = dF;
	}
}

//ustalenie ilości wyników funkcji
void ilosc_wynikow(int* amount)
{
	printf("podaj ilość wyników, które ma zwrocić funkcja: ");
	scanf_s("%d", amount);
	do
	{
		printf("ilość wyników musi być większa od 0\n");
		Sleep(1000);
		printf("podaj ilość wyników jeszcze raz: ");
		scanf_s("%d", &amount);
	} while (amount <= 0);
	printf("\n");
}

//wygenerowanie sygnału funkcji
void gen(double sygnal[], double wspol[], double dMin, double dMax, int amount)
{
	int i;
	double x;
	double delta = (dMax - dMin) / (double)amount;
	for (i = 0, x = dMin; x <= dMax && i < amount; x += delta, i++)
	{
		sygnal[i] = wspol[0] * sin(x / (wspol[1])) * cos(x * (wspol[2]));
	}
}

//wygenerowanie szumu sygnału
void szum_gen(double szum[], double sygnal[], int amount)
{
	srand((unsigned int)time(NULL));

	int i, r;

	for (i = 0; i < amount; i++)
	{
		r = rand() % 100 + 1;
		if (r <= 20)
		{
			szum[i] = sygnal[i] + (rand() % 1000 / 1000.0);
		}
		else
		{
			szum[i] = sygnal[i];
		}
	}
}

//filtrowanie zaszumionej funkcji
void filtrator(double szum[], double fszum[], int amount)
{
	int i;

	fszum[0] = (2 * szum[0] + 2 * szum[1] + szum[2]) / 5;
	fszum[1] = (2 * szum[0] + szum[1] + szum[2] + szum[3]) / 5;

	for (i = 2; i < amount - 2; i++)
	{
		fszum[i] = (szum[i - 2] + szum[i - 1] + szum[i] + szum[i + 1] + szum[i + 2]) / 5;
	}

	fszum[amount - 2] = (2 * szum[amount] + szum[amount - 1] + szum[amount - 2] + szum[amount - 3]) / 5;
	fszum[amount - 1] = (2 * szum[amount] + 2 * szum[amount - 1] + szum[amount - 2]) / 5;
}

//stworzenie pliku i zapisanie sygnału, sygnału zaszumionego i odfiltorwanego sygnału zaszumionego do pliku 
void fcreator(double sygnal[], double szum[], double fszum[], int amount)
{
	int i;

	FILE* values;
	fopen_s(&values, "values.csv", "w");
	for (i = 0; i < amount; i++)
	{
		fprintf_s(values, "%lf,%lf,%lf\n", sygnal[i], szum[i], fszum[i]);
	}
	fclose(values);
}

//odczyt sygnału, sygnału zaszumionego i odfiltrowanego sygnału zaszumionego z pliku
void freader(int amount, double sygnal[], double szum[], double fszum[])
{
	int i;

	printf("Wczytuję zgromadzone dane:\n\n");
	printf("sygnał, szum, po filtrowaniu\n\n");
	Sleep(1000);
	FILE* values;
	fopen_s(&values, "values.csv", "r");

	for (i = 0; i < amount; i++)
	{
		fscanf_s(values, "%lf,%lf,%lf", &sygnal[i], &szum[i], &fszum[i]);
		printf("%.3lf, %.3lf, %.3lf\n", sygnal[i], szum[i], fszum[i]);
		Sleep(5);
	}

	fclose(values);
}

void main()
{
	int amount;
	double wspol[Ilosc_wspol];
	double dMin, dMax;
	double* sygnal = NULL;
	double* szum = NULL;
	double* fszum = NULL;

	setlocale(LC_ALL, "Polish");

	printf("MENU\n");
	printf("1. Generuj funkcję\n");
	printf("2. Generuj szum funkcji\n");
	printf("3. Filtruj funkcję\n");
	printf("4. Zapisz funkcję\n");
	printf("5. Wczytaj funkcję\n");
	printf("6. Wyjście\n\n");

	while (1)
	{
		int x;
		scanf_s("%d", &x);
		switch (x)
		{
		case 1:
			printf("Funkcja: y = A * sin(x / B) * cos(x * C)\n\n");
			dziedzina(&dMin, &dMax);
			tablica(wspol);
			ilosc_wynikow(&amount);
			sygnal = malloc(amount * sizeof(*sygnal));
			gen(sygnal, wspol, dMin, dMax, amount);
			printf("Funkcję wygenerowano.\n\n");
			break;
		case 2:
			if (sygnal == NULL)
			{
				printf("Najpierw wygeneruj funkcję!\n\n");
			}
			else
			{
				szum = malloc(amount * sizeof(*szum));
				szum_gen(szum, sygnal, amount);
				printf("Szum funkcji wygenerowano.\n\n");
			}
			break;
		case 3:
			if (szum == NULL)
			{
				printf("Najpierw wygeneruj szum!\n\n");
			}
			else if (sygnal == NULL)
			{
				printf("Najpierw wygeneruj funkcję!\n\n");
			}
			else
			{
				fszum = malloc(amount * sizeof(*fszum));
				filtrator(szum, fszum, amount);
				printf("Odfiltrowanie nastąpiło poprawnie.\n\n");
			}
			break;
		case 4:
			if (sygnal == NULL)
			{
			printf("Najpierw wygeneruj funkcję!\n\n");
			}
			else if (szum == NULL)
			{
				printf("Najpierw wygeneruj szum!\n\n");
			}
			else if (fszum == NULL)
			{
				printf("Najpierw odfiltruj funkcję!\n\n");
			}
			else
			{
				fcreator(sygnal, szum, fszum, amount);
				printf("Funkcję zapisano.\n\n");
			}
			break;
		case 5:
			if (sygnal == NULL)
			{
			printf("Najpierw wygeneruj funkcję!\n\n");
			}
			else if (szum == NULL)
			{
				printf("Najpierw wygeneruj szum!\n\n");
			}
			else if (fszum == NULL)
			{
				printf("Najpierw odfiltruj funkcję!\n\n");
			}
			else
			{
				freader(amount, sygnal, szum, fszum);
				printf("\nWciśnij enter, aby zamknąć wyniki\n");
				getchar();
				if (getchar() == ("\n"))
				{
					break;
				}
			}
			break;
		case 6:
			exit(EXIT_SUCCESS);
			break;
		default:
			printf("Nie ma takiej opcji :/");
		}
		Sleep(1000);
		system("CLS");
		printf("MENU\n");
		printf("1. Generuj funkcję\n");
		printf("2. Generuj szum funkcji\n");
		printf("3. Filtruj funkcję\n");
		printf("4. Zapisz funkcję\n");
		printf("5. Wczytaj funkcję\n");
		printf("6. Wyjście\n\n");
	}
}