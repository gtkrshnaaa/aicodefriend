# Target & Compiler
TARGET := aicodefriend
CC := gcc

# Directories
SRC_DIR := src
CORE_DIR := $(SRC_DIR)/core
UI_DIR := $(SRC_DIR)/ui
API_DIR := $(SRC_DIR)/api
UTILS_DIR := $(SRC_DIR)/utils

# Flags & Libraries
CFLAGS := $(shell pkg-config --cflags gtk+-3.0 json-glib-1.0 libsoup-3.0 gtksourceview-3.0) -I$(SRC_DIR) -Wall
LIBS := $(shell pkg-config --libs gtk+-3.0 json-glib-1.0 libsoup-3.0 gtksourceview-3.0)

# Source & Object Files
SOURCES := $(wildcard $(SRC_DIR)/*.c $(CORE_DIR)/*.c $(UI_DIR)/*.c $(API_DIR)/*.c $(UTILS_DIR)/*.c)
OBJECTS := $(SOURCES:.c=.o)

# Main Rules

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "Linking..."
	@$(CC) -o $@ $^ $(LIBS)
	@echo "✅ $(TARGET) siap dijalankan!"

%.o: %.c
	@echo "Compiling $<..."
	@$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	@echo "🚀 Running application..."
	@./$(TARGET)
	@echo "🧹 Cleaning up..."
	@$(MAKE) clean

clean:
	@rm -f $(TARGET) $(OBJECTS)

# Utility Rules
LISTING_DIR := z_listing
LISTING_FILE := $(LISTING_DIR)/listing.txt
ROOT := .

list:
	@mkdir -p $(LISTING_DIR)
	@echo "Generating file listing into $(LISTING_FILE)..."
	@> $(LISTING_FILE)
	@find $(ROOT) -type f \
		! -path "$(LISTING_FILE)" \
		! -path "./$(LISTING_DIR)/*" \
		! -path "./.git/*" \
		! -path "./.idea/*" \
		! -name "$(TARGET)" \
		! -name "*.o" \
	| while read file; do \
		echo "=== $$file ===" >> $(LISTING_FILE); \
		cat $$file >> $(LISTING_FILE); \
		echo "\n" >> $(LISTING_FILE); \
	done
	@echo "✅ File listing generated at $(LISTING_FILE)"

.PHONY: all clean list run