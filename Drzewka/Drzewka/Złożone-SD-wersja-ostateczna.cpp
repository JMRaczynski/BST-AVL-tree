// Złożone SD.cpp : Defines the entry point for the console application.
//


#include"stdafx.h"
#include<iostream>
#include<fstream>
#include<chrono>
#include<string>
#include<chrono>
#include<random>
#include<vector>
#include<algorithm>
#include<Windows.h>

using namespace std;

struct node
{
	int value;
	node *rodzic;
	node *smson;
	node *lgson;

};

/*struct greater
{
template<class T>
bool operator()(T const &a, T const &b) const { return a > b; }
};*/

node *root;
int length, i = 0;
bool mamybst = false;

node *seek(int key, node *start)
{
	if (key == start->value)
	{
		return start;
	}
	else if (key < start->value && start->smson != NULL)
	{
		return seek(key, start->smson);
	}
	else if (key > start->value && start->lgson != NULL)
	{
		return seek(key, start->lgson);
	}
	else return NULL;
}

void addnode(int x, node *y)
{
	if (root == 0)
	{
		root = new node;
		root->value = x;
		root->rodzic = NULL;
		root->smson = NULL;
		root->lgson = NULL;
		mamybst = true;
		//cout << root -> value << endl;
	}
	else if (x > y->value)
	{
		if (y->lgson == NULL)
		{
			node *born = new node;
			born->value = x;
			born->rodzic = y;
			born->smson = NULL;
			born->lgson = NULL;
			y->lgson = born;
			//cout<< born -> value<<" "<<born -> rodzic -> value<<endl;
		}
		else
		{
			addnode(x, y->lgson);
		}
	}
	else
	{
		if (y->smson == NULL)
		{
			node *born = new node;
			born->value = x;
			born->rodzic = y;
			born->smson = NULL;
			born->lgson = NULL;
			y->smson = born;
			//cout<< born -> value<<" "<<born -> rodzic -> value<<endl;
		}
		else
		{
			addnode(x, y->smson);
		}
	}
}

void buildavl(vector <int>& data, int left, int right)
{
	if (left == right)
	{
		addnode(data[left], root);
	}
	else if (left < right)
	{
		int mid = (left + right) / 2;
		addnode(data[mid], root);
		buildavl(data, left, mid - 1);
		buildavl(data, mid + 1, right);
	}
}


void findbiggest(node *x)
{
	cout << x->value << " ";
	if (x->lgson != NULL)
	{
		findbiggest(x->lgson);
	}
}

node *findbiggestnode(node *x)
{
	if (x->lgson != NULL)
	{
		findbiggest(x->lgson);
	}
	else return x;
}

void deletenode(node *x)
{
	if (x->lgson == NULL && x->smson == NULL)//przypadek wezla bez potomkow
	{
		if (x->rodzic == NULL) root = 0;//wezel jest rootem
		else if (x->rodzic->lgson == x)//sprawdzanie ktorym synem swojego rodzica jest wezel do usuniecia
		{
			x->rodzic->lgson = NULL;
		}
		else
		{
			x->rodzic->smson = NULL;
		}
		delete x;
	}
	else if (x->lgson == NULL || x->smson == NULL)
	{
		if (x->lgson == NULL)//ma tylko mniejszego potomka
		{
			if (x->rodzic == NULL)//wezel do usuniecia jest korzeniem - jego jedyny syn(mniejszy) jest nowym rootem
			{
				root = x->smson;
				root->rodzic = NULL;
			}
			else if (x->rodzic->lgson == x)//usuwany wezel byl wiekszym synem
			{
				x->smson->rodzic = x->rodzic;
				x->rodzic->lgson = x->smson;

			}
			else//usuwany wezel byl mniejszym synem
			{
				x->smson->rodzic = x->rodzic;
				x->rodzic->smson = x->smson;
			}
		}
		else//usuwany wezel ma tylko wiekszego potomka
		{
			if (x->rodzic == NULL)
			{
				root = x->lgson;
				root->rodzic = NULL;
			}
			else if (x->rodzic->lgson == x)
			{
				x->lgson->rodzic = x->rodzic;
				x->rodzic->lgson = x->lgson;
			}
			else
			{
				x->lgson->rodzic = x->rodzic;
				x->rodzic->smson = x->lgson;
			}
		}
		delete x;
	}
	else
	{
		node *pom;
		pom = findbiggestnode(x->smson);
		x->value = pom->value;
		deletenode(pom);
	}
}

void preorder(node *x)
{
	cout << x->value << " ";
	if (x->smson != NULL)
	{
		preorder(x->smson);
	}
	if (x->lgson != NULL)
	{
		preorder(x->lgson);
	}
}

void inorder(node *x)
{
	if (x->smson != NULL)
	{
		inorder(x->smson);
	}
	cout << x->value << " ";
	if (x->lgson != NULL)
	{
		inorder(x->lgson);
	}
}

void postorderremoval(node *x)
{
	if (x->smson != NULL)
	{
		postorderremoval(x->smson);
	}
	if (x->lgson != NULL)
	{
		postorderremoval(x->lgson);
	}
	delete x;
}

void displaysubtree(int key)
{
	//if (seek(key, root) == NULL)
	{
	//	cout << "Podano nieistniejący klucz " << key << endl;
	}
	// else
	{
		preorder(seek(key, root));
	}
}

void rot2right(node *x)							//rotacja wokol x
{
	node *s = x->smson, *p = x->rodzic;			//s - small, p - parent
	if (s != NULL)								//x ma mniejszego lewego syna
	{
		x->smson = s->lgson;					//prawy syn lewego syna x jest nowym lewym synem x
		if (x->smson != NULL)
		{
			x->smson->rodzic = x;				//przypisanie x za rodzica nowemu synowi
		}
		s->lgson = x;							//x wiekszym prawym synem nowego wezla s (dawny lewy syn x)
		s->rodzic = p;							//dawny rodzic x nowy rodzic s
		x->rodzic = s;							// s - nowy rodzic x

		if (p != NULL)							//przypisanie nowego syna dawnemu rodzicowi x we wlasciwe miejsce
		{
			if (p->smson == x)
			{
				p->smson = s;
			}
			else
			{
				p->lgson = s;
			}
		}
		else root = s;
	}
}

void rot2left(node *x)								//rotacja wokol x
{
	node *l = x->lgson, *p = x->rodzic;				//l-large, p - parent
	if (l != NULL)									//x ma wiekszego prawego syna
	{
		x->lgson = l->smson;						//mniejszy syn wiekszego syna x jest nowym wiekszym synem x
		if (x->lgson != NULL)
		{
			x->lgson->rodzic = x;					//przypisanie x za rodzica nowemu synowi
		}
		l->smson = x;								//x mniejszym prawym synem nowego wezla l (dawny prawy syn x)
		l->rodzic = p;								//dawny rodzic x nowy rodzic l
		x->rodzic = l;								// l - nowy rodzic x
		if (p != NULL)								//przypisanie nowego syna dawnemu rodzicowi x we wlasciwe miejsce
		{
			if (p->smson == x)
			{
				p->smson = l;
			}
			else
			{
				p->lgson = l;
			}
		}
		else root = l;
	}
}

int log2(int x)
{
	int n = 1;
	while (n < x)
	{
		n *= 2;
	}
	n /= 2;
	return n;
}

void rotbalance()
{
	int n, i, s;
	node * p;
	n = 0;                           // W n zliczamy węzły
	p = root;
	while (p != NULL)
		if (p->smson != NULL)        // Jadąc od korzenia dopóki węzeł ma lewego (mniejszego) syna, obracamy wokol niego w prawo
		{
			rot2right(p);
			//cout << endl;
			p = p->rodzic;			 // Następnie za nowe p ustawiamy rodzica obecnego p (w wyniku rotacji w prawo p jest prawym synem
		}							 // swojego dawnego lewego syna)
		else
		{
			n++;                     // Jeśli wezel nie ma lewego syna przesuwamy sie w prawo i zliczamy wszystkie wezly w drzewie
			p = p->lgson;
		}
	s = n + 1 - log2(n + 1);		 // Wyznaczamy początkową liczbę obrotów
	p = root;
	for (i = 0; i < s; i++)
	{
		rot2left(p);                 // co drugi węzeł obracamy w lewo
		p = p->rodzic->lgson;
	}
	n = n - s;
	while (n > 1)
	{
		n /= 2;
		p = root;
		for (i = 0; i < n; i++)
		{
			rot2left(p);
			p = p->rodzic->lgson;
		}
	}
}

int main()
{
	srand(time(0));
	vector <int> ta;
	string choice;
	//int t[]={5,10,25,1,3,12,7,2,30,6};
	length = 10;

	/*
	ofstream zapis;
	zapis.open("wyniki.txt");
	float czas;
	int u[] = { 10,100,1000,2500,5000,7500,10000,12500,15000,20000,25000 };
	for (int h = 0; h < 11; h++)
	{
	length = u[h];
	for (int j = 0; j < 7; j++)
	{
	srand(time(0));
	ta.clear();
	for (i = 0; i < 2 * length; i++)
	{
	ta.push_back(2 * length - i);
	}
	random_shuffle(ta.begin(), ta.end());
	for (i = 0; i < length; i++)
	{
	ta.pop_back();
	}
	sort(ta.begin(), ta.end(), greater());
	for (i = 0; i < length; i++)
	{
	addnode(ta[i], root);
	}
	//cout << ta[0] << endl;
	clock_t start = clock();
	rotbalance();
	czas = float(clock() - start) / CLOCKS_PER_SEC;
	zapis << czas << endl;
	postorderremoval(root);
	root = 0;
	}
	zapis << endl << endl;
	}
	*/



	while (1)
	{
		if (mamybst == false)
		{
			ta.clear();
			for (i = 0; i < 2 * length; i++)
			{
				ta.push_back(2 * length - i);
			}
			random_shuffle(ta.begin(), ta.end());
			for (i = 0; i < length; i++)
			{
				ta.pop_back();
			}
			cout << "Wpisz A, jesli chcesz wygenerowac drzewo AVL" << endl;
			cout << "B jesli chcesz wygenerowac drzewo BST" << endl;
			cout << "Q jesli chcesz wylaczyc program" << endl;
			getline(cin, choice);
			if (choice == "A")
			{
				sort(ta.begin(), ta.end());
				buildavl(ta, 0, length - 1);
				mamybst = true;
			}
			else if (choice == "B")
			{
				for (i = 0; i < length; i++)
				{
					addnode(ta[i], root);
				}
				mamybst = true;
			}
			else if (choice == "Q")
			{
				cout << "Program wylaczy sie za 3 sekundy";
				Sleep(3000);
				break;
			}
			cout << endl;
		}
		else
		{
			cout << "Wpisz M jesli chcesz wyszukac maksimum i wypisac sciezke poszukiwania" << endl;
			cout << "I jesli chcesz wypisac elementy drzewa w kolejnosci in-order" << endl;
			cout << "P jesli chcesz wypisac elementy drzewa w kolejnosci pre-order" << endl;
			cout << "R jesli chcesz usunac drzewo metoda post-order" << endl;
			cout << "W jesli chcesz wypisac wybrane poddrzewo" << endl;
			cout << "Z jesli chcesz zrownowazyc drzewo" << endl;
			cout << "D jesli chcesz usunac wybrane wierzcholki drzewa" << endl;
			cout << "Q jesli chcesz wylaczyc program" << endl;
			getline(cin, choice);
			if (choice == "M")
			{
				findbiggest(root);
			}
			else if (choice == "I")
			{
				inorder(root);
			}
			else if (choice == "P")
			{
				preorder(root);
			}
			else if (choice == "R")
			{
				postorderremoval(root);
				root = 0;
				mamybst = false;
				cout << "Procedura zakonczona sukcesem" << endl;
			}
			else if (choice == "W")
			{
				int klucz;
				do
				{
					while (1)
					{
						cout << "Wpisz klucz korzenia poddrzewa, ktore chcesz wyswietlic" << endl;
						if (cin >> klucz) break;
						else
						{
							cin.clear();
							cin.ignore(100, '\n');
						}
					}
					cin.clear();
					cin.ignore(100, '\n');
					if (seek(klucz, root) == NULL) cout << "W drzewie nie ma takiego wierzcholka" << endl;
					else break;
				} while (1);
				displaysubtree(klucz);
				cout << endl;
			}
			else if (choice == "Z")
			{
				rotbalance();
				cout << "Procedura balansowania drzewa zakonczona sukcesem" << endl;
			}
			else if (choice == "D")
			{
				int ilosc;
				do
				{
					while (1)
					{
						cout << "Wpisz ilosc wierzcholkow do usuniecia" << endl;
						if (cin >> ilosc) break;
						else
						{
							cin.clear();
							cin.ignore(100, '\n');
						}
					}
					cin.clear();
					cin.ignore(100, '\n');
					if (ilosc < 0 || ilosc >= length) cout << "Zla wartosc" << endl;
					else break;
				} while (1);
				for (i = 0; i < ilosc; i++)
				{
					int klucz;
					do
					{
						while (1)
						{
							cout << "Wpisz klucz " << i + 1 << " wierzcholka do usuniecia" << endl;
							if (cin >> klucz) break;
							else
							{
								cin.clear();
								cin.ignore(100, '\n');
							}
						}
						cin.clear();
						cin.ignore(100, '\n');
						if (seek(klucz, root) == NULL) cout << "W drzewie nie ma takiego wierzcholka" << endl;
						else break;
					} while (1);
					deletenode(seek(klucz, root));
					cout << "Procedura usuwania wezla zakonczona sukcesem" << endl;
				}
				length -= ilosc;
				if (length == 0) mamybst = false;
			}
			else if (choice == "Q")
			{
				postorderremoval(root);
				cout << "Program wylaczy sie za 3 sekundy";
				Sleep(3000);
				break;

			}
			cout << endl;
		}
	}
}
