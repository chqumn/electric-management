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

int getNumKH()
{
    int n{};

    Customer data;
    ifstream rKH("KH.BIN", ios::binary);
    while (rKH.read((char *) &data, sizeof(Customer)))
    {
        n++;
    }
    rKH.close();

    return n;
}

/*  Chuan hoa ma khach hang/ma cong to */
int validCode()
{
    /*  Ma khach hang/ma cong to gom 6 chu so */
    char temp[7];
    /*  Ky tu nhap vao */
    char decimal{};
    /* Kiem soat do dai */
    int codeLen{0};
    do
    {
        decimal = getch();

        /*  Chi cho phep thuc hien them ky tu vao xau
            Khi ky tu do la so
            Va do dai xau < 6 */
        if (decimal >= '0' && decimal <= '9' && codeLen < 6)
        {
            temp[codeLen++] = decimal;
            std::cout << decimal;
        }

        /*  Neu nhan backspace se xoa ky tu sau cung */
        if (decimal == '\b' && codeLen > 0)
        {
            temp[--codeLen] = 0;
            cout << "\b \b";
        }
    }
    /*  Lap lai cho den khi do dai xau = 7 va bat gap phim Enter */
    while (decimal != 13 || codeLen < 6);

    std::cout << endl;
    /*  Chuyen tu char* sang int */
    return atoi(temp);
}

/*  Kiem tra ma khach hang co la duy nhat */
bool isUniqueCode(int cCode, int finder)
{
    /*  finder: phan tu can thay the trong ham thay the
        Neu finder = -1: Ham them ma khach hang -> chi kiem tra tinh duy nhat
        Neu finder != -1: Ham thay the -> kiem tra hoac ma khach hang cap nhat la duy nhat
        hoac trung voi ma khach hang truoc khi cap nhat (finder) */
    if (finder != cCode)
    {
        /*  Mo file KH.BIN de kiem tra */
        ifstream rKH("KH.BIN", ios::binary);

        /*  Doc danh sach khach hang */
        Customer data;
        while (rKH.read((char *) &data, sizeof(Customer)))
        {
            /*  Neu phat hien trung lap -> return false */
            if (data.cCode == cCode)
            {
                std::cout << "Da ton tai ma khach hang!\n"
                          "Yeu cau nhap lai ma khach hang:\n";
                return false;
            }
        }
        /*  Kiem tra het van chua trung lap -> duy nhat -> return true */
        return true;
    }
    /*  Ma khach hang thay the trung voi ma ban dau -> return true */
    return true;
}

/*  Chuan hoa ten khach hang */
void validName(char *cName)
{
    memset(cName, 0, MAX_LEN);
    /*  Co che tuong tu ham chuan hoa ma khach hang/ma cong to
        Khac o cho khong can kiem soat do dai
        Khong duoc bo trong mien ten */
    char letter{};
    int nameLen{0};
    /*  Bien kiem soat ten nhap vao co rong khong */
    bool isEmpty{true};
    do
    {
        letter = getch();

        /*  Cac ky tu hop le chi bao gom bang chu cai va khoang trang (blankspace)
            Khong bao gom ky tu dac biet va so */
        if ((letter >= 'a' && letter <= 'z')
                || (letter >= 'A' && letter <= 'Z')
                || letter == 32)
        {
            cName[nameLen++] = letter;
            std::cout << letter;
            isEmpty = false;
        }
        if (nameLen > 0 && letter == '\b')
        {
            cName[--nameLen] = 0;
            cout << "\b \b";
        }
        if (nameLen == 0)
        {
            isEmpty = true;
        }
    }
    /*  Ket thuc khi an Enter va xau khong rong */
    while (letter != 13 || isEmpty);
    std::cout <<  endl;
}

/*  Nhap dia chi */
void getAddress(char *cAdd)
{
    memset(cAdd, 0, MAX_LEN);
    /*  Tuong tu ham validName
        Cho phep nhap ky tu dac biet va so */
    char character{};
    int addLen{0};
    bool isEmpty{true};
    do
    {
        character = getch();

        if (character > 31 && character < 127)
        {
            cAdd[addLen++] = character;
            std::cout << character;
            isEmpty = false;
        }

        if (addLen > 0 && character == '\b')
        {
            cAdd[--addLen] = 0;
            cout << "\b \b";
        }
        if (addLen == 0)
        {
            isEmpty = true;
        }
    }
    while (character != 13 || isEmpty);
    std::cout << endl;
}

/*  Dieu huong lam viec voi file KH.BIN */
void KHThings()
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
        readKH();
        break;
    case '2':
        /*  (-1): Them moi hoan toan, nhap ma khach hang tren man hinh */
        appendKH(-1);
        break;
    case '3':
        updateKH();
        break;
    case '4':
        deleteKH();
        break;
    }
}

/*  Doc file KH.BIN */
void readKH()
{
    int n{0};

    ifstream rKH("KH.BIN", ios::binary);
    if (!rKH.good())
    {
        std::cout << "Xay ra loi trong qua trinh doc!\n";
        return;
    }

    Customer data;
    while (rKH.read((char *) &data, sizeof(Customer)))
    {
        std::cout << "Ban ghi thu " << ++n << ":\n";
        std::cout << "Ma khach hang: ";
        std::cout << setw(6) << std::setfill('0') << data.cCode << endl;
        std::cout << "Ten khach hang: " << data.cName << endl;
        std::cout << "Dia chi khach hang: " << data.cAdd << endl;
        std::cout << "Ma cong to: ";
        std::cout << setw(6) << std::setfill('0') << data.cMeter << endl;
        std::cout << endl;
    }
    rKH.close();

    if (n == 0)
    {
        std::cout << "File rong!\n";
        return;
    }
}

/*  Them khach hang */
void appendKH(int finder)
{
    /*  Doc so khach hang hien co */
    int n = getNumKH();

    /*  Mo file KH.BIN de ghi va kiem tra tinh trang */
    ofstream wKH("KH.BIN", ios::binary | ios_base::app);
    if(!wKH)
    {
        cout << "Khong mo duoc file!" << endl;
        return;
    }

    std::cout << "Ban ghi thu " << ++n << ": \n";

    /*  Khach hang can them */
    Customer newOne;

    /*  Neu finder >= 0: Them khach hang co ma = finder
        Them moi hoan toan: Nhap ma khach hang */
    if (finder < 0)
    {
        std::cout << "Nhap ma khach hang (VD:123456):\n";
        do
        {
            newOne.cCode = validCode();
        }
        while (!isUniqueCode(newOne.cCode, -1));
    }
    /*  Da co ma khach hang */
    else
    {
        newOne.cCode = finder;
        std::cout << "Ma khach hang: ";
        std::cout << setw(6) << std::setfill('0') << newOne.cCode << endl;
    }

    std::cout << "Nhap ten khach hang (VD: Nguyen Van A):\n";
    validName(newOne.cName);

    std::cout << "Nhap dia chi khach hang (VD: Ha Noi):\n";
    getAddress(newOne.cAdd);

    /*  Vi ma khach hang va ma cong to cua moi khach hang deu la duy nhat
        Nen su dung luon ma khach hang trung ma cong to cho thuan tien */
    newOne.cMeter = newOne.cCode;
    std::cout << "Ma cong to: ";
    std::cout << setw(6) << std::setfill('0') << newOne.cMeter << endl << endl;

    /*  Kiem tra tinh trang ghi file */
    if (!wKH.good())
    {
        std::cout << "Bi loi trong qua trinh ghi file!\n";
        return;
    }

    /*  Ghi vao cuoi file KH.BIN */
    wKH.write((char *) &newOne, sizeof(Customer));

    /*  Dong file */
    wKH.close();

    /*  Neu la them moi hoan toan, kiem tra co muon tiep tuc nhap
        Neu la them bo sung, quay lai viec truoc do */
    if (finder < 0)
    {
        std::cout << "Ban co muon nhap them khong? (y/n)\n";
        char control{};
        while (1)
        {
            control = getch();
            fflush(stdin);

            if (control == 'y' || control == 'Y')
            {
                system("cls");
                appendKH(-1);
                break;
            }
            else if (control == 'n' || control == 'N')
            {
                std::cout << "n\n";
                break;
            }
        }
    }
}

/*  Cap nhat thong tin khach hang */
void updateKH()
{
    int numKH = 0;

    /*  Mo file KH.BIN de doc */
    ifstream rKH("KH.BIN", ios::binary);

    /*  Bien luu du lieu khach hang */
    Customer data;

    /*  Nhap ma khach hang muon thay doi */
    std::cout << "Nhap ma khach hang can thay doi:\n";
    int finder = validCode();

    ofstream uKH("uKH.BIN", ios::binary);

    /*  Bien kiem tra co ton tai ma khach hang can thay doi
        Trong ban ghi ban dau khong */
    int isExist{0};

    while (rKH.read((char *) &data, sizeof(Customer)))
    {
        numKH++;
        if (data.cCode != finder)
        {
            uKH.write((char *) &data, sizeof(Customer));
        }
        else
        {
            /*  Ton tai ma can thay doi */
            isExist = 1;

            /*  Hien ra ban ghi cu */
            std::cout << "Cap nhat ban ghi so " << numKH << ":\n";
            std::cout << "Ma khach hang: " << data.cCode << endl;
            std::cout << "Ten khach hang: " << data.cName << endl;
            std::cout << "Dia chi khach hang: " << data.cAdd << endl;
            std::cout << "Ma cong to khach hang: " << data.cMeter << endl;
            std::cout << endl;

            /*  Cap nhat gia tri moi */
            std::cout << "Cap nhat ma khach hang: \n";
            do
            {
                data.cCode = validCode();
            }
            while (!isUniqueCode(data.cCode, finder));

            std::cout << "Cap nhat ten khach hang: \n";
            validName(data.cName);

            std::cout << "Cap nhat dia chi khach hang: \n";
            getAddress(data.cAdd);

            data.cMeter = data.cCode;
            std::cout << "Ma cong to khach hang: " << data.cMeter << endl << endl;

            /*  Viet ban ghi da cap nhat vao file temp */
            uKH.write((char *) &data, sizeof(Customer));

            std::cout << "Da cap nhat ban ghi!\n";
        }
    }
    uKH.close();

    remove("KH.BIN");
    rename("uKH.BIN", "KH.BIN");

    if (numKH == 0)
    {
        std::cout << "Khong ton tai ban ghi nao de cap nhat!\n";
        return;
    }

    if (!isExist)
    {
        std::cout << "Khong ton tai ma khach hang can cap nhat!\n";
        std::cout << "Ban co muon them khach hang nay? (y/n)\n";

        char confirm{};
        while (1)
        {
            confirm = getch();

            if (confirm == 'y' || confirm == 'Y')
            {
                std::cout << "y" << endl << endl;
                /*  Them nguoi dung co ma khach hang = finder */
                appendKH(finder);
                break;
            }
            else if (confirm == 'n' || confirm == 'N')
            {
                std::cout << "n";
                break;
            }
        }
    }
}

/*  Cac hanh dong lien quan den xoa tren file KH.BIN */
void deleteKH()
{
    std::cout << "Cac tuy chon xoa:\n"
              "1. Xoa toan bo file.\n"
              "2. Xoa theo so ban ghi (STT).\n"
              "3. Xoa theo ma khach hang.\n";
    char control{};
    while (1)
    {
        control = getch();
        if (control == '1' || control == '2' || control == '3')
        {
            break;
        }
    }
    system("cls");
    switch (control)
    {
    case '1':
        /*  Xac nhan xoa toan bo */
        confirmDel("KH.BIN");
        break;
    case '2':
        deleteViaRecNum();
        break;
    case '3':
        deleteViaCode();
        break;
    }
}

/*  Xoa dua tren STT */
void deleteViaRecNum()
{
    int n = getNumKH();

    if (n == 0)
    {
        std::cout << "Khong ton tai ban ghi nao de xoa!\n";
        return;
    }

    std::cout << "Ban muon xoa ban ghi so:\n";
    int delNum{};
    std::cin >> delNum;
    if (delNum > n)
    {
        std::cout << "Khong hop le!\n"
                  "STT muon xoa lon hon so ban ghi hien co (" << delNum << " > " << n << ").\n";
    }
    else
    {
        ifstream rKH("KH.BIN", ios::binary);
        ofstream dKH("dKH.BIN", ios::binary);

        Customer data;

        int numKH{0};

        while (rKH.read((char *) &data, sizeof(Customer)))
        {
            numKH++;
            if (delNum != numKH)
            {
                dKH.write((char *) &data, sizeof(Customer));
            }
            else
            {
                /*  Hien thi bna ghi can xoa
                    Xoa = khong ghi vao file cap nhat */
                std::cout << "Ban ghi so " << numKH << ":\n";
                std::cout << "Ma khach hang: " << data.cCode << endl;
                std::cout << "Ten khach hang: " << data.cName << endl;
                std::cout << "Dia chi khach hang: " << data.cAdd << endl;
                std::cout << "Ma cong to khach hang: " << data.cMeter << endl << endl;
                std::cout << "Da xoa ban ghi!" << endl;
            }
        }

        rKH.close();
        dKH.close();
        remove("KH.BIN");
        rename("dKH.BIN", "KH.BIN");
    }
}

/*  Xoa dua vao ma khach hang */
void deleteViaCode()
{
    int n = getNumKH();

    if (n == 0)
    {
        std::cout << "Khong ton tai ban ghi nao de xoa!\n";
        return;
    }

    std::cout << "Nhap ma khach hang can xoa:\n";
    int deleter{};
    std::cin >> deleter;

    Customer data;
    int numKH{};
    int isExist{};

    ifstream rKH("KH.BIN", ios::binary);
    ofstream dKH("dKH.BIN", ios::binary);

    while (rKH.read((char *) &data, sizeof(Customer)))
    {
        numKH++;
        if (data.cCode != deleter)
        {
            dKH.write((char *) &data, sizeof(Customer));
        }
        else
        {
            isExist = 1;
            std::cout << "\nXoa ban ghi so " << numKH << ":\n";
            std::cout << "Ma khach hang: " << data.cCode << endl;
            std::cout << "Ten khach hang: " << data.cName << endl;
            std::cout << "Dia chi khach hang: " << data.cAdd << endl;
            std::cout << "Ma cong to khach hang: " << data.cMeter << endl << endl;
            std::cout << "Da xoa ban ghi!" << endl;
        }
    }

    rKH.close();
    dKH.close();
    remove("KH.BIN");
    rename("dKH.BIN", "KH.BIN");

    if (!isExist)
    {
        std::cout << "Khong ton tai ma khach hang can xoa!\n";
    }
}
