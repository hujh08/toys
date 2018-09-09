// functions to calculate id given id of sub and in sub

int fid_row(int ofsub, int insub) {
    return insub+9*ofsub;
}

int fid_col(int ofsub, int insub) {
    return ofsub+9*insub;
}

int fid_blk(int ofsub, int insub) {
    return (3*(ofsub%3)+(insub%3))+9*(3*(ofsub/3)+(insub/3));
}