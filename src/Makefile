CXX = g++
CXXFLAGS = -std=c++11 -Wall -g -I. -I../include
SOURCE = test
BIN = test_bin
CPPFILES = sha1.cpp

test_builder:
	$(CXX) -o test_builder $(CXXFLAGS) $(CPPFILES) test_builder.cpp

test_runner:
	$(CXX) -o test_runner $(CXXFLAGS) $(CPPFILES) test_runner.cpp

test_stupid_ptr:
	$(CXX) -o $(BIN)/stupid_ptr_tests $(CXXFLAGS) $(CPPFILES) $(SOURCE)/stupid_ptr_tests.cpp

test_CSV:
	$(CXX) -o $(BIN)/CSV_tests $(CXXFLAGS) $(CPPFILES) $(SOURCE)/CSV_tests.cpp

test_Ticket:
	$(CXX) -o $(BIN)/Ticket_tests $(CXXFLAGS) $(CPPFILES) $(SOURCE)/Ticket_tests.cpp

test_map:
	$(CXX) -o $(BIN)/map_tests $(CXXFLAGS) $(CPPFILES) $(SOURCE)/map_tests.cpp

test_Datetime:
	$(CXX) -o $(BIN)/Datetime_tests $(CXXFLAGS) $(CPPFILES) $(SOURCE)/Datetime_tests.cpp

test_IntervalManip:
	$(CXX) -o $(BIN)/IntervalManip_tests $(CXXFLAGS) $(CPPFILES) $(SOURCE)/IntervalManip_tests.cpp

test_Segment:
	$(CXX) -o $(BIN)/Segment_tests $(CXXFLAGS) $(CPPFILES) $(SOURCE)/Segment_tests.cpp

test_Route:
	$(CXX) -o $(BIN)/Route_tests $(CXXFLAGS) $(CPPFILES) $(SOURCE)/Route_tests.cpp

test_Train:
	$(CXX) -o $(BIN)/Train_tests $(CXXFLAGS) $(CPPFILES) $(SOURCE)/Train_tests.cpp

test_Json:
	$(CXX) -o $(BIN)/Json_tests $(CXXFLAGS) $(CPPFILES) $(SOURCE)/Json_tests.cpp

test_Station:
	$(CXX) -o $(BIN)/Station_tests $(CXXFLAGS) $(CPPFILES) $(SOURCE)/Station_tests.cpp

test_User:
	$(CXX) -o $(BIN)/User_tests $(CXXFLAGS) $(CPPFILES) $(SOURCE)/User_tests.cpp

test_TrainBoom:
	$(CXX) -o $(BIN)/TrainBoom_tests $(CXXFLAGS) $(CPPFILES) $(SOURCE)/TrainBoom_tests.cpp

test_BinaryFile:
	$(CXX) -o $(BIN)/BinaryFile_tests $(CXXFLAGS) $(CPPFILES) $(SOURCE)/BinaryFile_tests.cpp
test_Order:
	$(CXX) -o $(BIN)/Order_tests $(CXXFLAGS) $(CPPFILES) $(SOURCE)/Order_tests.cpp
