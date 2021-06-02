// by: Kian Majlessi - 2048 GAME

#include <iostream>
#include <string.h>
#include <time.h>
#include <stdlib.h>
using namespace std;

const unsigned short MAXMAP = 20;

struct player
{
    char name[100];
    unsigned short map;
    unsigned int highscore;
    unsigned int gamescore;
    unsigned int game[MAXMAP][MAXMAP];
    bool newg = false;
    double timetake;
} plr;

void menu();
void guide();
bool datarw(char user[], unsigned short mp);
void fillrand();
void game();
bool check(unsigned int b[][MAXMAP]);
void leaderboard(unsigned short map);
void tocaps(char &in);
void showgrid();
void savetemp(unsigned int b[][MAXMAP]);
void gameundo(unsigned int b[][MAXMAP], unsigned int gametemp[MAXMAP][MAXMAP]);
void up(unsigned int &sc);
void down(unsigned int &sc);
void left(unsigned int &sc);
void right(unsigned int &sc);
bool savedata();
void sortdata();
bool gameover();

int main()
{
    menu();
    return 0;
}

void menu()
{
    system("cls");
    unsigned short menutemp, menulead, maptemp;
    char userin[100];
    cout << "------------- Welcome To The 2048 GAME ! -------------" << endl;
    cout << "\tMenu:" << endl;
    cout << "\t<1> : Guide Menu" << endl
         << "\t<2> : Leaderboard" << endl
         << "\t<3> : Small - 3x3" << endl
         << "\t<4> : Classic - 4x4" << endl
         << "\t<5> : Big - 5x5" << endl
         << "\t<6> : Bigger - 6x6" << endl
         << "\t<8> : Huge - 8x8" << endl
         << "\t<9> : Custom - nxn" << endl
         << "\t<0> : Exit" << endl;
    cout << endl
         << "\tEnter Your Choice ---> ";
    cin >> menutemp;
    switch (menutemp)
    {
    case 0:
        cout << "\tThanks For Playing 2048 (by: Kian Majlessi)\n"
             << endl;
        system("pause");
        exit(0);
    case 1:
        guide();
        menu();
    case 2:
        cout << endl
             << "\t---> Select Your Map : ";
        cin >> menulead;
        while (menulead > MAXMAP || menulead == 1)
        {
            cout << endl
                 << "\tMaximum of map size is : " << MAXMAP << " & Minimum of map size is : 2" << endl
                 << "\t---> Enter Your Map Size (nxn): ";
            cin >> menulead;
        }
        leaderboard(menulead);
        break;
    case 9:
        cout << endl
             << "\t---> Enter Your Map Size (nxn): ";
        cin >> maptemp;
        while (maptemp > MAXMAP || maptemp == 1)
        {
            cout << endl
                 << "\tMaximum of n is : " << MAXMAP << " & Minimum of n is : 2" << endl
                 << "\t---> Enter Your Map Size (nxn): ";
            cin >> maptemp;
        }
        cout << endl
             << "\t---> Enter Your Name : ";
        cin.ignore();
        cin.getline(userin, 100);
        if (datarw(userin, maptemp))
        {
            if (plr.newg)
                fillrand();
            game();
        }
        else
            cout << "\tError!";
        break;
    default:
        cout << endl
             << "\t---> Enter Your Name : ";
        cin.ignore();
        cin.getline(userin, 100);
        if (datarw(userin, menutemp))
        {
            if (plr.newg)
                fillrand();
            game();
        }
        else
            cout << "\tError!";
    }
    return;
}

void guide()
{
    system("cls");
    cout << "!------------- 2048 GAME | Guide Menu -------------!" << endl;
    cout << "\t<W> : UP" << endl
         << "\t<A> : LEFT" << endl
         << "\t<S> : DOWN" << endl
         << "\t<D> : RIGHT" << endl
         << "\t<U> : UNDO" << endl
         << "\t<N> : NEW GAME" << endl
         << "\t<E> : EXIT & SAVE\n"
         << endl;
    system("pause");
}

void leaderboard(unsigned short map)
{
    sortdata();
    FILE *fplay;
    fplay = fopen("data.dat", "r");
    if (!fplay)
    {
        fprintf(stderr, "\nNo Data Found!\n");
        return;
    }
    system("cls");
    cout << "!------------- 2048 GAME | Leaderboard | MAP : " << map << "x" << map << " -------------!" << endl;
    cout << endl
         << "Name\t\tHigh Score\tDuration (Last Game)" << endl;
    while (fread(&plr, sizeof(plr), 1, fplay))
        if (plr.map == map)
            cout << plr.name << "\t\t" << plr.highscore << "\t\t" << plr.timetake << "s" << endl;
    fclose(fplay);
    system("pause");
    menu();
}

bool datarw(char user[], unsigned short mp)
{
    FILE *fplayer;
    fplayer = fopen("data.dat", "a+");
    if (!fplayer)
    {
        fprintf(stderr, "\nError Opend File\n");
        return false;
    }

    while (fread(&plr, sizeof(plr), 1, fplayer))
    {
        if (!strcmp(plr.name, user) && plr.map == mp)
        {
            fclose(fplayer);
            return true;
        }
    }

    strcpy(plr.name, user);
    plr.gamescore = 0;
    plr.highscore = 0;
    plr.newg = true;
    plr.map = mp;
    for (int i = 0; i < plr.map; i++)
        for (int j = 0; j < plr.map; j++)
            plr.game[i][j] = 0;
    fwrite(&plr, sizeof(plr), 1, fplayer);
    fclose(fplayer);
    return true;
}

void game()
{
    unsigned int scoreun = 0;
    unsigned int gametemp[MAXMAP][MAXMAP] = {}, gundo[MAXMAP][MAXMAP] = {};
    char in;
    bool save = false;
    time_t start, end;
    start = time(NULL);
    gameundo(gundo, gametemp);
    while (true)
    {
        if (plr.gamescore > plr.highscore)
            plr.highscore = plr.gamescore;

        system("cls");
        cout << "!------------- 2048 GAME | " << plr.name << " | MAP : " << plr.map << "x" << plr.map
             << " | SCORE : " << plr.gamescore << " | HIGH SCORE : " << plr.highscore << " -------------!\n"
             << endl;
        showgrid();
        cout << endl
             << "\t(U : UNDO) | (N : NEW GAME) | (E : EXIT & SAVE) | (G : Guide Menu)" << endl
             << "\n\tEnter Your Choice ---> ";
        cin >> in;
        tocaps(in);

        if (in == 'G')
            guide();

        if (in == 'E')
        {
            end = time(NULL);
            plr.timetake = difftime(end, start);
            cout << endl
                 << "\tDuration : " << plr.timetake << "s" << endl;
            if (savedata())
            {
                cout << endl
                     << "\t!!! SAVED !!!\n"
                     << endl;
                system("pause");
                menu();
            }
            else
                cerr << "ERROR!";
        }

        if (in == 'U')
        {
            for (int i = 0; i < plr.map; i++)
                for (int j = 0; j < plr.map; j++)
                    plr.game[i][j] = gundo[i][j];
            plr.gamescore = scoreun;
            continue;
        }

        if (in == 'N')
        {
            plr.newg = true;
            plr.gamescore = 0;
            for (int i = 0; i < plr.map; i++)
                for (int j = 0; j < plr.map; j++)
                    plr.game[i][j] = 0;
            fillrand();
            game();
        }

        if (gameover())
        {
            cout << endl
                 << "\t!!! GAME OVER !!!\n"
                 << endl;
            system("pause");
        }

        savetemp(gametemp);
        if (save)
            scoreun = plr.gamescore;
        switch (in) // for movements
        {
        case 'W':
            up(plr.gamescore);
            break;
        case 'S':
            down(plr.gamescore);
            break;
        case 'A':
            left(plr.gamescore);
            break;
        case 'D':
            right(plr.gamescore);
            break;
        }

        if (!check(gametemp))
        {
            save = true;
            fillrand();
        }
        else
            save = false;

        if (save)
            gameundo(gundo, gametemp);
    }
}

void showgrid()
{
    for (int i = 0; i < plr.map; i++)
    {
        cout << "\t     |";
        for (int j = 0; j < plr.map; j++)
        {
            if (plr.game[i][j])
                printf("%4d    |", plr.game[i][j]);
            else
                printf("%4c    |", ' ');
        }
        cout << endl
             << endl;
    }
}

void fillrand()
{
    unsigned short i = 0, f, temp1, temp2;
    srand(time(NULL));
    if (plr.newg)
    {
        f = 2;
        plr.newg = false;
    }
    else
        f = 1;
    while (i < f)
    {
        temp1 = rand() % plr.map;
        temp2 = rand() % plr.map;
        if (!plr.game[temp2][temp1])
        {
            plr.game[temp2][temp1] = 2;
            i++;
        }
    }
    return;
}

void tocaps(char &in)
{
    if (in >= 'a' && in <= 'z')
        in -= 32;
    return;
}

bool check(unsigned int b[][MAXMAP])
{
    for (int i = 0; i < plr.map; i++)
    {
        for (int j = 0; j < plr.map; j++)
        {
            if (b[i][j] != plr.game[i][j])
                return false;
        }
    }

    return true;
}

void savetemp(unsigned int b[][MAXMAP])
{
    for (int i = 0; i < plr.map; i++)
        for (int j = 0; j < plr.map; j++)
            b[i][j] = plr.game[i][j];
}

void gameundo(unsigned int b[][MAXMAP], unsigned int gametemp[MAXMAP][MAXMAP])
{
    for (int i = 0; i < plr.map; i++)
        for (int j = 0; j < plr.map; j++)
            b[i][j] = gametemp[i][j];
}

void up(unsigned int &sc) // W
{
    for (int i = 0; i < plr.map; i++)
        for (int j = plr.map - 1; j > 0; j--)
            for (int k = 0; k < plr.map; k++)
            {
                if (!plr.game[j - 1][k])
                {
                    plr.game[j - 1][k] = plr.game[j][k];
                    plr.game[j][k] = 0;
                }

                if (plr.game[j - 1][k] == plr.game[j][k])
                {
                    sc += plr.game[j - 1][k] *= 2;
                    plr.game[j][k] = 0;
                }
            }
}

void down(unsigned int &sc) // S
{
    for (int i = 0; i < plr.map; i++)
        for (int j = 0; j < plr.map - 1; j++)
            for (int k = 0; k < plr.map; k++)
            {
                if (!plr.game[j + 1][k])
                {
                    plr.game[j + 1][k] = plr.game[j][k];
                    plr.game[j][k] = 0;
                }

                if (plr.game[j + 1][k] == plr.game[j][k])
                {
                    sc += plr.game[j + 1][k] *= 2;
                    plr.game[j][k] = 0;
                }
            }
}

void left(unsigned int &sc) // A
{
    for (int i = 0; i < plr.map; i++)
        for (int j = 0; j < plr.map; j++)
            for (int k = plr.map - 1; k > 0; k--)
            {
                if (!plr.game[j][k - 1])
                {
                    plr.game[j][k - 1] = plr.game[j][k];
                    plr.game[j][k] = 0;
                }

                if (plr.game[j][k - 1] == plr.game[j][k])
                {
                    sc += plr.game[j][k - 1] *= 2;
                    plr.game[j][k] = 0;
                }
            }
}

void right(unsigned int &sc) // D
{
    for (int i = 0; i < plr.map; i++)
        for (int j = 0; j < plr.map; j++)
            for (int k = 0; k < plr.map - 1; k++)
            {
                if (!plr.game[j][k + 1])
                {
                    plr.game[j][k + 1] = plr.game[j][k];
                    plr.game[j][k] = 0;
                }

                if (plr.game[j][k + 1] == plr.game[j][k])
                {
                    sc += plr.game[j][k + 1] *= 2;
                    plr.game[j][k] = 0;
                }
            }
}

bool gameover()
{
    unsigned int temp = 0, gtemp[MAXMAP][MAXMAP];
    bool flag = false;
    savetemp(gtemp);

    for (int i = 0; i < plr.map; i++)
        for (int j = 0; j < plr.map; j++)
            if (!gtemp[i][j])
                return false;

    up(temp);
    flag = check(gtemp);
    down(temp);
    flag = check(gtemp);
    left(temp);
    flag = check(gtemp);
    right(temp);
    flag = check(gtemp);

    for (int i = 0; i < plr.map; i++)
        for (int j = 0; j < plr.map; j++)
            plr.game[i][j] = gtemp[i][j];

    if (flag)
        return true;
    return false;
}

bool savedata()
{
    player plrtemp;
    FILE *fpgame;
    fpgame = fopen("data.dat", "r+");
    if (!fpgame)
    {
        fprintf(stderr, "\nError Opend File\n");
        return false;
    }

    while (fread(&plrtemp, sizeof(plrtemp), 1, fpgame))
    {
        if (!strcmp(plrtemp.name, plr.name) && plrtemp.map == plr.map)
        {
            fseek(fpgame, -(long int)sizeof(plrtemp), SEEK_CUR);
            fwrite(&plr, sizeof(plr), 1, fpgame);
            fclose(fpgame);
            return true;
        }
    }

    return false;
}

void sortdata()
{
    unsigned int count = 0, i = 0, imax;
    player *plrdata, m;
    FILE *fpdata;
    fpdata = fopen("data.dat", "r+");
    if (!fpdata)
    {
        fprintf(stderr, "\nError Opend File\n");
        return;
    }

    while (fread(&plr, sizeof(plr), 1, fpdata))
        count++;

    plrdata = new player[count];
    fseek(fpdata, 0, SEEK_SET);

    while (fread(&plr, sizeof(plr), 1, fpdata))
    {
        plrdata[i] = plr;
        i++;
    }

    fclose(fpdata);

    for (int j = 0; j < count - 1; j++)
    {
        m = plrdata[j];
        imax = j;
        for (int k = j + 1; k < count; k++)
        {
            if (plrdata[k].highscore > m.highscore)
            {
                m = plrdata[k];
                imax = k;
            }
        }
        plrdata[imax] = plrdata[j];
        plrdata[j] = m;
    }

    fpdata = fopen("data.dat", "w+");
    if (!fpdata)
    {
        fprintf(stderr, "\nError Opend File\n");
        return;
    }

    for (int j = 0; j < count; j++)
        fwrite(&plrdata[j], sizeof(plr), 1, fpdata);

    fclose(fpdata);
    delete[] plrdata;
    return;
}