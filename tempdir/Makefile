CC = g++ -std=c++11
CCFLAGS = -c
CXXFLAGS=
OUTFILE=temp.exe
TESTPATH=./TestCases/


debug: CCFLAGS += -g
debug: CXXFLAGS += -g
debug: clean finish
	gdb $(OUTFILE)


prof: CCFLAGS +=-ggdb -fno-omit-frame-pointer
prof: CXXFLAGS +=-ggdb -fno-omit-frame-pointer
prof: clean finish
	sudo perf record ./$(OUTFILE) $(TEST)

profile: CCFLAGS +=-g -pg
profile: CXXFLAGS +=-g -pg
profile: clean finish
	./$(OUTFILE) $(TEST)
	gprof -l $(OUTFILE) | awk -F "," '{print $$1, $$NF}' > profile.out
	less -S profile.out

testRelation: finish
	@cp $(OUTFILE) $(TESTPATH)
	@echo "\n"
	@/usr/bin/time -f "Time Taken %e seconds.\n"  ~/workspace/.testCase.sh $(TESTPATH)$(OUTFILE) $(TESTPATH)Test*.rel
	@rm $(TESTPATH)$(OUTFILE)

test: finish
	@cp $(OUTFILE) $(TESTPATH)
	@echo "\n"
	@/usr/bin/time -f "Time Taken %e seconds.\n"  ~/workspace/.testCase.sh $(TESTPATH)$(OUTFILE) $(TESTPATH)Test?.??? $(TESTPATH)Test??.???
	@rm $(TESTPATH)$(OUTFILE)
	@echo "Functions with pmccabe 8 or greater:"
	@pmccabe *.cpp *.h | grep "^[8-9]\|^[1-9][0-9]" || echo "None."

checkSpeed: finish
	@echo "\n"
	/usr/bin/time -f "Time Taken %e seconds.\n" ./$(OUTFILE) $(TESTPATH)Test?.??? $(TESTPATH)Test??.??? > log.out


all: finish

finish: Tokenizer.o Parser.o datalogProgram.o StateMachine.o main.o  Relation.o
	$(CC) $(CXXFLAGS) Tokenizer.o Parser.o datalogProgram.o StateMachine.o Relation.o main.o -o $(OUTFILE)

main.o: main.cpp Tokenizer.o Parser.o
	$(CC) $(CCFLAGS) main.cpp

Tokenizer.o: Tokenizer.cpp Tokenizer.h StateMachine.o Token.h
	$(CC) $(CCFLAGS) Tokenizer.cpp

StateMachine.o: StateMachine.h StateMachine.cpp Token.h
	$(CC) $(CCFLAGS) StateMachine.cpp

Parser.o: Parser.cpp Parser.h Token.h datalogProgram.h datalogObject.h Predicate.h Parameter.h Expression.h Token.h Scheme.h Fact.h Query.h Rule.h TwoWayMap.h
	$(CC) $(CCFLAGS) Parser.cpp

datalogProgram.o: datalogProgram.cpp datalogProgram.h datalogObject.h TwoWayMap.h Scheme.h Fact.h Query.h Rule.h
	$(CC) $(CCFLAGS) datalogProgram.cpp

Relation.o: Relation.cpp Relation.h TwoWayMap.h
	$(CC) $(CCFLAGS) Relation.cpp

clean:
	rm -rf *.o *.h.gch $(OUTFILE) profile.out
