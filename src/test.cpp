//https://stackoverflow.com/questions/45946033/c-reading-a-character-is-incorrect

#include<cstdio>

int main() {

    char ch = static_cast<char>(0x92);
    printf("%c - %x/%x\n", ch,ch,static_cast<unsigned char>(ch));
    return 0;
}
