.PHONY: all clean test run

BIN_DIR := output
BINS = main_app libplug.so

all: $(BIN_DIR) $(addprefix $(BIN_DIR)/, $(BINS))

$(BIN_DIR):
	-mkdir -p $@

$(BIN_DIR)/%.o: ./%.c
	$(CC) -c $< -o $@

$(BIN_DIR)/main_app: $(addprefix $(BIN_DIR)/, hotload.o main_app.o)
	gcc $? -o $@

$(BIN_DIR)/libplug.so: plug.c
	gcc -shared $< -o $@

clean:
	-rm -rf output

