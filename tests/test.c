int main() {
	int x = 2;
	int y = 3;

	int z = x * y;
	int i = z * x + y;
	int j = z + x / i;

	return x + y * z / i - j;
}
