#include <stdio.h>
#include <stdlib.h>

#define ARR_SIZE 0xFFFA - 0x0274 + 2
// FFFA - 0274 == пустые
int main()
{
    FILE* CrackFp = NULL;

    if ((CrackFp = fopen("crack2.bin", "wb")) == NULL)
    {
        fprintf(stderr, "Open file error\n");
        return EXIT_FAILURE;
    }
    char crack_input_data[ARR_SIZE] = {0x11};

    for (int i = 0; i < ARR_SIZE; i++)
    {
        crack_input_data[i] = 0x11;
    }

    // call address --> 0171
    crack_input_data[ARR_SIZE - 2] = 0x7B;
    crack_input_data[ARR_SIZE - 1] = 0x01;

    if (fwrite(crack_input_data, ARR_SIZE, 1, CrackFp) != 1)
    {
        fprintf(stderr, "Open file error\n");
        return EXIT_FAILURE;
    }

    fprintf(stderr, "Write success\n");

    fclose(CrackFp);

    return EXIT_SUCCESS;
}
