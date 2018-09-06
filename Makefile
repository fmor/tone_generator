
all : tone_generator

tone_generator : main.c

		gcc -o tone_generator main.c -lSDL2 -lm
clean:
	rm -f tone_generator;
