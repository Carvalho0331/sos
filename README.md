
<h1 align="center">🧠 SOS — Mini Operating System</h1>

<p align="center">
  <b>Kernel x86 minimalista desenvolvido do zero em C e Assembly</b>
  <br>
  Exploração prática da arquitectura de sistemas operativos e comunicação directa com hardware.
</p>

<p align="center">
  <img src="https://img.shields.io/github/repo-size/Carvalho0331/sos?style=for-the-badge">
  <img src="https://img.shields.io/github/last-commit/Carvalho0331/sos?style=for-the-badge">
  <img src="https://img.shields.io/github/license/Carvalho0331/sos?style=for-the-badge">
  <img src="https://img.shields.io/badge/Architecture-x86-blue?style=for-the-badge">
  <img src="https://img.shields.io/badge/Language-C%20%7C%20Assembly-orange?style=for-the-badge">
  <img src="https://img.shields.io/badge/Environment-Freestanding-darkgreen?style=for-the-badge">
</p>

---

# 📖 Sobre o Projecto

O **SOS (Simple Operating System)** é um kernel experimental desenvolvido do zero sobre a arquitectura **x86**, sem dependência de bibliotecas padrão (*freestanding environment*).

O projecto foi criado com foco educacional e experimental para compreender profundamente como sistemas operativos funcionam internamente:

- Processo de boot
- Gestão de interrupções
- Comunicação com hardware
- Estruturas do processador
- Drivers básicos
- Arquitectura low-level

Ao contrário de aplicações tradicionais, este kernel executa directamente sobre o hardware.

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
- [x] Escrita directa na memória de vídeo
- [x] Controlo do cursor via portas I/O
- [x] Logging serial via COM1
- [x] Global Descriptor Table (GDT)
- [x] Interrupt Descriptor Table (IDT)
- [x] Interrupt handlers em Assembly
- [x] PIC Remapping
- [x] Driver básico de teclado
- [x] Build automatizada com Makefile
- [x] Execução via QEMU / Bochs

---

## 🛠️ Em Desenvolvimento

- [ ] Paging
- [ ] Memory Manager
- [ ] Heap allocator
- [ ] PIT Timer
- [ ] Shell interactiva
- [ ] Sistema de ficheiros
- [ ] Syscalls
- [ ] Multitasking

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
.
├── Makefile                      # Sistema de build automatizado
├── README.md                     # Documentação principal
├── link.ld                       # Linker script do kernel
├── kernel.elf                    # Kernel compilado
├── os.iso                        # Imagem ISO bootável
│
├── asm/                          # Código Assembly low-level
│   ├── loader.s                  # Entrada Multiboot
│   ├── io.s                      # Operações I/O (inb/outb)
│   ├── interrupt.s               # Interrupt handlers ASM
│   ├── gdt_asm.s                 # Carregamento da GDT
│   │
│   ├── loader.o
│   ├── io.o
│   ├── interrupt.o
│   └── gdt_asm.o
│
├── include/                      # Headers do kernel
│   ├── gdt.h
│   ├── idt.h
│   ├── io.h
│   ├── keyboard.h
│   ├── pic.h
│   └── serial.h
│
├── src/                          # Código-fonte principal do kernel
│   ├── kmain.c                   # Entry point do kernel
│   ├── serial.c                  # Driver serial COM1
│   ├── gdt.c                     # Global Descriptor Table
│   ├── idt.c                     # Interrupt Descriptor Table
│   ├── pic.c                     # PIC remapping
│   ├── keyboard.c                # Driver de teclado
│   ├── interrupt_handler_c.c     # Handlers em C
│   │
│   ├── kmain.o
│   ├── serial.o
│   ├── gdt.o
│   ├── idt.o
│   ├── pic.o
│   ├── keyboard.o
│   └── interrupt_handler_c.o
│
└── iso/
    └── boot/
        ├── kernel.elf            # Kernel copiado para boot
        │
        └── grub/
            └── grub.cfg          # Configuração do GRUB
```

---

# 🧩 Organização da Arquitectura

## 📁 `asm/`

Contém código Assembly responsável pelas operações mais próximas do hardware e do processador:

- Inicialização do kernel
- Manipulação de interrupções
- Instruções privilegiadas
- Comunicação via portas I/O

---

## 📁 `include/`

Headers partilhados entre os módulos do kernel.

Define:
- Estruturas
- Funções
- Macros
- Interfaces do sistema

---

## 📁 `src/`

Implementação principal do kernel em C.

Responsável por:
- Inicialização do sistema
- Drivers básicos
- Gestão de interrupções
- Comunicação serial
- Teclado
- Estruturas do processador

---

## 📁 `iso/`

Estrutura final utilizada para gerar a ISO bootável com GRUB.

Aqui o kernel é preparado para execução no QEMU ou hardware real.

---

# ⚙️ Fluxo de Inicialização

```text
GRUB
  ↓
loader.s
  ↓
kmain.c
  ↓
Serial Initialization
  ↓
GDT Setup
  ↓
IDT Setup
  ↓
PIC Remapping
  ↓
Keyboard Driver
  ↓
Kernel Running
```

---

# 🧩 Tecnologias Utilizadas

| Tecnologia | Função |
|---|---|
| **C (Freestanding)** | Lógica principal do kernel |
| **NASM** | Programação Assembly |
| **GCC** | Compilação |
| **LD** | Linkagem do kernel |
| **GRUB** | Bootloader Multiboot |
| **QEMU** | Emulação |
| **Bochs** | Debugging |
| **GNU Make** | Automação da build |

---

# 🖥️ Compilar e Executar

## 📦 Dependências

Instalar:

- `i686-elf-gcc`
- `nasm`
- `grub`
- `xorriso`
- `qemu-system-x86`

---

## 🔨 Compilar

```bash
make
```

---

## ▶️ Executar

```bash
qemu-system-i386 -cdrom os.iso
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
- Arquitectura x86
- Bootloaders
- Linkers e ELF
- Programação Freestanding
- Gestão de interrupções
- Drivers básicos
- Comunicação directa com hardware
- Segmentação
- I/O ports
- Memória de vídeo VGA

---

# 🎯 Objectivo Educacional

Este kernel não foi criado apenas para “funcionar”.

Foi desenvolvido para compreender profundamente:

- Como o computador arranca
- Como o processador executa código
- Como interrupções funcionam
- Como drivers comunicam com hardware
- Como kernels reais são estruturados
- Como sistemas operativos modernos começaram

Cada componente é implementado manualmente para maximizar compreensão técnica.

---

# 📦 Estatísticas do Projecto

| Item | Quantidade |
|---|---|
| Directórios | 7 |
| Ficheiros | 35 |
| Linguagens | C + Assembly |
| Arquitectura | x86 |
| Ambiente | Freestanding |
| Bootloader | GRUB |

---

# 🛣️ Roadmap

## Fase 1 — Base do Kernel
- [x] Bootloader
- [x] VGA Text Mode
- [x] Serial COM1
- [x] GDT
- [x] IDT
- [x] Keyboard Driver



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

## Como contribuir

```bash
git clone https://github.com/Carvalho0331/sos.git
cd sos
```

Cria uma branch:

```bash
git checkout -b minha-feature
```

Faz commit:

```bash
git commit -m "feat: nova funcionalidade"
```

Envia para o GitHub:

```bash
git push origin minha-feature
```

Depois abre um Pull Request 🚀


---

# 👨‍💻 Autor

## Salimo Carvalho

