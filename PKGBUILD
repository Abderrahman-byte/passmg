pkgname=passmg
pkgver=2.0.0
pkgrel=1
pkgdesc="Yet another cli based password manager"
url="https://github.com/Abderrahman-byte/passmg"
license=('GPL3')
depends=(
    'sqlite3>=3.0'
    'openssl>=3.0'
)
makedepends=(
    'cmake>=3.11' 
    'make' 
    'git')
provides=('passmg')
arch=(
    'any'
)
source=()
md5sums=()

build() {
    cmake -S $startdir -B $startdir/build
    cmake --build $startdir/build 
}

package() {
    install -D $startdir/build/src/$pkgname $pkgdir/usr/bin/$pkgname
}
