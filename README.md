<h1 align="center">🧠 SOS — Mini Operating System</h1>

<p align="center">
  <b>Kernel x86 minimalista desenvolvido do zero em C e Assembly</b>
  <br>
  Exploração prática da arquitectura de sistemas operativos e comunicação directa com hardware.
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Architecture-x86-blue?style=for-the-badge">
  <img src="https://img.shields.io/badge/Language-C%20%7C%20Assembly-orange?style=for-the-badge">
  <img src="https://img.shields.io/badge/Environment-Freestanding-darkgreen?style=for-the-badge">
  <img src="https://img.shields.io/badge/Status-Feature--Complete-brightgreen?style=for-the-badge">
</p>

---

# 📖 Sobre o Projecto

O **SOS (Simple Operating System)** é um kernel experimental desenvolvido do zero sobre a arquitectura **x86**, sem dependência de bibliotecas padrão (*freestanding environment*).

---

# ✨ Features

## ✅ Implementado

- [x] Kernel Multiboot compatible
- [x] Boot via GRUB
- [x] VGA Text Mode (full driver with scrolling, colour, backspace)
- [x] Escrita directa na memória de vídeo
- [x] Controlo do cursor via portas I/O
- [x] Logging serial via COM1
- [x] Global Descriptor Table (GDT)
- [x] Interrupt Descriptor Table (IDT)
- [x] Interrupt handlers em Assembly
- [x] PIC Remapping
- [x] Driver básico de teclado
- [x] **PIT Timer** — IRQ0 @ 100 Hz, contador de ticks, `pit_sleep()`
- [x] **Shell interactiva** — comandos: `help`, `clear`, `ticks`, `meminfo`, `tasks`, `gui`, `reboot`
- [x] **Memory Manager** — bitmap allocator (páginas físicas) + heap first-fit (`kmalloc`/`kfree`)
- [x] **Heap Allocator** — 1 MB heap em 0x200000, coalescing de blocos livres
- [x] **Paging** — identity map 0–4 MB, `paging_enable()` disponível
- [x] **Syscalls** — INT 0x80 com `SYS_WRITE`, `SYS_EXIT`, `SYS_GETKEY`
- [x] **Multitasking** — cooperative round-robin (`task_create`, `task_yield`)
- [x] **GUI Mode** — menu gráfico em VGA text mode, navegação com setas/Enter/ESC
- [x] Build automatizada com Makefile
- [x] Execução via QEMU

---

# 🖥️ GUI — Modo Gráfico

O comando `gui` na shell lança o modo gráfico:

```
SOS> gui
```

### Controlos

| Tecla      | Acção                          |
|------------|-------------------------------|
| ↑ / ↓      | Mover selecção                |
| Enter      | Executar opção seleccionada   |
| ESC        | Voltar à shell                |

### Opções do menu

| Opção         | Descrição                                   |
|---------------|---------------------------------------------|
| System Info   | Arquitectura, endereços de memória, VGA     |
| Show Ticks    | Contador de ticks PIT e uptime aproximado   |
| About SOS     | Informação sobre o projecto                 |
| Exit GUI      | Sair do modo gráfico                        |

---

# 💻 Shell Interactiva

Após o boot aparece a prompt `SOS>`. Comandos disponíveis:

| Comando   | Descrição                                        |
|-----------|--------------------------------------------------|
| `help`    | Lista todos os comandos                          |
| `clear`   | Limpa o ecrã                                     |
| `ticks`   | Mostra o contador de ticks do PIT (IRQ0)         |
| `meminfo` | Mostra informação sobre o heap e memória física  |
| `tasks`   | Mostra estado das tarefas                        |
| `gui`     | Abre o modo gráfico                              |
| `reboot`  | Reinicia a máquina (triple fault)                |

---

# ⚙️ Arquitectura do Sistema

```text
                 +-------------------+
                 |       GRUB        |
                 |    Bootloader     |
                 +-------------------+
                           |
                           v
                 +-------------------+
                 |     loader.s      |
                 |  Multiboot ASM    |
                 +-------------------+
                           |
                           v
                 +-------------------+
                 |      kmain.c      |
                 |   Kernel Entry    |
                 +-------------------+
          |       |      |      |       |
          v       v      v      v       v
        GDT     IDT    PIT    MM    PAGING
                  |      |
                  v      v
               SYSCALL  SHELL ←── KEYBOARD
                           |
                           v
                          GUI
```

---

# 🗂️ Estrutura do Projecto

```text
.
├── Makefile                        # Build system
├── README.md                       # Documentação
├── link.ld                         # Linker script
│
├── asm/
│   ├── loader.s                    # Multiboot entry
│   ├── io.s                        # inb / outb
│   ├── interrupt.s                 # 256 ISR stubs + common handler
│   ├── gdt_asm.s                   # lgdt + segment reload
│   └── syscall_stub.s              # INT 0x80 ASM trampoline  [NEW]
│
├── include/
│   ├── gdt.h
│   ├── idt.h
│   ├── io.h
│   ├── keyboard.h
│   ├── pic.h
│   ├── serial.h
│   ├── vga.h                       # [NEW] Full VGA driver API
│   ├── pit.h                       # [NEW] PIT timer
│   ├── mm.h                        # [NEW] Memory manager + heap
│   ├── paging.h                    # [NEW] Paging
│   ├── syscall.h                   # [NEW] Syscall interface
│   ├── task.h                      # [NEW] Cooperative multitasking
│   ├── shell.h                     # [NEW] Interactive shell
│   └── gui.h                       # [NEW] GUI mode
│
├── src/
│   ├── kmain.c                     # Kernel entry (updated)
│   ├── vga.c                       # [NEW] VGA driver (scrolling, colour)
│   ├── gdt.c
│   ├── idt.c
│   ├── interrupt_handler_c.c       # Updated: PIT + shell routing
│   ├── keyboard.c
│   ├── pic.c
│   ├── serial.c
│   ├── pit.c                       # [NEW] PIT driver
│   ├── mm.c                        # [NEW] Page allocator + heap
│   ├── paging.c                    # [NEW] Identity-map paging
│   ├── syscall.c                   # [NEW] INT 0x80 handler
│   ├── task.c                      # [NEW] Cooperative scheduler
│   ├── shell.c                     # [NEW] Interactive shell
│   └── gui.c                       # [NEW] VGA GUI mode
│
└── iso/
    └── boot/
        └── grub/
            └── grub.cfg
```

---

# ⚙️ Fluxo de Inicialização

```text
GRUB
  ↓
loader.s (Multiboot)
  ↓
kmain.c
  ↓ GDT setup
  ↓ IDT setup
  ↓ PIC remap (IRQ0+IRQ1 unmasked)
  ↓ PIT init  (100 Hz)
  ↓ MM init   (bitmap + heap)
  ↓ Paging init (identity map, disabled)
  ↓ Syscall init (INT 0x80 gate)
  ↓ Keyboard init
  ↓ Serial init
  ↓ VGA init + welcome banner
  ↓ Shell init ("SOS> ")
  ↓ STI
  ↓ HLT loop (interrupts drive everything)
```

---

# 🧩 Tecnologias Utilizadas

| Tecnologia | Função |
|---|---|
| **C (Freestanding)** | Lógica principal do kernel |
| **NASM** | Assembly (loader, ISR stubs, syscall stub) |
| **GCC** | Compilação |
| **LD** | Linkagem do kernel |
| **GRUB** | Bootloader Multiboot |
| **QEMU** | Emulação |
| **GNU Make** | Automação da build |

---

# 🖥️ Compilar e Executar

## 📦 Dependências

```bash
# Debian / Ubuntu
sudo apt install gcc nasm grub-pc-bin grub-common xorriso qemu-system-x86
```

> **Nota:** usa `gcc` com `-m32`. Garante que tens as libc32 dev instaladas:
> `sudo apt install gcc-multilib`

---

## 🔨 Compilar

```bash
make
```

---

## ▶️ Executar (com janela QEMU)

```bash
make run
# ou directamente:
qemu-system-i386 -cdrom os.iso -serial stdio
```

## ▶️ Executar (headless / apenas serial)

```bash
make run-headless
```

---

## 🖱️ Testar o GUI

```
SOS> gui
```

Use ↑ ↓ para navegar, Enter para seleccionar, ESC para sair.

---

# 🗺️ Roadmap

## Fase 1 — Base do Kernel
- [x] Bootloader
- [x] VGA Text Mode
- [x] Serial COM1
- [x] GDT
- [x] IDT
- [x] Keyboard Driver

## Fase 2 — Sistema Operativo
- [x] PIT Timer
- [x] Shell interactiva
- [x] Memory Manager (bitmap + heap)
- [x] Heap Allocator (`kmalloc` / `kfree`)
- [x] Paging (identity map, enable/disable)
- [x] Syscalls (INT 0x80)
- [x] Multitasking cooperativo
- [x] GUI em VGA text mode

## Fase 3 — Futuro
- [ ] Paging completo com espaços de endereços separados
- [ ] Sistema de ficheiros (FAT12 ou simples)
- [ ] Preemptive multitasking via PIT
- [ ] Modo utilizador (ring 3)

---

# 📖 Referências

- OSDev Wiki
- Intel Software Developer Manuals
- JamesM Kernel Development Tutorials
- Low Level Learning
- Computer Systems: A Programmer's Perspective

---

# 👨‍💻 Autor

## Salimo Carvalho
