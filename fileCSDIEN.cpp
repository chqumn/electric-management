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

int isAppend{1};

/*  Nhap ky (1 - 12) */
int getPeriod()
{
    char temp[2];
    char decimal{};
    int codeLen{0};
    int period{};

    do
    {
        decimal = getch();

        if (decimal >= '0' && decimal <= '9' && codeLen < 2)
        {
            temp[codeLen++] = decimal;
            std::cout << decimal;
        }
        if (decimal == '\b' && codeLen > 0)
        {
            temp[--codeLen] = 0;
            cout << "\b \b";
        }

        period = atoi(temp);
    }
    while (decimal != 13 || period > 12 || period < 1);

    return atoi(temp);
}

/*  Nhap ngay (cho phep tu 10 - 20)
    Neu bo trong thi mac dinh la 20 */
int getDay()
{
    char temp[2];
    char decimal{};
    int codeLen{0};
    int day{};

    do
    {
        decimal = getch();

        if (decimal >= '0' && decimal <= '9' && codeLen < 2)
        {
            temp[codeLen++] = decimal;
            std::cout << decimal;
        }
        if (decimal == '\b' && codeLen > 0)
        {
            temp[--codeLen] = 0;
            cout << "\b \b";
        }

        day = atoi(temp);

        if (decimal == 13 && codeLen == 0)
        {
            std::cout << 20;
            return 20;
        }

    }
    while (decimal != 13 || day < 10 || day > 20);

    return atoi(temp);
}

/*  Nhap mot gia tri nguyen duong bat ky */
int getInt()
{
    char temp[11] = "          ";
    char decimal{};
    int codeLen{0};
    do
    {
        decimal = getch();

        if (decimal >= '0' && decimal <= '9')
        {
            temp[codeLen++] = decimal;
            std::cout << decimal;
        }
        if (decimal == '\b' && codeLen > 0)
        {
            temp[--codeLen] = 0;
            cout << "\b \b";
        }
    }
    while (decimal != 13);

    return atoi(temp);
}

/*  Viet mot ky vao file CSDIEN.BIN
    Dua vao ma khach hang cCode
    Va so ki index + 1 */
void writePeriod(int cCode, int index)
{
    fflush(stdin);
    ifstream rCS("CSDIEN.BIN", ios::binary);

    ElecStat singleRecord;
    ElecStat record[12];

    ofstream wCS("tempCSDIEN.BIN", ios::binary);

    while (rCS.read((char *) &singleRecord, sizeof(ElecStat)))
    {
        /*  Chi doc chu khong ghi.
            Vi da ghi truoc do tai ham appendAllPeriod.
            Hoac ham appendSelectivePeriod. */
        if (singleRecord.cCode == cCode)
        {
            record[0].cCode = singleRecord.cCode;
            record[0].eDay = singleRecord.eDay;
            record[0].eStat = singleRecord.eStat;

            /*  Cac ban ghi di theo goi 12 ban.
                Nen khi tim duoc mot ban ghi co ma khach hang trung voi ma can them.
                Can doc not 11 ban phia sau. */
            for (int i = 1; i < 12; i++)
            {
                rCS.read((char *) &record[i], sizeof(ElecStat));
            }
            break;
        }
        else
        {
            wCS.write((char *) &singleRecord, sizeof(ElecStat));
        }
    }

    system("cls");
    std::cout << "Ma khach hang: ";
    std::cout << setw(6) << std::setfill('0') << cCode << endl << endl;
    std::cout <<"+---------------------------------------+\n";
    std::cout << "| Ky\t| Ngay chot\t| Chi so dien\t|\n";
    std::cout <<"|---------------------------------------|\n";
    for (int i = 0; i < index; i++)
    {
        std::cout << "| ";
        std::cout << setw(2) << std::setfill(' ') << i + 1 << "\t| ";
        std::cout << record[i].eDay << "\t\t| ";
        std::cout << setw(14) << std::setfill(' ') << record[i].eStat << "|\n";
    }

    std::cout << "| ";
    std::cout << setw(2) << std::setfill(' ') << index + 1 << "\t| ";
    record[index].eDay = getDay();
    std::cout << "\t\t| ";
    record[index].eStat = getInt();
    std::cout << "\t\t|\n";

    system("cls");
    std::cout << "Ma khach hang: ";
    std::cout << setw(6) << std::setfill('0') << cCode << endl << endl;
    std::cout <<"+---------------------------------------+\n";
    std::cout << "| Ky\t| Ngay chot\t| Chi so dien\t|\n";
    std::cout <<"|---------------------------------------|\n";
    for (int i = 0; i < 12; i++)
    {
        if (record[i].eStat != -1)
        {
            std::cout << "| ";
            std::cout << setw(2) << std::setfill(' ') << i + 1 << "\t| ";
            if (i == index)
            {
                std::cout << record[i].eDay << "*\t\t| ";
                std::cout << setw(13) << std::setfill(' ') << record[i].eStat << "*|\n";
            }
            else
            {
                std::cout << record[i].eDay << "\t\t| ";
                std::cout << setw(14) << std::setfill(' ') << record[i].eStat << "|\n";
            }
        }
    }
    std::cout <<"+---------------------------------------+\n";

    char control{};

    if (index != 0 && record[index].eStat < record[index - 1].eStat)
    {
        std::cout << "Khong hop le!\n"
                  "Chi so dien ki truoc (" << record[index - 1].eStat
                  << ") lon hon chi so dien ki nay (" << record[index].eStat << ")!\n";
        rCS.close();
        std::cout << "Yeu cau nhap lai ky " << index + 1 << "!\n"
                  << "An phim bat ky de tiep tuc!";
        getch();
        writePeriod(cCode, index);
    }
    else
    {
        if (index < 11 && record[index + 1].eStat != -1 && record[index].eStat > record[index + 1].eStat)
        {
            std::cout << "Khong hop le!\n"
                      "Chi so dien ki nay (" << record[index].eStat
                      << ") lon hon chi so dien ki sau (" << record[index + 1].eStat << ")!\n";
            rCS.close();

            std::cout << "Yeu cau:\n";
            std::cout << "1. Nhap lai ky  nay (ky " << index + 1 << ")!\n"
                      << "2. Sua ki " << index + 2 << "!";

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
                writePeriod(cCode, index);
                return;
            case '2':
                std::cout << 2 << endl;
            }
        }
        /*  Ghi cac ban ghi cua ma khach hang da cho.
        Trong do co ban ghi da cap nhat. */
        for (int i = 0; i < 12; i++)
        {
            wCS.write((char *) &record[i], sizeof(ElecStat));
        }

        /*  Ghi phan con lai. */
        while (rCS.read((char *) &singleRecord, sizeof(ElecStat)))
        {
            wCS.write((char *) &singleRecord, sizeof(ElecStat));
        }
    }

    rCS.close();
    wCS.close();

    remove("CSDIEN.BIN");
    rename("tempCSDIEN.BIN", "CSDIEN.BIN");

    if (control == '2')
    {
        writePeriod(cCode, index + 1);
    }
}

/*  Dieu huong hoat dong voi file CSDIEN.BIN */
void CSDIENThings()
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
        readCSDIEN();
        break;
    case '2':
        appendCSDIEN();
        break;
    case '3':
        updateCSDIEN(-1, -1);
        break;
    case '4':
        deleteCSDIEN();
        break;
    }
}

/*  Doc file CSDIEN.BIN */
void readCSDIEN()
{
    ifstream rCS("CSDIEN.BIN", ios::binary);

    ElecStat record;
    int i = 1;

    std::cout << "| Ma khach hang\t| Ky\t| Ngay chot\t| Chi so dien\t|\n";

    while (rCS.read((char *) &record, sizeof(ElecStat)))
    {
        if (i == 1)
        {
            std::cout <<"|-------------------------------------------------------|\n";
        }

        if (record.eStat != -1)
        {
            std::cout << "| ";
            std::cout << setw(6) << std::setfill('0') << record.cCode <<  "\t| ";
            std::cout << setw(2) << std::setfill(' ') << i << "\t| ";
            std::cout << record.eDay << "\t\t| ";
            std::cout << setw(14) << std::setfill(' ') << record.eStat << "|\n";
        }
        i = i%12;
        i++;
    }
    std::cout << "+-------------------------------------------------------+\n";
    std::cout << "Da doc het file!\n";
}

/*  Them vao file CSDIEN */
void appendCSDIEN()
{
    std::cout << "Nhap ma khach hang muon them ban ghi:\n";
    int cCode = validCode();

    ifstream rCountKH("countKH.txt");
    int n{};
    rCountKH >> n;
    rCountKH.close();

    ifstream rKH("KH.BIN", ios::binary);

    int isExistCCode{0};

    Customer *data = new Customer[n];
    for (int i = 0; i < n; i++)
    {
        rKH.read((char *) &data[i], sizeof(Customer));
        if (data[i].cCode == cCode)
        {
            isExistCCode = 1;
            break;
        }
    }
    rKH.close();

    if (!isExistCCode)
    {
        std::cout << "Khong ton tai ma khach hang!\n";
        std::cout << "Ban co muon them khach hang nay? (y/n)\n";

        char confirm{};
        while (1)
        {
            confirm = getch();

            if (confirm == 'y' || confirm == 'Y')
            {
                std::cout << endl;
                appendKH(cCode);
                break;
            }
            else if (confirm == 'n' || confirm == 'N')
            {
                std::cout << "n\n";
                return;
            }
        }
    }

    ElecStat record;
    int isExistRecord{0};

    ifstream rCS("CSDIEN.BIN", ios::binary);
    while (rCS.read((char *) &record, sizeof(ElecStat)))
    {
        if (record.cCode == cCode)
        {
            isExistRecord = 1;
            break;
        }
    }
    rCS.close();

    if (!isExistRecord)
    {
        appendEmptyCSDIEN(cCode);
    }

    std::cout << "Ban muon lam gi?\n"
              "1. Them toan bo cac ky.\n"
              "2. Them mot ky.\n"
              "3. Co le de sau.\n";
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
        appendAllPeriod(cCode);
        break;
    case '2':
        isAppend = 1;
        appendSelectivePeriod(cCode, -1);
        break;
    case '3':
        break;
    }
}

/*  Them cac ban ghi trong cua ma khach hang */
void appendEmptyCSDIEN(int cCode)
{
    ofstream wCS("CSDIEN.BIN", ios::binary | ios_base::app);

    ElecStat data[12];

    for (int i = 0; i < 12; i++)
    {
        data[i].cCode = cCode;
        wCS.write((char *) &data[i], sizeof(ElecStat));
    }

    wCS.close();
}

/*  Them tat ca cac ban ghi cua ma khach hang */
void appendAllPeriod(int cCode)
{
    ifstream rCS("CSDIEN.BIN", ios::binary);
    ofstream wCS("tempCSDIEN.BIN", ios::binary |ios_base::app);

    ElecStat singleRecord;
    ElecStat record[12];

    while (rCS.read((char *) &singleRecord, sizeof(ElecStat)))
    {
        if (singleRecord.cCode == cCode)
        {
            /*  Cac ban ghi di theo goi 12 ban.
                Nen khi tim duoc mot ban ghi co ma khach hang trung voi ma can them.
                Can doc not 11 ban phia sau. */
            for (int i = 1; i < 12; i++)
            {
                rCS.read((char *) &record[i], sizeof(ElecStat));
            }
            break;
        }
        else
        {
            wCS.write((char *) &singleRecord, sizeof(ElecStat));
        }
    }

    /*  Dong hai file
        Vi du sao cung tiep tuc doc ghi tai ham writePeriod */
    rCS.close();
    wCS.close();

    /*  Sau khi tim duoc vi tri tham ban ghi.
        Ghi 12 ban ghi can them. */
    if (record[11].eStat != -1)
    {
        std::cout << "Tat ca cac ban ghi cua ma khach hang " << cCode << " da duoc them.\n";
    }
    else if (singleRecord.eStat != -1)
    {
        std::cout << "Da ton tai mot vai ban ghi cua ma khach hang " << cCode << endl;
        std::cout << "Ban muon lam gi?\n"
                  "1. Ghi de cac ban ghi cu.\n"
                  "2. Tiep tuc ghi tu phan con thieu.\n"
                  "3. Co le de sau.\n";
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
            for (int i = 0; i < 12; i++)
            {
                writePeriod(cCode, i);
            }
            break;
        case '2':
            for (int i = 1; i < 12; i++)
            {
                if (record[i].eStat == -1)
                {
                    writePeriod(cCode, i);
                }
            }
            break;
        case '3':
            break;
        }
    }
    else
    {
        for (int i = 0; i < 12; i++)
        {
            writePeriod(cCode, i);
        }
    }
}

/*  Chon ky de them ban ghi*/
void appendSelectivePeriod(int cCode, int index)
{
    ifstream rCS("CSDIEN.BIN", ios::binary);
    ofstream wCS("tempCSDIEN.BIN", ios::binary);

    ElecStat singleRecord;
    ElecStat record[12];

    while (rCS.read((char *) &singleRecord, sizeof(ElecStat)))
    {
        if (singleRecord.cCode == cCode)
        {
            record[0].cCode = singleRecord.cCode;
            record[0].eDay = singleRecord.eDay;
            record[0].eStat = singleRecord.eStat;
            /*  Cac ban ghi di theo goi 12 ban.
                Nen khi tim duoc mot ban ghi co ma khach hang trung voi ma can them.
                Can doc not 11 ban phia sau. */
            for (int i = 1; i < 12; i++)
            {
                rCS.read((char *) &record[i], sizeof(ElecStat));
            }
            break;
        }
        else
        {
            wCS.write((char *) &singleRecord, sizeof(ElecStat));
        }
    }

    rCS.close();
    wCS.close();

    if (index < 0)
    {
        std::cout << "Nhap ky muon them:\n";
        index = getPeriod() - 1;
        std::cout << endl;
    }
    else
    {
        std::cout << "Ky muon them: " << index + 1 << ".\n";
    }

    if (record[index - 1].eStat == -1)
    {
        std::cout << "Loi: Chua co ban ghi ky " << index << ".\n"
                  "Yeu cau nhap ky " << index << " truoc:\n"
                  "1. Ok!\n"
                  "2. Toi se quay lai sau.\n";

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
            isAppend = 1;
            appendSelectivePeriod(cCode, index - 1);
            break;
        case '2':
            isAppend = 0;
            return;
            break;
        }
    }

    if (isAppend)
    {
        writePeriod(cCode, index);
    }

    return;
}

/*  Cap nhat file CSDIEN */
void updateCSDIEN(int cCode, int index)
{
    if (cCode < 0 )
    {
        std::cout << "Nhap ma khach hang muon cap nhat ban ghi:\n";
        cCode = validCode();
    }
    else
    {
        std::cout << "Ma khach hang: " << cCode << ".\n";
    }

    ElecStat singleRecord;
    ElecStat record[12];

    ifstream rCS("CSDIEN.BIN", ios::binary);
    while (rCS.read((char *) &singleRecord, sizeof(ElecStat)))
    {
        if (singleRecord.cCode == cCode)
        {
            record[0].cCode = singleRecord.cCode;
            record[0].eDay = singleRecord.eDay;
            record[0].eStat = singleRecord.eStat;
            for (int i = 1; i < 12; i++)
            {
                rCS.read((char *) &record[i], sizeof(ElecStat));
            }
            break;
        }
    }
    rCS.close();

    if (record[0].eStat == -1)
    {
        std::cout << "Ma khach hang " << cCode << " chua co ban ghi nao de cap nhat!\n";
        std::cout << "Ket thuc cap nhat!\n";
        return;
    }

    std::cout << "Nhap ky muon cap nhat: ";
    index = getPeriod() - 1;
    std::cout << endl;

    if (record[index].eStat == -1)
    {
        std::cout << "Ky " << index + 1 << " chua co ban ghi de cap nhat.\n"
                  "Them ban ghi ki nay? (y/n)\n";
        char confirm{};

        while (1)
        {
            confirm = getch();

            if (confirm == 'y' || confirm == 'Y')
            {
                std::cout << "y\n";
                isAppend = 1;
                appendSelectivePeriod(cCode, index);
                break;
            }
            else if (confirm == 'n' || confirm == 'N')
            {
                std::cout << "n\n";
                std::cout << "Ket thuc cap nhat!\n";
            }
            break;
        }
    }
    updatePeriod(cCode, index);
}

/*  Cap nhat ban ghi cua ma khach hang cCode
    Ky index + 1 */
void updatePeriod(int cCode, int index)
{
    ifstream rCS("CSDIEN.BIN", ios::binary);
    ofstream wCS("tempCSDIEN.BIN", ios::binary);

    ElecStat singleRecord;
    ElecStat record[12];

    while (rCS.read((char *) &singleRecord, sizeof(ElecStat)))
    {
        if (singleRecord.cCode == cCode)
        {
            /*  Cac ban ghi di theo goi 12 ban.
                Nen khi tim duoc mot ban ghi co ma khach hang trung voi ma can them.
                Can doc not 11 ban phia sau. */
            for (int i = 1; i < 12; i++)
            {
                rCS.read((char *) &record[i], sizeof(ElecStat));
            }
            break;
        }
        else
        {
            wCS.write((char *) &singleRecord, sizeof(ElecStat));
        }
    }

    rCS.close();
    wCS.close();

    if (index < 0)
    {
        std::cout << "Nhap ky muon cap nhat:\n";
        index = getPeriod() - 1;
        std::cout << endl;
    }
    else
    {
        std::cout << "Ky muon cap nhat: " << index + 1 << ".\n";
    }

    writePeriod(cCode, index);

    return;
}

/*  Lam viec lien quan den xoa file CSDIEN */
void deleteCSDIEN()
{
    std::cout << "Lua chon kieu xoa?\n"
              "1. Xoa toan bo file.\n"
              "2. Xoa theo ma khach hang.\n"
              "3. Xoa ban ghi co ma khach hang khong thuoc file KH.\n";
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
        if (confirmDel("CSDIEN.BIN"))
        {
        }
        break;
    case '2':
        deleteRecViaCode();
        break;
    case '3':
        deleteGhostRecord();
        break;
    }
}

/*  Xoa toan bo cac ban ghi cua ma khach hang */
void deleteRecViaCode()
{
    std::cout << "Nhap ma khach hang ban muon xoa ban ghi:\n";
    int cCode = validCode();

    ifstream rCS("CSDIEN.BIN", ios::binary);
    ofstream wCS("tempCSDIEN.BIN", ios::binary);

    ElecStat record;
    int isExist{0};

    while (rCS.read((char *) &record, sizeof(ElecStat)))
    {
        if (record.cCode != cCode)
        {
            wCS.write((char *) &record, sizeof(ElecStat));
        }
        else
        {
            isExist = 1;
        }
    }

    rCS.close();
    wCS.close();

    remove("CSDIEN.BIN");
    rename ("tempCSDIEN.BIN", "CSDIEN.BIN");

    if (!isExist)
    {
        std::cout << "Khong co ban ghi nao co ma khach hang nhu tren!\n"
                  "Ket thuc xoa ban ghi!\n";
    }
    else
    {
        std::cout << "Da xoa ban ghi cua ma khach hang " << cCode << "!\n";
    }
}

/*  Xoa cac ban ghi ma (ma khach hang khong ton tai trong file KH.BIN) */
void deleteGhostRecord()
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
    ofstream wCS("tempCSDIEN.BIN", ios::binary);

    ElecStat record;
    int isExist{0};

    while (rCS.read((char *) &record, sizeof(ElecStat)))
    {
        isExist = 0;
        for (int i = 0; i < n; i++)
        {
            if (record.cCode == cusData[i])
            {
                wCS.write((char *) &record, sizeof(ElecStat));
                isExist = 1;
                break;
            }
        }
        if (isExist)
        {
            for (int i = 0; i < 11; i++)
            {
                rCS.read((char *) &record, sizeof(ElecStat));
                wCS.write((char *) &record, sizeof(ElecStat));
            }
        }
        else
        {
            std::cout << "Xoa ban ghi co ma khach hang " << record.cCode << ".\n";
            for (int i = 0; i < 11; i++)
            {
                rCS.read((char *) &record, sizeof(ElecStat));
            }
        }
    }

    rCS.close();
    wCS.close();

    remove("CSDIEN.BIN");
    rename ("tempCSDIEN.BIN", "CSDIEN.BIN");
}
