#ifndef FILEGIADIEN_H_INCLUDED
#define FILEGIADIEN_H_INCLUDED

/*  Cau truc gia dien */
struct ElecPrice
{
    /*  Muc tieu thu */
    int eUse{-1};
    /*  Electric price - gia dien cua muc */
    int ePrc{-1};
};

/*  Dem so muc gia dien */
int getNumGIADIEN();

/*  Viet gia dien (them hoac sua) */
void writeGIADIEN(int n, int level);

/*  Dieu huong hanh dong voi file GIADIEN.BIN */
void GIADIENThings();

/*  Doc file GIADIEN.BIN */
void readGIADIEN();

/*  Them mot muc gia dien */
void appendGIADIEN();

/*  Cap nhat gia dien tai mot muc */
void updateGIADIEN();

/*  Lien quan toi xoa tren file GIADIEN.BIN */
void deleteGIADIEN();

/*  Xoa mot muc gia dien */
void deletePriceLevel();

#endif // FILEGIADIEN_H_INCLUDED
