# 🧠 Mini Operating System (x86)

Este projeto é um sistema operativo simples desenvolvido do zero em C e Assembly, com o objetivo de compreender profundamente como um kernel funciona a nível de hardware.

---

## 🚀 Objetivo

Construir um kernel básico sem dependências de sistema operativo, explorando:

- Bootloader (Multiboot)
- **Framebuffer** (memória de vídeo)
- **Driver de porta série (COM1)** para logging
- **Interface `write()`** para output no ecrã
- **Controlo de cursor** via portas I/O
- **Segmentação (GDT)** – em progresso
- Comunicação direta com hardware x86

---

## ⚙️ Tecnologias

- C (freestanding - sem libc)
- Assembly (NASM)
- GNU Linker (ld)
- GRUB (bootloader)
- QEMU / Bochs (emulação)

---

## 📁 Estrutura do Projeto
iso/
└── boot/
└── grub/
└── grub.cfg

---

## 🖥️ Funcionalidades atuais

- ✅ Inicialização do kernel via Multiboot
- ✅ Escrita direta na memória de vídeo (`0xB8000`)
- ✅ Print básico no ecrã com **`write(buf, len)`** (avança cursor)
- ✅ Controlo do cursor (movimento via portas `0x3D4`/`0x3D5`)
- ✅ **Driver da porta série (COM1)** – configuração e escrita de strings
- ✅ **Logging duplo**: ecrã (utilizador) e série (depuração)
- ⏳ Segmentação (GDT) – próxima etapa

---

## 🧠 Conceitos estudados

- Arquitetura x86
- Processo de boot e Multiboot
- Memory-mapped I/O (framebuffer)
- I/O ports (cursor, série)
- Controlo de hardware sem sistema operativo
- Estrutura mínima de kernel

---

## 🚀 Como executar

```bash
make
make run
Para ver a saída da porta série no QEMU (log do kernel):

bash
qemu-system-i386 -cdrom os.iso -serial stdio
