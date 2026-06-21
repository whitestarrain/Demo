CFLAGS = -g -m32

# stack_frame.s: stack_frame.c
# 	$(CC) $(CFLAGS) -S stack_frame.c -o - | grep -vP '^	\.' | grep -v '.LF' > stack_frame.s

stack_frame.s: stack_frame.o
	# objdump -d -M intel -S  stack_frame.o > stack_frame.s
	llvm-objdump -d -S --no-show-raw-insn --x86-asm-syntax=intel stack_frame.o > stack_frame.s
	rm stack_frame.o
