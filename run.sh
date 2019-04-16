CC='~/llvm-build/bin/clang'
~/llvm-build/bin/clang -Xclang -load -Xclang build/hello/libHelloPass.* $1
rm a.out

