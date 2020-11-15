CC = g++ -std=c++11 -o3 -mavx2 -fopenmp -C:/Users/61648/OneDrive/Desktop/2020-2021-Autumn/C++/Assignments/Project1/OpenBLAS_Install/include

TARGET = main

SRC = main.cpp matoperation.cpp get_info.cpp gen_data.cpp get_data.cpp allocate_data.cpp

LDFLAGS = -fopenmp -lopenblas -C:/Users/61648/OneDrive/Desktop/2020-2021-Autumn/C++/Assignments/Project1/OpenBLAS_Install/lib/

$(TARGET):$(SRC)
	$(CC) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f *.o $(TARGET)