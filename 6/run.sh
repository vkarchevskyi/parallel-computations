export PATH="/opt/homebrew/opt/llvm/bin:$PATH"
export LDFLAGS="-L/opt/homebrew/opt/llvm/lib"
export CPPFLAGS="-I/opt/homebrew/opt/llvm/include"

# clang++ -fopenmp --std=c++11 -stdlib=libc++ 1-variant_lab4_task.cpp -o ../out && ../out
# clang++ -fopenmp --std=c++11 -stdlib=libc++ 1-variant_frobenius.cpp -o ../out && ../out
clang++ -fopenmp --std=c++11 -stdlib=libc++ 1-variant_inf.cpp -o ../out && ../out
