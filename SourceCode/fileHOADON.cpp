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

void HDThings()
{
    std::cout << "Nhap lua chon cua ban:\n"
              "1. Tao hoa don moi.\n"
              "2. Xem hoa don.\n"
              "3. Xoa file hoa don.\n";

    char control{};
    while (1)
    {
        control = getch();
        if (control == '1' || control == '2')
        {
            break;
        }
    }
    system("cls");
    switch (control)
    {
    case '1':
        createBill();
        break;
    case '2':
        readBill();
        break;
    case '3':
        remove("HOADON.BIN");
    }
}

/*  Money calculate */
int monCalculate(int eUsed)
{
    int n = getNumGIADIEN();

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
    int n = getNumKH();

    Customer oldOne;

    int *cCode = new int[n];

    /*  Cac ma khach hang co trong file KH
        Dung de kiem tra */
    ifstream rKH("KH.BIN", ios::binary);
    for (int i = 0; i < n; i++)
    {
        rKH.read((char *) &oldOne, sizeof(Customer));
        cCode[i] = oldOne.cCode;
    }
    rKH.close();

    ifstream rCS("CSDIEN.BIN", ios::binary);

    ElecStat record;
    Bill elecBill;

    /*  Bien tam de luu chi so dien ky dung truoc */
    int tempEStat;

    /*  Tinh hoa don, viet vao file */
    ofstream wHD("HOADON.BIN", ios::binary);

    /*  Kiem tra lan luot tung ban ghi CSDIEN */
    while (rCS.read((char *) &record, sizeof(ElecStat)))
    {
        /*  Kiem tra co ton tai ma khach hang trong file KH */
        for (int i = 0; i < n; i++)
        {
            if (record.cCode == cCode[i])
            {
                /*  Co ton tai */
                elecBill.isExist = 1;
            }
        }

        /*  Ma khach hang: checked */
        elecBill.cCode = record.cCode;

        for (int i = 0; i < 12; i++)
        {
            /*  Luong dien tieu thu: checked */
            if (i == 0)
            {
                elecBill.eUsed = record.eStat;
            }
            else
            {
                rCS.read((char *) &record, sizeof(ElecStat));
                elecBill.eUsed = record.eStat - tempEStat;
            }

            tempEStat = record.eStat;

            /*  Tien dien: checked */
            elecBill.eMon = monCalculate(elecBill.eUsed);

            if (record.eStat != -1)
            {
                wHD.write((char *) &elecBill, sizeof(Bill));
            }
        }
        elecBill.isExist = 0;
    }

    rCS.close();
    wHD.close();

    std::cout << "Da tao file HOADON.BIN.\n"
              "Ban co muon xem khong? (y/n)\n";

    char control{};
    while (1)
    {

        control = getch();
        fflush(stdin);

        if (control == 'y' || control == 'Y')
        {
            system("cls");
            readBill();
            break;
        }
        else if (control == 'n' || control == 'N')
        {
            std::cout << "n\n";
            break;
        }
    }
}

/*  Doc hoa don tu file */
void readBill()
{
    ifstream rHD("HOADON.BIN", ios::binary);

    if (!rHD.good())
    {
        std::cout << "Loi: chua tao file HOADON.BIN.\n"
                  "Ban co muon tao khong? (y/n)\n";

        char control{};
        while (1)
        {
            control = getch();
            fflush(stdin);
            if (control == 'y' || control == 'Y')
            {
                system("cls");
                createBill();
                break;
            }
            else if (control == 'n' || control == 'N')
            {
                std::cout << "n\n";
                break;
            }
        }
        return;
    }

    std::cout << "+-----------------------------------------------------+\n";
    std::cout << "|  Ma KH  | Ky | Dien nang tieu thu | Tien dien (VND) |\n";

    Bill elecBill;
    int preCode{-1};
    int n{};

    while (rHD.read((char *) &elecBill, sizeof(elecBill)))
    {
        if (elecBill.cCode != preCode)
        {
            std::cout << "|-----------------------------------------------------|\n";
            n = 0;

        }
        std::cout << "| ";
        std::cout << setw(6) << std::setfill('0') << elecBill.cCode;
        if (elecBill.isExist == 1)
        {
            std::cout << "  | ";
        }
        else
        {
            std::cout << "* | ";
        }
        std::cout << setw(2) << std::setfill(' ') << ++n << " | ";
        std::cout << setw(18) << std::setfill(' ') << elecBill.eUsed << " | ";
//                std::cout << setw(15) << std::setfill(' ') << elecBill.eMon << " |\n";
        int len = 0;
        int width = displayLen(elecBill.eMon, len);
        std::cout << setw(12 - width) << std::setfill(' ') << " ";
        moneyWithComma(elecBill.eMon);
        std::cout << " |" << endl;

        preCode = elecBill.cCode;
    }
    rHD.close();

    std::cout << "+-----------------------------------------------------+\n\n";
    std::cout << "(*): Ma khach hang khong ton tai trong file KH.BIN.\n";
}
