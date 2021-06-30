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

int getNumGIADIEN()
{
    int n{};

    ElecPrice scope;
    ifstream rGD("GIADIEN.BIN", ios::binary);
    while (rGD.read((char *) &scope, sizeof(ElecPrice)))
    {
        n++;
    }
    rGD.close();

    return n;
}

/*  Viet gia dien (them hoac sua) */
void writeGIADIEN(int n, int level)
{
    /*  Doc file GIADIEN
        Ghi vao file tempGIADIEN */

    ifstream rGD("GIADIEN.BIN", ios::binary);
    ofstream wGD("tempGIADIEN.BIN", ios::binary);

    /*  Ghi vao mang scope N muc gia */
    ElecPrice *scope = new ElecPrice[n];

    system("cls");
    std::cout << "+---------------------------------------+\n";
    std::cout << "| Muc\t| Tu kWh so\t| Muc gia\t|\n";
    std::cout << "+---------------------------------------+\n";
    for (int i = 0; i < n; i++)
    {
        /*  Doc den muc gia can ghi thi dung vong lap
            De thuc hien them/sua */
        if (i == level)
        {
            break;
        }

        /*  Neu khong thi doc muc gia, hien thi va ghi vao file temp */
        rGD.read((char *) &scope[i], sizeof(ElecPrice));
        std::cout << "| ";
        std::cout << setw(5) << std::setfill(' ') << i + 1 << "\t| ";
        std::cout << setw(14) << std::setfill(' ') << scope[i].eUse << "| ";
        std::cout << setw(14) << std::setfill(' ') << scope[i].ePrc << "|\n";
        wGD.write((char *) &scope[i], sizeof(ElecPrice));
    }

    /*  Hien thi muc gia can nhap */
    std::cout << "| ";
    std::cout << setw(5) << std::setfill(' ') << level + 1 << "\t| ";
    std::cout << setw(16) << std::setfill(' ') << "><| ";
    std::cout << setw(16) << std::setfill(' ') << "|\n";
    std::cout << "+---------------------------------------+\n\n";

    if (level > 0)
    {
        std::cout << "Ap dung tu kWh so: ";
        scope[level].eUse = getInt();
        std::cout << endl;
        while (scope[level].eUse <= scope[level - 1].eUse)
        {
            if (scope[level].eUse < scope[level - 1].eUse)
            {
                std::cout << "Khong hop le: so kWh ap dung hien tai "
                          "nho hon muc "
                          << level << "( " << scope[level].eUse
                          << " < " << scope[level - 1].eUse << ")!\n";
            }
            else
            {
                std::cout << "Khong hop le: muc ap dung hien tai "
                          " trung voi muc ap dung so " << level << endl;
            }
            std::cout << "Nhap lai so kWh bat dau ap dung: ";
            scope[level].eUse = getInt();
            std::cout << endl;
        }
    }
    else
    {
        /*  Neu muc gia = 1 thi phai bat dau tinh tu kWh thu nhat */
        scope[level].eUse = 1;
    }

    /*  Hien thi phan da nhap */
    system("cls");
    std::cout << "+---------------------------------------+\n";
    std::cout << "| Muc\t| Tu kWh so\t| Muc gia\t|\n";
    std::cout << "+---------------------------------------+\n";
    for (int i = 0; i < n; i++)
    {
        if (i == level)
        {
            break;
        }
        std::cout << "| ";
        std::cout << setw(5) << std::setfill(' ') << i + 1 << "\t| ";
        std::cout << setw(14) << std::setfill(' ') << scope[i].eUse << "| ";
        std::cout << setw(14) << std::setfill(' ') << scope[i].ePrc << "|\n";
    }
    std::cout << "| ";
    std::cout << setw(5) << std::setfill(' ') << level + 1 << "\t| ";
    std::cout << setw(14) << std::setfill(' ') << scope[level].eUse << "| ";
    std::cout << setw(16) << std::setfill(' ') << "><|\n";
    std::cout << "+---------------------------------------+\n\n";

    /*  Tuong tu voi muc gia ap dung */
    std::cout << "Muc gia ap dung: ";
    scope[level].ePrc = getInt();
    std::cout << endl;
    if (level > 0)
    {
        /*  Kiem tra muc gia hien tai co lon hon muc gia truoc do khong */
        while (scope[level].ePrc <= scope[level - 1].ePrc)
        {
            if (scope[level].ePrc < scope[level - 1].ePrc)
            {
                std::cout << "Khong hop le: gia ap dung hien tai "
                          "nho hon gia ap dung tai muc "
                          << level - 1 << "(" << scope[level].ePrc
                          << " < " << scope[level - 1].ePrc << ")!\n";
            }
            else
            {
                std::cout << "Khong hop le: gia ap dung hien tai "
                          " trung voi gia ap tai muc " << level << endl;
            }
            std::cout << "Nhap lai muc gia ap dung:\n";
            scope[level].ePrc = getInt();
            std::cout << endl;
        }
    }

    wGD.write((char *) &scope[level], sizeof(ElecPrice));

    /*  level < n - 1: su dung cho ham cap nhat
        level = n - 1: su dung cho ham them */
    if (level < n - 1)
    {
        /*  Gia tri rac.
            Khong lam gi ngoai tru doc gia tri truoc day cua scope[level] */
        ElecPrice trash;
        rGD.read((char *) &trash, sizeof(ElecPrice));
    }

    char control{};

    for (int i = level + 1; i < n; i++)
    {
        rGD.read((char *) &scope[i], sizeof(ElecPrice));
        wGD.write((char *) &scope[i], sizeof(ElecPrice));
    }

    if (level < n - 1)
    {
        /*  Bien kiem soat thong bao loi */
        int isError{0};
        if (scope[level + 1].eUse != -1 && scope[level].eUse >= scope[level + 1].eUse)
        {
            std::cout << "Khong hop le: So kWh ap dung hien tai ";
            if (scope[level].eUse > scope[level + 1].eUse)
            {
                std::cout << "lon hon muc " << level + 2 << " phia sau (";
                std::cout << scope[level].eUse << " > " << scope[level + 1].eUse << ")!\n";
            }
            else
            {
                std::cout << "trung voi muc gia " << level + 2 << " phia sau!\n";
            }

            /* Neu xuat hien loi */
            isError = 1;
        }

        if (scope[level + 1].ePrc != -1 && scope[level].ePrc >= scope[level + 1].ePrc)
        {
            std::cout << "Khong hop le: Muc gia ap dung hien tai ";
            if (scope[level].ePrc > scope[level + 1].ePrc)
            {
                std::cout << "lon hon muc gia " << level + 2 << " phia sau (";
                std::cout << scope[level].ePrc << " > " << scope[level + 1].ePrc << ")!\n";
            }
            else
            {
                std::cout << "trung voi muc gia " << level + 2 << " phia sau!\n";
            }

            isError = 1;
        }

        /*  Hien thong bao loi trong truong hop phat hien */
        if (isError)
        {
            std::cout << "Nhung gi ban co the lam:\n"
                      "1. Nhap lai muc gia nay.\n"
                      "2. Giu nguyen muc gia nay, sua muc gia sau (muc " << level + 2 << ").\n";

            while (1)
            {
                control = getch();
                if (control == '1' || control == '2')
                {
                    break;
                }
            }
            switch (control)
            {
            case '1':
                rGD.close();
                wGD.close();
                remove("tempGIADIEN.BIN");
                writeGIADIEN(n, level);
                return;
            case '2':
                break;
            }
        }
    }

    /*  Xoa file GIADIEN
        Doi ten file temp thanh file GIADIEN moi */
    rGD.close();
    wGD.close();
    remove("GIADIEN.BIN");
    rename("tempGIADIEN.BIN", "GIADIEN.BIN");

    /*  Hien thi cac muc gia hien co */
    system("cls");
    std::cout << "+---------------------------------------+\n";
    std::cout << "| Muc\t| Tu kWh so\t| Muc gia\t|\n";
    std::cout << "+---------------------------------------+\n";
    for (int i = 0; i < n; i++)
    {
        std::cout << "| ";
        std::cout << setw(5) << std::setfill(' ') << i + 1 << "\t| ";
        std::cout << setw(14) << std::setfill(' ') << scope[i].eUse << "| ";
        std::cout << setw(14) << std::setfill(' ') << scope[i].ePrc << "|\n";
    }
    std::cout << "+---------------------------------------+\n\n";

    delete[] scope;

    if (control == '2')
    {
        writeGIADIEN(n, level + 1);
    }
}

/*  Dieu huong hanh dong voi file GIADIEN.BIN */
void GIADIENThings()
{
    std::cout << "Ban muon lam gi?\n"
              "1. Xem file.\n"
              "2. Them du lieu.\n"
              "3. Sua du lieu.\n"
              "4. Xoa du lieu.\n";
    char control{};
    while (1)
    {
        control = getch();
        if (control == '1' || control == '2' ||
                control == '3' || control == '4' )
        {
            break;
        }
    }
    system("cls");

    switch (control)
    {
    case '1':
        readGIADIEN();
        break;
    case '2':
        appendGIADIEN();
        break;
    case '3':
        updateGIADIEN();
        break;
    case '4':
        deleteGIADIEN();
        break;
    }
}

/*  Doc file GIADIEN.BIN */
void readGIADIEN()
{
    ifstream rGD("GIADIEN.BIN", ios::binary);

    int n{};
    ElecPrice scope;

    system("cls");

    std::cout << "+---------------------------------------+\n";
    std::cout << "| Muc\t| Tu kWh so\t| Muc gia\t|\n";
    std::cout << "+---------------------------------------+\n";
    while (rGD.read((char *) &scope, sizeof(ElecPrice)))
    {
        std::cout << "| ";
        std::cout << setw(5) << std::setfill(' ') << ++n << "\t| ";
        std::cout << setw(14) << std::setfill(' ') << scope.eUse << "| ";
        std::cout << setw(14) << std::setfill(' ') << scope.ePrc << "|\n";
    }
    std::cout << "+---------------------------------------+\n";

    if (n == 0)
    {
        system("cls");
        std::cout << "File rong!\n";
    }

    rGD.close();
}

/*  Them mot muc gia dien */
void appendGIADIEN()
{
    int n = getNumGIADIEN();

    writeGIADIEN(n + 1, n);

    std::cout << "Ban co muon nhap them khong? (y/n)\n";
    char control{};
    while (1)
    {
        control = getch();
        fflush(stdin);

        if (control == 'y' || control == 'Y')
        {
            system("cls");
            appendGIADIEN();
            break;
        }
        else if (control == 'n' || control == 'N')
        {
            std::cout << "n\n";
            break;
        }
    }
}

/*  Cap nhat gia dien tai mot muc */
void updateGIADIEN()
{
    int n = getNumGIADIEN();

    std::cout << "Nhap muc gia dien muon cap nhat (hien co " << n << " muc gia):\n";
    std::cout << "Muc: ";
    int level = getInt();
    std::cout << endl;

    if (level > n)
    {
        std::cout << "Chua co muc gia " << level << "!\n";
        std::cout << "Ket thuc cap nhat!\n";
        return;
    }

    writeGIADIEN(n, level - 1);
}

/*  Lien quan toi xoa tren file GIADIEN.BIN */
void deleteGIADIEN()
{
    std::cout << "Cac tuy chon xoa:\n"
              "1. Xoa toan bo file.\n"
              "2. Xoa mot muc su dung.\n";
    char control{};
    while (1)
    {
        control = getch();
        if (control == '1' || control == '2')
        {
            break;
        }
    }

    switch (control)
    {
    case '1':
        std::cout << 1 << endl;
        confirmDel("GIADIEN.BIN");
        break;
    case '2':
        std::cout << 2 << endl;
        deletePriceLevel();
        break;
    }
}

/*  Xoa mot muc gia dien */
void deletePriceLevel()
{
    int n = getNumGIADIEN();

    if (n == 0)
    {
        std::cout << "Chua co muc gia nao de xoa!\n";
        return;
    }

    ifstream rGD("GIADIEN.BIN", ios::binary);
    if (!rGD.good())
    {
        std::cout << "Xay ra loi khi doc file!\n";
        return;
    }

    ElecPrice *scope = new ElecPrice[n];

    system("cls");

    std::cout << "+---------------------------------------+\n";
    std::cout << "| Muc\t| Tu kWh so\t| Muc gia\t|\n";
    std::cout << "+---------------------------------------+\n";
    for (int i = 0; i < n; i++)
    {
        rGD.read((char *) &scope[i], sizeof(ElecPrice));
        std::cout << "| ";
        std::cout << setw(5) << std::setfill(' ') << i + 1 << "\t| ";
        std::cout << setw(14) << std::setfill(' ') << scope[i].eUse << "| ";
        std::cout << setw(14) << std::setfill(' ') << scope[i].ePrc << "|\n";
    }
    std::cout << "+---------------------------------------+\n\n";
    rGD.close();

    std::cout << "Ban muon xoa muc gia so: ";
    int level = getInt() - 1;
    std::cout << endl;
    while (level + 1 > n)
    {
        std::cout << "Khong co muc gia " << level + 1 << " (toi da " << n << ")\n";
        std::cout << "Yeu cau nhap lai so muc: ";
        level = getInt() - 1;
        std::cout << endl;
    }

    if (level == 0)
    {
        scope[1].eUse = 1;
    }

    ofstream wGD("tempGIADIEN.BIN", ios::binary);
    for (int i = 0; i < n; i++)
    {
        if (i != level)
        {
            wGD.write((char *) &scope[i], sizeof(ElecPrice));
        }
    }
    wGD.close();

    remove("GIADIEN.BIN");
    rename("tempGIADIEN.BIN", "GIADIEN.BIN");

    std::cout << "Da xoa muc gia so " << level + 1 << ".\n";

    delete[] scope;
}
