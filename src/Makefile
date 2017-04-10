CXX = g++
CXXFLAGS = -std=c++11 -Wall -g -I.
BIN = test_bin

test_stupid_ptr:
	$(CXX) -O $(BIN)/STUPID_PTR_TESTS $(CXXFLAGS) TEST/STUPID_PTR_TESTS.CPP

test_CSV:
	$(CXX) -o $(BIN)/CSV_tests $(CXXFLAGS) test/CSV_tests.cpp

test_Ticket:		
	$(CXX) -o $(BIN)/Ticket_tests $(CXXFLAGS) test/Ticket_tests.cpp

test_map:
	$(CXX) -o $(BIN)/map_tests $(CXXFLAGS) test/map_tests.cpp

test_Datetime:
	$(CXX) -o $(BIN)/Datetime_tests $(CXXFLAGS) test/Datetime_tests.cpp

