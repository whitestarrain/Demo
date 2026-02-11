#include <assert.h>

typedef char bit;
typedef bit turth_table[4][3];

#define assert_truth_table(logic)                                              \
  do {                                                                         \
    for (int i = 0; i < 4; i++) {                                              \
      assert(logic(logic##_table[i][0], logic##_table[i][1]) ==                \
             logic##_table[i][2]);                                             \
    }                                                                          \
  } while (0)

bit nand(bit x, bit y) { return !(x & y); }

bit not(bit x) { return nand(x, x); }

bit and(bit x, bit y) {
  // return not(nand(x, y));
  return nand(nand(x, y), nand(x, y));
}

bit or(bit x, bit y) {
  // return nand(not(x), not(y));
  return nand(nand(x, x), nand(y, y));
}

bit nor(bit x, bit y) {
  // return not(or(x, y));
  return nand(nand(nand(x, x), nand(y, y)), nand(nand(x, x), nand(y, y)));
}

bit xor(bit x, bit y) {
  // return and(nand(x, y), or(x, y));
  return nand(nand(x, nand(x, y)), nand(nand(x, y), y));
}

int main(int argc, char *argv[]) {
  turth_table nand_table =
                  {
                      {0, 0, 1},
                      {0, 1, 1},
                      {1, 0, 1},
                      {1, 1, 0},
                  },
              and_table =
                  {
                      {0, 0, 0},
                      {0, 1, 0},
                      {1, 0, 0},
                      {1, 1, 1},
                  },
              or_table =
                  {
                      {0, 0, 0},
                      {0, 1, 1},
                      {1, 0, 1},
                      {1, 1, 1},
                  },
              nor_table =
                  {
                      {0, 0, 1},
                      {0, 1, 0},
                      {1, 0, 0},
                      {1, 1, 0},
                  },
              xor_table = {
                  {0, 0, 0},
                  {0, 1, 1},
                  {1, 0, 1},
                  {1, 1, 0},
              };

  assert_truth_table(nand);
  assert_truth_table(and);
  assert_truth_table(or);
  assert_truth_table(nor);
  assert_truth_table(xor);
  return 0;
}
