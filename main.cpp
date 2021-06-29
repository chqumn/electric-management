#include <iostream>
#include <fstream>
#include <cstdlib>
#include <conio.h>
#include <string.h>
#include <cstring>

#include "main.h"
#include "fileKH.h"
#include "fileCSDIEN.h"
#include "fileGIADIEN.h"
#include "fileHOADON.h"
#include "otherWork.h"

using namespace std;

/*  Dieu huong cac hoat dong chinh */
void flowControl()
{
    std::cout << "Nhap thao tac ban muon thuc hien:\n"
              "1. Lam viec voi file.\n"
              "2. Tinh dien nang tieu thu.\n"
              "3. In hoa don.\n"
              "4. Thoat chuong trinh.\n";

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

    switch (control)
    {
    case '1':
        workWithFile();
        isContinue();
        break;
    case '2':
        system("cls");
        createBill();
        isContinue();
        break;
    case '3':
        system("cls");
        printBill();
        isContinue();
        break;
    case '4':
        std::cout << "\nChuong trinh ket thuc!";
        break;
    }
}

/*  Lua chon lam viec voi file */
void workWithFile()
{
    system("cls");
    std::cout << "Chon file ban muon lam viec:\n"
              "1. KH.BIN\n"
              "2. CSDIEN.BIN\n"
              "3. GIADIEN.BIN\n";

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
        KHThings();
        break;
    case '2':
        CSDIENThings();
        break;
    case '3':
        GIADIENThings();
        break;
    }
}

/*  Sau moi hanh dong kiem tra nguoi dung co muon lam tiep khong */
void isContinue()
{
    std::cout << endl << "Ban co muon thuc hien thao tac khac? (y/n)\n";
    char control{};
    while (1)
    {

        control = getch();
        fflush(stdin);

        if (control == 'y' || control == 'Y')
        {
            system("cls");
            flowControl();
            break;
        }
        else if (control == 'n' || control == 'N')
        {
            std::cout << "n\n";
            std::cout << "Chuong trinh ket thuc!";
            break;
        }
    }
}

int main()
{
    /*  Bat dau bang lenh dieu huong chinh
        Tuy theo yeu cau nhap vao tu nguoi dung
        Ma goi den cac ham khac nhau */
    flowControl();

    return 0;
}

/*  Xac nhan xoa file *fileName */
bool confirmDel(std::string fileName)
{
    char confirm{};
    std::cout << "Ban co chac chan muon xoa file " << fileName << "? (y/n)\n";
    while (1)
    {
        confirm = getch();

        if (confirm == 'y' || confirm == 'Y')
        {
            std::cout << "y\n";
            remove(fileName.c_str());
            std::cout << "Da xoa file!\n";
            return true;
        }
        else if (confirm == 'n' || confirm == 'N')
        {
            std::cout << "n\n";
            std::cout << "Da huy bo thao tac xoa file!\n";
            return false;
        }
    }
    return false;
}
