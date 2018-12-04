#define BLOCK_SIZE 200000

float in[MATRIX_SIZE];
float out[MATRIX_SIZE];

double matrixElement(int row, int column);

void multiplyMatrixBlock(int fromRow, int toRow, int fromColumn, int toColumn) {
  // Vectors out & in are marked as present on the GPU already
  #pragma acc parallel loop present(out, in)
  for (int row = fromRow; row < toRow; ++row) {
    float sum = 0.0;
    // Let the GPU parallelize this summation
    #pragma acc loop vector reduction(+:sum)
    for (int column = fromColumn; column < toColumn; ++column) {
      sum += matrixElement(row, column) * in[column];
    }
    out[row] += sum;
  }
}

void matrixMultiply() {
  // Vectors out & in are copied onto the GPU here
  #pragma acc data copy(out) copyin(in)
  for (int i = 0; i <= MATRIX_SIZE / BLOCK_SIZE; i++) {
    int startI = i * BLOCK_SIZE;
    int endI = min(MATRIX_SIZE, (i + 1) * BLOCK_SIZE);
    for (int j = 0; j <= MATRIX_SIZE / BLOCK_SIZE; j++) {
      int startJ = j * BLOCK_SIZE;
      int endJ = min(MATRIX_SIZE, (j + 1) * BLOCK_SIZE);
      multiplyMatrixBlock(startI, endI, startJ, endJ);
    }
  }
}