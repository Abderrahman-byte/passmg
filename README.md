# Passmg

A simple cli based password manager with multi-users and interactive shell.

## Building From Sources

### Install dependencies

#### rhel-based distros :

```shell
dnf install -y openssl-devel libsqlite3x-devel 
```

#### debian-based distros :

```shell
apt install -y libssl-dev libsqlite3-devel
```

#### pacman :

```shell
pacman -S sqlite openssl
```

### Compiling

```shell
cmake -S . -B build 
cmake --build build
install ./build/src/passmg -D /usr/bin/
```

## Arch/Manjaro 

[passmg package](https://aur.archlinux.org/packages/passmg) for Arch Linux and derived distros is available via AUR.

