<p align="center">
  <img src="assets/kernel-banner.png" width="900" alt="SOS Kernel Banner">
</p>

<h1 align="center">🧠 SOS — Mini Operating System</h1>

<p align="center">
  <b>Kernel x86 minimalista desenvolvido do zero em C e Assembly</b>
  <br>
  Exploração prática dos fundamentos de sistemas operativos, hardware e arquitectura low-level.
</p>

<p align="center">
  <img src="https://img.shields.io/github/repo-size/Carvalho0331/sos?style=for-the-badge">
  <img src="https://img.shields.io/github/last-commit/Carvalho0331/sos?style=for-the-badge">
  <img src="https://img.shields.io/github/license/Carvalho0331/sos?style=for-the-badge">
  <img src="https://img.shields.io/badge/Architecture-x86-blue?style=for-the-badge">
  <img src="https://img.shields.io/badge/Language-C%20%7C%20Assembly-orange?style=for-the-badge">
</p>

---

# 📖 Sobre o Projecto

O **SOS (Simple Operating System)** é um kernel experimental construído directamente sobre a arquitectura **x86**, sem dependência de bibliotecas padrão (*freestanding environment*).

O objectivo do projecto é compreender, na prática, como um sistema operativo comunica com o hardware em baixo nível:

- Inicialização via bootloader
- Gestão de interrupções
- Escrita directa em memória de vídeo
- Comunicação através de portas I/O
- Configuração de tabelas do processador
- Drivers básicos de hardware

Este projecto é focado em aprendizagem profunda de:
- Sistemas Operativos
- Arquitectura de Computadores
- Programação Low-Level
- Engenharia de Sistemas

---

# 🚀 Demonstração

<p align="center">
  <img src="assets/demo.gif" width="850">
</p>

---

# ✨ Features

## ✅ Implementado

- [x] Kernel Multiboot compatible
- [x] Boot via GRUB
- [x] VGA Text Mode
- [x] Escrita directa no framebuffer
- [x] Controlo do cursor via portas I/O
- [x] Logging serial (COM1)
- [x] Global Descriptor Table (GDT)
- [x] Interrupt Descriptor Table (IDT)
- [x] Interrupt handlers em Assembly
- [x] PIC Remapping
- [x] Driver básico de teclado
- [x] Build system com Makefile
- [x] Execução via QEMU / Bochs

---

## 🛠️ Em Desenvolvimento

- [ ] Paging
- [ ] Memory Manager
- [ ] Heap allocator
- [ ] Timer (PIT)
- [ ] Shell interactiva
- [ ] Sistema de ficheiros
- [ ] Multitasking
- [ ] Syscalls

---

# ⚙️ Arquitectura do Sistema

```text
                +-------------------+
                |       GRUB        |
                |   Bootloader      |
                +-------------------+
                          |
                          v
                +-------------------+
                |     loader.s      |
                |   Multiboot ASM   |
                +-------------------+
                          |
                          v
                +-------------------+
                |      kmain.c      |
                |   Kernel Entry    |
                +-------------------+
                   |      |      |
                   v      v      v
                 GDT     IDT   SERIAL
                   |
                   v
                KEYBOARD
```

---

# 🗂️ Estrutura do Projecto

```text
os/
├── iso/
│   └── boot/
│       └── grub/
│           └── grub.cfg          # Configuração do GRUB
│
├── loader.s                       # Bootloader Assembly
├── kmain.c                        # Entry point do kernel
├── link.ld                        # Linker script
├── Makefile                       # Sistema de build
│
├── io.s / io.h                    # Operações I/O
├── serial.c / serial.h            # Driver serial COM1
│
├── gdt.c / gdt.h / gdt_asm.s      # Global Descriptor Table
├── idt.c / idt.h                  # Interrupt Descriptor Table
├── interrupt.s                    # Interrupt handlers ASM
│
├── pic.c / pic.h                  # PIC remapping
├── keyboard.c / keyboard.h        # Driver de teclado
│
└── README.md
```

---

# 🧩 Tecnologias Utilizadas

| Tecnologia | Função |
|---|---|
| **C (Freestanding)** | Lógica principal do kernel |
| **NASM** | Assembly low-level |
| **GCC** | Compilador |
| **LD** | Linkagem do kernel |
| **GRUB** | Bootloader Multiboot |
| **QEMU** | Emulação |
| **Bochs** | Debugging |
| **GNU Make** | Automação da build |

---

# 🖥️ Compilar e Executar

## 📦 Dependências

Instalar:

- GCC Cross Compiler (`i686-elf-gcc`)
- NASM
- GRUB
- QEMU

---

## 🔨 Build

```bash
make
```

---

## ▶️ Executar

```bash
qemu-system-i386 -cdrom myos.iso
```

---

# 🖨️ Output Serial

Exemplo de inicialização:

```text
[OK] Serial initialized
[OK] GDT loaded
[OK] IDT loaded
[OK] PIC remapped
[OK] Keyboard initialized
```

---

# 📸 Screenshots

<p align="center">
  <img src="assets/screen1.png" width="850">
</p>

---

# 📚 Conceitos Explorados

Este projecto explora conceitos fundamentais de:

- Sistemas Operativos
- Gestão de memória
- Interrupções de hardware
- Arquitectura x86
- Bootloaders
- Linkagem e ELF
- Programação freestanding
- Comunicação directa com hardware
- Drivers básicos

---

# 🎯 Objectivo Educacional

Este kernel não foi criado apenas para “funcionar”.

Foi desenvolvido para compreender profundamente:

- Como o computador arranca
- Como o processador comunica com dispositivos
- Como interrupções funcionam
- Como um kernel controla o hardware
- Como sistemas operativos modernos começaram

Cada componente é implementado manualmente para maximizar compreensão técnica.

---

# 🛣️ Roadmap

## Fase 1 — Base do Kernel
- [x] Boot
- [x] VGA
- [x] Serial
- [x] GDT
- [x] IDT
- [x] Keyboard

## Fase 2 — Gestão de Memória
- [ ] Paging
- [ ] Heap
- [ ] Physical Memory Manager

## Fase 3 — Kernel Interactivo
- [ ] Shell
- [ ] Syscalls
- [ ] Multitasking

## Fase 4 — Sistema Completo
- [ ] Filesystem
- [ ] Drivers
- [ ] User mode
- [ ] Process scheduler

---

# 📖 Referências

- OSDev Wiki
- Intel Software Developer Manuals
- JamesM Kernel Development Tutorials
- Low Level Learning
- Computer Systems: A Programmer's Perspective

---

# 🤝 Contribuições

Contribuições, sugestões e melhorias são bem-vindas.

Se quiseres colaborar:

1. Faz fork do projecto
2. Cria uma branch
3. Implementa melhorias
4. Abre um Pull Request

---

# 📜 Licença

Distribuído sob licença MIT.

---

# 👨‍💻 Autor

## Salimo Carvalho

Programador em formação focado em:
- Sistemas Operativos
- Desenvolvimento Low-Level
- Engenharia de Software
- Arquitectura de Computadores

---

<p align="center">
  Feito com café, Assembly e curiosidade infinita ☕
</p>
