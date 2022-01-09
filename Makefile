all:
	g++ -std=c++17 -O2 \
	main.cpp \
	AudioStream.cpp \
	Network.cpp \
	DAC_Control.cpp \
	-l pthread \
	-o app.bin