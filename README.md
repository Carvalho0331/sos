
# 🧠 Mini Operating System (x86)

Este projeto é um sistema operativo simples desenvolvido do zero em C e Assembly, com o objetivo de compreender profundamente como um kernel funciona a nível de hardware.

---

# 🚀 Objetivo

Construir um kernel básico sem dependências de sistema operativo, explorando:

- Bootloader (Multiboot)
- Framebuffer (memória de vídeo)
- Drivers básicos
- Comunicação direta com hardware x86
- Estrutura inicial de um sistema operativo

---

# ⚙️ Tecnologias

- C (freestanding - sem libc)
- Assembly (NASM)
- GNU Linker (ld)
- GRUB (bootloader)
- QEMU / Bochs (emulação)

---

# 📁 Estrutura do Projeto

iso/
└── boot/
    └── grub/
        └── grub.cfg

kmain.c
loader.s
link.ld
Makefile
README.md

---

# 🖥️ Funcionalidades atuais

- Inicialização de kernel via Multiboot
- Escrita direta na memória de vídeo (0xB8000)
- Print básico no ecrã (framebuffer)
- Estrutura inicial de drivers

---

# 🧠 Conceitos estudados

- Arquitetura x86
- Processo de boot
- Memory-mapped I/O
- Interação direta com hardware
- Programação de sistemas de baixo nível
- Estrutura de kernel

---

# 🚀 Como executar

```bash
make
make run
