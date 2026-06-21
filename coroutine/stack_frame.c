int callee() {
  int x = 0;
  return x;
}

int caller() {
  callee();
  return 0;
}

int main(int argc, char *argv[]) { caller(); }
