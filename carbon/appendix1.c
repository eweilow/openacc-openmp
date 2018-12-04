for (int row = 0; row < MATRIX_SIZE; ++row) {
  for (int column = 0; column < MATRIX_SIZE; ++column) {
    out[row] += matrixElement(row, column) * in[column];
  }
}