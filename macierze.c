#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

struct Macierz
{
    int wiersze, kolumny;
    float *elementy;
};
////////////////////////////////////////////////////////////////////
struct Macierz utworz(int wiersze, int kolumny)
{
    struct Macierz m={wiersze, kolumny, calloc(wiersze * kolumny, sizeof(float))};
    return m;
}
////////////////////////////////////////////////////////////////////
//funkcja wypisania macierzy
void wypisz(struct Macierz m)
{
    int i, j;
    printf("[");

    for(i = 0; i < m.wiersze; i++)
    {
        for(j = 0; j < m.kolumny; j++)
        {
            printf("%.1f\n", m.elementy[i*m.kolumny+j]);
        }
    }
    if(i < (m.wiersze - 1))
    {
        printf("\n");
    }
    printf("]");
}
////////////////////////////////////////////////////////////////////
struct Macierz wczytaj(char nazwa[])
{
    struct Macierz m;
    FILE *fin;
    fin = fopen(nazwa, "r");
    fscanf(fin, "%d", &m.wiersze);
    fscanf(fin, "%d", &m.kolumny);

    m=utworz(m.wiersze, m.kolumny);
    for(int i = 0; i <m.wiersze; i++)
        for(int j = 0;j <m.kolumny; j++)
        {
            fscanf(fin, "%f", &m.elementy[i*m.kolumny+j]);
        }

    fclose(fin);
    return m;
}
//////////////////////////////////////////////////////////////////// 
void zapisz(char nazwa[] , struct Macierz m)
{
    FILE *fout =  fopen(nazwa, "w+");
    int i, j;
    fprintf(fout, "%d\n %d\n", m.wiersze, m.kolumny);
    for (i = 0; i <(m.wiersze*m.kolumny); i++) 
    {
        fprintf(fout, "%.1f\n", m.elementy[i]);
    }
    fclose(fout);
}
////////////////////////////////////////////////////////////////////
void zniszcz(struct Macierz m)
{
    free(m.elementy);
}
////////////////////////////////////////////////////////////////////
struct Macierz razy_skalar(int k, struct Macierz m)
{
    
    for (int i = 0; i < m.wiersze; i++) 
        for (int j = 0; j < m.kolumny; j++)
            m.elementy[i*m.kolumny+j] = m.elementy[i*m.kolumny+j] * k;

    return m;
}
////////////////////////////////////////////////////////////////////
struct Macierz zmniejszona(struct Macierz m, int kolumna, int wiersz)
{
    struct Macierz min = utworz(m.wiersze - 1, m.kolumny - 1);
    int i = 0, j = 0;
    for (i;i<m.wiersze*m.kolumny;i++)
    {
    if ((i >= (wiersz+1)*m.kolumny||i < (wiersz*m.kolumny)) && (i%m.kolumny != kolumna ))
        {
            min.elementy[j] = m.elementy[i];
            j++;
        }
    }
    return min;
}
////////////////////////////////////////////////////////////////////
float wyznacznik(struct Macierz m, int wiersz)// żeby zrobić wyznacznik, macierz musi być kwadratowa
{
    
    float wynik = 0;
    if(m.wiersze != m.kolumny)
    {
    printf("macierz nie jest kwadratowa!\n");
    exit(-1);
    }

    else if(m.wiersze > 1)
    {
    for(int i = 0; i < m.wiersze; i++)
        {
            
            struct Macierz min = zmniejszona(m, i, wiersz);
            wynik += pow(-1, i + wiersz*1) * m.elementy[i + wiersz * m.kolumny] * wyznacznik(min, 0);
        }
    }
    else
    wynik = m.elementy[0];

    return wynik;
}
////////////////////////////////////////////////////////////////////
void wypisztxt(char *nazwa, struct Macierz m)
{
    FILE *fin = fopen(nazwa, "w+");
    int i, j;
    fprintf(fin, "%d %d\n", m.wiersze, m.kolumny);
    {
        for(i = 0; i < m.wiersze; i++)
        {
            for(j = 0; j < m.kolumny; j++)
            {
                fprintf(fin, "%f", m.elementy[i*m.kolumny+j]);
            }
        }
        fprintf(fin, "\n");
    }
    
    fclose(fin);
}
////////////////////////////////////////////////////////////////////
struct Macierz pomnoz_macierze(struct Macierz A, struct Macierz B)
{
    struct Macierz iloczyn = utworz(A.wiersze, B.kolumny);
    for (int i = 0; i < A.wiersze; i++) 
        for (int j = 0; j < B.kolumny; j++)
        {
            for(int n = 0;n <A.kolumny; n++)
            {
                iloczyn.elementy[i * iloczyn.kolumny+j] += 
                A.elementy[i * iloczyn.kolumny+n] * B.elementy[j+n*iloczyn.kolumny];
            }
        }
    return iloczyn;
}
////////////////////////////////////////////////////////////////////
struct Macierz transpozycja(struct Macierz m)
{
    struct Macierz wynik=utworz(m.wiersze, m.kolumny);  
    for(int i = 0; i < m.wiersze; i++)
        for(int j = 0; j < m.kolumny; j++)
        {
            wynik.elementy[i*m.kolumny+j] = m.elementy[i+j*m.kolumny];
        }

    return wynik;
}
////////////////////////////////////////////////////////////////////
struct Macierz macierz_dolaczona(struct Macierz m)
{
    struct Macierz X=utworz(m.wiersze,m.kolumny);
    int i, j;
    for(i = 0; i < m.wiersze; i++)
    for(j = 0; j < m.kolumny; j++)
    {
        X.elementy[i*m.kolumny+j]=wyznacznik(zmniejszona(m,j,i),0)*pow(-1,i+j); // pow biblioteki matematycznej
    }
return transpozycja(X);
}
////////////////////////////////////////////////////////////////////
struct Macierz odwrotna(struct Macierz m)
{
    struct Macierz dolaczona=macierz_dolaczona(m);
    struct Macierz odwrocona=utworz(m.wiersze,m.kolumny);
    for(int i = 0; i <m.wiersze; i++)
    for(int j = 0; j <m.kolumny; j++)
    odwrocona.elementy[i*m.kolumny+j]=(1/wyznacznik(m,0))*dolaczona.elementy[i*m.kolumny+j];
    return odwrocona;
}
////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{

    struct Macierz a=wczytaj("A.txt");
    struct Macierz b=wczytaj("B.txt");

    macierz_dolaczona(a);

    printf("Macierz a:\n");
    wypisz(a);
    printf("Macierz a odwrotna:\n");
    wypisz(odwrotna(a));
    printf("Macierz a dołączona:\n");
    wypisz(macierz_dolaczona(a));
    printf("wynik mnożenia: a * odwrotność z b\n");
    wypisz(pomnoz_macierze(a,odwrotna(a)));
////////////////////////////////////////////////////////////
    printf("Macierz b:\n");
    wypisz(b);
    printf("Macierz b odwrotna:\n");
    wypisz(odwrotna(b));
    printf("wynik mnożenia: a * odwrotność z b\n");
    wypisz(pomnoz_macierze(b,odwrotna(b)));

    zniszcz(a);
    zniszcz(b);
    
    zapisz("wynik.txt",a);
}
