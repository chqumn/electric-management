#include <iostream>
#include <fstream>
#include <cstdlib>
#include <conio.h>
#include <string.h>
#include <cstring>
#include <iomanip>

#include "main.h"
#include "fileKH.h"
#include "fileCSDIEN.h"
#include "fileGIADIEN.h"
#include "fileHOADON.h"
#include "otherWork.h"

using namespace std;

/*  Money calculate */
int monCalculate(int eUsed)
{
    ifstream rCountGD("countGD.txt");
    int n{};
    rCountGD >> n;
    rCountGD.close();

    ifstream rGD("GIADIEN.BIN", ios::binary);

    ElecPrice *scope = new ElecPrice[n];

    for (int i = 0; i < n; i++)
    {
        rGD.read((char *) &scope[i], sizeof(ElecPrice));
    }
    rGD.close();

    ElecPrice temp[12];

    for (int i = 0; i < n - 1; i++)
    {
        temp[i].eUse = scope[i + 1].eUse - scope[i].eUse;
        temp[i].ePrc = scope[i].ePrc;
    }
    temp[n - 1].eUse = 2147483647;
    temp[n - 1].ePrc = scope[n - 1].ePrc;

    int money{};
    int i{0};
    while (i < n && eUsed > 0)
    {
        if (temp[i].eUse >= eUsed)
        {
            money = money + eUsed*temp[i].ePrc;
            eUsed = 0;
        }
        else
        {
            money = money + temp[i].eUse*temp[i].ePrc;
            eUsed = eUsed - temp[i].eUse;
        }
        i++;
    }

    return money;
}

/*  Tao va hien thi hoa don kha dung */
void createBill()
{
    ifstream rCountKH("countKH.txt");
    int n{};
    rCountKH >> n;
    rCountKH.close();

    Customer oldOne;

    int *cusData = new int[n];

    ifstream rKH("KH.BIN", ios::binary);
    for (int i = 0; i < n; i++)
    {
        rKH.read((char *) &oldOne, sizeof(Customer));
        cusData[i] = oldOne.cCode;
    }
    rKH.close();

    ifstream rCS("CSDIEN.BIN", ios::binary);

    ElecStat record;
    Bill elecBill;
    int dozen{0};
    int tempEStat;

    std::cout << "+-----------------------------------------------------+\n";
    std::cout << "|  Ma KH  | Ky | Dien nang tieu thu | Tien dien (VND) |\n";

    ofstream wHD("HOADON.BIN", ios::binary);

    while (rCS.read((char *) &record, sizeof(ElecStat)))
    {
        for (int i = 0; i < n; i++)
        {
            if (record.cCode == cusData[i])
            {
                elecBill.isExist = 1;
            }
        }

        elecBill.cCode = record.cCode;

        for (int i = 0; i < 12; i++)
        {
            dozen = dozen%12;
            if (dozen == 0)
            {
                std::cout << "|-----------------------------------------------------|\n";
                elecBill.eUsed = record.eStat;
            }
            else
            {
                rCS.read((char *) &record, sizeof(ElecStat));
                elecBill.eUsed = record.eStat - tempEStat;
            }
            tempEStat = record.eStat;
            elecBill.eMon = monCalculate(elecBill.eUsed);

            wHD.write((char *) &elecBill, sizeof(Bill));

            if (record.eStat != -1)
            {
                if (elecBill.isExist == 1)
                {
                    std::cout << "| " << elecBill.cCode << "  | ";
                }
                else
                {
                    std::cout << "| " << elecBill.cCode << "* | ";
                }
                std::cout << setw(2) << std::setfill(' ') << i + 1 << " | ";
                std::cout << setw(18) << std::setfill(' ') << elecBill.eUsed << " | ";
//                std::cout << setw(15) << std::setfill(' ') << elecBill.eMon << " |\n";
                int len = 0;
                int width = displayLen(elecBill.eMon, len);
                std::cout << setw(12 - width) << std::setfill(' ') << " ";
                moneyWithComma(elecBill.eMon);
                std::cout << " |" << endl;
            }

            dozen++;
        }
        elecBill.isExist = 0;
    }
    std::cout << "+-----------------------------------------------------+\n\n";
    std::cout << "(*): Ma khach hang khong ton tai trong file KH.BIN.\n";

    rCS.close();
    wHD.close();

}
