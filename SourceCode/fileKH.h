#ifndef FILEKH_H_INCLUDED
#define FILEKH_H_INCLUDED

#define MAX_LEN 150

/*  Cau truc khach hang cua moi ban ghi thuoc file KH.BIN */
struct Customer
{
    int cCode{};
    char cName[MAX_LEN] = {};
    char cAdd[MAX_LEN] = {};
    int cMeter{};
};

/*  Doc so khach hang trong file KH.BIN */
int getNumKH();

/*  Dieu huong lam viec voi file KH.BIN */
void KHThings();

/*  Doc file KH.BIN */
void readKH();

/*  Them khach hang */
void appendKH(int finder);
/*  Chuan hoa ma khach hang/ma cong to */
int validCode();
/*  Kiem tra ma khach hang co la duy nhat */
bool isUniqueCode(int cCode, int kt);
/*  Chuan hoa ten khach hang */
void validName(char *name);
/*  Nhap dia chi */
void getAddress(char *cAdd);

/*  Cap nhat thong tin khach hang */
void updateKH();

/*  Cac hanh dong lien quan den xoa tren file KH.BIN */
void deleteKH();
/*  Xoa dua tren STT */
void deleteViaRecNum();
/*  Xoa dua vao ma khach hang */
void deleteViaCode();

#endif // FILEKH_H_INCLUDED
