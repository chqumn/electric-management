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
        ifstream rCountKH("countKH.txt");
        int n{};
        rCountKH >> n;
        rCountKH.close();

        /*  Mo file KH.BIN de kiem tra */
        ifstream rKH("KH.BIN", ios::binary);

        /*  Doc danh sach khach hang */
        Customer *data = new Customer[n];
        for (int i = 0; i < n; i++)
        {
            rKH.read((char *) &data[i], sizeof(Customer));
        }
        rKH.close();

        for (int i = 0; i < n; i ++)
        {
            /*  Neu phat hien trung lap -> return false */
            if (data[i].cCode == cCode)
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

    ofstream wCountKH("countKH.txt");
    wCountKH << n;
    wCountKH.close();
}

/*  Them khach hang */
void appendKH(int finder)
{
    /*  Doc so khach hang hien co */
    ifstream rCountKH("countKH.txt");
    int n{};
    rCountKH >> n;
    rCountKH.close();

    /*  Tang so khach hang len 1
        Ghi lai vao file countKH*/
    ofstream wCountKH("countKH.txt");
    wCountKH << ++n;
    wCountKH.close();

    /*  Mo file KH.BIN de ghi va kiem tra tinh trang */
    ofstream wKH("KH.BIN", ios::binary | ios_base::app);
    if(!wKH)
    {
        cout << "Khong mo duoc file!" << endl;
        return;
    }

    std::cout << "Ban ghi thu " << n << ": \n";

    /*  Khach hang can them */
    Customer newOne;

    /*  Them moi hoan toan: Nhap ma khach hang */
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
    ifstream rCountKH("countKH.txt");
    int n{};
    rCountKH >> n;
    rCountKH.close();

    if (n != 0)
    {
        /*  Mo file KH.BIN de doc */
        ifstream rKH("KH.BIN", ios::binary);

        /*  Mang luu du lieu khach hang */
        Customer *data = new Customer[n];
        for (int i = 0; i < n; i++)
        {
            /*  Doc tat ca du lieu da co */
            rKH.read((char *) &data[i], sizeof(Customer));
        }
        rKH.close();

        ofstream uKH("uKH.BIN", ios::binary);

        /*  Bien kiem tra co ton tai ma khach hang can thay doi
            Trong ban ghi ban dau khong */
        int isExist{0};

        /*  Nhap ma khach hang muon thay doi */
        std::cout << "Nhap ma khach hang can thay doi:\n";
        int finder = validCode();

        for (int i = 0; i < n; i++)
        {
            if (finder != data[i].cCode)
            {
                /*  Neu khac, ghi vao file temp */
                uKH.write((char *) &data[i], sizeof(Customer));
            }
            else
            {
                /*  Ton tai ma can thay doi */
                isExist = 1;

                /*  Hien ra ban ghi cu */
                std::cout << "Cap nhat ban ghi so " << n + 1 << ":\n";
                std::cout << "Ma khach hang: " << data[i].cCode << endl;
                std::cout << "Ten khach hang: " << data[i].cName << endl;
                std::cout << "Dia chi khach hang: " << data[i].cAdd << endl;
                std::cout << "Ma cong to khach hang: " << data[i].cMeter << endl;
                std::cout << endl;

                /*  Cap nhat gia tri moi */
                std::cout << "Cap nhat ma khach hang: \n";
                do
                {
                    data[i].cCode = validCode();
                }
                while (!isUniqueCode(data[i].cCode, finder));

                std::cout << "Cap nhat ten khach hang: \n";
                validName(data[i].cName);

                std::cout << "Cap nhat dia chi khach hang: \n";
                getAddress(data[i].cAdd);

                data[i].cMeter = data[i].cCode;
                std::cout << "Ma cong to khach hang: " << data[i].cMeter << endl << endl;

                /*  Viet ban ghi da cap nhat vao file temp */
                uKH.write((char *) &data[i], sizeof(Customer));

                std::cout << "Da cap nhat ban ghi!\n";
            }
        }

        uKH.close();

        remove("KH.BIN");
        rename("uKH.BIN", "KH.BIN");

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
                    std::cout << endl;
                    /*  Them nguoi dung co ma khach hang = finder */
                    appendKH(finder);
                    break;
                }
                else if (confirm == 'n' || confirm == 'N')
                {
                    break;
                }
            }
        }
    }
    else
    {
        /* n = 0: file rong */
        std::cout << "Khong ton tai ban ghi nao de cap nhat!\n";
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
        if (confirmDel("KH.BIN"))
        {
            /*  Xoa file countKH.txt di kem */
            remove("countKH.txt");
        }
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
    ifstream rCountKH("countKH.txt");
    int n{};
    rCountKH >> n;
    rCountKH.close();

    if (n != 0)
    {
        std::cout << "Ban muon xoa ban ghi so:\n";
        int recNum{};
        std::cin >> recNum;
        if (recNum > n)
        {
            std::cout << "Khong hop le!\n"
                      "STT muon xoa lon hon so ban ghi hien co (" << n << " < " << recNum << ").\n";
        }
        else
        {
            ifstream rKH("KH.BIN", ios::binary);

            Customer *data = new Customer[n];
            for (int i = 0; i < n; i++)
            {
                rKH.read((char *) &data[i], sizeof(Customer));
            }
            rKH.close();

            ofstream dKH("dKH.BIN", ios::binary);
            for (int i = 0; i < recNum - 1; i++)
            {
                dKH.write((char *) &data[i], sizeof(Customer));
            }
            for (int i = recNum; i < n; i++)
            {
                dKH.write((char *) &data[i], sizeof(Customer));
            }
            dKH.close();
            remove("KH.BIN");
            rename("dKH.BIN", "KH.BIN");

            std::cout << "Ban ghi so " << recNum << ":\n";
            std::cout << "Ma khach hang: " << data[recNum - 1].cCode << endl;
            std::cout << "Ten khach hang: " << data[recNum - 1].cName << endl;
            std::cout << "Dia chi khach hang: " << data[recNum - 1].cAdd << endl;
            std::cout << "Ma cong to khach hang: " << data[recNum - 1].cMeter << endl << endl;
            std::cout << "Da xoa ban ghi!" << endl;

            ofstream wTempCountKH("tempCountKH.txt");
            wTempCountKH << --n;
            wTempCountKH.close();

            remove("countKH.txt");
            rename("tempCountKH.txt", "countKH.txt");
        }
    }
    else
    {
        std::cout << "Khong ton tai ban ghi nao de xoa!\n";
    }
}

/*  Xoa dua vao ma khach hang */
void deleteViaCode()
{
    ifstream rCountKH("countKH.txt");
    int n{};
    rCountKH >> n;
    rCountKH.close();

    if (n != 0)
    {
        std::cout << "Nhap ma khach hang can xoa:\n";
        int deleter{};
        std::cin >> deleter;

        int location{ -1 };

        Customer *data = new Customer[n];
        ifstream rKH("KH.BIN", ios::binary);
        ofstream wDelKH("dKH.BIN", ios::binary);
        for (int i = 0; i < n; i ++)
        {
            rKH.read((char *) &data[i], sizeof(Customer));
            if (data[i].cCode != deleter)
            {
                wDelKH.write((char *) &data[i], sizeof(Customer));
            }
            else
            {
                location = i;
            }
        }

        rKH.close();
        wDelKH.close();

        if (location != -1)
        {
            ofstream wTempCountKH("wTempCountKH.txt");
            wTempCountKH << --n;
            wTempCountKH.close();

            remove("countKH.txt");
            rename("wTempCountKH.txt", "countKH.txt");

            remove("KH.BIN");
            rename("dKH.BIN", "KH.BIN");

            std::cout << "Ban ghi so " << location + 1 << ":\n";
            std::cout << "Ma khach hang: " << data[location].cCode << endl;
            std::cout << "Ten khach hang: " << data[location].cName << endl;
            std::cout << "Dia chi khach hang: " << data[location].cAdd << endl;
            std::cout << "Ma cong to khach hang: " << data[location].cMeter << endl << endl;
            std::cout << "Da xoa ban ghi!" << endl;
        }
        else
        {
            std::cout << "Khong ton tai ma khach hang can xoa!\n";
        }
    }
    else
    {
        std::cout << "Khong ton tai ban ghi nao de xoa!\n";
    }
}
