void allocMat(double **mat, int r, int c);
void allocVec(double **vec, int l);
void assignMat(double *mat, int r, int c);
void assignVec(double *vec, int l);
double *mvp(double *mat, double *vec, int r, int c);
void printMatVec(double *mat, double *vec, double *res, int r, int c);