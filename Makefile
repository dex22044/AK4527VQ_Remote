all:
	g++ -std=c++17 \
	main.cpp \
	AudioStream.cpp \
	-l pthread \
	-o app.bin