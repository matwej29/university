int main(){
  int a = 15;
  int b = 40;

  int c = gcd(a, b);
}

int gcd(int a, int b){
  while (a > 0 && b > 0){
    if (a - b > 0){
      a = a - b;
    } else {
      b = b - a;
    }
  }
  return a + b;
}