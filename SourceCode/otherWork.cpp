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

/*  Xau ky tu tien
    Can khai bao toan cuc */
char monString[50];

/*  Kiem tra da co so nao dau tien chua
    Neu chua, chu so dau tien se viet hoa */
int isFirst{1};

/*  Viet bo ba chu so */
char *threeDigits(int money)
{
    char firstNum[10][5]= {"", "Mot", "Hai", "Ba", "Bon", "Nam", "Sau", "Bay", "Tam", "Chin"};
    char numChar[10][7]= {" khong", " mot", " hai", " ba", " bon", " nam", " sau", " bay", " tam", " chin"};

    int unit{};
    int dozens{};
    int hundred{};

    monString[0] = 0;

    unit = money % 10;
    money /= 10;

    dozens = money % 10;

    hundred = money / 10;

    if (!isFirst)
    {
        strcat(monString, numChar[hundred]);
        strcat(monString, " tram");
    }
    else if (hundred > 0)
    {
        strcat(monString, firstNum[hundred]);
        strcat(monString, " tram");
        isFirst = 0;
    }

    if (dozens > 0)
    {
        if (!isFirst)
        {
            if (dozens != 1)
            {
                strcat(monString, numChar[dozens]);
            }
            strcat(monString, " muoi");
        }
        else
        {
            if (dozens == 1)
            {
                strcat(monString, "Muoi");
            }
            else
            {
                strcat(monString, firstNum[dozens]);
                strcat(monString, " muoi");
            }
            isFirst = 0;
        }
    }
    if (unit > 0)
    {
        if (dozens == 0 && !isFirst)
        {
            strcat(monString, " linh");
        }

        if (dozens > 1 && unit == 4)
        {
            strcat(monString, " tu");
        }
        else if (unit == 5 && (dozens != 0))
        {
            strcat(monString, " lam");
        }
        else if (unit == 5 && dozens == 0)
        {
            strcat(monString, " nam");
        }
        else if (!isFirst)
        {
            strcat(monString, numChar[unit]);
        }
        else
        {
            strcat(monString, firstNum[unit]);
            isFirst = 0;
        }
    }
    return monString;
}

/*  Hien thi so thanh xau ky tu */
void displayString(int money)
{
    isFirst = 1;
    int unit{};
    int thousand{};
    int million{};
    int billion{};

    if (money == 0)
        std::cout << "Khong dong.";

    else
    {
        unit = money % 1000;
        money /= 1000;

        thousand = money % 1000;
        money /= 1000;

        million = money % 1000;

        billion = money / 1000;

        if (billion > 0)
            std::cout << threeDigits(billion) << " ty";

        if (million > 0)
            printf("%s trieu", threeDigits(million));

        if (thousand > 0)
            printf("%s nghin", threeDigits(thousand));

        if (unit > 0)
            printf("%s", threeDigits(unit));

        printf(" dong.");
    }
}

/*  Hien thi so tien voi dau phay (de doc) */
void moneyWithComma(int money)
{
    if (money < 1000)
    {
        std::cout << setw(3) << std::setfill(' ') << money;
        return;
    }

    moneyWithComma(money/1000);
    printf (",%03d", money%1000);
}

/*  Hien thi do dai so tien (muc dich can chinh le) */
int displayLen(int money, int len)
{
    while (money > 999)
    {
        len = len + 4;
        money = money/1000;
    }

    return len;
}

/*  In hoa don theo ma khach hang va ky */
void printBill()
{
    std::cout << "Nhap ma khach hang: ";
    int cCode = validCode();
    std::cout << endl;

    int n = getNumKH();

    int isExistCus{0};

    Customer thisCustomer;

    ifstream rKH("KH.BIN", ios::binary);
    for (int i = 0; i < n; i++)
    {
        rKH.read((char *) &thisCustomer, sizeof(Customer));
        if (thisCustomer.cCode == cCode)
        {
            isExistCus = 1;
            break;
        }
    }
    rKH.close();

    if (!isExistCus)
    {
        std::cout << "Khong ton tai ma khach hang " << cCode << " trong file KH.BIN.\n";
        std::cout << "In hoa don da ngung!\n";
        return;
    }

    std::cout << "Nhap ky muon in hoa don: ";
    int index = getPeriod() - 1;
    std::cout << endl;

    ElecStat temp;
    ElecStat record[12];

    ifstream rCS("CSDIEN.BIN", ios::binary);
    while (rCS.read((char *) &record[0], sizeof(ElecStat)))
    {
        if (record[0].cCode == cCode)
        {
            for (int i = 1; i < 11; i++)
            {
                rCS.read((char *) &record[i], sizeof(ElecStat));
            }
            if (record[index].eStat == -1)
            {
                std::cout << "Chua co ban ghi ky " << index + 1 << "!\n";
                std::cout << "Ngung in hoa don!\n";
                return;
            }
            break;
        }
    }

    Bill thisBill;
    thisBill.cCode = cCode;
    if (index == 0)
    {
        thisBill.eUsed = record[index].eStat;
    }
    else
    {
        thisBill.eUsed = record[index].eStat - record[index - 1].eStat;
    }
    thisBill.eMon = monCalculate(thisBill.eUsed);
    thisBill.isExist = 1;

    system("cls");
    std::cout << "\t         >>>HOA DON TIEN DIEN<<<\n\n";
    std::cout << "Ma khach hang\t: " << cCode << endl;
    std::cout << "Ten khach hang\t: " << thisCustomer.cName << endl;
    std::cout << "Dia chi\t\t: " << thisCustomer.cAdd << endl;
    std::cout << "Ma so cong to\t: " << thisCustomer.cMeter << endl << endl;

    std::cout << "Ky: " << index + 1 << "\t\tTu ngay: ";
    if (index == 0)
    {
        std::cout << 0 << "\t";
    }
    else
    {
        std::cout << record[index - 1].eDay << " ky " << index;
    }
    std::cout << "\tDen ngay: " << record[index].eDay << " ky " << index + 1 << "\n\n";

    std::cout << "Dien nang tieu thu: " << thisBill.eUsed << " kWh.\n";
    std::cout << "Tien dien\t  : ";
    moneyWithComma(thisBill.eMon);
    std::cout << " dong.\n";
    std::cout << "Thue (10%)\t  : ";
    moneyWithComma(thisBill.eMon/10);
    std::cout << " dong.\n";

    std::cout << "Tong cong so tien phai thanh toan: ";
    moneyWithComma(11*thisBill.eMon/10);
    std::cout << " dong.\n";

    int money = 11*thisBill.eMon/10;
    std::cout << "So tien (viet bang chu): ";
    displayString(money);
    std::cout << endl;
}
