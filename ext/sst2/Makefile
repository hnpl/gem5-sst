SST_VERSION=SST-10.1.0
GEM5_LIB=gem5_opt

LDFLAGS=-shared -fno-common ${shell pkg-config ${SST_VERSION} --libs} -L../../build/X86/
CXXFLAGS=-std=c++17 -g -O2 -fPIC ${shell pkg-config ${SST_VERSION} --cflags} ${shell python3-config --includes} -I../../build/X86/ -I../../ext/pybind11/include/
CPPFLAGS+=-MMD -MP
SRC=$(wildcard *.cc)

.PHONY: clean all

all: libgem5.so

libgem5.so: $(SRC:%.cc=%.o)
	${CXX} ${CPPFLAGS} ${LDFLAGS} $? -o $@ -l${GEM5_LIB}

-include $(SRC:%.cc=%.d)

clean:
	${RM} *.[do] libgem5.so
