CC = g++
BUILD_DIR = build
SRC_DIR = main
INCLUDE_DIR = include
CFLAGS = -std=c++11 -Wall -Werror -I$(INCLUDE_DIR)

OBJECTS = \
	$(BUILD_DIR)/FTPSystem.o \
	$(BUILD_DIR)/User.o \
	$(BUILD_DIR)/Logger.o \
	$(BUILD_DIR)/UserManager.o \
	$(BUILD_DIR)/FileManager.o \
	$(BUILD_DIR)/JsonReader.o \
	
FTPSystemSensitivityList = \
	$(SRC_DIR)/FTPSystem.cpp \
	$(INCLUDE_DIR)/FTPSystem.h \

JsonReaderSensitivityList = \
	$(SRC_DIR)/JsonReader.cpp \
	$(INCLUDE_DIR)/JsonReader.h \

UserSensitivityList = \
	$(SRC_DIR)/User.cpp \
	$(INCLUDE_DIR)/User.h \

LoggerSensitivityList = \
	$(SRC_DIR)/Logger.cpp \
	$(INCLUDE_DIR)/Logger.h \

UserManagerSensitivityList = \
	$(SRC_DIR)/UserManager.cpp \
	$(INCLUDE_DIR)/UserManager.h \

FileManagerSensitivityList = \
	$(SRC_DIR)/FileManager.cpp \
	$(INCLUDE_DIR)/FileManager.h \

ServerSensitivityList = \
	$(SRC_DIR)/Server.cpp \
	$(INCLUDE_DIR)/Server.h \

ClientSensitivityList = \
	$(SRC_DIR)/Client.cpp \
	$(INCLUDE_DIR)/Client.h \

all: $(BUILD_DIR) Server.out Client.out

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

Server.out: $(BUILD_DIR)/Server.o $(OBJECTS)
	$(CC) $(CFLAGS) -o Server.out $(BUILD_DIR)/Server.o $(OBJECTS)

Client.out: $(BUILD_DIR)/Client.o $(OBJECTS)
	$(CC) $(CFLAGS) -o Client.out $(BUILD_DIR)/Client.o $(OBJECTS)

$(BUILD_DIR)/FTPSystem.o: $(FTPSystemSensitivityList)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/FTPSystem.cpp -o $(BUILD_DIR)/FTPSystem.o

$(BUILD_DIR)/JsonReader.o: $(JsonReaderSensitivityList)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/JsonReader.cpp -o $(BUILD_DIR)/JsonReader.o

$(BUILD_DIR)/User.o: $(UserSensitivityList)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/User.cpp -o $(BUILD_DIR)/User.o

$(BUILD_DIR)/Logger.o: $(LoggerSensitivityList)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/Logger.cpp -o $(BUILD_DIR)/Logger.o

$(BUILD_DIR)/UserManager.o: $(UserManagerSensitivityList)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/UserManager.cpp -o $(BUILD_DIR)/UserManager.o

$(BUILD_DIR)/FileManager.o: $(FileManagerSensitivityList)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/FileManager.cpp -o $(BUILD_DIR)/FileManager.o

$(BUILD_DIR)/Server.o: $(ServerSensitivityList)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/Server.cpp -o $(BUILD_DIR)/Server.o

$(BUILD_DIR)/Client.o: $(ClientSensitivityList)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/Client.cpp -o $(BUILD_DIR)/Client.o

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR) *.o *.out