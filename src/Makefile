CXX = g++
CXXFLAGS = -std=c++11 -Wall -g -I. -I../include
SOURCE = test
BIN = test_bin

test_builder:
	$(CXX) -o test_builder $(CXXFLAGS) test_builder.cpp

test_runner:
	$(CXX) -o test_runner $(CXXFLAGS) test_runner.cpp

test_stupid_ptr:
	$(CXX) -o $(BIN)/stupid_ptr_tests $(CXXFLAGS) $(SOURCE)/stupid_ptr_tests.cpp

test_CSV:
	$(CXX) -o $(BIN)/CSV_tests $(CXXFLAGS) $(SOURCE)/CSV_tests.cpp

test_Ticket:		
	$(CXX) -o $(BIN)/Ticket_tests $(CXXFLAGS) $(SOURCE)/Ticket_tests.cpp

test_map:
	$(CXX) -o $(BIN)/map_tests $(CXXFLAGS) $(SOURCE)/map_tests.cpp

test_Datetime:
	$(CXX) -o $(BIN)/Datetime_tests $(CXXFLAGS) $(SOURCE)/Datetime_tests.cpp

test_IntervalManip:
	$(CXX) -o $(BIN)/IntervalManip_tests $(CXXFLAGS) $(SOURCE)/IntervalManip_tests.cpp

test_Segment:
	$(CXX) -o $(BIN)/Segment_tests $(CXXFLAGS) $(SOURCE)/Segment_tests.cpp

test_Route:
	$(CXX) -o $(BIN)/Route_tests $(CXXFLAGS) $(SOURCE)/Route_tests.cpp

test_Train:
	$(CXX) -o $(BIN)/Train_tests $(CXXFLAGS) $(SOURCE)/Train_tests.cpp

test_Json:
	$(CXX) -o $(BIN)/Json_tests $(CXXFLAGS) $(SOURCE)/Json_tests.cpp
