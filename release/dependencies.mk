CPP_SRCS += \
../src/LR.cpp \
../src/grammar.cpp \
../src/main.cpp \
../src/my_str.cpp \
../src/tinystr.cpp \
../src/tinyxml.cpp \
../src/tinyxmlerror.cpp \
../src/tinyxmlparser.cpp 

OBJS += \
./bin/LR.o \
./bin/grammar.o \
./bin/main.o \
./bin/my_str.o \
./bin/tinystr.o \
./bin/tinyxml.o \
./bin/tinyxmlerror.o \
./bin/tinyxmlparser.o 

CPP_DEPS += \
./bin/LR.d \
./bin/grammar.d \
./bin/main.d \
./bin/my_str.d \
./bin/tinystr.d \
./bin/tinyxml.d \
./bin/tinyxmlerror.d \
./bin/tinyxmlparser.d 

bin/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '
