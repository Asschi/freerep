################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../BREP_Algo/BREP_Algo_Extrude.cpp 

CPP_DEPS += \
./BREP_Algo/BREP_Algo_Extrude.d 

OBJS += \
./BREP_Algo/BREP_Algo_Extrude.o 


# Each subdirectory must supply rules for building sources it contributes
BREP_Algo/%.o: ../BREP_Algo/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/jon/workspace/FreeREP/Headers" -O0 -g3 -Wall -c -fmessage-length=0 `pkg-config --cflags --libs gtk+-2.0 gtkglext-1.0 gtkglext-x11-1.0` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


