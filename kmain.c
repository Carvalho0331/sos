void kmain(void)
{
    char *video = (char*) 0xB8000;

    char *msg1 = "Hi Mrs Salimo";
    char *msg2 = "\nWelcome to OS";

    int i = 0;

    // primeira mensagem
    for (i = 0; msg1[i] != '\0'; i++)
    {
        video[i * 2] = msg1[i];
        video[i * 2 + 1] = 0x07;
    }

    int offset = i; // continua depois da primeira linha

    // segunda mensagem
    for (i = 0; msg2[i] != '\0'; i++)
    {
        video[(offset + i) * 2] = msg2[i];
        video[(offset + i) * 2 + 1] = 0x07;
    }
}
