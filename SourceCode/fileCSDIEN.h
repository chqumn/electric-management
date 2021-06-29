#ifndef FILECSDIEN_H_INCLUDED
#define FILECSDIEN_H_INCLUDED

/*  Electric stats - Chi so dien */
struct ElecStat
{
    /*  Ma khach habg */
    int cCode{-1};
    /*  Ngay chot chi so dien */
    int eDay{20};
    /*  Chi so dien */
    int eStat{-1};
};

/*  Viet mot ky vao file CSDIEN.BIN
    Dua vao ma khach hang cCode
    Va so ki index + 1 */
void writePeriod(int cCode, int index);

/*  Dieu huong hoat dong voi file CSDIEN.BIN */
void CSDIENThings();

/*  Doc file CSDIEN.BIN */
void readCSDIEN();

/*  Them vao file CSDIEN */
void appendCSDIEN();
/*  Them cac ban ghi trong cua ma khach hang */
void appendEmptyCSDIEN(int cCode);
/*  Them tat ca cac ban ghi cua ma khach hang */
void appendAllPeriod(int cCode);
/*  Chon ky de them ban ghi*/
void appendSelectivePeriod(int cCode, int index);

/*  Cap nhat file CSDIEN */
void updateCSDIEN(int cCode, int index);
/*  Cap nhat ban ghi cua ma khach hang cCode
    Ky index + 1 */
void updatePeriod(int cCode, int index);

/*  Lam viec lien quan den xoa file CSDIEN */
void deleteCSDIEN();
/*  Xoa toan bo cac ban ghi cua ma khach hang */
void deleteRecViaCode();
/*  Xoa cac ban ghi ma (ma khach hang khong ton tai trong file KH.BIN) */
void deleteGhostRecord();

/*  Nhap ky (1 - 12) */
int getPeriod();
/*  Nhap ngay (cho phep tu 10 - 20)
    Neu bo trong thi mac dinh la 20 */
int getDay();
/*  Nhap mot gia tri nguyen duong bat ky */
int getInt();


#endif // FILECSDIEN_H_INCLUDED
