#include<iostream>
#include<string>
#include<fstream>
#include<locale.h>

#define MAX_N 20
#define MAX_ROADS 210
#define INPUT "map.txt"
using namespace std;


struct city
{
	char name=NULL;
	city *cities[MAX_N];
	int cost[MAX_N];
	int length[MAX_N];
	int countOut = 0;
};

city* way[MAX_ROADS];
city* minWay[MAX_ROADS];
int min = -1,minC,minL;
int wayCount = 0;
int minWayCount = 0;
int sumC = 0; 
int sumL = 0;

void search(city* A, city* B)
{
	int i, j;
	bool check;
	way[wayCount] = A;
	wayCount++;
	for (i = 0; i < A->countOut; i++)
	{
		check = true;
		for (j = 0; j < wayCount; j++) if (A->cities[i] == way[j]) check = false;
		if (A == B)
		{
			if ((min == (-1)) || ((sumC + sumL) < min))
			{
				for (int k = 0; k < MAX_N; k++)
				{
					minWay[k] = way[k];
					min = (sumC + sumL);
					minWayCount = wayCount;
					minC = sumC;
					minL = sumL;
				}
			}
			check = false;
		}
		if (check)
		{
			sumC = sumC + A->cost[i];
			sumL = sumL+A->length[i];
			if (((sumC+sumL)<min)||(min==(-1))) search(A->cities[i], B);
			sumC = sumC - A->cost[i];
			sumL = sumL - A->length[i];
		}
		if (wayCount>=0) way[wayCount] = NULL;
	}
	wayCount--;
}

int main()
{
	setlocale(LC_ALL, "Russian");
	int i,countCities=0,cost,length,numA,numB;
	char aIn = NULL, bIn = NULL;
	bool check;
	city *cities[MAX_N],*A,*B;
	fstream graph(INPUT);
	for (i = 0; i < MAX_N; i++) cities[i] = NULL;
	if (graph)
	{
		graph >> aIn;
		check = false;
		while (aIn != '.')
		{
			if ((graph.eof())|| (countCities == MAX_N))
			{
				check = true;
				break;
			}
			for (i = 0; i < MAX_N; i++)
			{
				if (cities[i] == NULL)
				{
					cities[i] = new city;
					cities[i]->name = aIn;
					countCities++;
					break;
				}
				else
					if (aIn == cities[i]->name)
					{
						cout << "Город " << aIn << " уже есть." << endl;
						break;
					}
			}
			graph >> aIn;
		}
		if (check)
		{
			cout << "Ошибка во входном файле." << endl;
		}
		else
		{
			while (!graph.eof())
			{
				aIn = bIn = NULL;
				cost = length = -1;
				graph >> aIn >> bIn;
				graph >> cost >> length;
				if ((aIn == NULL) || (bIn == NULL) || (cost < 0) || (length < 0))
				{
					cout << "Ошибка во входном файле." << endl;
					break;
				}
				A = B = NULL;
				for (i = 0; i < countCities; i++)
				{
					if (aIn == cities[i]->name) A = cities[i];
					if (bIn == cities[i]->name) B = cities[i];
				}
				
				if ((A != NULL) && (B != NULL)) 
				{
					if (A != B)
					{
						check = false;
						for (i = 0; i < A->countOut; i++)
							if (A->cities[i] == B)
							{
								if ((A->cost[i] + A->length[i]) >= (cost + length))
								{
									check = true;
									numA = i;
									for (int j = 0; j < A->countOut; j++)
										if (B->cities[j] == A) numB = j;
								}
							}
						if (!check)
						{
							A->cities[A->countOut] = B;
							B->cities[B->countOut] = A;
							A->cost[A->countOut] = B->cost[B->countOut] = cost;
							A->length[A->countOut] = B->length[B->countOut] = length;
							A->countOut++;
							B->countOut++;
						}
						else
						{
							A->cities[numA] = B;
							B->cities[numB] = A;
							A->cost[numA] = B->cost[numB] = cost;
							A->length[numA] = B->length[numB] = length;
						}
					}
				}
				else
				{
					cout << "Ошибка во входном файле.1" << endl;
				}
			}
			graph.close();
		}
		A = B = NULL;
		for (i = 0; i < countCities; i++)
		{
			if ('A' == cities[i]->name) A = cities[i];
			else if ('B' == cities[i]->name) B = cities[i];
		}
		if ((A != NULL) && (B != NULL))
		{
			search(A, B);
			if (min != (-1))
			{
				cout << "Самый оптимальный путь: ";
				for (i = 0; i < minWayCount; i++) cout << minWay[i]->name << " ";
				cout << " Длина пути: " << minL << ". Потрачено денег на дорогу: " << minC << "." << endl;
			}
			else 
			{
				cout << "Между городами A и B нет пути.";
			}
		}
		else
		{
			cout << "Нет города A или B.";
		}
	}
	else
	{
		cout << "Входной файл отсутствует.";
	}
	return 0;
}