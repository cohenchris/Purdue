
int myprintf(const char *, ...);

int main() {
    int n = 12, i;
    for (i = 0; i < n; ++i) {
        myprintf("%d, %d, %d\n", i, i * 25, i * -37);
    }
    myprintf("%d, %d, %d, %d\n", 0, -1, 2147483647, -2147483648);
    myprintf("%X, %X, %X, %X\n", 0, -1, 2147483647, -2147483648);
    myprintf("%d %c %s %x %s\n", 32767, '\t', "tab padding", -1168197103, " game");
    myprintf("Integer %d is %x in hexadecimal. Does it look like baseball?\n", -1168197103, -1168197103);
    myprintf("Integer %d is %x in hexadecimal. Does it look like BOLDFACE?\n", -1340212530, -1340212530);
    return 0;
}
