CC =g++

CFLAGS = -wall -Wextra

SRCS = cmos.l cmos.cpp

TARGET = cmos scanner

all: $(TARGET)

scanner: cmos.l
	flex cmos.l
	$(CC) -o scanner lex.yy.c

cmos : cmos.cpp
	$(CC) -o cmos cmos.cpp

test: all PlagarismDetector
	chmod +x PlagarismDetector
	./PlagarismDetector Examples

clean:
	rm -f $(TARGET) lex.yy.c tokens.txt PlagiarismReport.txt