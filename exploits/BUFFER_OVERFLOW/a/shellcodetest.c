/*shellcodetest.c*/ 
char code[] = "bytecode will go here!";
int main(int argc, char **argv)
{
	int (*func)();
	func = (int (*)()) code;
	(int)(*func)();
}
