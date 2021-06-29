#ifndef FILEHOADON_H_INCLUDED
#define FILEHOADON_H_INCLUDED

/*  Cau truc file HOADON.BIN*/
struct Bill
{
    /*  Khong can bien gia tri ky
        Doc theo bo 12 ban <=> 12 ky */
    int cCode{-1};
    /*  Luong dien tieu thu */
    int eUsed{-1};
    /*  Tien dien */
    int eMon{-1};
    /*  Bien kiem tra ton tai */
    int isExist{0};
};

/*  Tao va hien thi hoa don kha dung */
void createBill();

/*  Money calculate */
int monCalculate(int eUsed);

#endif // FILEHOADON_H_INCLUDED
