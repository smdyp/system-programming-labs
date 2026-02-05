#include <unistd.h>

int main() {
    char buf[10];
    int n;
    
    // Читаем из stdin и пишем в stdout
    while ((n = read(0, buf, sizeof(buf))) > 0) {
        write(1, buf, n);
    }
    
    return 0;
}