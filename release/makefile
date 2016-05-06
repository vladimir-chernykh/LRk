RM := rm -rf

-include dependencies.mk

all: $(OBJS)	
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++  -o "LRk" $(OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

clean:
	-$(RM) $(OBJS)$(CPP_DEPS) LRk
	-@echo ' '
