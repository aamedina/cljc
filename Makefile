CC = clang++

LLVM_BINARY_PATH = /usr/local/opt/llvm/bin
CLANG_INCLUDE_PATH = /usr/local/opt/llvm/lib/clang/3.4/include
FRAMEWORKS = -framework Foundation -framework CoreFoundation
RPATH= /usr/local/opt/llvm/lib

CPPFLAGS = `$(LLVM_BINARY_PATH)/llvm-config --cxxflags` -fobjc-arc -std=c++11 -stdlib=libc++ -fobjc-call-cxx-cdtors
LDFLAGS = `$(LLVM_BINARY_PATH)/llvm-config --ldflags` -stdlib=libc++ -rpath $(RPATH)
LIBS = `$(LLVM_BINARY_PATH)/llvm-config --libs` -ledit -lcityhash $(FRAMEWORKS) -lclangAST -lclangASTMatchers -lclangAnalysis -lclangBasic -lclangCodeGen -lclangDriver -lclangDynamicASTMatchers -lclangEdit -lclangFormat -lclangFrontend -lclangFrontendTool -lclangIndex -lclangLex -lclangParse -lclangRewriteCore -lclangRewriteFrontend -lclangSema -lclangSerialization -lclangStaticAnalyzerCheckers -lclangStaticAnalyzerCore -lclangStaticAnalyzerFrontend -lclangTooling

all:
	$(CC) $(CPPFLAGS) -o clojure.o -c src/main.mm 
	$(CC) $(LDFLAGS) -o clojure clojure.o $(LIBS) 

test: all
	./clojure

clean:
	rm -rf *.o clojure
