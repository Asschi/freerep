################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Geom/Geom_Plane.cpp \
../Geom/Geom_Vec3.cpp 

CPP_DEPS += \
./Geom/Geom_Plane.d \
./Geom/Geom_Vec3.d 

OBJS += \
./Geom/Geom_Plane.o \
./Geom/Geom_Vec3.o 


# Each subdirectory must supply rules for building sources it contributes
Geom/%.o: ../Geom/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/jon/workspace/FreeREP/Headers" -O3 -Wall -c -fmessage-length=0 `pkg-config --cflags --libs gtk+-2.0 gtkglext-1.0 gtkglext-x11-1.0` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


