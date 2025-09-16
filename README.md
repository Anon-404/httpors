# 🚀 httpors

`httpors` is a **lightweight, multi-threaded HTTP probing tool** built with `libcurl`. It quickly fetches HTTP response info from domains or URLs, such as **status codes, IP addresses, content length, content type, and redirect locations**.  

---

## ✨ Features

- ⚡ Multi-threaded HTTP probing  
- 📂 Read domains from a file or stdin  
- 🟢 Display HTTP status codes (color-coded)  
- 🌐 Show resolved IP addresses  
- 📏 Display content length  
- 🗂️ Show content type  
- 🔗 Show redirect location if present  
- 💾 Save output to a file  
- 🖥️ Simple and clean CLI interface  

---

## 🛠️ Installation

**Dependencies:**

- GCC / Clang  
- `libcurl` development library  
- `CMake` (3.10 or higher)  

---

### 💻 Linux (Debian/Ubuntu)

```bash
sudo apt update
sudo apt install build-essential cmake libcurl4-openssl-dev
```
### 🏔️ Linux (Arch/Manjaro)

```bash
sudo pacman -Syu base-devel cmake curl
```

### 📱 Termux (Android)

```bash
pkg update && pkg upgrade
pkg install clang cmake curl
```

### 🔧 Build with CMake

```bash
git clone <your-repo-url>
cd httpors
mkdir build
cd build
cmake ..
make
```

The compiled binary `httpors` will be available inside the `build/` directory.

## 📖 Usage

```bash
./httpors [options]
```

### Options

| Flag | Description |
|------|-------------|
| `-f, --file <file>` | 📂 Read domains from a file |
| `-sc, --status-code` | 🟢 Show HTTP status code |
| `-ip` | 🌐 Show resolved IP address |
| `-cl, --content-length` | 📏 Show response content length |
| `-ct, --content-type` | 🗂️ Show response content type |
| `--location` | 🔗 Show redirect location (if any) |
| `-o, --output <file>` | 💾 Save output to a file |
| `-h, --help` | ❓ Show this help menu |

### Examples

```bash
./httpors -f subdomains.txt --sc --ip --cl
subfinder -d example.com | ./httpors --sc --ip
```

### 👤 Author
**Anon404 (William Steven)**
