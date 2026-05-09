```markdown
# 🧠 Mini Operating System (x86)

![GitHub repo size](https://img.shields.io/github/repo-size/Carvalho0331/sos?style=flat-square)
![GitHub last commit](https://img.shields.io/github/last-commit/Carvalho0331/sos?style=flat-square)
![License](https://img.shields.io/badge/license-MIT-blue?style=flat-square)

> Um kernel minimalista desenvolvido do zero em **C** e **Assembly**, com foco na compreensão dos fundamentos de sistemas operativos e interacção directa com o hardware x86.

---

## 🎯 Objectivo

Construir um kernel **freestanding** (sem libc) que execute directamente sobre o hardware, explorando:

- ✔️ Bootloader Multiboot
- ✔️ Escrita na memória de vídeo (framebuffer)
- ✔️ Controlo do cursor por I/O ports
- ✔️ Logging via porta série (COM1)
- ✔️ Segmentação (GDT)
- ✔️ Interrupções (IDT) e teclado básico

---

## 📦 Tecnologias

| Ferramenta       | Função                          |
|------------------|---------------------------------|
| **C** (freestanding) | Linguagem principal do kernel |
| **NASM**         | Assembly para boot e I/O        |
| **GCC + LD**     | Compilação e linkagem           |
| **GRUB**         | Bootloader (multiboot)          |
| **QEMU / Bochs** | Emulação e debug                |

---

## 🗂️ Estrutura do Projecto

```text
os/
├── iso/
│   └── boot/
│       └── grub/
│           └── grub.cfg          # Configuração do GRUB
├── kmain.c                        # Ponto de entrada em C
├── loader.s                       # Assembly de arranque
├── link.ld                        # Script de ligação
├── Makefile                       # Build system
├── io.s / io.h                    # I/O (outb, inb)
├── serial.c / serial.h            # Driver da porta série
├── gdt.c / gdt.h / gdt_asm.s      # GDT (segmentação)
├── idt.c / idt.h                  # IDT (interrupções)
├── interrupt.s                    # Handlers assembly
├── pic.c / pic.h                  # PIC (remapeamento + ACK)
├── keyboard.c / keyboard.h        # Teclado básico (scancodes)
└── README.md                      # Este ficheiro
```

---

## ✨ Funcionalidades Implementadas

| Módulo                     | Estado | Descrição                                                                 |
|----------------------------|--------|---------------------------------------------------------------------------|
| **Boot (Multiboot)**       | ✅      | Carrega o kernel a partir do GRUB                                         |
| **Framebuffer (0xB8000)**  | ✅      | Escrita directa na memória de vídeo (80×25 texto)                         |
| **Cursor (portas 0x3D4/5)**| ✅      | Movimento do cursor via `outb`                                            |
| **Driver de ecrã `write()`**| ✅      | Escreve strings e avança cursor                                           |
| **Porta série (COM1)**     | ✅      | Logging para ficheiro/terminal (QEMU `-serial stdio`)                     |
| **GDT**                    | ✅      | Null, code e data segments (base 0, limite 4GB, DPL=0)                    |
| **IDT**                    | ✅      | 256 entradas, handlers genéricos em assembly                              |
| **PIC**                    | ✅      | Remapeamento para 0x20-0x2F, máscaras e `pic_acknowledge`                 |
| **Teclado básico**         | ✅      | Leitura de scancodes, tradução para ASCII (sem modificadores)             |

---

## 🚀 Como Executar

```bash
make          # compila o kernel e gera a ISO
make run      # inicia o QEMU com a ISO
```

Para visualizar os **logs da porta série** (útil para depuração):

```bash
qemu-system-i386 -cdrom os.iso -serial stdio
```

Quando o kernel arrancar, verás:

```text
Kernel started with interrupts
Press any key...
```

Cada tecla premida será mostrada no ecrã e também enviada para a porta série.

---

## 🧪 Exemplo de Código (kmain.c)

```c
void kmain(void)
{
    gdt_init();                 // inicializa segmentação
    idt_init();                 // inicializa tabela de interrupções
    pic_remap();                // remapeia PIC
    pic_mask(0xFD, 0xFF);       // habilita apenas o teclado
    keyboard_init();            // prepara o teclado
    __asm__ volatile ("sti");   // activa interrupções

    serial_initialize();        // inicializa porta série
    serial_write(COM1, "Kernel started with interrupts\n");
    write("Press any key...\n", 18);

    while(1);                   // aguarda interrupções
}
```

---

## 🧠 Conceitos Fundamentais Explorados

- **Memory‑mapped I/O** – O framebuffer como área de memória dedicada.
- **I/O ports** – Comunicação com o cursor (0x3D4/0x3D5) e com o PIC.
- **GDT** – Segmentação mínima para definir níveis de privilégio.
- **IDT** – Tabela que associa números de interrupção a handlers.
- **PIC** – Controlador de interrupções hardware (remapeamento e acknowledge).
- **Teclado PS/2** – Leitura de scancodes e conversão para ASCII.

---

## 🔜 Próximos Passos (Roadmap)

- [ ] Suporte a `\n` e `\b` no `write()` (newline e backspace)
- [ ] Melhorias no teclado: Shift, Caps Lock, Enter, Backspace
- [ ] Buffer circular para teclado
- [ ] Shell simples (comandos `help`, `clear`, `echo`, `reboot`)
- [ ] Timer (PIT) para base de tempo e multitarefa
- [ ] Paging (memória virtual)
- [ ] Modo utilizador (ring 3) e syscalls

---

## 👨‍💻 Autor

**Salimo Carvalho** – [GitHub](https://github.com/Carvalho0331)  
Projecto de estudo sobre **sistemas operativos**, **arquitectura x86** e **programação de baixo nível**.

---

