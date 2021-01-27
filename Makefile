CC=g++
CFLAGS=-g -Wall
OBJS=main.o Activation.o Analysis.o Control.o MatrixMultiplyUnit.o UnifiedBuffer.o WeightFIFO.o
TARGET=main.out
 
$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)
 
main.o: MatrixMultiplyUnit.h UnifiedBuffer.h WeightFIFO.h Activation.h Analysis.h main.cpp
Activation.o: Activation.h MatrixMultiplyUnit.h Activation.cpp
Analysis.o: Analysis.h MatrixMultiplyUnit.h Analysis.cpp
Control.o: UnifiedBuffer.h WeightFIFO.h MatrixMultiplyUnit.h Control.h Control.cpp
MatrixMultiplyUnit.o: UnifiedBuffer.h WeightFIFO.h Analysis.h MatrixMultiplyUnit.h MatrixMultiplyUnit.cpp
UnifiedBuffer.o: UnifiedBuffer.h UnifiedBuffer.cpp
WeightFIFO.o: WeightFIFO.h WeightFIFO.cpp