export PATH="/opt/homebrew/opt/llvm/bin:$PATH"
export LDFLAGS="-L/opt/homebrew/opt/llvm/lib"
export CPPFLAGS="-I/opt/homebrew/opt/llvm/include"

clang++ -fopenmp --std=c++11 -stdlib=libc++ lab8.c -o ../out && ../out
